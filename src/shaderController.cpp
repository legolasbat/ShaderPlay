#include "shaderController.h"

#include <string.h>

bool ShaderController::LoadNewShader(const char* vertexShaderData, const char* fragmentShaderData)
{
	Shader tempShader;

	bool result = tempShader.loadShaderProgramFromData(vertexShaderData, fragmentShaderData);

	if (!result) {
		strcpy(currentShader.errorMessage, tempShader.errorMessage);
		tempShader.clear();
		return result;
	}

	currentShader.clear();

	currentShader.id = tempShader.id;

	u_resolution = currentShader.getUniformLocation("u_resolution");
	u_time = currentShader.getUniformLocation("u_time");
	u_color = currentShader.getUniformLocation("u_color");

	return result;
}

Shader ShaderController::GetCurrentShader() const
{
	return currentShader;
}

const char* ShaderController::GetError() {
	return currentShader.GetErrorMessage();
}
