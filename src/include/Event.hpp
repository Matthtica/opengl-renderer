#pragma once
#include <SDL2/SDL_events.h>
#include <functional>

// TODO: remove git repo from imgui becoz some file have been modified
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
    Keyboard() {};
public:
    static Keyboard& get();

    void addAction(uint32_t keycode, std::function<void()> fn);

    void handle(SDL_Event& event);
};

class Event {
    SDL_Event event;
    Mouse& mouse;
    Keyboard& keyboard;

    Event();
public:
    static Event& get();

    void doEvent(bool& done, const float deltatime) ;

    friend class Application;
};
