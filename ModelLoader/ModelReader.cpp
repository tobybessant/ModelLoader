using namespace std;

#include "ModelReader.h"
#include "MeshConfig.h"

MeshConfig ModelReader::parse(const char* path)
{
	string line;
	ifstream file(path);

	if (file.is_open()) {

		MeshConfig configuration;

		vector<GLfloat> vertices;
		vector<GLfloat> texture_coordinates;
		vector<GLfloat> texture_coordinates_mapped;
		vector<GLuint> vertex_indices;
		vector<GLfloat> vertex_normals;
		vector<string> materials;

		while (getline(file, line)) {
			vector<string> result = split(line, ' ');
			string type = result[0];

			if (type == "v") {
				for (unsigned int i = 1; i < result.size(); i = i + 1) {
					std::string::size_type sz;
					GLfloat vertex_float = stof(result[i], &sz);
					vertices.push_back(vertex_float);
				}
			}
			else if (type == "vt") {
				for (unsigned int i = 1; i < result.size(); i = i + 1) {
					std::string::size_type sz;
					GLfloat texture_coord_float = stof(result[i], &sz);
					texture_coordinates.push_back(texture_coord_float);
					cout << texture_coord_float << endl;
				}
			}
			else if (type == "vn") {
				for (unsigned int i = 1; i < result.size(); i = i + 1) {
					std::string::size_type sz;
					GLfloat vertex_normal = stof(result[i], &sz);
					vertex_normals.push_back(vertex_normal);
				}
			}
			else if (type == "f") {
				vector<string> indexes = split(result[1], '/');
				vertex_indices.push_back(stoi(indexes[0]));
				
				indexes = split(result[2], '/');
				vertex_indices.push_back(stoi(indexes[0]));

				indexes = split(result[3], '/');
				vertex_indices.push_back(stoi(indexes[0]));

				indexes = split(result[4], '/');
				vertex_indices.push_back(stoi(indexes[0]));

				indexes = split(result[3], '/');
				vertex_indices.push_back(stoi(indexes[0]));

				indexes = split(result[2], '/');
				vertex_indices.push_back(stoi(indexes[0]));
				/*
				for (unsigned int i = 1; i < result.size(); i = i + 1) {
					vector<string> indexes = split(result[i], '/');
					vertex_indices.push_back(stoi(indexes[0]));
					
					for (unsigned int j = 0; j < 6; j = j + 1) {
						if (j == 0) {
							vertex_indices.push_back(stoi(indexes[j]));
							cout << indexes[j] << endl;
						} 
						else if (j == 2) {
							
							GLuint coordinate_index = stoi(indexes[j]);
							texture_coordinates_mapped.push_back(texture_coordinates[coordinate_index]);
							cout << indexes[j] << " (" << texture_coordinates[coordinate_index] << ") " <<endl;
							
						}
					}
					
				}
				*/
			}
			else if (type == "mtllib") {
				materials.push_back(result[1]);
			}
			// check data parsed correctly
			configuration.vertices = vertices;
			configuration.indices = vertex_indices;
			configuration.texture_coords = texture_coordinates;
			configuration.materials = materials;
		}
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