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

void Shader::useProgram()
{
	GL_EXEC(glUseProgram(glProgramID));
}

void Shader::setUniformBool(const std::string &name, bool value) const
{
	GLuint uniformLocation;
	GL_EXEC(uniformLocation= glGetUniformLocation(glProgramID, name.c_str()));
	GL_EXEC(glUniform1i(uniformLocation, (int)value));
}

void Shader::setUniformInt(const std::string &name, int value) const
{
	GLuint uniformLocation;
	GL_EXEC(uniformLocation = glGetUniformLocation(glProgramID, name.c_str()));
	GL_EXEC(glUniform1i(uniformLocation, value));
}

void Shader::setUniformFloat(const std::string &name, float value) const
{
	GLuint uniformLocation;
	GL_EXEC(uniformLocation = glGetUniformLocation(glProgramID, name.c_str()));
	GL_EXEC(glUniform1f(uniformLocation, value));
}

Shader::~Shader()
{
	//practice RAII approach here
	GL_EXEC(glDeleteProgram(glProgramID));
}
