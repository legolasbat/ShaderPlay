#pragma once

#include <SDL3/SDL.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include "planeMesh.h"
#include "shaderController.h"
#include "timer.h"
#include "textureLoader.h"

class ShaderPlay {
protected:
	SDL_Window* window;
	SDL_GLContext glContext;

	ShaderController shaderController;
	Plane mainPlane;

	Timer timer;
	TextureLoader textureLoader;

	const int WIDTH = 1280;
	const int HEIGHT = 720;

	bool showErrorMessageWidget = false;
	bool showTextureExplorer = false;
	int selectedTexture = -1;

	int activeTextures[4];
	bool activeTexturesFlip[4];

public:
	ShaderPlay();
	~ShaderPlay();
	void Run();

private:
	bool initSDLWindow();
	void initImGui();
	void initShaderController();

	void DrawImGui();
	void Draw();

	void imgui_pre_render();
	void imgui_post_render();

	void DrawChannelTexture(int index);
};
