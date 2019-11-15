#pragma once
#include "Mesh.h"

class Object
{
public:
	void setName(char* &_name);
	
	void addMesh(Mesh mesh);
	void render(GLuint& _program);

	void init();
	void destroy();

private:
	char* name;
	std::vector<Mesh> meshes;
};

