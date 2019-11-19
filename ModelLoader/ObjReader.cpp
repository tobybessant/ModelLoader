#include "ObjReader.h"
#include <map>

using namespace std;

void ObjReader::parse(string &path, Model &model)
{
	FILE* fp;
	errno_t err;
	char  line[255];

	std::map<std::string, Material>materials;

	vector<glm::vec3> vertexStore;
	vector<glm::vec2> textureStore;
	vector<glm::vec3> normalStore;

	vector<GLuint> indices;
	vector<Vertex> vertices;
	GLuint currentVertexCount = 0;
	GLuint linesRead = 0;

	Object* templateObject = nullptr;
	Mesh* templateMesh = nullptr;

	Model loadedModel = Model();

	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			linesRead++;
			if (strstr(line, "mtllib ") != NULL) {
				string mtlPath = getDirectory(path) + getFileName(line);
				cout << mtlPath;
				loadMtl(mtlPath, materials);
			}
			if (strstr(line, "o ") != NULL) {
				if (templateObject != nullptr) {
					model.addObject(*templateObject);
				}

				templateObject = &Object();
				char* namePtr = strchr(line, ' ');
				templateObject->setName(namePtr);
			}

			if (strstr(line, "v ") != NULL) {
				vertexStore.push_back(createVector3(line));
			}

			if (strstr(line, "vt ") != NULL) {
				textureStore.push_back(createVector3(line));
			}

			if (strstr(line, "vn ") != NULL) {
				normalStore.push_back(createVector3(line));
			}

			if(strstr(line, "usemtl ")) {
				if (templateMesh != nullptr) {
					templateMesh->setIndices(indices);
					templateMesh->setVertexes(vertices);

					templateMesh->init();
					templateObject->addMesh(*templateMesh);
					currentVertexCount = 0;
				}

				// wipe 
				templateMesh = new Mesh();
				string materialName = getValue(line);
				templateMesh->setMaterial(materials[materialName]);


				indices = vector<GLuint>();
				vertices = vector<Vertex>();
			}

			if (strstr(line, "f ") != NULL) {

				glm::vec3 v;
				glm::vec2 vt;
				glm::vec3 vn;

				char* index = nullptr;
				char* nextIndex = nullptr;

				index = strtok_s((char*)line, " /", &nextIndex);
				index = strtok_s(nextIndex, " /", &nextIndex);

				int indexType = 0;
				while (index != NULL) {

					if (indexType % 3 == 0) {
						v = vertexStore.at(std::stoi(index) - 1);
					}
					else if (indexType % 3 == 1) {
						vt = textureStore.at(std::stoi(index) - 1);
					}
					else {
						GLuint vi = std::stoi(index);
						vn = normalStore.at(vi - 1);

						glm::vec4 vcol = glm::vec4(templateMesh->getMaterialColour(), 1.0f);

						Vertex newV = Vertex(v, vn, vt, vcol);
						vertices.push_back(newV);
					}

					index = strtok_s(nextIndex, " /", &nextIndex);
					indexType++;
				}

				if (indexType ==  12) {
					indices.push_back(0 + currentVertexCount);
					indices.push_back(1 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);
					indices.push_back(0 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);
					indices.push_back(3 + currentVertexCount);

					currentVertexCount = currentVertexCount + 4;
				}
				else {
					indices.push_back(0 + currentVertexCount);
					indices.push_back(1 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);

					currentVertexCount = currentVertexCount + 3;
				}
			}
		}
	}

	if (templateMesh != nullptr) {
		templateMesh->setIndices(indices);
		templateMesh->setVertexes(vertices);
		templateMesh->init();
		templateObject->addMesh(*templateMesh);
	}

	if (templateObject != nullptr) {
		model.addObject(*templateObject);
	}
}


void ObjReader::loadMtl(std::string& mtlPath, std::map<std::string, Material>& materials)
{
	FILE* mtlFp;
	errno_t mtlErr;
	char mtlLine[255];

	Material* tempMaterial = nullptr;
	string currentMaterial = "";

	char* pos;
	if ((pos = strchr((char*)mtlPath.c_str(), '\n')) != NULL)
		* pos = '\0';


	mtlErr = fopen_s(&mtlFp, mtlPath.c_str(), "r");
	if (mtlErr == 0) {
		while (fgets(mtlLine, sizeof(mtlLine), mtlFp) != NULL)
		{
			if ((pos = strchr(mtlLine, '\n')) != NULL)
				* pos = '\0';

			if (strstr(mtlLine, "newmtl ")) {
				currentMaterial = getValue(mtlLine);
				Material mat = Material();
				materials[currentMaterial] = mat;
				materials[currentMaterial].name = currentMaterial;
			} 
			else if (strstr(mtlLine, "map_Kd ") != NULL) {
				// get diffuse path
				materials[currentMaterial].diffuseTextureMapPath = getDirectory(mtlPath) + getFileName(mtlLine);
			} 
			else if (strstr(mtlLine, "map_d ") != NULL) {
				// get alpha path
				materials[currentMaterial].alphaTextureMapPath = getDirectory(mtlPath) + getFileName(mtlLine);
			} 
			else if (strstr(mtlLine, "Ka ")) {
				materials[currentMaterial].ambient = createVector3(mtlLine);
			}
			else if (strstr(mtlLine, "Kd ") != NULL) {
				materials[currentMaterial].diffuse = createVector3(mtlLine);
			}
			else if (strstr(mtlLine, "Ks ") != NULL) {
				materials[currentMaterial].specular = createVector3(mtlLine);
			}
			else if (strstr(mtlLine, "d ") != NULL) {
				materials[currentMaterial].dissolve = stof(getValue(mtlLine));
			}
		}
	}
}

glm::vec3 ObjReader::createVector3(char* line) {

	char* token;
	char* nextToken = nullptr;

	float values[3] { 0, 0, 0 };
	int counter = 0;

	token = strtok_s(line, " ", &nextToken);
	while (token != NULL) {
		if (counter > 0) {
			values[counter - 1] = stof(token);
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	return glm::vec3(values[0], values[1], values[2]);

}

glm::vec2 ObjReader::createVector2(char* line) {
	char* token;
	char* nextToken = nullptr;

	float values[2] { 0, 0 };
	int counter = 0;

	token = strtok_s(line, " ", &nextToken);
	while (token != NULL) {
		if (counter > 0) {
			values[counter - 1] = stof(token);
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	return glm::vec2(values[0], values[1]);

}

std::string ObjReader::getValue(char* line)
{
	char* pos;
	if ((pos = strchr(line, '\n')) != NULL)
		* pos = '\0';

	string lineString = (string)line;
	string duplicate = lineString;

	char* token;
	char* nextToken = nullptr;

	char* values[1]{ 0 };
	int counter = 0;

	token = strtok_s((char*)duplicate.c_str(), " ", &nextToken);
	while (token != NULL) {
		if (counter > 0) {
			values[counter - 1] = token;
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	return (string)values[0];
}

std::string ObjReader::getDirectory(std::string& originalPath) {
	
	string directory;
	const size_t lastSlashIndex = originalPath.rfind('/');
	if (std::string::npos != lastSlashIndex)
	{
		directory = originalPath.substr(0, lastSlashIndex);
	}
	directory += '/';

	return directory;
}

std::string ObjReader::getFileName(char* line)
{
	char* result = strchr(line, ' ');
	*(++result);
	return (string)result;
}
