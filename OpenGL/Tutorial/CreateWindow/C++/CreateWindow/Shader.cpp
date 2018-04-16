#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "GLErrorCheck.h"

Shader::Shader(const char*vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexSrcString;
	std::string fragmentSrcString;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexSrcString = vShaderStream.str();
		fragmentSrcString = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return;
	}
	
	const char * vertexSrc = vertexSrcString.c_str();
	const char * fragmentSrc = fragmentSrcString.c_str();

	unsigned int vertexShader, fragmentShader;

	GL_EXEC(vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GL_EXEC(glShaderSource(vertexShader, 1, &vertexSrc, NULL));
	GL_COMPILE(vertexShader);

	GL_EXEC(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
	GL_EXEC(glShaderSource(fragmentShader, 1, &fragmentSrc, NULL));
	GL_COMPILE(fragmentShader);

	GL_EXEC(glProgramID = glCreateProgram());
	GL_EXEC(glAttachShader(glProgramID, vertexShader));
	GL_EXEC(glAttachShader(glProgramID, fragmentShader));
	GL_LINK(glProgramID);

	//Clean up shader after program linked, save memory
	GL_EXEC(glDetachShader(glProgramID, vertexShader));
	GL_EXEC(glDeleteShader(vertexShader));
	
	GL_EXEC(glDetachShader(glProgramID, fragmentShader));
	GL_EXEC(glDeleteShader(fragmentShader));
}

void Shader::UseProgram()
{
	GL_EXEC(glUseProgram(glProgramID));
}

const unsigned int Shader::GetAttributeLocation(const std::string &name)
{
  unsigned int attributeLocation;
  GL_EXEC(attributeLocation = glGetAttribLocation(glProgramID, name.c_str()));
  return attributeLocation;
}

void Shader::SetUniformBool(const std::string &name, const bool value)
{
	GLuint uniformLocation;
	GL_EXEC(uniformLocation= glGetUniformLocation(glProgramID, name.c_str()));
	GL_EXEC(glUniform1i(uniformLocation, (int)value));
}

void Shader::SetUniformInt(const std::string &name, const int value)
{
	GLuint uniformLocation;
	GL_EXEC(uniformLocation = glGetUniformLocation(glProgramID, name.c_str()));
	GL_EXEC(glUniform1i(uniformLocation, value));
}

void Shader::SetUniformFloat(const std::string &name, const float value)
{
	GLuint uniformLocation;
	GL_EXEC(uniformLocation = glGetUniformLocation(glProgramID, name.c_str()));
	GL_EXEC(glUniform1f(uniformLocation, value));
}

void Shader::SetUniformMatrix4fv(const std::string &name, const glm::mat4 matrix)
{
  GLuint uniformLocation;
  GL_EXEC(uniformLocation = glGetUniformLocation(glProgramID, name.c_str()));
  
  GL_EXEC(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
}

Shader::~Shader()
{
	//practice RAII approach here
	GL_EXEC(glDeleteProgram(glProgramID));
}
