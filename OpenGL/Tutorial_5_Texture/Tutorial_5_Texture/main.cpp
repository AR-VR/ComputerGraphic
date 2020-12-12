#include<iostream>
#include<fstream>
#include<sstream>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	unsigned int ebo;
	GL_EXEC(glGenBuffers(1, &ebo));
	
	// 2. copy our vertices array in a buffer for OpenGL to use
	GL_EXEC(glBindVertexArray(vao));

	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	Triangle triangle;
	const std::vector<float> VertexData = triangle.ShapeWithTexture();
	const unsigned int BufferSize = VertexData.size() * sizeof(float);
	//vbo[] <- triangleVertex
	GL_EXEC(glBufferData(GL_ARRAY_BUFFER, BufferSize, VertexData.data(), GL_STATIC_DRAW));

	unsigned int indices[] = {
		0, 1, 2
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int textureID;
	{
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
		// tell stb_image.h to flip loaded texture's on the y-axis.
		stbi_set_flip_vertically_on_load(true);

		//Somehow it does not support png
		unsigned char *data = stbi_load("Texture\\coordinate.jpg", &width, &height, &nrChannels, 0);
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
	}

	const string vertexStr = LoadFileString("Shader\\texture.vert");
	const string fragStr = LoadFileString("Shader\\texture.frag");
	{		
		Shader ndcShader = Shader(vertexStr.c_str(), fragStr.c_str());
		const unsigned int PositionLayoutLocation = ndcShader.GetAttributeLocation("inPosition");
		const unsigned int ElementPerVertex = 3;
		const unsigned int ElementPerTex = 2;
		const unsigned int VertexStride = ElementPerVertex * sizeof(float);
		const unsigned int VertexTexStride = (ElementPerVertex + ElementPerTex) * sizeof(float);

		const void* const VertexOffsetPointer = (void*)0;
		// vao[location] <- vbo[0]
		GL_EXEC(glVertexAttribPointer(PositionLayoutLocation, ElementPerVertex, GL_FLOAT, GL_FALSE, VertexTexStride, VertexOffsetPointer));
		//The reason why the fuck we need this: https://www.gamedev.net/forums/topic/655785-is-glenablevertexattribarray-redundant/
		GL_EXEC(glEnableVertexAttribArray(PositionLayoutLocation));


		const unsigned int TextureLayoutLocation = ndcShader.GetAttributeLocation("inTexCoord");
		const void* const TextureOffsetPointer = (void*)VertexStride;

		GL_EXEC(glVertexAttribPointer(TextureLayoutLocation, ElementPerTex, GL_FLOAT, GL_FALSE, VertexTexStride, TextureOffsetPointer));
		//The reason why the fuck we need this: https://www.gamedev.net/forums/topic/655785-is-glenablevertexattribarray-redundant/
		GL_EXEC(glEnableVertexAttribArray(TextureLayoutLocation));

		Camera camera;
		const float fovYDegree = 60;
		const float zNear = 0.1f, zFar = 99.0f;
		camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);
		glm::vec3 eye(0, 0, 1);
		glm::vec3 center(0, 0, 0);
		glm::vec3 upDirection(0, 1, 0);
		camera.LookAt(eye, center, upDirection);
		Model model;
		model.Scale(1, 1, 1);
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);
			model.Rotate(yaw, pitch, roll);
			GL_EXEC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GL_EXEC(glClear(GL_COLOR_BUFFER_BIT));

			glActiveTexture(GL_TEXTURE0);
			GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));
			
			ndcShader.UseProgram();
			ndcShader.SetUniformMatrix4fv("model", glm::transpose(model.GetModelMatrix()));
			ndcShader.SetUniformMatrix4fv("view", glm::transpose(camera.GetView()));
			ndcShader.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjection()));
			
			GL_EXEC(glBindVertexArray(vao));
			GL_EXEC(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		GL_EXEC(glDeleteVertexArrays(1, &vao));
		GL_EXEC(glDeleteBuffers(1, &vbo));
		GL_EXEC(glDeleteBuffers(1, &ebo));
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