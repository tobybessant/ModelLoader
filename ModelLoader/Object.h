#pragma once
#include "Mesh.h"

class Object
{
public:
	void setName(std::string &_name);
	
	void addMesh(Mesh mesh);
	void render(GLuint* _program);

private:
	std::string name;
	std::vector<Mesh> meshes;
};

