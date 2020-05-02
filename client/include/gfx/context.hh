#ifndef IMU_CONTEXT_H
#define IMU_CONTEXT_H

#include <glm/glm.hpp>
#include <gfx/light.hh>
#include <gfx/texture.hh>

struct DrawObjectsContext
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 view_pos;
	DirectionalLight &light;
	unsigned int diff_slot;
	unsigned int spec_slot;
	
	DrawObjectsContext(
		glm::mat4 view,
		glm::mat4 projection,
		glm::vec3 view_pos,
		DirectionalLight &light,
		unsigned int diff_slot,
		unsigned int spec_slot
	) :
		view(view),
		projection(projection),
		view_pos(view_pos),
		light(light),
		diff_slot(diff_slot),
		spec_slot(spec_slot)
	{}
};

#endif //IMU_CONTEXT_H
