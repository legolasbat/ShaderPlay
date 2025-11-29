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

const int WIDTH = 1280;
const int HEIGHT = 720;

static void imgui_pre_render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

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
	SDL_Window* window = SDL_CreateWindow("SDL Main Window", WIDTH, HEIGHT,
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

	// Frame Buffer and Textures
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	GLuint frameTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &frameTexture);
	glBindTexture(GL_TEXTURE_2D, frameTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);

	GLuint depthTexture;
	glCreateTextures(GL_TEXTURE_2D, 1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(frameTexture, buffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

		// Update window size
		int w = 0, h = 0;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		glClear(GL_COLOR_BUFFER_BIT);

		imgui_pre_render();

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x / 2, viewport->Size.y));
		ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginTabBar("Shader Properties");
		ImGui::BeginTabItem("Text Editor");
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
		ImGui::EndTabItem();
		ImGui::EndTabBar();
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

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x + viewport->Size.x / 2, viewport->Pos.y));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x / 2, viewport->Size.y));
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		const ImVec2 mSize = { viewportPanelSize.x, viewportPanelSize.y };

		// add rendered texture to ImGUI scene window
		ImGui::Image((ImTextureID)(intptr_t)(frameTexture), mSize);

		ImGui::End();

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

	glDeleteTextures(1, &frameTexture);
	glDeleteTextures(1, &depthTexture);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteBuffers(1, &buffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &VAO);

	// Cleanup SDL
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
