#include "Mesh.h"

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);

	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(cPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Texture]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);

	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(tPosition);
}
