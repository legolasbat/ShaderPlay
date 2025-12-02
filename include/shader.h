#pragma once
#include <glad/glad.h>

#include <string>

struct Shader
{
	GLuint id = 0;

	std::string errorMessage = "";

	bool loadShaderProgramFromFile(const char* vertexShaderPath,
		const char* fragmentShaderPath);

	bool loadShaderProgramFromData(const char* vertexShaderData,
		const char* fragmentShaderData);

	GLint createShaderFromData(const char* data, GLenum shaderType);

	void bind() const;

	void clear();

	GLint getUniformLocation(const char* name) const;

	std::string GetErrorMessage() const;
};
