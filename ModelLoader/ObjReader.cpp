#include "ObjReader.h"
#include <regex>

using namespace std;

Model ObjReader::parse(const char* &path)
{
	FILE* fp;
	errno_t err;
	char  line[255];

	vector<glm::vec3> vertexStore;
	vector<glm::vec2> textureStore;
	vector<glm::vec3> normalStore;

	vector<Vertex> vertices;
	vector<GLuint> indices;

	GLuint currentVertexCount = 0;

	err = fopen_s(&fp, path, "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, "v ") != NULL) {
				vertexStore.push_back(createVector3(line));
			}

			if (strstr(line, "vt ") != NULL) {
				textureStore.push_back(createVector3(line));
			}

			if (strstr(line, "vn ") != NULL) {
				normalStore.push_back(createVector3(line));
			}

			if (strstr(line, "f ") != NULL) {
				currentVertexCount = vertices.size();
				createFaceVertices(line, &vertexStore, &textureStore, &normalStore, &vertices, &indices, &currentVertexCount);
			}
		}
	}
	return Model();
}

bool ObjReader::verifyFile(const char* path)
{
	return true;
}

glm::vec3 ObjReader::createVector3(char* line) {
	char* token;
	char* nextToken = nullptr;

	float values[3];
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

	float values[2];
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

void ObjReader::createFaceVertices(char* line, vector<glm::vec3>* positions, vector<glm::vec2>* textureCoordinates, vector<glm::vec3>* normals, vector<Vertex>* vertices, vector<GLuint>* indices, GLuint* currentVertexCount)
{
	/*
	vector<Vertex> result;

	char* token;
	char* nextToken = nullptr;

	vector<char*> faceVertices;
	std::string vertex(line);
	int faceVertexCount = 0;
	smatch vertexMatch;
	regex vertexData("\\d+\/\\d+\/\\d+");
	while (regex_search(vertex, vertexMatch, vertexData)) {
		string tempVertexIndices = vertexMatch.str(0);
		faceVertexCount++;
		glm::vec3 v;
		glm::vec2 vt;
		glm::vec3 vn;

		std::string index(vertexMatch.str(0));
		smatch indexMatch;
		regex indexData("\\d+");
		int indexType = 0;
		while (regex_search(index, indexMatch, indexData)) {

			switch (indexType) {
				case 0:
					v = positions->at(stoi(indexMatch.str(0)) - 1);
					break;
				case 1:
					vt = textureCoordinates->at(stoi(indexMatch.str(0)) - 1);
					break;
				case 2:
					vn = normals->at(stoi(indexMatch.str(0)) - 1);
					break;
			}
			indexType++;
			index = indexMatch.suffix().str();
		}

		vertices->push_back(Vertex(v, vn, vt));

		// suffix to find the rest of the string. 
		vertex = vertexMatch.suffix().str();
	}*/
	
	vector<string> tokens;
	string token;
	istringstream tokenStream(line);
	while (getline(tokenStream, token, ' '))
	{
		tokens.push_back(token.c_str());
	}

	for (int i = 1; i < tokens.size(); i++) {
		glm::vec3 v;
		glm::vec2 vt;
		glm::vec3 vn;

		char* index;
		char* nextIndex = nullptr;

		int indices[3];
		int counter = 0;
		char* temp = _strdup(tokens[i].c_str());
		index = strtok_s(_strdup(temp), "/", &nextIndex);
		while (index != NULL) {

			indices[counter] = stoi(index);

			// next token
			counter++;
			index = strtok_s(NULL, "/", &nextIndex);
		}

		v = positions->at(indices[0] - 1);
		vt = textureCoordinates->at(indices[1] - 1);
		vn = normals->at(indices[2] - 1);

		vertices->push_back(Vertex(v, vn, vt));
	}

	if (tokens.size() - 1 == 4) {
		indices->push_back(2 + *currentVertexCount);
		indices->push_back(1 + *currentVertexCount);
		indices->push_back(0 + *currentVertexCount);
		indices->push_back(3 + *currentVertexCount);
		indices->push_back(2 + *currentVertexCount);
		indices->push_back(0 + *currentVertexCount);
	}
	else {
		indices->push_back(2 + *currentVertexCount);
		indices->push_back(1 + *currentVertexCount);
		indices->push_back(0 + *currentVertexCount);
	}

}