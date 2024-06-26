#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
 
struct Material {
	glm::vec3 color;
	float emission;
	float roughness;

	sf::Color* col() { return new sf::Color(this->color.r, this->color.g, this->color.b); }
	
	Material(glm::vec3 c): color(c) {}
	~Material() = default;
};

#endif
