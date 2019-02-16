#include "Shape.h"
#include <gtc/type_ptr.hpp>


void Shape::Scale(float x, float y, float z)
{
  float scaleMat[] =
  {
    x, 0, 0, 0,
    0, y,	0, 0,
    0, 0, z, 0,
    0, 0, 0, 1
  };
  scale = glm::make_mat4(scaleMat);
}

static inline glm::mat4 RotateYaw(float yawDegree)
{
  float yawRadian = glm::radians(yawDegree);
  float rotYawArray[] =
  {
    cos(yawRadian), -sin(yawRadian),	0, 0,
    sin(yawRadian), cos(yawRadian),		0, 0,
    0,							0,								1, 0,
    0,							0,								0, 1,
  };
  return glm::make_mat4(rotYawArray);
}

static inline glm::mat4 RotatePitch(float pitchDegree)
{
  float pitchRadian = glm::radians(pitchDegree);
  float rotPitchArray[] =
  {
    cos(pitchRadian),	0, sin(pitchRadian), 0,
    0,								1, 0,								 0,
    -sin(pitchRadian),0, cos(pitchRadian), 0,
    0,								0, 0,								 1
  };
  return glm::make_mat4(rotPitchArray);
}

static inline glm::mat4 RotateRoll(float rollDegree)
{
  float rollRadian = glm::radians(rollDegree);
  float rotRollArray[] =
  {
    1, 0,								0,								0,
    0, cos(rollRadian), -sin(rollRadian), 0,
    0, sin(rollRadian), cos(rollRadian),	0,
    0, 0,								0,								1
  };
  return glm::make_mat4(rotRollArray);
}

void Shape::Rotate(float yawDegree, float pitchDegree, float rollDegree)
{
  rotation = RotateYaw(yawDegree)*RotatePitch(pitchDegree)*RotateRoll(rollDegree);
}

void Shape::Translate(float x, float y, float z)
{
  float translateMat[]
  {
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1
  };
  translation = glm::make_mat4(translateMat);
}

const glm::mat4 Shape::GetModelMatrix()
{
  return translation * rotation*scale;
}

const void* const Shape::VERTEX_OFFSET_POINTER = (void*)0;
const void* const Shape::TEXTURE_OFFSET_POINTER = (void*)(VERTEX_UNITS * sizeof(float));
const void* const Shape::NORMAL_OFFSET_POINTER = (void*)((VERTEX_UNITS + TEXTURE_UNITS) * sizeof(float));
