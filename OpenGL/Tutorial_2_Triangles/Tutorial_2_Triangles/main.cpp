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

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
static string LoadFileString(const char* filePath);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv) {

#ifndef _DEBUG
    FreeConsole();
#endif

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
	const unsigned int bufferSize = 9 * sizeof(float);
	GL_EXEC(glBufferData(GL_ARRAY_BUFFER, bufferSize, triangle.ForShape(), GL_STATIC_DRAW));

	const unsigned int LayoutLocation = 0;
	const unsigned int SizePerVertex = 3;
	const void* const VERTEX_OFFSET_POINTER = (void*)0;
	GL_EXEC(glVertexAttribPointer(LayoutLocation, SizePerVertex, GL_FLOAT, GL_FALSE, SizePerVertex * sizeof(float), VERTEX_OFFSET_POINTER));
	GL_EXEC(glEnableVertexAttribArray(LayoutLocation));

	GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_EXEC(glBindVertexArray(0));


	unsigned int glProgramID;
	GL_EXEC(glProgramID = glCreateProgram());


	const string vertexStr= LoadFileString("Shader\\ndc.vert");
	const char * const vertexSrc = vertexStr.c_str();
	unsigned int vertexShader;
	GL_EXEC(vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GL_EXEC(glShaderSource(vertexShader, 1, &vertexSrc, NULL));
	GL_COMPILE(vertexShader);
	GL_EXEC(glAttachShader(glProgramID, vertexShader));


	const string fragStr = LoadFileString("Shader\\ndc.frag");
	const char * const fragSrc = fragStr.c_str();
	unsigned int fragShader;
	GL_EXEC(fragShader = glCreateShader(GL_FRAGMENT_SHADER));
	GL_EXEC(glShaderSource(fragShader, 1, &fragSrc, NULL));
	GL_COMPILE(fragShader);
	GL_EXEC(glAttachShader(glProgramID, fragShader));

	GL_LINK(glProgramID);

	GL_EXEC(glDetachShader(glProgramID, vertexShader));
	GL_EXEC(glDeleteShader(vertexShader));
	
	GL_EXEC(glDetachShader(glProgramID, fragShader));
	GL_EXEC(glDeleteShader(fragShader));


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        GL_EXEC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GL_EXEC(glClear(GL_COLOR_BUFFER_BIT));


		//GL_EXEC(glDeleteBuffers(1, &vbo));

		GL_EXEC(glUseProgram(glProgramID));
		GL_EXEC(glBindVertexArray(vao));
		const unsigned int vertexCount = 3;
		GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, vertexCount));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	GL_EXEC(glDeleteVertexArrays(1, &vao));
	GL_EXEC(glDeleteBuffers(1, &vbo));
	GL_EXEC(glDeleteProgram(glProgramID));

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