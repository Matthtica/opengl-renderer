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
        case SDL_MOUSEMOTION:
            x = event.motion.x;
            y = event.motion.y;
            printf("mouse moved: %d %d\n", x, y);
            break;
    }
}

Keyboard& Keyboard::get() {
    static Keyboard keyboard;
    return keyboard;
}

void Keyboard::addAction(uint32_t keycode, std::function<void()> fn) {
    keymap[keycode] = fn;
}

void Keyboard::handle(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        uint32_t code = event.key.keysym.scancode;
        if (keymap[code] != nullptr) keymap[code]();
        else printf("keycode: %d\n", code);
    }
}

Event::Event(): mouse(Mouse::get()), keyboard(Keyboard::get()) { }

Event& Event::get() {
    static Event e;
    return e;
}

void Event::doEvent(bool& done, const float deltatime) {
    mouse.refresh();
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            done = true;
        mouse.handle(event);
        keyboard.handle(event);
    }
    if (mouse.left) printf("left\n");
    if (mouse.right) printf("right\n");
}
