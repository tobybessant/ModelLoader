#pragma once

#include "Vertex.h"
#include "Material.h"

#include <vector>
#include <string>
#include <GL\glew.h>

struct MeshConfig {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;
};