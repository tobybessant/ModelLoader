#pragma once
#include "MeshConfig.h"

#include "Vertex.h"

#include <vector>
#include <GL\glew.h>
#include <glm\ext\vector_float3.hpp>
#include <glm\fwd.hpp>

class Mesh
{
public:
	Mesh();
	void setup(MeshConfig& config);
	void init();
	void render(GLuint* _program);
	void translate(glm::vec3 translation);
	void rotate(GLfloat amount, glm::vec3 axis);
	void scaleModel(glm::vec3 scale);

private:
	enum VBO { Vertices, Indices, BUFFER_COUNT };
	enum shaderAtributes { vPosition, nPosition, tPosition };

	unsigned int VAO;
	GLuint Buffers[BUFFER_COUNT];

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material material;

	GLuint texture;

	void initBuffers();
	void createTexture();
};

