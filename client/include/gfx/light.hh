#ifndef IMU_LIGHT_H
#define IMU_LIGHT_H

#include <glm/glm.hpp>

class DirectionalLight
{
public:
	glm::vec3 look_at;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
	DirectionalLight(
		glm::vec3 look_at,
		glm::vec3 dir,
		glm::vec3 amb,
		glm::vec3 diff,
		glm::vec3 spec
	):
		look_at(look_at),
		direction(dir),
		ambient(amb),
		diffuse(diff),
		specular(spec)
	{};
};

#endif //IMU_LIGHT_H
