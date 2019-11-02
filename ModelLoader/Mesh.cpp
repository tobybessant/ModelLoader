#include "Mesh.h"

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp> //includes GLM
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define BUFFER_OFFSET(a) ((void*)(a))

enum VBO { Triangles, Indices, Colours, Texture, BUFFER_COUNT };
enum Attrib_IDs { vPosition, cPosition, tPosition };

GLuint Buffers[BUFFER_COUNT];
GLuint texture1;

Mesh::Mesh(std::vector<GLfloat> _vertices, std::vector<GLuint> _indices, std::vector<GLfloat> _colours, std::vector<GLfloat> _texture_coords) 
{
	vertices = _vertices;
	indices = _indices;
	colours = _colours;
	texture_coords = _texture_coords;

	initBuffers();
	createTexture();
}

void Mesh::render(GLuint* _program)
{
	glBindTexture(GL_TEXTURE_2D, texture1);
	GLuint numVertices = indices.size();
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);

	glUniform1i(glGetUniformLocation(*_program, "texture1"), 0);

	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;

	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(*_program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
}

void Mesh::translate(glm::vec3 translation)
{
}

void Mesh::rotate(GLfloat amount, glm::vec3 axis)
{
}

void Mesh::scaleModel(glm::vec3 scale)
{
}

void Mesh::initBuffers()
{
	glGenBuffers(BUFFER_COUNT, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours[0]) * colours.size(), &colours[0], 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(cPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Texture]);
	glBufferData(GL_ARRAY_BUFFER, texture_coords.size() * sizeof(GLfloat), &texture_coords[0], GL_STATIC_DRAW);
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
