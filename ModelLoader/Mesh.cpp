#include "Mesh.h"

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

enum VBO { Triangles, Indices, Colours, Texture, BUFFER_COUNT };
enum Attrib_IDs { vPosition, cPosition, tPosition };

GLuint Buffers[BUFFER_COUNT];
GLuint texture1;

Mesh::Mesh(std::vector<std::vector<GLfloat>> _vertices, std::vector<std::vector<GLuint>> _indicies, std::vector<std::vector<GLfloat>> _colours, std::vector<std::vector<GLfloat>> _texture_coords)
{
	vertices = _vertices;
	indicies = _indicies;
	colours = _colours;
	texture_coords = _texture_coords;

	initBuffers();
}

void Mesh::render()
{
}

void Mesh::translate()
{
}

void Mesh::rotate()
{
}

void Mesh::scale()
{
}

void Mesh::initBuffers()
{
	glGenBuffers(BUFFER_COUNT, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), &indicies[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), &colours[0], 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(cPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Texture]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), &texture_coords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(tPosition);
}

void Mesh::createTexture()
{
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("media/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}
