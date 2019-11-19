#pragma once
#include <string>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GL\glew.h>

// material struct to contain material details
struct Material {
	std::string name;
	glm::vec3 ambient; // Ka
	glm::vec3 diffuse; // Kd
	glm::vec3 specular; // Ks
	GLfloat specularExponent; // Ks
	GLfloat dissolve; // d
	std::string alphaTextureMapPath; // map_d
	std::string diffuseTextureMapPath; // map_Kd 
};