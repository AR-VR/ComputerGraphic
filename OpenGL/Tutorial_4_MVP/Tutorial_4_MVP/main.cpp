#include<iostream>
#include<fstream>
#include<sstream>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


#include "GLErrorCheck.h"

#include "Triangle.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static string LoadFileString(const char* filePath);
// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
static float yaw = 0;
float pitch = 0;
float roll = 0;

int main(int argc, char** argv) {

    glfwInit();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    //Buffer size update callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


	//Vertex buffer object
	unsigned int vao;
	GL_EXEC(glGenVertexArrays(1, &vao));
	unsigned int vbo;
	GL_EXEC(glGenBuffers(1, &vbo));
	// 2. copy our vertices array in a buffer for OpenGL to use
	GL_EXEC(glBindVertexArray(vao));
	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	Triangle triangle;
	const std::vector<float> VertexData = triangle.ForShape();
	const unsigned int BufferSize = VertexData.size() * sizeof(float);
	//vbo[] <- triangleVertex
	GL_EXEC(glBufferData(GL_ARRAY_BUFFER, BufferSize, VertexData.data(), GL_STATIC_DRAW));


	const string vertexStr = LoadFileString("Shader\\mvp.vert");
	const string fragStr = LoadFileString("Shader\\ndc.frag");
	{
		
		Shader ndcShader = Shader(vertexStr.c_str(), fragStr.c_str());
		const unsigned int LayoutLocation = ndcShader.GetAttributeLocation("inPosition");
		const unsigned int ElementPerVertex = 3;
		const unsigned int VertexStride = ElementPerVertex * sizeof(float);
		const void* const VertexOffsetPointer = (void*)0;
		// vao[location] <- vbo[0]
		GL_EXEC(glVertexAttribPointer(LayoutLocation, ElementPerVertex, GL_FLOAT, GL_FALSE, VertexStride, VertexOffsetPointer));
		//The reason why the fuck we need this: https://www.gamedev.net/forums/topic/655785-is-glenablevertexattribarray-redundant/
		GL_EXEC(glEnableVertexAttribArray(LayoutLocation));
		Camera camera;
		const float fovYDegree = 60;
		const float zNear = 0.1f, zFar = 99.0f;
		camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);
		glm::vec3 eye(0, 0, 1);
		glm::vec3 center(0, 0, 0);
		glm::vec3 upDirection(0, 1, 0);
		camera.LookAt(eye, center, upDirection);

		while (!glfwWindowShouldClose(window))
		{
			processInput(window);
			Model model;
			model.Scale(1, 1, 1);
			model.Rotate(yaw, pitch, roll);
			GL_EXEC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GL_EXEC(glClear(GL_COLOR_BUFFER_BIT));
			ndcShader.UseProgram();
			ndcShader.SetUniformMatrix4fv("model", glm::transpose(model.GetModelMatrix()));
			ndcShader.SetUniformMatrix4fv("view", glm::transpose(camera.GetView()));
			ndcShader.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjection()));

			GL_EXEC(glBindVertexArray(vao));
			const unsigned int VertexCount = triangle.ForShape().size() / ElementPerVertex;
			GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, VertexCount));

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		GL_EXEC(glDeleteVertexArrays(1, &vao));
		GL_EXEC(glDeleteBuffers(1, &vbo));
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
        return;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_S) {
		yaw += 1;
		std::cout << "Yaw: " << yaw << std::endl;
		return;
	}

	if (key == GLFW_KEY_W) {
		yaw -= 1;
		std::cout << "Yaw: " << yaw << std::endl;
		return;
	}

	if (key == GLFW_KEY_D) {
		pitch += 1;
		std::cout << "Pitch: " << yaw << std::endl;
		return;
	}

	if (key == GLFW_KEY_A) {
		pitch -= 1;
		std::cout << "Pitch: " << pitch << std::endl;
		return;
	}

	if (key == GLFW_KEY_Z) {
		roll += 1;
		std::cout << "Roll: " << roll << std::endl;
		return;
	}

	if (key == GLFW_KEY_X) {
		roll -= 1;
		std::cout << "Roll: " << roll << std::endl;
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		yaw = 0;
		pitch = 0;
		roll = 0;
		std::cout << "Reset" << std::endl;
		return;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    GL_EXEC(glViewport(0, 0, width, height));
}

static string LoadFileString(const char* filePath)
{
	ifstream fileStream;

	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		fileStream.open(filePath);

		stringstream fileStringStream;
		// read file's buffer contents into streams
		fileStringStream << fileStream.rdbuf();
		// close file handlers
		fileStream.close();
		return fileStringStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
		std::cerr << e.what() << std::endl;
		DEBUG_THROW;
		return string();
	}
}