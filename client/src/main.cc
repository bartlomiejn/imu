#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gfx/shader.hh>
#include <gfx/camera.hh>
#include <gfx/model.hh>
#include <gfx/draw_pass.hh>
#include <gfx/cube.hh>

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
	glm::vec3(1.0f, 1.0f, 1.0f));	// Specular

float shininess = 128.0f;
Image diff("assets/diff.jpg");
Image spec("assets/spec.jpg");
Texture difftex(&diff, layout_rgb, filter_linear);
Texture spectex(&spec, layout_rgb, filter_linear);
Material mtl(&difftex, &spectex, shininess);

MaterialShader mtl_shader("vertex.glsl");

Cube cubemesh(1.0f, 1.0f, 1.0f);
std::shared_ptr<Model> cube;

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
bool mouse_is_left_pressed = false;

float movement_speed = 5.0f;

void
on_framebuffer_resize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
on_mouse_movement(GLFWwindow *window, double x_pos, double y_pos)
{
	static bool first_call = true;

	if (first_call)
	{
		mouse_last_x = x_pos;
		mouse_last_y = y_pos;
		first_call = false;
	}
	
	if (mouse_is_left_pressed)
	{
		float x_offset = x_pos - mouse_last_x;
		float y_offset = mouse_last_y - y_pos;

		mouse_last_x = x_pos;
		mouse_last_y = y_pos;
		
		x_offset *= mouse_sensitivity;
		y_offset *= mouse_sensitivity;
		
		camera.rotate(x_offset, y_offset);
	}
}

void
on_mouse_event(GLFWwindow* window, int button, int action, int mods)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;

	if (action == GLFW_PRESS)
	{
		mouse_is_left_pressed = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
    else if (action == GLFW_RELEASE)
	{
		mouse_is_left_pressed = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} 
}

void
handle_keyboard_input(GLFWwindow *window)
{
	float per_frame_speed = movement_speed * delta_time;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(direction_forward, per_frame_speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(direction_backward, per_frame_speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(direction_left, per_frame_speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(direction_right, per_frame_speed);
}

void 
update_dirlight()
{
	dirlight.look_at = camera.position();
}

int 
main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	// Create window and make current
	GLFWwindow* window = glfwCreateWindow(
		window_width, window_height, "IMU Client", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Resize window to initial size
	glViewport(0, 0, window_width, window_height);
	
	// Set callbacks for mouse/keyboard input
	glfwSetFramebufferSizeCallback(window, on_framebuffer_resize);
	glfwSetCursorPosCallback(window, on_mouse_movement);
	glfwSetMouseButtonCallback(window, on_mouse_event);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		glfwTerminate();
		return -1;
	}
	
	std::cout << "OpenGL version: " << (char*)glGetString(GL_VERSION)
		  << std::endl;

	// Initialize all assets
	try
	{
		mtl_shader.try_create_and_link();
		diff.try_load();
		spec.try_load();
		difftex.load();
		spectex.load();
		cubemesh.load();
	}
	catch (ShaderCompileFailure &err)
	{
		std::cout << "Shader compilation failure." << std::endl;
		glfwTerminate();
		return -1;
	}
	catch (ShaderLinkFailure &err)
	{
		std::cout << "Shader link failure." << std::endl;
		glfwTerminate();
		return -1;
	}
	catch (ImageLoadFailure &err)
	{
		std::cout << "Image loading failure: " << err.filename
			  << std::endl;
		glfwTerminate();
		return -1;
	}
	catch (FramebufferGenerationFailure &err)
	{
		std::cout << "Framebuffer generation failure." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Generate cube model
	glm::vec3 pos(0.0f, 0.0f, 0.0f);
	cube = std::make_shared<Model>(&cubemesh, &mtl_shader, &mtl, pos);
	
	// Main loop
	while (!glfwWindowShouldClose(window)) 
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		handle_keyboard_input(window);
		update_dirlight();

		draw_pass.draw(cube);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}
