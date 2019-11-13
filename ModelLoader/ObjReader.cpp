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

	Object* templateObject = nullptr;
	Mesh* templateMesh = nullptr;

	Model loadedModel = Model();

	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
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
					//TODO extract material name
					//templateMesh->setMaterial(materials[0]);
					templateMesh->init();
					templateObject->addMesh(*templateMesh);
					currentVertexCount = 0;
				}

				// wipe 
				Mesh newMesh = Mesh();
				templateMesh = &newMesh;

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

						Vertex newV = Vertex(v, vn, vt);
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
				// TODO: check the value the line pointer points to is not being modified / make sure to duplicate value.
				if (tempMaterial != nullptr) {
					string key = tempMaterial->name;
					materials[key] = *tempMaterial;
				}

				Material mat = Material();
				mat.name = getValue(mtlLine);
				tempMaterial = &mat;
			}

			if (strstr(mtlLine, "Ka ")) {
				tempMaterial->ambient = createVector3(mtlLine);
			}

			if (strstr(mtlLine, "Kd ") != NULL) {
				tempMaterial->diffuse = createVector3(mtlLine);
			}

			if (strstr(mtlLine, "Ks ") != NULL) {
				tempMaterial->specular = createVector3(mtlLine);
			}

			if (strstr(mtlLine, "d ") != NULL) {
				tempMaterial->dissolve = stof(getValue(mtlLine));
			}

			if (strstr(mtlLine, "map_d ") != NULL) {
				// get alpha path
				tempMaterial->alphaTextureMapPath = getDirectory(mtlPath) + getFileName(mtlLine);
			}

			if (strstr(mtlLine, "map_Kd ") != NULL) {
				// get diffuse path
				tempMaterial->diffuseTextureMapPath = getDirectory(mtlPath) + getFileName(mtlLine);
			}
		}
	}

	if (tempMaterial != nullptr) {
		materials[tempMaterial->name] = *tempMaterial;
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

char* ObjReader::getValue(char* line)
{
	int length = strlen(line);
	char dupLine[length];
	strcpy();
	char* token;
	char* nextToken = nullptr;

	char* values[1]{ 0 };
	int counter = 0;

	token = strtok_s(dupLine, " ", &nextToken);
	while (token != NULL) {
		if (counter > 0) {
			values[counter - 1] = token;
		}
		// next token
		counter++;
		token = strtok_s(NULL, " ", &nextToken);
	}

	return values[0];
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
