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

GLfloat _vertices[][3] = {
	{0.5f,  0.5f, -0.5f},  //0 top right
	{0.5f, -0.5f, -0.5f},  //1 bottom right
	{-0.5f, -0.5f, -0.5f}, //2 bottom left
	{-0.5f,  0.5f, -0.5f},  //3 top left

	{0.5f,  0.5f, 0.5f},  //4 top right
	{0.5f, -0.5f, 0.5f},  //5 bottom right
	{-0.5f, -0.5f, 0.5f}, //6 bottom left
	{-0.5f,  0.5f, 0.5f}  //7 top left 
};


GLuint _indices[][3] = {  // note that we start from 0!
	{0, 3, 1},  // first Triangle front
	{3, 2, 1},   // second Triangle

	{4, 7, 0 },
	{7, 3, 0 },

	{1, 2, 5 },
	{2, 6, 5 },

	{5, 4, 0 },
	{0, 1, 5 },

	{2, 3, 7 },
	{7, 6, 2 },

	{4, 5, 7 },  // first Triangle back
	{7, 5, 6 }   // second Triangle
};

GLfloat  _colours[][4] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f },
};
GLfloat  _texture_coords[] = {
	 1.0f, 1.0f,
	 1.0f, 0.0f,
	 0.0f, 0.0f,
	 0.0f, 1.0f,

	 0.0f, 1.0f,
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 1.0f, 1.0f,
};

Mesh::Mesh()
{
	initBuffers();
	createTexture();
}

void Mesh::render(GLuint* _program)
{
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, sizeof(_vertices), GL_UNSIGNED_INT, 0);

	glUniform1i(glGetUniformLocation(*_program, "texture1"), 0);
	
	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;

	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(*_program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(_colours), _colours, 0);
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(cPosition);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Texture]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_texture_coords), _texture_coords, GL_STATIC_DRAW);
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
