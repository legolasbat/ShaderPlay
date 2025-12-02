#pragma once

#include <SDL3/SDL.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"

#include "planeMesh.h"
#include "shaderController.h"

class ShaderPlay {
protected:
	SDL_Window* window;
	SDL_GLContext glContext;

	ShaderController shaderController;
	Plane mainPlane;

	const int WIDTH = 1280;
	const int HEIGHT = 720;

public:
	ShaderPlay();
	~ShaderPlay();
	void Run();

private:
	bool initSDLWindow();
	void initImGui();
	void initShaderController();

	void imgui_pre_render();
};
