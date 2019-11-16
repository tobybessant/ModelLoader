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
	~Mesh();

	void setup(MeshConfig& config);
	void setIndices(std::vector<GLuint> i);
	void setVertexes(std::vector<Vertex> v);
	void setMaterial(Material& mat);

	glm::vec3 getMaterialColour();

	void init();
	void render(GLuint& _program);
	void destroy();
private:
	enum VBO { Vertices, Indices, BUFFER_COUNT };
	enum shaderAtributes { vPosition, nPosition, tPosition, cPosition };

	GLuint Buffers[BUFFER_COUNT];

	GLuint VAO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	
	Material material;
	GLuint texture;

	GLfloat offset = 0.0f;

	void initBuffers();
	void createTexture();
};
