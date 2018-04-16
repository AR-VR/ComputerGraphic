#include<iostream>

#include <glad/glad.h>
#include <glfw3.h>
#include "GLErrorCheck.h"
#include "Shader.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Cube.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static Camera camera;
static Cube cube;

static float zNear = 0.1, zFar = 99.0;
static float fovYDegree = 60;

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
  Shader shaderProgram = Shader("Shader\\vertex.glsl", "Shader\\fragment.glsl");
  
	unsigned int vao, vbo;
	GL_EXEC(glGenVertexArrays(1, &vao));
	GL_EXEC(glGenBuffers(1, &vbo));
	//1. Bind vertex array object (container) first
	GL_EXEC(glBindVertexArray(vao));

	//2. Bind VBO
	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	//Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
  GL_EXEC(glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::vertices), Cube::vertices, GL_STATIC_DRAW));

	//3. Configure vertex attributes (bind to shader variable from my understanding)
  const unsigned int VERTEX_ATTRIBUTE = shaderProgram.GetAttributeLocation("inPosition");;
  GL_EXEC(glVertexAttribPointer(VERTEX_ATTRIBUTE, VERTEX_UNITS, GL_FLOAT, GL_FALSE, Cube::ELEMENTS_PER_VERTEX * sizeof(float), (void*)0));
  GL_EXEC(glEnableVertexAttribArray(VERTEX_ATTRIBUTE));

  
  const unsigned int TEXTURE_ATTRIBUTE = shaderProgram.GetAttributeLocation("inTexCoord");;
  GL_EXEC(glVertexAttribPointer(TEXTURE_ATTRIBUTE, TEXTURE_UNITS, GL_FLOAT, GL_FALSE, Cube::ELEMENTS_PER_VERTEX * sizeof(float), (void*)(VERTEX_UNITS * sizeof(float))));
  GL_EXEC(glEnableVertexAttribArray(TEXTURE_ATTRIBUTE));

	//4. Unbind VBO, prevent overwritten/polluted
	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//5. unbind VAO
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

  camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);
  //shaderProgram.SetUniformMatrix4fv("view", glm::transpose(viewMatrix));
  //shaderProgram.SetUniformMatrix4fv("model", glm::transpose(cube.GetModelMatrix()));
  glEnable(GL_DEPTH_TEST);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		GL_EXEC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    
    GL_EXEC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!

    GL_EXEC(shaderProgram.UseProgram());
    //;
    GL_EXEC(glBindVertexArray(vao));

    GL_EXEC(glActiveTexture(GL_TEXTURE0));
		GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));
    shaderProgram.SetUniformMatrix4fv("model", glm::transpose(cube.GetModelMatrix()));
    glm::mat4 viewMatrix = camera.LookAt(glm::vec3(0, 0, 5.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    shaderProgram.SetUniformMatrix4fv("view", glm::transpose(viewMatrix));
    shaderProgram.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjectionMatrix()));

		//Since we went through 0, 1, 3 first Triangle, 1, 2, 3 second Triangle, so total 6 elements(vertices)
    GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, 36));
		GL_EXEC(glBindVertexArray(0));
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	GL_EXEC(glDeleteBuffers(1, &vbo));
	GL_EXEC(glDeleteVertexArrays(1, &vao));
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

static float yawAngle = 0;
static float pitchAngle = 0;
static float rollAngle = 0;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
    return;
	}

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    rollAngle+=0.1;
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    rollAngle -= 0.1;
  } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    pitchAngle += 0.1;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    pitchAngle -= 0.1;
  }

  cube.Rotate(yawAngle, pitchAngle, rollAngle);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
  float aspectRatio = ((float)width) / ((float)height);
  camera.PerspectiveProjection(fovYDegree, aspectRatio, zNear, zFar);
}