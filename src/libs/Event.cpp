#include <Event.hpp>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <imgui_impl_sdl2.h>
#include <SDL2/SDL_mouse.h>

void Event::doEvent(bool& done, const float deltatime) {
    mouse.refresh();
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            done = true;
        for (uint32_t i = 0; i < SDL_NUM_SCANCODES; ++i)
            if (keymap[i] != nullptr) keymap[i]();
    }
}

void Event::addAction(uint32_t index, std::function<void ()> fn) {
    keymap[index] = fn;
}
