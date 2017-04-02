// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:
static inline float angleToRadian(const float degree)
{
	return degree * pi / 180.0;
}

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
	
  // You will change this return call
  return mat3();
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
	float angle = -angleToRadian(degrees);
	mat3 rotation = mat3
	( cos(angle),  0, sin(angle),
		0,					 1, 0,
	  -sin(angle), 0, cos(angle));
	eye = rotation*eye;
	up = rotation*up;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
	float angle = angleToRadian(degrees);
	mat3 rotation = mat3(
		1,	0,					0,
		0,	cos(angle), -sin(angle),
		0,  sin(angle), cos(angle)
	);
	eye = rotation*eye;
	up = rotation*up;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
	mat4 translation = mat4(
		1,			0,			0,			0,
		0,			1,			0,			0,
		0,			0,			1,			0,
		-eye.x, -eye.y, -eye.z, 1
	);
	vec3 eyeNorm = glm::normalize(eye);
	vec3 upNorm = glm::normalize(up);
	vec3 s = glm::cross(-eyeNorm, upNorm);
	vec3 u = glm::cross(glm::normalize(s), -eyeNorm);
	mat4 rotation = mat4(
		s[0],		u[0],		eyeNorm[0], 0,
		s[1],		u[1],		eyeNorm[1], 0,
		s[2],		u[2],		eyeNorm[2], 0,
		0,			0,			0,					1
	);
  // You will change this return call
	return rotation*translation;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
