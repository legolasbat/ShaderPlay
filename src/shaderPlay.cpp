#include "shaderPlay.h"

#include <openglErrorReporting.h>

#include <iostream>
#include <string>

#include "timer.h"
#include "shaderText.h"

ShaderPlay::ShaderPlay() {
	if (!initSDLWindow()) {
		return;
	}
	initImGui();

	initShaderController();
	mainPlane.init();
}

void ShaderPlay::Run() {
	Timer timer;
	bool showErrorMessageWidget = false;

	// Main event loop
	bool running = true;
	while (running)
	{
		timer.Step();

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
		SDL_GetWindowSizeInPixels(window, &w, &h);
		glViewport(0, 0, w, h);

		glClear(GL_COLOR_BUFFER_BIT);

		imgui_pre_render();

		ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoCollapse);
		if (ImGui::CollapsingHeader("Text Editor")) {
			if (ImGui::Button("Reload Shader")) {
				std::cout << "Reloading..." << std::endl;
				std::string fragShaderText = std::string(fragShaderHeader) + std::string(fragShaderBody) + std::string(fragShaderMain);
				if (!shaderController.LoadNewShader(vertShaderText, fragShaderText.c_str())) {
					showErrorMessageWidget = true;
					std::cout << shaderController.GetError() << std::endl;
				}
				else {
					showErrorMessageWidget = false;
					timer.Reset();
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset Time")) {
				timer.Reset();
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause")) {
				timer.Pause();
			}

			std::string timeData = "Time: " + std::to_string(timer.GetTime());
			std::string framerateData = "Framerate: " + std::to_string(timer.GetFramerate());
			std::string timerData = timeData + " | " + framerateData;
			ImGui::SameLine();
			ImGui::Text(timerData.c_str());
			ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::InputTextMultiline("##ShaderText", fragShaderBody, sizeof(fragShaderBody), size);
		}
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
		glUniform1f(shaderController.u_time, timer.GetTime());
		glUniform1i(shaderController.u_frame, timer.GetFrame());

		mainPlane.Draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//view port stuff
		ImGuiIO& io = ImGui::GetIO();
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
}

bool ShaderPlay::initSDLWindow() {
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// Create a window
	window = SDL_CreateWindow("ShaderPlay", WIDTH, HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	// Create OpenGL context
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	// Load OpenGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DestroyContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	// Enable OpenGL error reporting
	enableReportGlErrors();

	return true;
}

void ShaderPlay::initImGui() {
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
}

void ShaderPlay::initShaderController() {
	std::string fragShaderText = std::string(fragShaderHeader) + std::string(fragShaderBody) + std::string(fragShaderMain);
	shaderController.LoadNewShader(vertShaderText, fragShaderText.c_str());

	shaderController.currentShader.bind();
}

void ShaderPlay::imgui_pre_render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

ShaderPlay::~ShaderPlay() {
	shaderController.currentShader.clear();

	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
