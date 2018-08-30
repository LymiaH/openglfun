#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// "method C++" from: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	else
	{
		return "";
	}
}

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

GLuint makeShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success;
	std::string infoLog;
	int length;

	// Create Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderString = get_file_contents(vertexShaderPath);
	const GLchar* vertexShaderSource = vertexShaderString.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		infoLog.resize(length);
		glGetShaderInfoLog(vertexShader, length, NULL, &infoLog[0]);
		std::cerr << "Vertex Shader failed to compile!" << std::endl << infoLog << std::endl;
	}

	// Create Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderString = get_file_contents(fragmentShaderPath);
	const GLchar* fragmentShaderSource = fragmentShaderString.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		infoLog.resize(length);
		glGetShaderInfoLog(fragmentShader, length, NULL, &infoLog[0]);
		std::cerr << "Fragment Shader failed to compile!" << std::endl << infoLog << std::endl;
	}

	// Combine the Vertex Shader and Fragment Shader into a Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	// Check for linking errors
	if (!success) {
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
		infoLog.resize(length);
		glGetProgramInfoLog(shaderProgram, length, NULL, &infoLog[0]);
		std::cerr << "Shader Program linking failed!" << std::endl << infoLog << std::endl;
	}

	//Delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void setupHelloTriangle(GLuint &shaderProgram, GLuint &VAO) {
	// Load Shader Program
	shaderProgram = makeShaderProgram("./shaders/triangle.vert", "./shaders/triangle.frag");

	// Make and bind a Vertex Array Object to store vertex attribute state changes
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Each row is the coordinate for a corner of the triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // Bottom Left
		 0.5f, -0.5f, 0.0f, // Bottom Right
		 0.0f,  0.5f, 0.0f  // Top Centre
	};
	// The ordering matters anti clockwise means the rendered face is towards you, clockwise means it's away.

	// These verticies need to be sent to the graphics card, the way this is done is through a Vertex Buffer Object (VBO)
	GLuint VBO;
	// Create a single buffer and save the id
	glGenBuffers(1, &VBO);
	// Bind the VBO for future operations, VBOs are type GL_ARRAY_BUFFER
	// You can operate on each type of buffer simultaneously
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the vertices into the VBO (currently bound array buffer)
	// The last parameter informs the graphics card the frequency of changes to the data and how it will be used
	// Refer to: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml#description
	/* The frequency of access may be one of these:
		STREAM  - The data store contents will be modified once and used at most a few times.
		STATIC  - The data store contents will be modified once and used many times.
		DYNAMIC - The data store contents will be modified repeatedly and used many times.
	*/
	/* The nature of access may be one of these:
		DRAW	- The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
		READ	- The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
		COPY	- The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Let the program know where to link the data (to the currently bound GL_ARRAY_BUFFER):
	// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
	// 0					- location for the shader program to reference
	// 3					- number of verticies
	// FL_FLOAT				- type of data
	// GL_FALSE				- if this was true it would convert the data to be [-1, 1] or [0, 1] for signed and unsigned data types respecively
	// 3 * sizeof(float)	- size of each vertex (you can actually leave this at 0 if it is tightly packed)
	// (void*)0				- Used for the offset from the start of the GL_ARRAY_BUFFER. Requires a cast since the method definition
	//						  didn't change from when it used to be the address of the buffer in memory.
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind the VAO to stop storing the state changes
	glBindVertexArray(0);
	
	// Unbind the buffer AFTER, so it remains bound when you restore it with the VAO? Not sure if this is needed.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderHelloTriangle(GLuint &shaderProgram, GLuint &VAO)
{
	// Use the Shader Program
	glUseProgram(shaderProgram);
	// Restore vertex attribute state using VBO
	glBindVertexArray(VAO);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VAO
	glBindVertexArray(0);
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

	// This configures the viewport transform, which maps OpenGL's -1 to 1 world space to window coordinates
	// The transformation occurs just before OpenGL makes the fragments for the fragment shader
	glViewport(0, 0, 800, 600);

	// Register handler for when window is resized to update the viewport
	glfwSetFramebufferSizeCallback(window, onWindowResize);

	//Init
	GLuint triangleShaderProgram;
	GLuint triangleVAO;
	setupHelloTriangle(triangleShaderProgram, triangleVAO);

	// Main render loop
	while (!glfwWindowShouldClose(window))
	{
		// Process inputs
		processInput(window);

		// Clear the frame buffer by filling it with a colour
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render Stuff goes here
		renderHelloTriangle(triangleShaderProgram, triangleVAO);

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
