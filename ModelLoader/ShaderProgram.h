#pragma once

#include <GL\glew.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vertShader, const char* fragShader);
	void use();
	void update();
private:
	GLuint program;
	float *backgroundColour[4];
};

