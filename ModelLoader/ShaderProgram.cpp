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
	glClearColor(0.5f, 0.7f, 0.1f, 1.0f);
}

void ShaderProgram::update()
{
	glClear(GL_COLOR_BUFFER_BIT);
}


