#pragma once

#include <GL\glew.h>

class ShaderProgram
{
public:
	GLuint program;
	ShaderProgram(const char* vertShader, const char* fragShader);
	void update();
	void setBackgroundColour(float colour[]);
private:
	float *backgroundColour[4];
};

