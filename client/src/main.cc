#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gfx/shader.hh>
#include <gfx/camera.hh>
#include <gfx/model.hh>
#include <gfx/draw_pass.hh>

const unsigned int window_width = 1280;
const unsigned int window_height = 720;

glm::vec3 camerapos = glm::vec3(-14.0f, 6.0f, 0.0f);
glm::vec3 lightdir = glm::vec3(8.0f, -20.0f, 4.0f);

Camera camera(
	camerapos,
	glm::vec3(0.0f, 1.0f, 0.0f), 	// World up vector
	0.0f, 							// Yaw
	0.0f, 							// Pitch
	45.0f); 						// FOV

DirectionalLight dirlight(
	camera.position(), 				// Look At point
	lightdir,	     				// Reversed direction vector
	glm::vec3(0.2f, 0.2f, 0.2f), 	// Ambient
	glm::vec3(0.5f, 0.5f, 0.5f), 	// Diffuse
	glm::vec3(1.0f, 1.0f, 1.0f));	// Specular)

MaterialShader mtl_shader("vertex.glsl");

std::vector<std::shared_ptr<Model>> models;
DrawRenderPass draw_pass(
	dirlight,
	mtl_shader,
	camera,
	window_width,
	window_height);

float delta_time = 0.0f; /// Time between current frame and last frame
float last_frame = 0.0f; /// Time of last frame

float mouse_last_x = window_width / 2;
float mouse_last_y = window_height / 2;
float mouse_sensitivity = 0.05f;

float movement_speed = 5.0f;

void
on_framebuffer_resize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	GLFWwindow* window = glfwCreateWindow(
		window_width, window_height, "IMU Client", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, on_framebuffer_resize);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}
	
	std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION)
		  << std::endl;
	
	while (!glfwWindowShouldClose(window)) 
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}
