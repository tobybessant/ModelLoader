#include "DaeReader.h"

bool DaeReader::verifyFile(std::string& path)
{
	return false;
}

void DaeReader::parse(std::string& path, Model& model)
{
	FILE* fp;
	errno_t err;
	char  line[255];

	std::map<std::string, Material>materials;

	std::vector<glm::vec3> vertexStore;
	std::vector<glm::vec2> textureStore;
	std::vector<glm::vec3> normalStore;

	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;
	GLuint currentVertexCount = 0;
	GLuint linesRead = 0;

	Object* templateObject = nullptr;
	Mesh* templateMesh = nullptr;

	Model loadedModel = Model();

	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {

	}
}
