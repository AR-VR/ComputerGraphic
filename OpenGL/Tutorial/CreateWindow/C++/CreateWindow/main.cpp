#include<iostream>

#include <glad/glad.h>
#include <glfw3.h>
#include "GLErrorCheck.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const GLchar* const vertexShaderSrc =
"#version 330 core																							\n"
"layout (location = 0) in vec3 position;												\n" //This define position variable at vertex attribute pointer index 0
"void main() {																									\n"
"  gl_Position = vec4(position.x, position.y, position.z, 1.0); \n" //Convert vec3 position to vec4 position (eventually for w division?)
"}";

const GLchar* const fragmentShaderSrc =
"#version 330 core																							\n"
"out vec4 FragColor;																						\n"
"void main() {																									\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);										\n" //Set all the drawing to this color
"}";


int InitGL() {
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
	GL_EXEC(glDetachShader(glProgram, vertexShader));
	GL_EXEC(glDeleteShader(vertexShader));
	GL_EXEC(glDetachShader(glProgram, fragmentShader));
	GL_EXEC(glDeleteShader(fragmentShader));

	return glProgram;
}


int main(int argc, char** argv) {

#ifndef _DEBUG
	FreeConsole();
#endif

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

	//Init GL program
	//GLuint glProgram = InitGL();
	Shader shaderProgram = Shader("Shader\\vertex.glsl", "Shader\\fragment.glsl");

	//NDC Coordinate System
	//     1
	// -1     1
	//    -1

	//Texture Coordinate
	//(0,1)       (1,1)
	//
	//
	//(0,0)       (1,0)

	//Vertex Order
	// 3	   0
	//    
	// 2     1
	//
	const float vertices[] = {
		 0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // top right, red
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,		// bottom right, green
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,		// bottom left, blue
		-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f		// top left, white
	};

	const unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int vao, vbo, ebo;
	GL_EXEC(glGenVertexArrays(1, &vao));
	GL_EXEC(glGenBuffers(1, &vbo));
	GL_EXEC(glGenBuffers(1, &ebo));

	//1. Bind vertex array object (container) first
	GL_EXEC(glBindVertexArray(vao));

	//2. Bind VBO
	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
	GL_EXEC(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_EXEC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	GL_EXEC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//3. Configure vertex attributes (bind to shader variable from my understanding)
	int vertexIndex = 0;
	GL_EXEC(glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GL_EXEC(glEnableVertexAttribArray(vertexIndex));

	int colorIndex = 1;
	GL_EXEC(glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GL_EXEC(glEnableVertexAttribArray(colorIndex));

	int textureIndex = 2;
	GL_EXEC(glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GL_EXEC(glEnableVertexAttribArray(textureIndex));

	//4. Unbind VBO, prevent overwritten/polluted
	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//5. unbind VAO, not a must
	GL_EXEC(glBindVertexArray(0));

	unsigned int textureID;
	GL_EXEC(glGenTextures(1, &textureID));
	GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));
	// set the texture wrapping/filtering options (on the currently bound texture object)
	GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//When scale down, make it more blocked pattern
	GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	//When scale up, make it more linear pattern
	GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//Load texture into GPU
	int width, height, nrChannels;
	unsigned char *data = stbi_load("Texture\\wall.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
	//Free image data after loaded to GPU
	stbi_image_free(data);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		GL_EXEC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GL_EXEC(glClear(GL_COLOR_BUFFER_BIT));

		GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));
		//GL_EXEC(glUseProgram(glProgram));
		GL_EXEC(shaderProgram.useProgram());

		GL_EXEC(glBindVertexArray(vao));
		//Since we went through 0, 1, 3 first Triangle, 1, 2, 3 second Triangle, so total 6 elements(vertices)
		GL_EXEC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		GL_EXEC(glBindVertexArray(0));
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	GL_EXEC(glDeleteBuffers(1, &vbo));
	GL_EXEC(glDeleteBuffers(1, &ebo));
	GL_EXEC(glDeleteVertexArrays(1, &vao));
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