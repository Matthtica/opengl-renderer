#include <ImGuiRenderer.hpp>
#include <SDL_timer.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <filesystem>

void ImGuiRenderer::instantiate(SDL_Window* window, SDL_GLContext& context) {
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

void ImGuiRenderer::begin() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiRenderer::end() const {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // TODO: Make Application singleton.
    /*
    Application& app = Application::Get();
    io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        //SDL_GLContext context;
        //SDL_Window* win = SDL_GL_GetCurrentWindow();
        //SDL_GL_MakeCurrent(win, currentContext);
    }*/
}

void ImGuiRenderer::global_setting() {
    uint64_t currentTick = SDL_GetTicks64();
    delta_time = currentTick - tick;
    tick = currentTick;
    ImGui::Begin("App");
    ImGui::Text("Application frame time: %.00f\t fps: %.00f", delta_time, 1000.0f / delta_time);
    ImGui::End();
}

void ImGuiRenderer::render(std::vector<Drawable*>& drawables) {
    begin();

    static bool status = true;
    ImGui::ShowDemoWindow(&status);

    for (const Drawable* node : drawables) node->drawImgui();

    end();
}

ImGuiRenderer::~ImGuiRenderer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
