#pragma once
#include "Object.h"
#include "ShaderProgram.h"

class Model
{
public:
	void addObject(Object object);
	void render(GLuint* _program);
private:
	std::vector<Object> objects;
};

