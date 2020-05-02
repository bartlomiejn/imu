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
	unsigned int width;
	unsigned int height;
	float near_plane = 0.1f;
	float far_plane = 100.0f;
	
	DrawRenderPass(
		DirectionalLight &light,
		MaterialShader &shader,
		Camera &camera,
		unsigned int width,
		unsigned int height
	) :
		light(light),
		shader(shader),
		camera(camera),
		width(width),
		height(height)
	{}
	
	template <typename Container> void
	draw(Container models)
	{
        draw_setup();

		DrawObjectsContext ctx = generate_context();
		
		for (const auto &model : models)
			model->draw(ctx);
	}

    void
    draw(std::shared_ptr<Model> model)
    {
        draw_setup();

        DrawObjectsContext ctx = generate_context();

        model->draw(ctx);
    }

private:
    void
    draw_setup()
    {
        glViewport(0, 0, width, height);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);
    }

    DrawObjectsContext
    generate_context()
    {
        // Generate view matrix
		glm::mat4 view = camera.view_matrix();
		
		// Generate perspective projection
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.fov()),
			(float)width / (float)height,
			near_plane,
			far_plane);
		
        return DrawObjectsContext(
			view,
			projection,
			camera.position(),
			light,
			0,
			1);
    }
};

#endif //IMU_DRAW_PASS_H
