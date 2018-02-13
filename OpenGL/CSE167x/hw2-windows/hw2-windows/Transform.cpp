// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
    return ret;

		vec3 normalAxis = glm::normalize(axis);
		// YOUR CODE FOR HW1 HERE
		float k1 = normalAxis.x;
		float k2 = normalAxis.y;
		float k3 = normalAxis.z;
		mat3 crossPoductK = mat3(
			0, -k3, k2,
			k3, 0, -k1,
			-k2, k1, 0);

		float angleInRadian = glm::radians(degrees);
		mat3 identity = mat3(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1);

		ret = identity
			+ sin(angleInRadian)*crossPoductK
			+ (1 - cos(angleInRadian))*crossPoductK*crossPoductK;

		// You will change this return call
		return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
	mat3 rotation = rotate(-degrees, up);
	eye = rotation*eye;
	up = rotation*up;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.
	vec3 axis = glm::cross(eye, up);

	// YOUR CODE FOR HW1 HERE 
	mat3 rotation = rotate(-degrees, axis);
	eye = rotation*eye;
	up = rotation*up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
		mat4 translation = mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-eye.x, -eye.y, -eye.z, 1
			);
		vec3 eyeNorm = glm::normalize(eye);
		vec3 upNorm = glm::normalize(up);
		vec3 s = glm::cross(-eyeNorm, upNorm);
		vec3 u = glm::cross(glm::normalize(s), -eyeNorm);
		mat4 rotation = mat4(
			s[0], u[0], eyeNorm[0], 0,
			s[1], u[1], eyeNorm[1], 0,
			s[2], u[2], eyeNorm[2], 0,
			0, 0, 0, 1
			);
		ret = rotation*translation;
    // Likely the same as in HW 1.  
    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
		
		float t = zNear*glm::tan(glm::radians(fovy)/2);
		float b = -t;
		float r = aspect*t;
		float l = -r;
		ret = mat4(
			2 * zNear / (r-l), 0, 0, 0,
			0, 2 * zNear / (t-b), 0, 0,
			0, 0, -(zFar + zNear) / (zFar - zNear), -1,
			0, 0, -2 * zFar*zNear / (zFar - zNear), 0
			);
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
