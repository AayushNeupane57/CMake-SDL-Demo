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

using namespace std;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
void subFunc();

int main(int argc, char** argv)
{
	subFunc();
	auto image = make_unique<StageImage>();
	
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

	GLuint programID = LoadShaders("./assets/vertex_shader.vsh", "./assets/fragment_shader.fsh");
  GLuint matrixID = glGetUniformLocation(programID, "u_matrix");
  glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  while (running)
  {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			bool hasMouseFocus = SDL_GetWindowFlags(mainWindow.get()) & SDL_WINDOW_MOUSE_FOCUS;
			bool leftMouseButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				running = false;
			}
			// TODO: try getting button state from the event rather than querying MouseState
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
		bool show_test_window = true;
		ImGui_ImplSdlGL3_NewFrame(mainWindow.get());
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
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

void subFunc() {
	auto stage = make_unique<RenderStage>(1280, 720, "My Test Window");
	stage->Render();
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
    
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }
    
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
    
    
    
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }
    
    
    
    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }
    
    
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}


