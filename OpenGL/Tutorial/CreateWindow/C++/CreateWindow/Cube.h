#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define VERTEX_UNITS 3
#define TEXTURE_UNITS 2
#define NORMAL_UNITS 3

class Cube
{
public:
	Cube();
	const glm::mat4 GetModelMatrix();
	void Scale(float x, float y, float z);
	void Rotate(float yawDegree, float pitchDegree, float rollDegree);
	void Translate(float x, float y, float z);
	~Cube();

  //Define in header, init in cpp file

  static const unsigned int VERTICES_COUNT = 36;
  static const unsigned int ELEMENTS_PER_VERTEX = VERTEX_UNITS + TEXTURE_UNITS + NORMAL_UNITS;
  static const float vertices[VERTICES_COUNT*ELEMENTS_PER_VERTEX];

private:
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::mat4 translation;
};

