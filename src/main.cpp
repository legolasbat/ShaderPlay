#pragma once
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <openglErrorReporting.h>
#undef main

#include <iostream>

#include "shaderText.h"
#include "shaderController.h"

#pragma region imgui
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"
#pragma endregion

float triangleData[] = {
	// positions	colors
	1, 1, 0,		1, 0, 0,	// vertex 1
	-1, 1, 0,		0, 1, 0,	// vertex 2
	-1, -1, 0,		0, 0, 1,	// vertex 3
	1, -1, 0,		0, 0, 1		// vertex 4
};

unsigned short indices[] = {
	0, 1, 2,
	0, 2, 3
};

bool showErrorMessageWidget = false;

int main(int argc, char* argv[])
{
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// Create a window
	SDL_Window* window = SDL_CreateWindow("SDL Main Window", 680, 480,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Create OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Load OpenGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DestroyContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Enable OpenGL error reporting
	enableReportGlErrors();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.f;
		style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

	ImGui_ImplSDL3_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 460");

#pragma region vao

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

#pragma endregion

#pragma region buffer

	// Create buffer
	GLuint buffer = 0;
	glGenBuffers(1, &buffer);

	// Bind buffer and send data
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

	// Attribute 0: Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	// Attribute 1: Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));

#pragma endregion

#pragma region index buffer

	// Create buffer
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);

	// Bind buffer and send data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#pragma endregion

	glBindVertexArray(0);

#pragma region shader loading

	ShaderController shaderController;

	//shader.loadShaderProgramFromFile(RESOURCES_PATH "basic_shader.vert", RESOURCES_PATH "main_shader.frag");
	shaderController.LoadNewShader(vertShaderText, fragShaderText);

	shaderController.currentShader.bind();

#pragma endregion

	// Main event loop
	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
			{
				running = false;
			}
		}

#pragma region imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
#pragma endregion

		// Update window size
		int w = 0, h = 0;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		glClear(GL_COLOR_BUFFER_BIT);

		//ImGui::Begin("Color Selector");
		//static float color[3] = { 0.5, 0.5, 0.5 };
		//ImGui::ColorEdit3("Color: ", color);
		//ImGui::End();

		ImGui::Begin("Shader Editor");
		if (ImGui::Button("Reload Shader")) {
			std::cout << "Reloading..." << std::endl;
			if (!shaderController.LoadNewShader(vertShaderText, fragShaderText)) {
				showErrorMessageWidget = true;
				std::cout << shaderController.GetError() << std::endl;
			}
			else {
				showErrorMessageWidget = false;
			}
		}
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::InputTextMultiline("##ShaderText", fragShaderText, sizeof(fragShaderText), size);
		ImGui::End();

		if (showErrorMessageWidget) {
			ImGui::Begin("Error Message");
			ImGui::Text(shaderController.GetError());
			ImGui::End();
		}

		shaderController.currentShader.bind();

		static float resolution[3] = { 0.0, 0.0, 1.0 };
		resolution[0] = w;
		resolution[1] = h;

		glUniform3fv(shaderController.u_resolution, 1, resolution);
		glUniform1f(shaderController.u_time, (float)clock() / CLOCKS_PER_SEC);
		//glUniform3fv(shaderController.u_color, 1, color);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//view port stuff
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_GL_SwapWindow(window);
	}

	shaderController.currentShader.clear();

	glDeleteBuffers(1, &buffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &VAO);

	// Cleanup SDL
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
