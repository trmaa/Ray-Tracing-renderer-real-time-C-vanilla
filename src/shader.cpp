#include "./window.hpp"
#include "./camera.hpp"
#include "./scene.hpp"
#include <SFML/Graphics/Color.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int2.hpp>

sf::Color shader(int* x, int* y, glm::ivec2* buff_v, Camera* cam, Scene* scn, sf::Color* lastCol) 
{

const int index = *x + *y * buff_v->x;

glm::vec3 sc = *scn->sky_color();
sf::Color col = sf::Color(sc.r*255, sc.g*255, sc.b*255);
Ray* ray = cam->ray(index);
cam->cast(x, y, buff_v);

int bounces = 10;
for (int i = 0; i < bounces; i++) {
	std::vector<float> times;
	for (int j = 0; j < scn->sphere()->size(); j++) {
		times.push_back(scn->sphere(j)->checkCollision(ray));
	}
	auto t = std::min_element(times.begin(), times.end(), [](float a, float b){
		return std::abs(a) < std::abs(b); 
	}); //t is a pointer
	int id = std::distance(times.begin(), t);
	Sphere* sphere = scn->sphere(id);

	if (*t <= 0) {
		if (i < 1) {
			return col;
		}
		sf::Color lc = *lastCol; 
		glm::vec3 nc = sc * glm::vec3(lc.r, lc.g, lc.b);
		col = sf::Color(nc.r, nc.g, nc.b);
		return col;
	}

	col = *sphere->material.col();
	 
	glm::vec3 hitPoint = ray->f(*t);
	glm::vec3 normal = glm::normalize(hitPoint - sphere->center);

	glm::vec3 reflected_direction = ray->direction - 2.0f * glm::dot(ray->direction, normal) * normal;

	//diffusion, (bright, importanc)

	ray->origin = hitPoint + normal*0.001f;
	ray->direction = reflected_direction;
	
	*lastCol = col;
}    
return col;

}
