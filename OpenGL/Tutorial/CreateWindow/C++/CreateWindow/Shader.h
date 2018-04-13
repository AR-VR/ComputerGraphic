#pragma once

class Shader
{
public:
	Shader(const char*vertexShaderSrc, const char* fragmentShaderSrc);
	void useProgram();
	~Shader();

private:
	unsigned int glProgramID;
};

