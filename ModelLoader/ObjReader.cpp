#include "ObjReader.h"
#include <map>

using namespace std;

void ObjReader::parse(string &path, Model &model)
{
	// init file reader components
	FILE* fp;
	errno_t err;
	char  line[255];

	// declare map of materials where the key is the material name
	std::map<std::string, Material>materials;

	// declare stores for the vertex attribute source data
	vector<glm::vec3> vertexStore;
	vector<glm::vec2> textureStore;
	vector<glm::vec3> normalStore;

	// declare vectors for the indices and vertices to add to each mesh
	vector<GLuint> indices;
	vector<Vertex> vertices;

	// counter to store the indices offset after each face is built
	GLuint currentVertexCount = 0;
	GLuint linesRead = 0;

	// declare nullptrs to store and build the data for each mesh
	Object* templateObject = nullptr;
	Mesh* templateMesh = nullptr;

	// open file stream to read file line by line
	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			linesRead++;

			// check whether each line contains the specified token (mtllib, o, v, etc)
			if (strstr(line, "mtllib ") != NULL) {
				// build material file path
				string mtlPath = getDirectory(path) + getFileName(line);
				
				// use mtl path to load materials into material store
				loadMtl(mtlPath, materials);
			}
			if (strstr(line, "o ") != NULL) {
				// if token is o and the templateObject pointer is not null we can add the current o to the model before resetting it for the next
				if (templateObject != nullptr) {
					model.addObject(*templateObject);
				}

				// generate new object and set its name
				templateObject = &Object();
				char* namePtr = strchr(line, ' ');
				templateObject->setName(namePtr);
			}

			if (strstr(line, "v ") != NULL) {
				// generate a vector 3 from the line if the token is v (positional vertex) and add it to the positional vertex store
				vertexStore.push_back(createVector3(line));
			}

			if (strstr(line, "vt ") != NULL) {
				// generate a vector 3 from the line if the token is vt (vertex texture) and add it to the vertex texture store
				textureStore.push_back(createVector3(line));
			}

			if (strstr(line, "vn ") != NULL) {
				// generate a vector 3 from the line if the token is vn (vertex normal) and add it to the vertex normal store
				normalStore.push_back(createVector3(line));
			}

			if(strstr(line, "usemtl ")) {
				// if token is 'usemtl' and the template mesh is not null we can add the generated vertices / indices to the mesh, 
				// init OpenGL resources, and add that mesh to the template object we are currently working with. 
				if (templateMesh != nullptr) {
					templateMesh->setIndices(indices);
					templateMesh->setVertexes(vertices);

					templateMesh->init();
					templateObject->addMesh(*templateMesh);
					currentVertexCount = 0;
				}

				// reset mesh data for next mesh
				templateMesh = new Mesh();
				string materialName = getValue(line);
				templateMesh->setMaterial(materials[materialName]);


				indices = vector<GLuint>();
				vertices = vector<Vertex>();
			}

			if (strstr(line, "f ") != NULL) {
				// if token is 'f' then a new face has been hit and vertex objects can be constructed.
				// declare variables to store each vertex attribute
				glm::vec3 v;
				glm::vec2 vt;
				glm::vec3 vn;

				char* index = nullptr;
				char* nextIndex = nullptr;

				// tokenise face on ' /' (space slash) to iterate over each individual face index
				index = strtok_s((char*)line, " /", &nextIndex);
				index = strtok_s(nextIndex, " /", &nextIndex);

				int indexType = 0;
				while (index != NULL) {

					// identify which vertex attribute to get source data from based on the indexType (counter) % 3 value
					if (indexType % 3 == 0) {
						// if it is the first index of the 3 (indexType == 0) then it is a positional vector index
						v = vertexStore.at(std::stoi(index) - 1);
					}
					else if (indexType % 3 == 1) {
						// if it is the second index of the 3 (indexType == 1) then it is a texture vector index
						vt = textureStore.at(std::stoi(index) - 1);
					}
					else {
						// else it is the normal and we can assume we have covered all 3 of a given vertex's indices
						vn = normalStore.at(std::stoi(index) - 1);

						// set colour of the vertex from the template mesh material
						glm::vec4 vcol = glm::vec4(templateMesh->getMaterialColour(), 1.0f);

						// build new vertex using the fetched data and add to the vertices vector
						Vertex newV = Vertex(v, vn, vt, vcol);
						vertices.push_back(newV);
					}

					// search next index
					index = strtok_s(nextIndex, " /", &nextIndex);
					indexType++;
				}

				// generate indices for the face
				// if the index type is 12 i.e. we have traversed 12 index values we can infer the face was defined as a quad
				if (indexType ==  12) {
					// trianglate the indexes to re-use vertices
					indices.push_back(0 + currentVertexCount);
					indices.push_back(1 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);
					indices.push_back(0 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);
					indices.push_back(3 + currentVertexCount);

					// update the offset accordingly
					currentVertexCount = currentVertexCount + 4;
				}
				else {
					// no need to triangulate the face
					indices.push_back(0 + currentVertexCount);
					indices.push_back(1 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);

					// update the offset accordingly
					currentVertexCount = currentVertexCount + 3;
				}
			}
		}
	}

	// clear-out statement to add the last mesh to the object
	if (templateMesh != nullptr) {
		templateMesh->setIndices(indices);
		templateMesh->setVertexes(vertices);
		templateMesh->init();
		templateObject->addMesh(*templateMesh);
	}

	// clear-out statement to add the last object to the model
	if (templateObject != nullptr) {
		model.addObject(*templateObject);
	}
}

// read in the mtl file path and a reference to the store for the materials
void ObjReader::loadMtl(std::string& mtlPath, std::map<std::string, Material>& materials)
{
	// init file stream components
	FILE* mtlFp;
	errno_t mtlErr;
	char mtlLine[255];

	// init a string to store the current mesh being built
	string currentMaterial = "";

	// remove new-line character from path
	char* pos;
	if ((pos = strchr((char*)mtlPath.c_str(), '\n')) != NULL)
		* pos = '\0';

	// open file stream
	mtlErr = fopen_s(&mtlFp, mtlPath.c_str(), "r");
	if (mtlErr == 0) {
		while (fgets(mtlLine, sizeof(mtlLine), mtlFp) != NULL)
		{
			// remove newline character from line
			if ((pos = strchr(mtlLine, '\n')) != NULL)
				* pos = '\0';

			if (strstr(mtlLine, "newmtl ")) {
				// if token is newmtl then set the currentMaterial value and add to map with blank material
				currentMaterial = getValue(mtlLine);
				Material mat = Material();
				materials[currentMaterial] = mat;
				materials[currentMaterial].name = currentMaterial;
			} 
			else if (strstr(mtlLine, "map_Kd ") != NULL) {
				// get diffuse path and add to currentMaterial map entry
				materials[currentMaterial].diffuseTextureMapPath = getDirectory(mtlPath) + getFileName(mtlLine);
			} 
			else if (strstr(mtlLine, "map_d ") != NULL) {
				// get alpha path and add to currentMaterial map entry
				materials[currentMaterial].alphaTextureMapPath = getDirectory(mtlPath) + getFileName(mtlLine);
			} 
			else if (strstr(mtlLine, "Ka ")) {
				// add ambient vector3 to currentMaterial map entry
				materials[currentMaterial].ambient = createVector3(mtlLine);
			}
			else if (strstr(mtlLine, "Kd ") != NULL) {
				// add diffuse vector3 to currentMaterial map entry
				materials[currentMaterial].diffuse = createVector3(mtlLine);
			}
			else if (strstr(mtlLine, "Ks ") != NULL) {
				// add specular vector3 to currentMaterial map entry
				materials[currentMaterial].specular = createVector3(mtlLine);
			}
			else if (strstr(mtlLine, "d ") != NULL) {
				// add dissolve value to currentMaterial map entry
				materials[currentMaterial].dissolve = stof(getValue(mtlLine));
			}
		}
	}
}

// when given a line with 4 space separated components where the last 3 are float values, 
// this function will return a vec3 using said values
glm::vec3 ObjReader::createVector3(char* line) {

	char* token;
	char* nextToken = nullptr;

	// array to store final values
	float values[3] { 0, 0, 0 };
	int counter = 0;

	// tokenise line on space
	token = strtok_s(line, " ", &nextToken);
	while (token != NULL) {
		// if current token index is not 0 (first) add to final values array
		if (counter > 0) {
			values[counter - 1] = stof(token);
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	// use values in array to build and return a vec3
	return glm::vec3(values[0], values[1], values[2]);

}

// when given a line with 3 space separated components where the last 2 are float values, 
// this function will return a vec2 using said values
glm::vec2 ObjReader::createVector2(char* line) {
	char* token;
	char* nextToken = nullptr;

	// array to store final values
	float values[2] { 0, 0 };
	int counter = 0;

	// tokenise line on space
	token = strtok_s(line, " ", &nextToken);
	while (token != NULL) {
		// if current token index is not 0 (first) add to final values array
		if (counter > 0) {
			values[counter - 1] = stof(token);
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	// use values in array to build and return a vec2
	return glm::vec2(values[0], values[1]);

}

// when given a line with 2 space separated components this function will return the second
std::string ObjReader::getValue(char* line)
{
	// remove newline character
	char* pos;
	if ((pos = strchr(line, '\n')) != NULL)
		* pos = '\0';

	// duplicate as to not lose the variable when scope is left
	string lineString = (string)line;
	string duplicate = lineString;

	char* token;
	char* nextToken = nullptr;

	// array to store final value
	char* values[1]{ 0 };
	int counter = 0;

	// tokenise on space and iterate over line
	token = strtok_s((char*)duplicate.c_str(), " ", &nextToken);
	while (token != NULL) {
		// if element is not the first, add to values array
		if (counter > 0) {
			values[counter - 1] = token;
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	// return string-casted value
	return (string)values[0];
}

// when given a line with 2 space separated components return the latter as a string
std::string ObjReader::getFileName(char* line)
{
	// get pointer to first ocurrence of space character
	char* result = strchr(line, ' ');
	
	// increment pointer location to start of the word following the space character
	*(++result);

	// return pointer to start of second word

	return (string)result;
}
