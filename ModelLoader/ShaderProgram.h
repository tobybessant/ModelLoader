#pragma once

#include <GL\glew.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vertShader, const char* fragShader);
	void use();
	void update();
	GLuint& id();
private:
	GLuint program;
};

