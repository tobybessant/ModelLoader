#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm\ext\vector_float3.hpp>
#include <glm\fwd.hpp>

class Mesh
{
public:
	Mesh(std::vector<GLfloat> _vertices,
		 std::vector<GLuint> _indices,
		 std::vector<GLfloat> _colours,
		 std::vector<GLfloat> _texture_coords);
	void render(GLuint* _program);
	void translate(glm::vec3 translation);
	void rotate(GLfloat amount, glm::vec3 axis);
	void scaleModel(glm::vec3 scale);
private:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> colours;
	std::vector<GLfloat> texture_coords;

	void initBuffers();
	void createTexture();
};

