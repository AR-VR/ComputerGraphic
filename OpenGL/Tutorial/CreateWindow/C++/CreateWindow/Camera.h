#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
private:
  glm::mat4 projection;
  glm::mat4 worldToCameraMatrix;
  glm::vec3 cameraPosition;
public:
	Camera();
	~Camera();
  const glm::mat4 PerspectiveProjection(float fovyDegree, float aspect, float zNear, float zFar);
	const glm::mat4 OrthoGraphicProjection(float left, float right, float top, float bottom, float far, float near);
  const glm::mat4 LookAt(glm::vec3 camPosition, glm::vec3 center, glm::vec3 upDirection);
  void AdvanceProjection(glm::mat4 projectionMatrix);

  const glm::vec3 GetCameraPosition() { return cameraPosition; }
  const glm::mat4 GetProjectionMatrix() { return projection; }
  const glm::mat4 GetViewMatrix() { return worldToCameraMatrix; }
};

