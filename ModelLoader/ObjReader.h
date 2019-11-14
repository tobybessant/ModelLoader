#pragma once
#include "Model.h"
#include "Vertex.h"

#include <GL\glew.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

class ObjReader
{
public:
	void parse(std::string &path, Model &model);

private:
	enum indexType { position = 1, texture = 2, normal = 3 };

	glm::vec3 createVector3(char* line);
	glm::vec2 createVector2(char* line);
	std::string getValue(char* line);

	void loadMtl(std::string& mtlPath, std::map<std::string, Material>& materials);
	std::string getDirectory(std::string& originalPath);
	std::string getFileName(char* line);
}; 
