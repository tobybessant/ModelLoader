#pragma once

#include "Vertex.h"

#include <vector>
#include <string>
#include <GL\glew.h>

struct MeshConfig {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<std::string> materials;
};