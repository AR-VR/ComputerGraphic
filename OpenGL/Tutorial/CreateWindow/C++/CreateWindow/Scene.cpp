#include "Scene.h"

#include "GLErrorCheck.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION

Scene::Scene() {
  GL_EXEC(glGenVertexArrays(1, &vertexArrayObject));
  GL_EXEC(glGenBuffers(1, &vertexBufferObject));
}

Scene::~Scene()
{
  if(glIsTexture(textureID)) {
    GL_EXEC(glDeleteTextures(1, &textureID));
  }
  GL_EXEC(glDeleteBuffers(1, &vertexBufferObject));
  GL_EXEC(glDeleteVertexArrays(1, &vertexArrayObject));
}

//Load vertex, texture coordinate and normal data for a full triangle shape
void Scene::LoadShape(Shape& shape, Shader& program)
{
  //Bind vertex array object (container) first
  GL_EXEC(glBindVertexArray(vertexArrayObject));

  //Bind VBO
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject));
  //Load vertices data to GPU, and GL_STATIC_DRAW means data will not be modified after loading
  GL_EXEC(glBufferData(GL_ARRAY_BUFFER, shape.GetTriangleDataBufferSize(), shape.GetDataBufferForTriangle(), GL_STATIC_DRAW));
  
  //3. Configure vertex attributes (bind to shader variable from my understanding)
    //3. Configure vertex attributes (bind to shader variable from my understanding)
  const unsigned int VERTEX_ATTRIBUTE = program.GetAttributeLocation("inPosition");
  GL_EXEC(glVertexAttribPointer(VERTEX_ATTRIBUTE, Shape::VERTEX_UNITS, GL_FLOAT, GL_FALSE, Shape::ELEMENTS_PER_VERTEX * sizeof(float), Shape::VERTEX_OFFSET_POINTER));
  GL_EXEC(glEnableVertexAttribArray(VERTEX_ATTRIBUTE));

  const unsigned int TEXTURE_ATTRIBUTE = program.GetAttributeLocation("inTexCoord");
  GL_EXEC(glVertexAttribPointer(TEXTURE_ATTRIBUTE, Shape::TEXTURE_UNITS, GL_FLOAT, GL_FALSE, Shape::ELEMENTS_PER_VERTEX * sizeof(float), Shape::TEXTURE_OFFSET_POINTER));
  GL_EXEC(glEnableVertexAttribArray(TEXTURE_ATTRIBUTE));

  //4. Configure normal attributes (bind to shader variable from my understanding)
  const unsigned int NORMAL_ATTRIBUTE = program.GetAttributeLocation("inNormal");
  GL_EXEC(glVertexAttribPointer(NORMAL_ATTRIBUTE, Shape::NORMAL_UNITS, GL_FLOAT, GL_FALSE, Shape::ELEMENTS_PER_VERTEX * sizeof(float), Shape::NORMAL_OFFSET_POINTER));
  GL_EXEC(glEnableVertexAttribArray(NORMAL_ATTRIBUTE));
  //Unbind VBO, prevent overwritten/polluted
  GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

bool Scene::LoadShapeTexture(std::string& textureFilePath)
{
  bool loadTextureSuceed = false;
  int width, height, nrChannels;
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);
  if (data) 
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
    
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
    loadTextureSuceed = true;
  }
  else 
  {
    std::cout << "Failed to load texture: " << textureFilePath << std::endl;
    DEBUG_THROW;
  }
  //Free image data after loaded to GPU
  stbi_image_free(data);
  return loadTextureSuceed;
}