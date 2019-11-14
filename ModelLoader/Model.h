#pragma once
#include "Object.h"
#include "ShaderProgram.h"

#include <glm\ext\vector_float3.hpp>
#include <glm\fwd.hpp>

class Model
{
public:
	void addObject(Object object);
	void render(GLuint& _program);

	void init();
private:
	std::vector<Object> objects;
};

