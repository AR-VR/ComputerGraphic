#ifndef GLERRORCHECK_H
#define GLERRORCHECK_H

#include <glad/glad.h>
#include<iostream>

//Very useful light-weight error checking that helps debug OpenGL

void CheckOpenGLExecutionError(const char* stmt, const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << stmt << " " << error.c_str() << " | " << file << " (" << line << ")" << std::endl;
		abort();
	}
}

#ifdef _DEBUG
#define GL_EXEC(stmt) do { \
	stmt; \
	CheckOpenGLExecutionError(#stmt, __FILE__, __LINE__); \
} while (0)
#else
#define GL_EXEC(stmt) stmt
#endif


void CheckOpenGLCompileError(GLint shader, const char* fname, int line) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("OpenGL error at %s:%i\n", fname, line);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	} else {
		std::cout << "Shader Compile Succeed" << std::endl;
	}
}

#ifdef _DEBUG
#define GL_COMPILE(shader) do { \
	glCompileShader(shader); \
	CheckOpenGLCompileError(shader, __FILE__, __LINE__); \
} while (0)
#else
#define GL_COMPILE(shader) do { \
	glCompileShader(shader); \
} while (0)
#endif

void CheckOpenGLLinkError(GLint shaderProgram, const char* fname, int line) {
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("OpenGL error at %s:%i\n", fname, line);
		std::cout << "ERROR::SHADER PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	} else {
		std::cout << "Program Link Succeed" << std::endl;
	}
}

#ifdef _DEBUG
#define GL_LINK(program) do { \
	glLinkProgram(program); \
	CheckOpenGLLinkError(program, __FILE__, __LINE__); \
} while (0)
#else
#define GL_LINK(shader) do { \
	glLinkProgram(program); \
} while (0)
#endif

#endif
