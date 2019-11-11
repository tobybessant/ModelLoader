#include "Model.h"

void Model::addObject(Object object)
{
	objects.push_back(object);
}

void Model::render(GLuint* _program)
{
	for (Object o : objects) {
		o.render(_program);
	}
}

void Model::init()
{
	for (Object o : objects) {
		o.init();
	}
}
