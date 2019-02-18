#pragma once
#include "Shader.h"
#include "Camera.h"

enum LightType
{
  Ambient,
  Spot
};

struct Light
{
  LightType type;
  glm::vec3 lightColor;
  glm::vec3 lightPosition;
};
class Scene
{
private:
  unsigned int vertexArrayObject;
  unsigned int vertexBufferObject;
  unsigned int textureID;

public:
  Scene();
  ~Scene();
  void LoadShape(Shape& shape, Shader& program);
  bool LoadShapeTexture(std::string& textureFilePath);
  virtual void Draw(Camera& camera, Shape& shape, Shader& program) {}

protected:
  unsigned int GetVertexArrayObject() { return vertexArrayObject; }
  unsigned int GetVertexBufferObject() { return vertexBufferObject; }
  unsigned int GetTexture() { return textureID; }
};

