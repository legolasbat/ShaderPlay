#include "shaderController.h"

#include <string.h>

bool ShaderController::LoadNewShader(const char* vertexShaderData, const char* fragmentShaderData)
{
	Shader tempShader;

	bool result = tempShader.loadShaderProgramFromData(vertexShaderData, fragmentShaderData);

	if (!result) {
		currentShader.errorMessage = tempShader.errorMessage;
		tempShader.clear();
		return result;
	}

	currentShader.clear();

	currentShader.id = tempShader.id;

	u_resolution = currentShader.getUniformLocation("iResolution");
	u_time = currentShader.getUniformLocation("iTime");
	u_frame = currentShader.getUniformLocation("iFrame");
	u_mouse = currentShader.getUniformLocation("iMouse");

	return result;
}

Shader ShaderController::GetCurrentShader() const
{
	return currentShader;
}

std::string ShaderController::GetError() {
	return currentShader.GetErrorMessage();
}
