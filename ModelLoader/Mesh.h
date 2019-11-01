#pragma once
#include <vector>
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	void render(GLuint* _program);
	void translate();
	void rotate();
	void scale();
private:
	std::vector<std::vector<GLfloat>> vertices;
	std::vector<std::vector<GLuint>> indicies;
	std::vector<std::vector<GLfloat>> colours;
	std::vector<GLfloat> texture_coords;

	void initBuffers();
	void createTexture();
};

