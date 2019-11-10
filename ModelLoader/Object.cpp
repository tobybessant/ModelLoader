#include "Object.h"

void Object::setName(std::string& _name)
{
	name = _name;
}

void Object::addMesh(Mesh mesh)
{
	meshes.push_back(mesh);
}

void Object::render(GLuint* _program)
{
	for (Mesh m : meshes) {
		m.render(_program);
	}
}
