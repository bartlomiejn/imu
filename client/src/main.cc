#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gfx/shader.hh>
#include <gfx/camera.hh>

const unsigned int window_width = 1280;
const unsigned int window_height = 720;

Camera camera(
	glm::vec3(-14.0f, 6.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f), 	// World up vector
	0.0f, 							// Yaw
	0.0f, 							// Pitch
	45.0f); 						// FOV

glm::vec3 lightdir = glm::vec3(8.0f, -20.0f, 4.0f);

DirectionalLight dirlight(
	camera.position(), 				// Look At point
	lightdir,	     				// Reversed direction vector
	glm::vec3(0.2f, 0.2f, 0.2f), 	// Ambient
	glm::vec3(0.5f, 0.5f, 0.5f), 	// Diffuse
	glm::vec3(1.0f, 1.0f, 1.0f));	// Specular)

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
		glViewport(0, 0, window_width, window_height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}
