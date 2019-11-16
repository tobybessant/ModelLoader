#include "Mesh.h"

#include "MeshConfig.h"

#include "Vertex.h"

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

Mesh::Mesh() 
{
}

void Mesh::setup(MeshConfig& _config)
{
	vertices = _config.vertices;
	indices = _config.indices;
	material = _config.material;
}

void Mesh::init()
{
	initBuffers();
	createTexture();
}

void Mesh::render(GLuint &_program)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glUniform1i(glGetUniformLocation(_program, "texture1"), 0);

	//glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
}

void Mesh::destroy()
{
	glBindVertexArray(VAO);
	glDeleteBuffers(1, &Buffers[Vertices]);
	glDeleteBuffers(1, &Buffers[Indices]);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::setIndices(std::vector<GLuint> i)
{
	indices = i;
}

void Mesh::setVertexes(std::vector<Vertex> v)
{
	vertices = v;
}

void Mesh::setMaterial(Material &mat)
{
	material = mat;
}

glm::vec3 Mesh::getMaterialColour()
{
	return material.diffuse;
}

void Mesh::initBuffers()
{
	// create and bind VAO buffers
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate VBO / EBO
	glGenBuffers(BUFFER_COUNT, Buffers);

	// load vertex data into buffers
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// bind and load EBO data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// tell opengl how to parse vertex data
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, position)));
	glEnableVertexAttribArray(vPosition);
	
	glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(nPosition);

	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, texture)));
	glEnableVertexAttribArray(tPosition);

	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(offsetof(Vertex, colour)));
	glEnableVertexAttribArray(cPosition);
	
	//glBindVertexArray(0);
}

void Mesh::createTexture()
{
	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	
	if (!material.diffuseTextureMapPath.empty()) {
		GLint width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char* data = stbi_load(material.diffuseTextureMapPath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}
	else {
		GLubyte texData[] = { 255, 255, 255, 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	}
}
