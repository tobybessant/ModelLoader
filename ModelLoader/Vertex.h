#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>

// vertex struct to aggregate vertex attrributes
struct Vertex {
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
	glm::vec4 colour;

	Vertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _tex, glm::vec4 _colour) {
		this->position = _position;
		this->normal = _normal;
		this->texture = _tex;
		this->colour = _colour;
	}
};