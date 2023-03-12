#include <Application.hpp>
#include <sdl.hpp>
#include <imgui_impl_sdl2.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application::Application(const char* name, int width, int height): e(Event::get()) {
    WIDTH = width;
    HEIGHT = height;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    context = SDL_GL_CreateContext(window);

    // Vsync 0: off, 1: on
    SDL_GL_SetSwapInterval(1);
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    imgui_renderer.instantiate(window, context);

    e.addAction(SDL_SCANCODE_W, [this](){
        camera.ProcessKeyboard(GL::CameraMovement::FORWARD, deltatime);
    });
    e.addAction(SDL_SCANCODE_S, [this](){
        camera.ProcessKeyboard(GL::CameraMovement::BACKWARD, deltatime);
    });
    e.addAction(SDL_SCANCODE_A, [this](){
        camera.ProcessKeyboard(GL::CameraMovement::LEFT, deltatime);
    });
    e.addAction(SDL_SCANCODE_D, [this](){
        camera.ProcessKeyboard(GL::CameraMovement::RIGHT, deltatime);
    });

    setup_uniform_buffer();
    preset();
}

Application& Application::get(const char* name, int width, int height) {
    static Application application(name, width, height);
    return application;
}

void Application::run() {
    while (!done) {
        // TODO: extract into separate event system
        e.doEvent(done, deltatime);
        update_preset();
        update_uniform_buffer();

        renderer.draw();
        imgui_renderer.render(renderer.objects);

        SDL_GL_SwapWindow(window);
        uint32_t curtick = SDL_GetTicks64();
        deltatime = curtick - tick;
        tick = curtick;
    }
}

void Application::setup_uniform_buffer() {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-(WIDTH / 2.0f), WIDTH / 2.0f, HEIGHT / 2.0f, -(HEIGHT / 2.0f), -1.0f, 1.0f);
    glGenBuffers(1, &ubo); // WARN: Memory leak.
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Application::update_uniform_buffer() {
    glm::mat4 view = camera.GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Application::preset() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::update_preset() {
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    WIDTH = (int)io.DisplaySize.x;
    HEIGHT = (int)io.DisplaySize.y;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Application::~Application() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
