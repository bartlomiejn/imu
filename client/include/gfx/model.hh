#ifndef IMU_MODEL_H
#define IMU_MODEL_H

#include <glm/glm.hpp>
#include <gfx/mesh.hh>
#include <gfx/shader.hh>
#include <gfx/material.hh>
#include <gfx/context.hh>

class Model
{
public:
	Mesh *mesh;
	Shader *shader;
	Material *material;
	glm::vec3 translation;
	glm::vec3 rotation_axis;
	float rotation_angle_rad;
	glm::vec3 scale;
	
	Model(Mesh *mesh,
		Shader *shader,
		Material *material,
		glm::vec3 translation,
		glm::vec3 rotation_axis = glm::vec3(0.0f, 0.1f, 0.0f),
		float rotation_angle_rad = 0.0f,
		glm::vec3 scale = glm::vec3(1.0f));
	
	glm::mat4 model_matrix() const;
	
	/// Draw in a draw objects context.
	/// \param ctx
	void draw(DrawObjectsContext &ctx) const;
	
	/// Draw overriding the stored shader. Used in depth map pass.
	/// \param shader
	void draw(Shader &override_shader) const;
};

#endif //IMU_MODEL_H
