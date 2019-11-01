#pragma once
#include <vector>
#include <GL\glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Mesh
{
public:
	Mesh(std::vector<std::vector<GLfloat>> vertices, 
		 std::vector<std::vector<GLuint>> indicies,
		 std::vector<std::vector<GLfloat>> colours,
		 std::vector<std::vector<GLfloat>> texture_coords);
	void render();
	void translate();
	void rotate();
	void scale();
private:
	std::vector<std::vector<GLfloat>> vertices;
	std::vector<std::vector<GLuint>> indicies;
	std::vector<std::vector<GLfloat>> colours;
	std::vector<std::vector<GLfloat>> texture_coords;

	void initBuffers();
	void createTexture();
};

