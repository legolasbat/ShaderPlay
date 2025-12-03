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
	u_timeDelta = currentShader.getUniformLocation("iTimeDelta");
	u_frameRate = currentShader.getUniformLocation("iFrameRate");
	u_frame = currentShader.getUniformLocation("iFrame");
	u_mouse = currentShader.getUniformLocation("iMouse");
	u_texture0 = currentShader.getUniformLocation("iChannel0");
	u_texture1 = currentShader.getUniformLocation("iChannel1");
	u_texture2 = currentShader.getUniformLocation("iChannel2");
	u_texture3 = currentShader.getUniformLocation("iChannel3");
	u_date = currentShader.getUniformLocation("iDate");

	return result;
}

Shader ShaderController::GetCurrentShader() const
{
	return currentShader;
}

std::string ShaderController::GetError() const {
	return currentShader.GetErrorMessage();
}
