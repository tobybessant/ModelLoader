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
			if (strstr(line, "mtlib ")) {

			}
			if (strstr(line, "o ")) {
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
					// TODO: fetch and add material
					templateMesh->setIndices(indices);
					templateMesh->setVertexes(vertices);
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
					indices.push_back(2 + currentVertexCount);
					indices.push_back(1 + currentVertexCount);
					indices.push_back(0 + currentVertexCount);
					indices.push_back(3 + currentVertexCount);
					indices.push_back(2 + currentVertexCount);
					indices.push_back(0 + currentVertexCount);

					currentVertexCount = currentVertexCount + 4;
				}
				else {
					indices.push_back(2 + currentVertexCount);
					indices.push_back(1 + currentVertexCount);
					indices.push_back(0 + currentVertexCount);

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

bool ObjReader::verifyFile(const char* path)
{
	return true;
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
