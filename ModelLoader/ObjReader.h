#pragma once
#include "Model.h"
#include "Vertex.h"

#include <GL\glew.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class ObjReader
{
public:
	void parse(const char* &path, Model &model);
	bool verifyFile(const char* path);

private:
	enum indexType { position = 1, texture = 2, normal = 3 };

	glm::vec3 createVector3(char* line);
	glm::vec2 createVector2(char* line);
	void createFaceVertices(char* line, std::vector<glm::vec3>* positions, 
													std::vector<glm::vec2>* textureCoordinates, 
													std::vector<glm::vec3>* normals,
													std::vector<Vertex>* vertices,
													std::vector<GLuint>* indices,
													GLuint* currentVertexCount);
}; 
