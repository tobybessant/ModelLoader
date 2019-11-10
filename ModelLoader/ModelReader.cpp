using namespace std;

#include <glm/glm.hpp> //includes GLM
#include "ModelReader.h"
#include "MeshConfig.h"
#include "Vertex.h"
#include <map>
#include "Material.h"

bool ModelReader::verifyFile(std::string& path)
{
	ifstream f(path.c_str());
	return f.good();
}

MeshConfig ModelReader::parse(string& path)
{
	vector<string> pathComponents = split(path, '/');
	string fileName = pathComponents[pathComponents.size() - 1];
	pathComponents.pop_back();

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

		map<string, Material>materials;

		while (getline(file, line)) {
			vector<string> result = split(line, ' ');
			string type = result[0];
			if (type == "mtllib") {
				pathComponents.push_back(result[1]);
				string mtlPath;
				for (unsigned int i = 0; i < pathComponents.size(); i++) {
					mtlPath = mtlPath + pathComponents[i] + (i != pathComponents.size() - 1 ? '/' : '\0');
				}

				string mtlLine;
				ifstream mtlFile(mtlPath);

				string currentMaterial = "";

				while (getline(mtlFile, mtlLine)) {
					vector<string> mtlResult = split(mtlLine, ' ');
					if (mtlResult.size() > 0) {
						string mtlType = mtlResult[0];
						if (mtlType == "newmtl") {
							currentMaterial = mtlResult[1];
							Material mat = Material();
							materials[currentMaterial] = mat;
							materials[currentMaterial].name = currentMaterial;
						}
						else if (mtlType == "Ka") {
							std::string::size_type sz;
							materials[currentMaterial].ambient = glm::vec3(stof(mtlResult[1], &sz), stof(mtlResult[2], &sz), stof(mtlResult[3], &sz));
						}
						else if (mtlType == "Kd") {
							std::string::size_type sz;
							materials[currentMaterial].diffuse = glm::vec3(stof(mtlResult[1], &sz), stof(mtlResult[2], &sz), stof(mtlResult[3], &sz));
						}
						else if (mtlType == "Ks") {
							std::string::size_type sz;
							materials[currentMaterial].specular = glm::vec3(stof(mtlResult[1], &sz), stof(mtlResult[2], &sz), stof(mtlResult[3], &sz));
						}
						else if (mtlType == "d") {
							std::string::size_type sz;
							materials[currentMaterial].dissolve = stof(mtlResult[1], &sz);
						}
						else if (mtlType == "map_d") {
							string alphaPath = "";
							for (unsigned int i = 0; i < pathComponents.size() - 1; i++) {
								alphaPath = alphaPath + pathComponents[i] + '/';
							}
							materials[currentMaterial].alphaTextureMapPath = alphaPath + mtlResult[1];
						}
						else if (mtlType == "map_Kd") {
							string diffusePath = "";
							for (unsigned int i = 0; i < pathComponents.size() - 1; i++) {
								diffusePath = diffusePath + pathComponents[i] + '/';
							}
							materials[currentMaterial].diffuseTextureMapPath = diffusePath + mtlResult[1];
						}
					}
				}
			}
			else if (type == "v") {
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

