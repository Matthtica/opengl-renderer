#pragma once
#include <renderer.hpp>
#include <SDL2/SDL.h>
#include <ImGuiRenderer.hpp>
#include <sdl.hpp>
#include <mutex>
#include <Event.hpp>

class Application {
    SDL_Window* window;
    SDL_GLContext context;
    Event& e;

    GL::Camera camera;

    int WIDTH, HEIGHT;
    uint64_t tick;
    float deltatime = 0.0f;

    // TODO: Seperate things related to render in AbstractWorld class
    // TODO: Require seperate event system
    uint32_t ubo;

    void setup_uniform_buffer();

    void update_uniform_buffer();

    void preset();

    void update_preset();

    Application(const char* name, int width, int height);

    Application() = delete;
public:
    std::mutex data_mut;

    bool done = false;
    Renderer renderer;
    ImGuiRenderer imgui_renderer;

    static Application& get(const char* name, int width, int height);

    void run();

    ~Application();
};
