#include "Model.h"
#include <GLFW\glfw3.h>

Model::Model()
{
	// creating the model matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	// creating the view matrix
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
}

void Model::addObject(Object object)
{
	objects.push_back(object);
}

void Model::render(GLuint& _program)
{	
	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;

	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(_program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	for (Object o : objects) {
		o.render(_program);
	}
}

void Model::destroy()
{
	for (Object o : objects) {
		o.destroy();
	}
}

void Model::translate(glm::vec3 translation)
{
	model = glm::translate(model, translation);
}

void Model::rotate(GLfloat amount, glm::vec3 axis)
{
	model = glm::rotate(model, amount, axis);
}

void Model::scale(glm::vec3 scale)
{
	model = glm::scale(model, scale);
}
