#pragma once

#include <glm.hpp>
#include <vector>
class Shape
{
public:
  Shape() : scale(), rotation(), translation() {}
  const glm::mat4 GetModelMatrix();
  void Scale(float x, float y, float z);
  void Rotate(float yawDegree, float pitchDegree, float rollDegree);
  void Translate(float x, float y, float z);
  const unsigned int GetVerticesCountTriangle() { return verticesCountForTriangle; }
  const float* const GetDataBufferForTriangle() { return vertices.data(); }
  const unsigned int GetTriangleDataBufferSize() { return sizeof(float)*vertices.size(); }
  ~Shape() { }

  static const unsigned int VERTEX_UNITS = 3;
  static const unsigned int TEXTURE_UNITS = 2;
  static const unsigned int NORMAL_UNITS = 3;
  //static const unsigned int VERTICES_COUNT = 36;
  static const unsigned int ELEMENTS_PER_VERTEX = VERTEX_UNITS + TEXTURE_UNITS + NORMAL_UNITS;
  //Define in header, init in cpp file
  //static const float vertices[VERTICES_COUNT*ELEMENTS_PER_VERTEX];
  static const void* const VERTEX_OFFSET_POINTER;
  static const void* const TEXTURE_OFFSET_POINTER;
  static const void* const NORMAL_OFFSET_POINTER;
protected:
  unsigned int verticesCountForTriangle;
  std::vector<float> vertices;

private:
  glm::mat4 scale;
  glm::mat4 rotation;
  glm::mat4 translation;
};

