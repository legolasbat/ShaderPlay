#include "shaderPlay.h"

#include <openglErrorReporting.h>

#include <iostream>
#include <string>

#include "shaderText.h"

ShaderPlay::ShaderPlay() {
	if (!initSDLWindow()) {
		return;
	}
	initImGui();

	initShaderController();
	mainPlane.init();
	textureLoader.LoadDefaultTextures();

	activeTextures[0] = -1;
	activeTextures[1] = -1;
	activeTextures[2] = -1;
	activeTextures[3] = -1;
}

void ShaderPlay::Run() {
	timer.Start();

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

		imgui_pre_render();

		DrawImGui();

		Draw();

		imgui_post_render();

		SDL_GL_SwapWindow(window);
	}
}

void ShaderPlay::Draw() {
	// Update window size
	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(window, &w, &h);
	glViewport(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT);
	shaderController.currentShader.bind();

	// Resolution
	if (shaderController.u_resolution != -1) {
		static float resolution[3] = { 0.0, 0.0, 1.0 };
		resolution[0] = w;
		resolution[1] = h;

		glUniform3fv(shaderController.u_resolution, 1, resolution);
	}

	// Time
	if (shaderController.u_time != -1) {
		glUniform1f(shaderController.u_time, timer.GetTime());
	}

	// TimeDelta
	if (shaderController.u_timeDelta != -1) {
		glUniform1f(shaderController.u_timeDelta, timer.GetTimeDelta());
	}

	// FrameRate
	if (shaderController.u_frameRate != -1) {
		glUniform1f(shaderController.u_frameRate, timer.GetFramerate());
	}

	// Frame
	if (shaderController.u_frame != -1) {
		glUniform1i(shaderController.u_frame, timer.GetFrame());
	}

	// Date
	if (shaderController.u_date != -1) {
		float date[4] = { 0 };
		auto current_date = timer.GetDate();
		date[0] = current_date.year;
		date[1] = current_date.month;
		date[2] = current_date.day;
		date[3] = current_date.time;
		glUniform4fv(shaderController.u_date, 1, date);
	}

	// Mouse
	if (shaderController.u_mouse != -1) {
		static float mouseState[4] = { 0 };
		float mx, my;
		auto buttons = SDL_GetMouseState(&mx, &my);

		if (buttons & SDL_BUTTON_LEFT) {
			// Mouse.xy = Position
			mouseState[0] = mx;
			mouseState[1] = my;
			mouseState[1] = h - mouseState[1];

			// Mouse.zw = Position last click
			// sign(z) = Hold
			// sign(w) = Press
			if (mouseState[2] <= 0.0f) {
				mouseState[2] = mouseState[0];
				mouseState[3] = mouseState[1];
			}
			else if (mouseState[3] > 0.0f) {
				mouseState[3] *= -1;
			}
		}
		else {
			if (mouseState[2] > 0.0f) {
				mouseState[2] *= -1;
			}
		}

		glUniform4fv(shaderController.u_mouse, 1, mouseState);
	}

	// Textures
	if (shaderController.u_texture0 != -1) {
		if (activeTextures[0] != -1) {
			textureLoader.GetTextures(activeTextures[0])->UseTexture(0);
			glUniform1i(shaderController.u_texture0, 0);
		}
	}
	if (shaderController.u_texture1 != -1) {
		if (activeTextures[1] != -1) {
			textureLoader.GetTextures(activeTextures[1])->UseTexture(1);
			glUniform1i(shaderController.u_texture1, 1);
		}
	}
	if (shaderController.u_texture2 != -1) {
		if (activeTextures[2] != -1) {
			textureLoader.GetTextures(activeTextures[2])->UseTexture(2);
			glUniform1i(shaderController.u_texture2, 2);
		}
	}
	if (shaderController.u_texture3 != -1) {
		if (activeTextures[0] != -1) {
			textureLoader.GetTextures(activeTextures[3])->UseTexture(3);
			glUniform1i(shaderController.u_texture3, 3);
		}
	}

	mainPlane.Draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderPlay::DrawImGui() {
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
	if (ImGui::CollapsingHeader("Texture Picker")) {
		// iChannel0
		DrawChannelTexture(0);
		ImGui::SameLine();
		// iChannel1
		DrawChannelTexture(1);
		// iChannel2
		DrawChannelTexture(2);
		ImGui::SameLine();
		// iChannel3
		DrawChannelTexture(3);
	}
	ImGui::End();

	if (showErrorMessageWidget) {
		ImGui::Begin("Error Message");
		ImGui::Text(shaderController.GetError().c_str());
		ImGui::End();
	}

	if (showTextureExplorer) {
		ImGui::Begin("Texture Explorer");

		if (ImGui::Button("Cancel")) {
			showTextureExplorer = false;
		}

		GLuint textureID = 0;
		int textureWidth = 0, textureHeight = 0;
		int imageWidth = 128, imageHeight = 128;
		for (int i = 0; i < numberDefaultTextures; i++) {
			textureLoader.GetTextures(i)->GetTextureID(&textureID);
			textureLoader.GetTextures(i)->GetTextureSize(&textureWidth, &textureHeight);
			ImGui::BeginGroup();
			ImGui::Text("size = %d x %d", textureWidth, textureHeight);
			ImGui::PushID(i);
			if (ImGui::ImageButton("DefaultTexture", (ImTextureID)(intptr_t)textureID, ImVec2(imageWidth, imageHeight))) {
				activeTextures[selectedTexture] = i;
				showTextureExplorer = false;
			}
			ImGui::PopID();
			ImGui::EndGroup();
			if ((i + 1) % 5 != 0 && i != numberDefaultTextures - 1) {
				ImGui::SameLine();
			}
		}

		ImGui::End();
	}
}

void ShaderPlay::DrawChannelTexture(int index) {
	GLuint textureID = 0;
	int textureWidth = 0, textureHeight = 0;
	int imageWidth = 256, imageHeight = 256;
	int activeTexture = activeTextures[index];
	if (activeTexture != -1) {
		textureLoader.GetTextures(activeTexture)->GetTextureID(&textureID);
		textureLoader.GetTextures(activeTexture)->GetTextureSize(&textureWidth, &textureHeight);
	}
	else {
		textureID = 0;
	}
	// iChannelX
	ImGui::BeginGroup();
	ImGui::Text("iChannel%d", index);
	ImGui::Text("size = %d x %d", textureWidth, textureHeight);
	if (activeTexture != -1) {
		ImGui::SameLine();
		ImGui::PushID(index);
		if (ImGui::Button("Remove Texture")) {
			activeTextures[index] = -1;
		}
		ImGui::PopID();
	}
	ImGui::PushID(index);
	if (ImGui::ImageButton("iChannel", (ImTextureID)(intptr_t)textureID, ImVec2(imageWidth, imageHeight))) {
		showTextureExplorer = true;
		selectedTexture = index;
	}
	ImGui::PopID();
	ImGui::EndGroup();
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

void ShaderPlay::imgui_post_render() {
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
}

ShaderPlay::~ShaderPlay() {
	shaderController.currentShader.clear();

	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
