#include "Cube.h"

Cube::Cube()
{
  scale = glm::mat4();
  rotation = glm::mat4();
  translation = glm::mat4();
}

void Cube::Scale(float x, float y, float z)
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

void Cube::Rotate(float yawDegree, float pitchDegree, float rollDegree)
{
  rotation = RotateYaw(yawDegree)*RotatePitch(pitchDegree)*RotateRoll(rollDegree);
}

void Cube::Translate(float x, float y, float z)
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

const glm::mat4 Cube::GetModelMatrix()
{
  return translation*rotation*scale;
}

Cube::~Cube() {}

//     7		    4
// 3		    2
//
//
//     6		    5
// 0		    1
const float Cube::vertices[VERTICES_COUNT*ELEMENTS_PER_VERTEX] =
{
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,//0
   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,//1
   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,//2
   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,//2
  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,//3
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,//0

  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,//6
   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,//5
   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,//4
   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,//4
  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,//7
  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,//6

  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

  0.5f,   0.5f,  0.5f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
  0.5f,   0.5f, -0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
  0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
  0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
  0.5f,  -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
  0.5f,   0.5f,  0.5f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,-1.0f, 0.0f,
   0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,-1.0f, 0.0f,
   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,-1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,-1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,-1.0f, 0.0f,

  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f
};