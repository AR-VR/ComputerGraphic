#include<iostream>

#include <glad/glad.h>
#include <glfw3.h>
#include "GLErrorCheck.h"

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const GLchar* const vertexShaderSrc =
"#version 330 core																							\n"
"layout (location = 0) in vec3 position;												\n"
"void main() {																									\n"
"  gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
"}";

const GLchar* const fragmentShaderSrc =
"#version 330 core																							\n"
"out vec4 FragColor;																						\n"
"void main() {																									\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);										\n"
"}";

int main(int argc, char** argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GL Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//Buffere size update callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	GLuint vertexShader;
	GL_EXEC(vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GL_EXEC(glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL));
	GL_COMPILE(vertexShader);

	GLuint fragmentShader;
	GL_EXEC(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
	GL_EXEC(glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL));
	GL_COMPILE(fragmentShader);

	GLuint glProgram;
	GL_EXEC(glProgram = glCreateProgram());
	GL_EXEC(glAttachShader(glProgram, vertexShader));
	GL_EXEC(glAttachShader(glProgram, fragmentShader));
	GL_LINK(glProgram);
	//Delete shader after program set up
	GL_EXEC(glDeleteShader(vertexShader));
	GL_EXEC(glDeleteShader(fragmentShader));

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}