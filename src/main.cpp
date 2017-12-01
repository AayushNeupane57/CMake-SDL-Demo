#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
	#include <SDL_opengl.h>
#else                   // Include standard MacOS OpenGL headers
  #include <OpenGL/gl3.h>
#endif
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include "../src/render_stage.h"
#include "../src/stage_image.h"
#include "../src/gl_utils.h"
#include "../src/file_dialog.h"

using namespace std;

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL failed to initialize: " << SDL_GetError() << endl;
		return 1;
  }
	cout << "SDL initialized successfully" << endl;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	unique_ptr<SDL_Window, void(*)(SDL_Window*)> mainWindow( SDL_CreateWindow(
			"SDL2 OpenGL Demo",				// title
			SDL_WINDOWPOS_CENTERED,		// pos x
			SDL_WINDOWPOS_CENTERED,		// pos y
			1280,											// width
			720,											// height
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE), SDL_DestroyWindow);

	SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow.get());
  if (glContext == NULL)
  {
    cout << "OpenGL context failed to initialize" << endl;
  }
	// init glew, if platform requires it
#ifndef __APPLE__
  GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
			cout << "Error initializing GLEW: " << glewGetErrorString(glewError) << endl;
	}
#endif

	ImGui_ImplSdlGL3_Init(mainWindow.get());

	// Set UI fontface
	auto io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("./assets/Roboto-Regular.ttf", 16.0f);
	ImGui::GetStyle().FrameRounding = 0.0f;
	ImGui::GetStyle().WindowRounding = 0.0f;

	// Create and load image quad
	auto image = make_unique<StageImage>();
	image->SetImage("./assets/03_HiRes_upload.jpg");

	bool running = true;
  GLuint vertexArrayID;
  glGenVertexArrays(1, &vertexArrayID);
  glBindVertexArray(vertexArrayID);


  GLuint vertexBufferID;
	int size = image->GetVertexBuffer().size() * sizeof(GLfloat);
  glGenBuffers(1, &vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  glBufferData(
		GL_ARRAY_BUFFER,
		size,
		image->GetVertexBuffer().data(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	GLuint programID = GLUtils::CreateProgram("./assets/vertex_shader.vsh", "./assets/fragment_shader.fsh");
  GLuint matrixID = glGetUniformLocation(programID, "u_matrix");
  glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	bool show_test_window = false;
	std::string showHideButtonText = "Show ImGui Test Window";
  while (running)
  {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			bool hasMouseFocus = SDL_GetWindowFlags(mainWindow.get()) & SDL_WINDOW_MOUSE_FOCUS;
			bool leftMouseButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			auto io = ImGui::GetIO();

			if (event.type == SDL_QUIT) {
				running = false;
			}
			// don't process mouse events when interacting with imgui window
			if (!io.WantCaptureMouse) {
				if (event.type == SDL_MOUSEMOTION && leftMouseButtonDown)
				{
					if (hasMouseFocus) {
						cameraPosition.x += (event.motion.xrel) * (1.0f / image->CameraZoom);
						cameraPosition.y += (-event.motion.yrel) * (1.0f / image->CameraZoom);
					}
				}
				if (event.type == SDL_MOUSEWHEEL) {
					image->CameraZoom += (event.wheel.y * 0.01f);
					if (image->CameraZoom < 0) {
						image->CameraZoom = 0;
					}
				}
			}
		}
		ImGui_ImplSdlGL3_NewFrame(mainWindow.get());
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::Text("Test Text");
		if (ImGui::Button(showHideButtonText.c_str())) {
			show_test_window = !show_test_window;
			showHideButtonText = show_test_window ? "Hide ImGui Test Window" : "Show ImGui Test Window";
		};

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open")) {
					string filePath = FileDialog::openFile();
					cout << filePath << endl;
					if (filePath != "") {
						image->SetImage(filePath);
						// Update existing VBO
						glBufferSubData(
							GL_ARRAY_BUFFER,
							0,
							size,
							image->GetVertexBuffer().data()
						);
					}
        }
				ImGui::MenuItem("Save", NULL, false, false);
				ImGui::MenuItem("Save As", NULL, false, false);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (show_test_window) {
			ImGui::ShowTestWindow();
		}

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 srt_matrix = image->GetMatrix(cameraPosition, glm::vec2(1280, 720));
    glUseProgram(programID);
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &srt_matrix[0][0]);
		glBindTexture(GL_TEXTURE_2D, image->textureID);
		glBindVertexArray(vertexArrayID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    ImGui::Render();
    SDL_GL_SwapWindow(mainWindow.get());
    SDL_Delay(16);
  }

	// teardown
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	return 0;
}
