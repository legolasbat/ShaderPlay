#pragma once
#include <glad/glad.h>

const int MAX_SIZE_MESSAGE = 2048;

struct Shader
{
	GLuint id = 0;

	char errorMessage[MAX_SIZE_MESSAGE] = "";

	bool loadShaderProgramFromFile(const char* vertexShaderPath,
		const char* fragmentShaderPath);

	bool loadShaderProgramFromData(const char* vertexShaderData,
		const char* fragmentShaderData);

	GLint createShaderFromData(const char* data, GLenum shaderType);

	void bind();

	void clear();

	GLint getUniformLocation(const char* name);

	const char* GetErrorMessage();
};
