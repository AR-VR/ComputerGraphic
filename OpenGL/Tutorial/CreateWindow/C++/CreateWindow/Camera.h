#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	~Camera();
  const glm::mat4 PerspectiveProjection(float fovyDegree, float aspect, float zNear, float zFar);
	const glm::mat4 OrthoGraphicProjection(float left, float right, float top, float bottom, float far, float near);
  const glm::mat4 LookAt(glm::vec3 cameraPosition, glm::vec3 center, glm::vec3 upDirection);
  void AdvanceProjection(glm::mat4 projectionMatrix);
  const glm::mat4 GetProjectionMatrix();
private:
	glm::mat4 projection;
	glm::mat4 worldToCameraMatrix;

};

