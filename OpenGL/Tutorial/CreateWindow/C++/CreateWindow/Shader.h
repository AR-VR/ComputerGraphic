#pragma once
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const char*vertexShaderSrc, const char* fragmentShaderSrc);
	void UseProgram();
  const unsigned int GetAttributeLocation(const std::string &name);
	void SetUniformBool(const std::string &name, const bool value);
	void SetUniformInt(const std::string &name, const int value);
	void SetUniformFloat(const std::string &name, const float value);
  void SetUniformMatrix4fv(const std::string &name, const glm::mat4 matrix);
  void SetUniform3fv(const std::string &name, const glm::vec3 vector);
	~Shader();

private:
	unsigned int glProgramID;
};

