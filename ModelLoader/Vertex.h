#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec2 texture_coordinates;
	glm::vec3 normal;

	Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _tex) {
		this->position = _position;
		this->normal = _normal;
		this->texture_coordinates = _tex;
	}
};