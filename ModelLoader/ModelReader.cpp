using namespace std;
#include <glm/glm.hpp> //includes GLM
#include "ModelReader.h"
#include "MeshConfig.h"
#include "Vertex.h"

MeshConfig ModelReader::parse(const char* path)
{
	string line;
	ifstream file(path);

	GLint indexCount = 0;

	if (file.is_open()) {

		MeshConfig configuration;
		vector<Vertex> vertices;

		vector<GLuint> vertexIndices;
		int indiceTracker = 0;
		int currentVertexCount = 0;

		vector<glm::vec3> vertex_positions;
		vector<glm::vec3> vertex_normals;
		vector<glm::vec2> vertex_texture_coords;

		vector<string> materials;

		while (getline(file, line)) {
			vector<string> result = split(line, ' ');
			string type = result[0];

			if (type == "v") {
				glm::vec3 tempVector;
				std::string::size_type sz;

				tempVector.x = stof(result[1], &sz);
				tempVector.y = stof(result[2], &sz);
				tempVector.z = stof(result[3], &sz);

				vertex_positions.push_back(tempVector);
			}
			else if (type == "vt") {
				glm::vec2 tempTexture;
				std::string::size_type sz;

				tempTexture.x = stof(result[1], &sz);
				tempTexture.y = stof(result[2], &sz);
				
				vertex_texture_coords.push_back(tempTexture);
			}
			else if (type == "vn") {
				glm::vec3 tempNormal;
				std::string::size_type sz;

				tempNormal.x = stof(result[1], &sz);
				tempNormal.y = stof(result[2], &sz);
				tempNormal.z = stof(result[3], &sz);

				vertex_normals.push_back(tempNormal);
			}
			else if (type == "f") {

				vector<string> indexValues;

				for (unsigned int i = 1; i < result.size(); i = i + 1) {

					indexValues = split(result[i], '/');
					
					GLint vIndex = stoi(indexValues[0]) - 1;
					GLint vtIndex = stoi(indexValues[1]) - 1;
					GLint vnIndex = stoi(indexValues[2]) - 1;

					glm::vec3 vPos = vertex_positions[vIndex];
					glm::vec2 vtPos = vertex_texture_coords[vtIndex];
					glm::vec3 vnPos = vertex_normals[vnIndex];

					Vertex v(vPos, vnPos, vtPos);

					vertices.push_back(v);
					indiceTracker++;

				}
				// if quad, add indices respective of 2 triangles
				if (result.size() > 4) {
					vertexIndices.push_back(2 + currentVertexCount);
					vertexIndices.push_back(1 + currentVertexCount);
					vertexIndices.push_back(0 + currentVertexCount);
					vertexIndices.push_back(3 + currentVertexCount);
					vertexIndices.push_back(2 + currentVertexCount);
					vertexIndices.push_back(0 + currentVertexCount);
				}
				else {
					vertexIndices.push_back(2 + currentVertexCount);
					vertexIndices.push_back(1 + currentVertexCount);
					vertexIndices.push_back(0 + currentVertexCount);
				}
				currentVertexCount += indiceTracker;
				indiceTracker = 0;
			}
		}
		configuration.indices = vertexIndices;
		configuration.vertices = vertices;

		file.close();
		return configuration;
	}
}

vector<string> ModelReader::split(const string& s, char delimiter)
{
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}