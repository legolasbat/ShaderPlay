#pragma once
#include <glad/glad.h>

#include "shader.h"

struct ShaderController {
	Shader currentShader;

	GLint u_resolution = 0;
	GLint u_time = 0;
	GLint u_timeDelta = 0;
	GLint u_frameRate = 0;
	GLint u_frame = 0;
	GLint u_mouse = 0;
	GLint u_texture0 = 0;
	GLint u_texture1 = 0;
	GLint u_texture2 = 0;
	GLint u_texture3 = 0;
	GLint u_date = 0;

	bool LoadNewShader(const char* vertexShaderData,
		const char* fragmentShaderData);

	Shader GetCurrentShader() const;

	std::string GetError() const;
};
