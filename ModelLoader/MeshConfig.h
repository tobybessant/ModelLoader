#pragma once

#include <vector>
#include <string>
#include <GL\glew.h>

struct MeshConfig {
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> colours;
	std::vector<GLfloat> texture_coords;
	std::vector<std::string> materials;
};