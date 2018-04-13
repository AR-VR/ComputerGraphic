#pragma once
#include <string>

class Shader
{
public:
	Shader(const char*vertexShaderSrc, const char* fragmentShaderSrc);
	void useProgram();
	void setUniformBool(const std::string &name, bool value) const;
	void setUniformInt(const std::string &name, int value) const;
	void setUniformFloat(const std::string &name, float value) const;
	~Shader();

private:
	unsigned int glProgramID;
};

