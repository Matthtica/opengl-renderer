#include <fgl.hpp>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace fgl {
SDL_Window* window = nullptr;
SDL_GLContext context;

void init_sdl2_with_gl(const char* name,int width, int height) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    if (!window) {
        printf("Window already created. \n Multiple invocations of init function\n");
        return;
    }
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    context = SDL_GL_CreateContext(window);

    // Vsync 0: off, 1: on
    SDL_GL_SetSwapInterval(1);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
}

void init_imgui_with_gl() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // WARN::
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF("../resources/Segoe-UI-Font/SEGOEUI.TTF", 18);
    io.Fonts->Build();
    io.FontDefault = io.Fonts->Fonts[0];
    ImGuiStyle& style = ImGui::GetStyle();
    style.IndentSpacing = 0;
    style.WindowPadding.x = 0;
    style.WindowPadding.y = 0;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(41.0f/255, 45.0f/255, 62.0f/255, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(199.0f/255, 146.0f/255, 234.0f/255, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.0f/255, 203.0f/255, 107.0f/255, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(255.0f/255, 203.0f/255, 107.0f/255, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(255.0f/255, 203.0f/255, 0.0f/255, 1.0f));
    /*
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }*/

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void destroy_sdl2_with_gl() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void destroy_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

}
