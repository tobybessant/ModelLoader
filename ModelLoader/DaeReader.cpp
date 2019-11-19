#include "DaeReader.h"
#include <string>
#include <iostream>
#include <regex>

struct DAESourceData {
	std::string stride;
	std::string semantic;
	std::vector<GLfloat> data;
};


void DaeReader::parse(std::string& path, Model& model)
{
	FILE* fp;
	errno_t err;
	char line[500];

	std::map<std::string, DAESourceData> dataStores;
	std::vector<GLuint> vertexDefinitions;

	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;

	std::string fileString;
	std::string fileStringCpy;

	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			fileString += line;
		}
	}

	fileStringCpy = fileString;

	// SET DATA STORES
	std::smatch matches;
	std::regex dataSourceExpression("<source.+id=\"([\\s\\S]+?)\"[\\s\\S]*?>[\\s\\S]+?<float_array [\\s\\S]+?>([\\s\\S]+?)<\/float_array>[\\s\\S]*?stride=\"([\\s\\S]+?)\"");

	while (std::regex_search(fileStringCpy, matches, dataSourceExpression)) {
		std::cout << "> Processing " << matches[1] << " . . ." << std::endl;

		DAESourceData source = DAESourceData();
		
		source.stride = matches[3];

		std::vector<GLfloat> v;
		std::string values = matches[2];
		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			v.push_back(std::stof(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// add match data to map
		source.data = v;
		dataStores[matches[1]] = source;
		fileStringCpy = matches.suffix();
	}

	// GET VERTICES INPUTS
	std::map<std::string, std::string> vertInputs;

	std::regex vertPositions("<vertices.*id=\"(.*?)\".*>[\\s\\S]*?<input.*POSITION.*source=\"(.*?)\".*\/>[\\s\\S]*?<\/vertices>");
	while (std::regex_search(fileStringCpy, matches, vertPositions)) {
		//std::string id = matches[1];
		//std::string idRef = id.substr(1, id.size());

		std::string source = matches[2];
		std::string sourceRef = source.substr(1, source.size());

		vertInputs[matches[1]] = sourceRef;

		// add match data to map
		fileStringCpy = matches.suffix();
	}

	// LOAD INPUT DATA
	fileStringCpy = fileString;
	std::map<GLuint, std::string> triInputs;

	std::regex triVertex("<input.*(?=.*semantic=\"(VERTEX)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");
	std::regex triNormal("<input.*(?=.*semantic=\"(NORMAL)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");
	std::regex triTexcoord("<input.*(?=.*semantic=\"(TEXCOORD)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");
	std::regex triColour("<input.*(?=.*semantic=\"(COLOR)\").*(?=.*offset=\"(.*?)\").*(?=.*source=\"(.*?)\")");

	std::vector<std::regex> triInputExpressions = {
		triNormal,
		triTexcoord,
		triVertex,
		triColour
	};

	std::cout << "> Preparing vertex data. . ." << std::endl;
	// get triangle inputs
	for (int i = 0; i < triInputExpressions.size(); i++) {

		while (std::regex_search(fileStringCpy, matches, triInputExpressions[i])) {
			std::string source = matches[3];
			std::string source_ref = source.substr(1, source.size());
			
			// remove hash
			if (vertInputs.count(source_ref) > 0 ) {
				triInputs[std::stoi(matches[2])] = vertInputs[source_ref];
				dataStores[vertInputs[source_ref]].semantic = matches[1];
			}	
			else {
				triInputs[std::stoi(matches[2])] = source_ref;
				dataStores[source_ref].semantic = matches[1];
			}

			// add match data to map
			fileStringCpy = matches.suffix();
		}
		fileStringCpy = fileString;
	}
	
	// FETCH INDICES
	fileStringCpy = fileString;
	std::regex vertexDefinitionsExpression("<triangles.*?>[\\s\\S]*?<p>([\\s\\S]*?)</p>[\\s\\S]*?</triangles>");

	std::cout << "> Loading indices. . ." << std::endl;
	while (std::regex_search(fileStringCpy, matches, vertexDefinitionsExpression)) {
		std::string values = matches[1];

		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			vertexDefinitions.push_back(std::stoi(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// add match data to map
		fileStringCpy = matches.suffix();
	}

	// BUILD VERTICES
	std::cout << "> Building model vertices. . ." << std::endl;
	for (int i = 0; i < vertexDefinitions.size(); i = i + triInputs.size()) {
		GLuint offset = i;

		glm::vec3 v = glm::vec3();
		glm::vec3 vn = glm::vec3();
		glm::vec2 vt = glm::vec2();
		glm::vec4 vc = glm::vec4();

		for (int j = 0; j < triInputs.size(); j++) {
			std::string sourceId = triInputs[j];
			
			GLuint currentIndex = vertexDefinitions[i + j];
			
			// fetch data from source and add to vertex attributes above
			DAESourceData source = dataStores[sourceId];

			if (source.semantic == "VERTEX") {
				v.x = source.data[currentIndex * 3];
				v.y = source.data[currentIndex * 3 + 1];
				v.z = source.data[currentIndex * 3 + 2];
			}
			else if (source.semantic == "NORMAL") {
				vn.x = source.data[currentIndex * 3];
				vn.y = source.data[currentIndex * 3 + 1];
				vn.z = source.data[currentIndex * 3 + 2];
			}
			else if (source.semantic == "TEXCOORD") {
				vt.s = source.data[currentIndex * 2];
				vt.t = source.data[currentIndex * 2 + 1];
			}
			else if (source.semantic == "COLOR") {
				vc.r = source.data[currentIndex * 4];
				vc.g = source.data[currentIndex * 4 + 1];
				vc.b = source.data[currentIndex * 4 + 2];
				vc.a = source.data[currentIndex * 4 + 3];
			}

		}

		Vertex vertex = Vertex(v, vn, vt, vc);
		vertices.push_back(vertex);

		// update offset for the next set
		offset += triInputs.size();
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}
	std::cout << "> Finishing up. . ." << std::endl;

	// LOAD MATERIAL
	std::regex materialExpression("<library_images>[\\s\\S]*?<init_from>([\\s\\S]+?)</init_from>[\\s\\S]*?<\/library_images>");

	// generate model
	Mesh m = Mesh();

	// add material if there is one
	fileStringCpy = fileString;
	while (std::regex_search(fileStringCpy, matches, materialExpression)) {
		std::string material_source = matches[1];

		Material mat = Material();
		mat.diffuseTextureMapPath = FileReader::getDirectory(path) + material_source;
		m.setMaterial(mat);
		break;
	}

	m.setVertexes(vertices);
	m.setIndices(indices);
	m.init();

	Object o = Object();
	o.addMesh(m);

	model.addObject(o);
}
