#include<iostream>
#include<fstream>
#include<sstream>
#include <glad/glad.h>
#include <glfw3.h>
#include "GLErrorCheck.h"
#include "Shader.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Cube.h"
#include "Camera.h"
#include "PhongScene.h"

/*
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION
*/
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static Camera camera;
static Cube cube;

static float zNear = 0.1f, zFar = 99.0f;
static float fovYDegree = 60;

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

static void RenderPhong(GLFWwindow* window)
{
  //Init GL program
  string vertexSrc = LoadFileString("Shader\\phong.vert");
  string fragmentSrc = LoadFileString("Shader\\phong.frag");
  if (vertexSrc.empty() || fragmentSrc.empty()) {
    std::cout << "Failed to Load Shader" << std::endl;
    DEBUG_THROW;
    return;
  }
  //This block is to clean up shader, somehow in AMD graphics card glfwTerminate conflicted with glDeleteProgram
  Shader shaderProgram = Shader(vertexSrc.c_str(), fragmentSrc.c_str());
  PhongScene scene;
  scene.LoadShape(cube, shaderProgram);
  scene.LoadShapeTexture(std::string("Texture\\wall.jpg"));

  camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);
  glm::vec3 eye(0, 0, 2);
  glm::vec3 center(0, 0, 0);
  glm::vec3 upDirection(0, 1, 0);
  glm::mat4 viewMatrix = camera.LookAt(eye, center, upDirection);
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);
    
    scene.Draw(camera, cube, shaderProgram);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
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
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GL Window", NULL, NULL);
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

  RenderPhong(window);
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
    rollAngle += 1;
  }
  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    rollAngle -= 1;
  }
  else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    pitchAngle += 1;
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    pitchAngle -= 1;
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    yawAngle += 1;
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    yawAngle -= 1;
  }
  else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) 
  {
    yawAngle = 0;
    pitchAngle = 0;
    rollAngle = 0;
  }
  cube.Rotate(yawAngle, pitchAngle, rollAngle);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  GL_EXEC(glViewport(0, 0, width, height));
  float aspectRatio = ((float)width) / ((float)height);
  camera.PerspectiveProjection(fovYDegree, aspectRatio, zNear, zFar);
}