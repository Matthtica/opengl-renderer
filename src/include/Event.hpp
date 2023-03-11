#pragma once
#include "sdl.hpp"
#include <SDL2/SDL_events.h>
#include <vector>
#include <functional>

class Event {
    SDL_Event event;
    SDL::Mouse mouse;
    std::function<void()> keymap[SDL_NUM_SCANCODES];
public:
    Event() {};

    void doEvent(bool& done, const float deltatime) ;

    void addAction(uint32_t index, std::function<void()> fn);
};
