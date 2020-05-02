#ifndef IMU_DRAW_PASS_H
#define IMU_DRAW_PASS_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gfx/light.hh>
#include <gfx/shader.hh>
#include <gfx/framebuffer.hh>
#include <gfx/model.hh>
#include <gfx/camera.hh>
#include <gfx/texture.hh>
#include <gfx/depth_pass.hh>
#include <gfx/context.hh>

class DrawRenderPass
{
public:
	DirectionalLight &light;
	MaterialShader &shader;
	Camera &camera;
	DepthRenderPass &depth_pass;
	unsigned int width;
	unsigned int height;
	float near_plane = 0.1f;
	float far_plane = 100.0f;
	
	DrawRenderPass(
		DirectionalLight &light,
		MaterialShader &shader,
		Camera &camera,
		DepthRenderPass &depth_pass,
		unsigned int width,
		unsigned int height
	) :
		light(light),
		shader(shader),
		camera(camera),
		depth_pass(depth_pass),
		width(width),
		height(height)
	{}
	
	/// Draws a depth map for provided models.
	///
	/// \tparam Container Container type
	/// \param models Container of `std::reference_wrapper` of `Model`s
	template <typename Container> void
	draw(Container models)
	{
		glViewport(0, 0, width, height);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);
		
		// Generate view matrix
		glm::mat4 view = camera.view_matrix();
		
		// Generate perspective projection
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.fov()),
			(float)width / (float)height,
			0.1f,
			255.0f);
		
		DrawObjectsContext ctx(
			view,
			projection,
			depth_pass.light_view_projection(),
			camera.position(),
			light,
			0,
			1,
			2,
			depth_pass.depth_texture());
		
		for (const auto &model : models)
			model->draw(ctx);
	}
};

#endif //IMU_DRAW_PASS_H
