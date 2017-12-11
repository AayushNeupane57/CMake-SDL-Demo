#include "demo_scene.h"
#include "app_platform.h"
#include <stdio.h>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

#include "gl_utils.h"
#include "renderer.h"
#include "file_dialog.h"

void DemoScene::Initialize() {
  show_test_window = false;
  showHideButtonText = "Show ImGui Test Window";
    cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    image = std::make_unique<StageImage>();
    image->SetImage("./assets/03_HiRes_upload.jpg");
  
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
  
    programID = GLUtils::CreateProgram("./assets/vertex_shader.vsh", "./assets/fragment_shader.fsh");
    matrixID = glGetUniformLocation(programID, "u_matrix");
    cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		ImGui_ImplSdlGL3_Init(GetWindowPtr());
		auto io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("./assets/Roboto-Regular.ttf", 16.0f);
		ImGui::GetStyle().FrameRounding = 0.0f;
		ImGui::GetStyle().WindowRounding = 0.0f;
}

void DemoScene::Update() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        auto winPtr = GetWindowPtr();
        bool hasMouseFocus = SDL_GetWindowFlags(winPtr) & SDL_WINDOW_MOUSE_FOCUS;
        bool leftMouseButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
        ImGui_ImplSdlGL3_ProcessEvent(&event);
        auto io = ImGui::GetIO();
        
        //if (event.type == SDL_QUIT) {
        //    running = false;
        //}
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
}

void DemoScene::Render() {
  ImGui_ImplSdlGL3_NewFrame(GetWindowPtr());
  ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
  ImGui::Text("Test Text");
  if (ImGui::Button(showHideButtonText.c_str())) {
    show_test_window = !show_test_window;
    showHideButtonText = show_test_window ? "Hide ImGui Test Window" : "Show ImGui Test Window";
  };
  
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open")) {
        std::string filePath = FileDialog::openFile();
        std::cout << filePath << std::endl;
        if (filePath != "") {
          int size = 0;
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
  
  // draw image quad
  Renderer::DrawVertices(vertexArrayID, GL_TRIANGLES, 6);

	// Draw UI elements
	ImGui::Render();

	SDL_GL_SwapWindow(GetWindowPtr());
}
