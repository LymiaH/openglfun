#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void onWindowResize(GLFWwindow* window, int width, int height)
{
	// Update the viewport mapping
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello You~", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Makes the created window the context for the current thread
	glfwMakeContextCurrent(window);

	// Load OpenGL functions for the OS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set Window Size
	// This maps the default range of -1 to 1 for the opengl view to be in window coordinates instead
	glViewport(0, 0, 800, 600);

	// Register handler for when window is resized to update the viewport
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	// Main render loop
	while (!glfwWindowShouldClose(window))
	{
		// Process inputs
		processInput(window);

		// Clear the frame buffer by filling it with a colour
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render Stuff goes here

		// Display what was rendered in the current loop
		glfwSwapBuffers(window);

		// Check for events that have been raised and runs the callbacks
		glfwPollEvents();
	}

	// Clean up
	//glfwDestroyWindow(window); // glfwTerminate() should destroy all window so this isn't really needed
	glfwTerminate();
	return 0;
}
