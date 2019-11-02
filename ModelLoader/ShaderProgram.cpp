#include "ShaderProgram.h"
#include "LoadShaders.h"
#include "GLEWServices.h"

#include <GL\glew.h>
#include <cstddef>

enum VAO { ArrayBuffer = 1 };

const GLint NumVAOs = ArrayBuffer;
GLuint VAOs[NumVAOs];

ShaderProgram::ShaderProgram(const char* vertShader, const char* fragShader)
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[0]);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertShader },
		{ GL_FRAGMENT_SHADER, fragShader },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ShaderProgram::update()
{
	glPointSize(3.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[0]);
}

void ShaderProgram::use() {
	glUseProgram(program);
}

GLuint* ShaderProgram::id() {
	return &program;
}
