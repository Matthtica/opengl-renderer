#include <Event.hpp>
#include <SDL2/SDL_video.h>
#include <imgui_impl_sdl2.h>
#include <SDL2/SDL_mouse.h>

void Mouse::refresh() {
    SDL_GetMouseState(&x, &y);
}

void Mouse::handle(SDL_Event& event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) left = true;
            if (event.button.button == SDL_BUTTON_RIGHT) right = true;
            if (event.button.button == SDL_BUTTON_MIDDLE) middle = true;
            printf("mouse button down: %d\n", event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) left = false;
            if (event.button.button == SDL_BUTTON_RIGHT) right = false;
            if (event.button.button == SDL_BUTTON_MIDDLE) middle = false;
            printf("mouse button up: %d\n", event.button.button);
            break;
    }
}

Event::Event(): mouse(Mouse::get()) { }

void Event::doEvent(bool& done, const float deltatime) {
    mouse.refresh();
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            done = true;
        if (event.type == SDL_KEYDOWN) {
            uint32_t code = event.key.keysym.scancode;
            if (keymap[code] != nullptr) keymap[code]();
        }
        mouse.handle(event);
        switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) done = true;
                break;
            case SDL_KEYDOWN:
                uint32_t code = event.key.keysym.scancode;
                if (keymap[code] != nullptr) keymap[code]();
                break;
        }
    }
    if (mouse.left) printf("left\n");
}

void Event::addAction(uint32_t keycode, std::function<void()> fn) {
    keymap[keycode] = fn;
}
