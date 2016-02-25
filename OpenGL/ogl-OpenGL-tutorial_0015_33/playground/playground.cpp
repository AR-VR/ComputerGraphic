#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>

using namespace glm;
using namespace std;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
    glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    //////////////////////////////////////////////////////////////
    static const int kTriangleSize = 3;
    vector<GLuint> vertexArrayIDs(kTriangleSize);
    
    GLuint arrayID;
    glGenVertexArrays(1, &arrayID);
    for(int i=0; i<vertexArrayIDs.size(); i++)
    {
        GLuint arrayID;
        glGenVertexArrays(1, &arrayID);
        vertexArrayIDs[i] = arrayID;
        glBindVertexArray(vertexArrayIDs[i]);
    }
    
    GLuint programID = LoadShaders("Vertex.vert", "Fragment.frag");
    //////////////////////////////////////////////////////////////
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(glm::vec3(6,3,3), // Camera is at (4,3,3), in World Space
                                       glm::vec3(0,0,0), // and looks at the origin
                                       glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                       );
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
    vector<GLuint> vertexBuffers(kTriangleSize);
    for(int i=0; i<vertexBuffers.size(); i++)
    {
        GLfloat vertexBufferData [] = {
            -1.0f, -1.0f, i,
            1.0f, -1.0f, i,
            0.0f,  1.0f, i,
        };
        GLuint bufferID;
        glGenBuffers(1, &bufferID);
        vertexBuffers[i] = bufferID;
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
    }

	do{
        glClear( GL_COLOR_BUFFER_BIT );
        
        glUseProgram(programID);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        // 1rst attribute buffer : vertices
        for(int i=0; i<vertexBuffers.size(); i++)
        {
            glEnableVertexAttribArray(i);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
            glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );
            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
            glDisableVertexAttribArray(i);
        }
        
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    glDeleteProgram(programID);

    // Cleanup VBO
    for(int i=0; i<vertexBuffers.size(); i++)
    {
        glDeleteBuffers(1, &vertexBuffers[i]);
        glDeleteVertexArrays(1, &vertexArrayIDs[i]);
    }
    
    
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

