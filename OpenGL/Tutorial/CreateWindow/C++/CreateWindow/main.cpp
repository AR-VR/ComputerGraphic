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
  unsigned int vao, vbo;
  GL_EXEC(glGenVertexArrays(1, &vao));
  GL_EXEC(glGenBuffers(1, &vbo));
  //1. Bind vertex array object (container) first
  GL_EXEC(glBindVertexArray(vao));

  //2. Bind VBO
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  //Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
  GL_EXEC(glBufferData(GL_ARRAY_BUFFER, cube.GetTriangleVerticesBufferSize(), cube.GetVerticesForTriangle().data(), GL_STATIC_DRAW));

  //3. Configure vertex attributes (bind to shader variable from my understanding)
  const unsigned int VERTEX_ATTRIBUTE = shaderProgram.GetAttributeLocation("inPosition");
  GL_EXEC(glVertexAttribPointer(VERTEX_ATTRIBUTE, Cube::VERTEX_UNITS, GL_FLOAT, GL_FALSE, Cube::ELEMENTS_PER_VERTEX * sizeof(float), Cube::VERTEX_OFFSET_POINTER));
  GL_EXEC(glEnableVertexAttribArray(VERTEX_ATTRIBUTE));

  const unsigned int TEXTURE_ATTRIBUTE = shaderProgram.GetAttributeLocation("inTexCoord");
  GL_EXEC(glVertexAttribPointer(TEXTURE_ATTRIBUTE, Cube::TEXTURE_UNITS, GL_FLOAT, GL_FALSE, Cube::ELEMENTS_PER_VERTEX * sizeof(float), Cube::TEXTURE_OFFSET_POINTER));
  GL_EXEC(glEnableVertexAttribArray(TEXTURE_ATTRIBUTE));

  //4. Configure normal attributes (bind to shader variable from my understanding)
  const unsigned int NORMAL_ATTRIBUTE = shaderProgram.GetAttributeLocation("inNormal");
  GL_EXEC(glVertexAttribPointer(NORMAL_ATTRIBUTE, Cube::NORMAL_UNITS, GL_FLOAT, GL_FALSE, Cube::ELEMENTS_PER_VERTEX * sizeof(float), Cube::NORMAL_OFFSET_POINTER));
  GL_EXEC(glEnableVertexAttribArray(NORMAL_ATTRIBUTE));

  //5. Unbind VBO, prevent overwritten/polluted
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //6. unbind VAO
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
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("Texture\\wall.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    DEBUG_THROW;
  }
  GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
  //Free image data after loaded to GPU
  stbi_image_free(data);

  camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);

  GL_EXEC(glEnable(GL_DEPTH_TEST));
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    GL_EXEC(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

    GL_EXEC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!

    GL_EXEC(shaderProgram.UseProgram());

    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glActiveTexture(GL_TEXTURE0));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));

    glm::mat4 modelMatrix = cube.GetModelMatrix();
    shaderProgram.SetUniformMatrix4fv("model", glm::transpose(modelMatrix));

    glm::vec3 eye(0, 0, 2);
    glm::vec3 center(0, 0, 0);
    glm::vec3 upDirection(0, 1, 0);
    glm::mat4 viewMatrix = camera.LookAt(eye, center, upDirection);
    shaderProgram.SetUniformMatrix4fv("view", glm::transpose(viewMatrix));

    shaderProgram.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjectionMatrix()));

    //https://github.com/AR-VR/ComputerGraphic/wiki/Transformation-of-Surface-Normal
    glm::mat4 normalTransform(glm::transpose(glm::inverse(viewMatrix*modelMatrix)));
    shaderProgram.SetUniformMatrix4fv("normalTransform", glm::transpose(normalTransform));
    glm::vec3 lightPosition(1, 1, 1);
    shaderProgram.SetUniform3fv("lightPosition", lightPosition);

    shaderProgram.SetUniform3fv("cameraPosition", eye);

    shaderProgram.SetUniform3fv("spotLightColor", glm::vec3(0, 1, 1));
    shaderProgram.SetUniform3fv("ambientLightColor", glm::vec3(1, 1, 1));
    GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, cube.GetVerticesCountTriangle()));
    GL_EXEC(glBindVertexArray(0));

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GL_EXEC(glDeleteBuffers(1, &vbo));
  GL_EXEC(glDeleteVertexArrays(1, &vao));
}

static void RenderSpecular(GLFWwindow* window)
{
  //Init GL program
  string vertexSrc = LoadFileString("Shader\\specular.vert");
  string fragmentSrc = LoadFileString("Shader\\specular.frag");
  if (vertexSrc.empty() || fragmentSrc.empty()) {
    std::cout << "Failed to Load Shader" << std::endl;
    DEBUG_THROW;
    return;
  }
  //This block is to clean up shader, somehow in AMD graphics card glfwTerminate conflicted with glDeleteProgram
  Shader shaderProgram = Shader(vertexSrc.c_str(), fragmentSrc.c_str());
  unsigned int vao, vbo;
  GL_EXEC(glGenVertexArrays(1, &vao));
  GL_EXEC(glGenBuffers(1, &vbo));
  //1. Bind vertex array object (container) first
  GL_EXEC(glBindVertexArray(vao));

  //2. Bind VBO
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  //Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
  GL_EXEC(glBufferData(GL_ARRAY_BUFFER, cube.GetTriangleVerticesBufferSize(), cube.GetVerticesForTriangle().data(), GL_STATIC_DRAW));

  //3. Configure vertex attributes (bind to shader variable from my understanding)
  shaderProgram.SetVertex("inPosition");
  shaderProgram.SetTexture("inTexCoord");
  shaderProgram.SetNormal("inNormal");

  //5. Unbind VBO, prevent overwritten/polluted
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //6. unbind VAO
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
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("Texture\\wall.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    DEBUG_THROW;
  }
  GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
  //Free image data after loaded to GPU
  stbi_image_free(data);

  camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);

  GL_EXEC(glEnable(GL_DEPTH_TEST));
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    GL_EXEC(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

    GL_EXEC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!

    GL_EXEC(shaderProgram.UseProgram());

    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glActiveTexture(GL_TEXTURE0));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));

    glm::mat4 modelMatrix = cube.GetModelMatrix();
    shaderProgram.SetUniformMatrix4fv("model", glm::transpose(modelMatrix));

    glm::vec3 eye(2, 0, 2);
    glm::vec3 center(0, 0, 0);
    glm::vec3 upDirection(0, 1, 0);
    glm::mat4 viewMatrix = camera.LookAt(eye, center, upDirection);
    shaderProgram.SetUniformMatrix4fv("view", glm::transpose(viewMatrix));

    shaderProgram.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjectionMatrix()));
    
    //https://github.com/AR-VR/ComputerGraphic/wiki/Transformation-of-Surface-Normal
    glm::mat4 normalTransform(glm::transpose(glm::inverse(viewMatrix*modelMatrix)));
    shaderProgram.SetUniformMatrix4fv("normalTransform", glm::transpose(normalTransform));

    glm::vec3 lightPosition(0, 0, 1);
    shaderProgram.SetUniform3fv("lightPosition", lightPosition);

    shaderProgram.SetUniform3fv("cameraPosition", eye);

    shaderProgram.SetUniform3fv("spotLightColor", glm::vec3(0.5, 0.5, 0.5));
    GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, cube.GetVerticesCountTriangle()));
    GL_EXEC(glBindVertexArray(0));

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GL_EXEC(glDeleteBuffers(1, &vbo));
  GL_EXEC(glDeleteVertexArrays(1, &vao));
}

static void RenderDiffuse(GLFWwindow* window)
{
  //Init GL program
  string vertexSrc = LoadFileString("Shader\\diffuse.vert");
  string fragmentSrc = LoadFileString("Shader\\diffuse.frag");
  if (vertexSrc.empty() || fragmentSrc.empty()) {
    std::cout << "Failed to Load Shader" << std::endl;
    DEBUG_THROW;
    return;
  }
  //This block is to clean up shader, somehow in AMD graphics card glfwTerminate conflicted with glDeleteProgram
  Shader shaderProgram = Shader(vertexSrc.c_str(), fragmentSrc.c_str());
  unsigned int vao, vbo;
  GL_EXEC(glGenVertexArrays(1, &vao));
  GL_EXEC(glGenBuffers(1, &vbo));
  //1. Bind vertex array object (container) first
  GL_EXEC(glBindVertexArray(vao));

  //2. Bind VBO
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  //Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
  GL_EXEC(glBufferData(GL_ARRAY_BUFFER, cube.GetTriangleVerticesBufferSize(), cube.GetVerticesForTriangle().data(), GL_STATIC_DRAW));

  //3. Configure vertex attributes (bind to shader variable from my understanding)
  shaderProgram.SetVertex("inPosition");
  shaderProgram.SetTexture("inTexCoord");
  shaderProgram.SetNormal("inNormal");

  //5. Unbind VBO, prevent overwritten/polluted
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //6. unbind VAO
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
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("Texture\\wall.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    DEBUG_THROW;
  }
  GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
  //Free image data after loaded to GPU
  stbi_image_free(data);

  camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);

  GL_EXEC(glEnable(GL_DEPTH_TEST));
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    GL_EXEC(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

    GL_EXEC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!

    GL_EXEC(shaderProgram.UseProgram());
    
    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glActiveTexture(GL_TEXTURE0));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));

    glm::mat4 modelMatrix = cube.GetModelMatrix();
    shaderProgram.SetUniformMatrix4fv("model", glm::transpose(modelMatrix));

    glm::vec3 eye(0, 0, 2);
    glm::vec3 center(0, 0, 0);
    glm::vec3 upDirection(0, 1, 0);
    glm::mat4 viewMatrix = camera.LookAt(eye, center, upDirection);
    shaderProgram.SetUniformMatrix4fv("view", glm::transpose(viewMatrix));
    shaderProgram.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjectionMatrix()));

    //https://github.com/AR-VR/ComputerGraphic/wiki/Transformation-of-Surface-Normal
    glm::mat4 normalTransform(glm::transpose(glm::inverse(viewMatrix*modelMatrix)));
    shaderProgram.SetUniformMatrix4fv("normalTransform", glm::transpose(normalTransform));
    glm::vec3 lightPosition(0, 0, 2);
    shaderProgram.SetUniform3fv("lightPosition", lightPosition);

    shaderProgram.SetUniform3fv("diffuseLightColor", glm::vec3(0.5, 0.5, 0.5));
    GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, cube.GetVerticesCountTriangle()));
    GL_EXEC(glBindVertexArray(0));

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GL_EXEC(glDeleteBuffers(1, &vbo));
  GL_EXEC(glDeleteVertexArrays(1, &vao));
}

static void RenderBasic(GLFWwindow* window)
{
  //Init GL program
  string vertexSrc = LoadFileString("Shader\\basic.vert");
  string fragmentSrc = LoadFileString("Shader\\basic.frag");
  if (vertexSrc.empty() || fragmentSrc.empty()) {
    std::cout << "Failed to Load Shader" << std::endl;
    DEBUG_THROW;
    return;
  }
  //This block is to clean up shader, somehow in AMD graphics card glfwTerminate conflicted with glDeleteProgram
  Shader shaderProgram = Shader(vertexSrc.c_str(), fragmentSrc.c_str());
  
  unsigned int vao, vbo;
  GL_EXEC(glGenVertexArrays(1, &vao));
  GL_EXEC(glGenBuffers(1, &vbo));
  //1. Bind vertex array object (container) first
  GL_EXEC(glBindVertexArray(vao));

  //2. Bind VBO
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  //Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
  GL_EXEC(glBufferData(GL_ARRAY_BUFFER, cube.GetTriangleVerticesBufferSize(), cube.GetVerticesForTriangle().data(), GL_STATIC_DRAW));

  //3. Configure vertex attributes (bind to shader variable from my understanding)
  shaderProgram.SetVertex("inPosition");
  shaderProgram.SetTexture("inTexCoord");

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
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("Texture\\wall.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    DEBUG_THROW;
  }
  GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
  //Free image data after loaded to GPU
  stbi_image_free(data);

  camera.PerspectiveProjection(fovYDegree, ((float)SCR_WIDTH) / ((float)SCR_HEIGHT), zNear, zFar);

  GL_EXEC(glEnable(GL_DEPTH_TEST));
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
    glm::mat4 modelMatrix = cube.GetModelMatrix();
    shaderProgram.SetUniformMatrix4fv("model", glm::transpose(modelMatrix));

    glm::vec3 eye(0, 0, 2);
    glm::vec3 center(0, 0, 0);
    glm::vec3 upDirection(0, 1, 0);
    glm::mat4 viewMatrix = camera.LookAt(eye, center, upDirection);
    shaderProgram.SetUniformMatrix4fv("view", glm::transpose(viewMatrix));

    shaderProgram.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjectionMatrix()));
    shaderProgram.SetUniform3fv("lightColor", glm::vec3(1, 1, 1));
    shaderProgram.SetUniformFloat("ambientStrength", 0.9f);
    GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, cube.GetVerticesCountTriangle()));
    GL_EXEC(glBindVertexArray(0));

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  GL_EXEC(glDeleteBuffers(1, &vbo));
  GL_EXEC(glDeleteVertexArrays(1, &vao));
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

  //RenderBasic(window);
  //RenderDiffuse(window);
  //RenderSpecular(window);
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