#pragma once
#include <SDL2/SDL_events.h>
#include <functional>

class Mouse {
    Mouse() {};
public:
    int x, y;
    bool left, right, middle;

    static Mouse& get() {
        static Mouse mouse;
        return mouse;
    }

    void refresh();

    void handle(SDL_Event& event);
};

class Keyboard {
    std::function<void()> keymap[SDL_NUM_SCANCODES];
public:
};

class Event {
    SDL_Event event;
    Mouse& mouse;
    std::function<void()> keymap[SDL_NUM_SCANCODES];

    Event();
public:

    static Event& get() {
        static Event e;
        return e;
    }

    void doEvent(bool& done, const float deltatime) ;

    void addAction(uint32_t keycode, std::function<void()> fn);
};
