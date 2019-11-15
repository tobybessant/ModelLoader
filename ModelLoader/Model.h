#pragma once
#include "Object.h"
#include "ShaderProgram.h"

#include <glm\ext\vector_float3.hpp>
#include <glm\fwd.hpp>

class Model
{
public:
	Model();

	void addObject(Object object);
	void render(GLuint& _program);
	void destroy();

	void translate(glm::vec3 translation);
	void rotate(GLfloat amount, glm::vec3 axis);
	void scale(glm::vec3 scale);
private:
	std::vector<Object> objects;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 900.0f);
};

