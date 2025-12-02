#pragma once
#include <glad/glad.h>

#include "shader.h"

struct ShaderController {
	Shader currentShader;

	GLint u_resolution = 0;
	GLint u_time = 0;
	GLint u_frame = 0;

	bool LoadNewShader(const char* vertexShaderData,
		const char* fragmentShaderData);

	Shader GetCurrentShader() const;

	std::string GetError();
};
