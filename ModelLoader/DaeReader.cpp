#include "DAELoader.h"
#include <string>
#include <iostream>
#include <regex>

// struct to store key information about a DAE input source 
struct DAESourceData {
	std::string stride;
	std::string semantic;
	std::vector<GLfloat> data;
};

void DAELoader::parse(std::string& path, Model& model)
{
	// init file reader components
	FILE* fp;
	errno_t err;
	char line[500];

	// init map for data stores and vector for vertex definitions
	std::map<std::string, DAESourceData> dataStores;
	std::vector<GLuint> vertexDefinitions;

	// init result indices and vertices vectors
	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;

	// init strings to store whole file
	std::string fileString;
	std::string fileStringCpy;

	// read entire file into a single string for effective regex searching
	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			fileString += line;
		}
	}

	// copy string so original state can be preserved
	fileStringCpy = fileString;

	// SET DATA STORES
	std::smatch matches;

	// this regex searches for source tags with an enclosed float array. Capture groups specify the source id, data and stride
	std::regex dataSourceExpression("<source.+id=\"([\\s\\S]+?)\"[\\s\\S]*?>[\\s\\S]+?<float_array [\\s\\S]+?>([\\s\\S]+?)<\/float_array>[\\s\\S]*?stride=\"([\\s\\S]+?)\"");

	// search file string for matches
	while (std::regex_search(fileStringCpy, matches, dataSourceExpression)) {
		std::cout << "  > Processing " << matches[1] << " . . ." << std::endl;

		// init new source object
		DAESourceData source = DAESourceData();
		
		// set stride
		source.stride = matches[3];

		// add values to float vector
		std::vector<GLfloat> v;
		std::string values = matches[2];
		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			v.push_back(std::stof(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// set data
		source.data = v;

		// add source to map where source id is the key
		dataStores[matches[1]] = source;

		// search remainder of file
		fileStringCpy = matches.suffix();
	}

	// GET VERTICES INPUTS
	std::map<std::string, std::string> vertInputs;

	// this regex searches for a position input within a vertices element
	std::regex vertPositions("<vertices.*id=\"(.*?)\".*>[\\s\\S]*?<input.*POSITION.*source=\"(.*?)\".*\/>[\\s\\S]*?<\/vertices>");
	while (std::regex_search(fileStringCpy, matches, vertPositions)) {

		// fetch source id without hash
		std::string source = matches[2];
		std::string sourceRef = source.substr(1, source.size());

		// add the position input id into a map where the key is the vertices id (so it can be interchanged later)
		vertInputs[matches[1]] = sourceRef;

		// search remainder of file
		fileStringCpy = matches.suffix();
	}

	// MATCH INPUTS TO SOURCE DATA
	fileStringCpy = fileString;
	std::map<GLuint, std::string> triInputs;

	// a set of regex expressions that search for each type of input and capture the semantic, offset, and source id
	std::regex triVertex("<input.*(?=.*semantic=\"(VERTEX)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");
	std::regex triNormal("<input.*(?=.*semantic=\"(NORMAL)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");
	std::regex triTexcoord("<input.*(?=.*semantic=\"(TEXCOORD)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");
	std::regex triColour("<input.*(?=.*semantic=\"(COLOR)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");

	// iterable list of aformentioned regex expressions
	std::vector<std::regex> triInputExpressions = {
		triNormal,
		triTexcoord,
		triVertex,
		triColour
	};

	std::cout << "  > Preparing vertex data. . ." << std::endl;
	
	// search the file using each regex to match each input with its source
	for (int i = 0; i < triInputExpressions.size(); i++) {

		while (std::regex_search(fileStringCpy, matches, triInputExpressions[i])) {

			// capture source id and remove hash
			std::string source = matches[3];
			std::string source_ref = source.substr(1, source.size());
			
			// if the source id for the given input matches the vertices element id, swap it out for the enclosed vertices input (positiions)
			if (vertInputs.count(source_ref) > 0 ) {
				// match and add source id and its data to the data stores
				triInputs[std::stoi(matches[2])] = vertInputs[source_ref];
				
				// set the semantic of the data source
				dataStores[vertInputs[source_ref]].semantic = matches[1];
			}	
			else {
				// match and add source id and its data to the data stores
				triInputs[std::stoi(matches[2])] = source_ref;

				// set the semantic of the data source
				dataStores[source_ref].semantic = matches[1];
			}

			// add match data to map
			fileStringCpy = matches.suffix();
		}
		
		// search the remainder of the file
		fileStringCpy = fileString;
	}
	
	// LOAD INDICES
	fileStringCpy = fileString;

	// this regex searches for a 'p' tag inside the 'triangles' tags. Capture group pulls out the enclosed vertex definition indices
	std::regex vertexDefinitionsExpression("<triangles.*?>[\\s\\S]*?<p>([\\s\\S]*?)</p>[\\s\\S]*?</triangles>");

	std::cout << "  > Loading indices. . ." << std::endl;
	while (std::regex_search(fileStringCpy, matches, vertexDefinitionsExpression)) {
		// capture index values
		std::string values = matches[1];

		char* token;
		char* nextToken = nullptr;

		// tokenise, convert to float and add to vector
		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			vertexDefinitions.push_back(std::stoi(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// search the remainder of the file
		fileStringCpy = matches.suffix();
	}

	// BUILD VERTICES
	std::cout << "  > Building model vertices. . ." << std::endl;

	// iterate over vertex definitions indices in 'chunks', interrogate which data store it references, and build a vertex
	for (int i = 0; i < vertexDefinitions.size(); i = i + triInputs.size()) {

		// prepare variables to store data
		glm::vec3 v = glm::vec3();
		glm::vec3 vn = glm::vec3();
		glm::vec2 vt = glm::vec2();
		glm::vec4 vc = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		// iterate over each index within the chunk and interrogate which data store it is referencing based on its semantic
		for (int j = 0; j < triInputs.size(); j++) {
			// get the source id from the triInputs using the current offset (j)
			std::string sourceId = triInputs[j];
			
			// using the current vertex (i) and the current index within it (j), get the current index value from vertex definitions
			GLuint currentIndex = vertexDefinitions[i + j];
			
			// using the source id, fetch the corresponding input source object
			DAESourceData source = dataStores[sourceId];

			// depending on the sources semantic, build appropriate vertex attribute
			if (source.semantic == "VERTEX") {
				// TODO: change the multiplier offset to be the stride of the input data

				// check that the extreme index (largest) is present in the data source it is referencing
				if ((currentIndex * 3 + 2) > source.data.size() - 1)
					throw std::exception();

				v.x = source.data[currentIndex * 3];
				v.y = source.data[currentIndex * 3 + 1];
				v.z = source.data[currentIndex * 3 + 2];
			}
			else if (source.semantic == "NORMAL") {
				// check that the extreme index (largest) is present in the data source it is referencing
				if ((currentIndex * 3 + 2) > source.data.size() - 1)
					throw std::exception();

				vn.x = source.data[currentIndex * 3];
				vn.y = source.data[currentIndex * 3 + 1];
				vn.z = source.data[currentIndex * 3 + 2];
			}
			else if (source.semantic == "TEXCOORD") {
				// check that the extreme index (largest) is present in the data source it is referencing
				if ((currentIndex * 2 + 1) > source.data.size() - 1)
					throw std::exception();

				vt.s = source.data[currentIndex * 2];
				vt.t = source.data[currentIndex * 2 + 1];
			}
			else if (source.semantic == "COLOR") {
				// check that the extreme index (largest) is present in the data source it is referencing
				if ((currentIndex * 4 + 3) > source.data.size() - 1)
					throw std::exception();

				vc.r = source.data[currentIndex * 4];
				vc.g = source.data[currentIndex * 4 + 1];
				vc.b = source.data[currentIndex * 4 + 2];
				vc.a = source.data[currentIndex * 4 + 3];

				std::cout << "Data size" << source.data.size() << std::endl;
				std::cout << "Largest col index: " << (currentIndex * 3 + 3) << std::endl;
			}

		}

		// once data has been interrogated, build vertex and add to final vertices vector
		Vertex vertex = Vertex(v, vn, vt, vc);
		vertices.push_back(vertex);
	}

	// generate indices. Since the vertices have already been triangluated, the indices are 0-n vertices
	for (size_t i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}
	std::cout << "  > Finishing up. . ." << std::endl;

	// LOAD MATERIAL
	// this regex searches file for an image source within the library images tags. Capture group returns the image file name
	std::regex materialExpression("<library_images>[\\s\\S]*?<init_from>([\\s\\S]+?)</init_from>[\\s\\S]*?<\/library_images>");

	// generate mesh to load data into
	Mesh mesh = Mesh();

	// add material if there is one
	fileStringCpy = fileString;
	while (std::regex_search(fileStringCpy, matches, materialExpression)) {
		std::string materialSource = matches[1];

		// if the material source is not dae template image for no texture then add material using the found texture
		if (materialSource != "notexture.png") {
			Material mat = Material();
			mat.diffuseTextureMapPath = getDirectory(path) + materialSource;
			mesh.setMaterial(mat);
		}
		break;
	}

	// set the vertices and indices for the mesh
	mesh.setVertexes(vertices);
	mesh.setIndices(indices);

	// init OpenGL components
	mesh.init();

	// create template object to load mesh into
	Object o = Object();
	o.addMesh(mesh);

	// load object into model that was handed into the function
	model.addObject(o);

	std::cout << "  > Complete" << std::endl;
	std::cout << "  ==================================" << std::endl;
}
