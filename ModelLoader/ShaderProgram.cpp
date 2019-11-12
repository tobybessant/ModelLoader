#include "ShaderProgram.h"
#include "LoadShaders.h"
#include "GLEWServices.h"

#include <GL\glew.h>
#include <cstddef>

ShaderProgram::ShaderProgram(const char* vertShader, const char* fragShader)
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertShader },
		{ GL_FRAGMENT_SHADER, fragShader },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glClearColor(0.2f, 0.0f, 0.2f, 0.2f);
}

void ShaderProgram::update()
{
	//glPointSize(3.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ShaderProgram::use() {
	glUseProgram(program);
}

GLuint* ShaderProgram::id() {
	return &program;
}
