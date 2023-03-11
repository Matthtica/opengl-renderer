#pragma once
#include <SDL_video.h>
#include <imgui.h>
#include <functional>
#include <drawable.hpp>


class ImGuiRenderer {
    uint64_t tick = 0;
    float delta_time;
    // TODO: imgui configuration constants and init file loading require
    void begin() const;

    void end() const;

    void global_setting();

    void instantiate(SDL_Window* window, SDL_GLContext& context);

    ImGuiRenderer() {};
public:
    ImGuiRenderer(const ImGuiRenderer& ) = delete;
    ImGuiRenderer& operator=(const ImGuiRenderer&) = delete;
    ImGuiRenderer(ImGuiRenderer&&) = delete;
    ImGuiRenderer& operator=(ImGuiRenderer&&) = delete;

    void onupdate();

    void render(std::vector<Drawable*>& drawables);

    friend class Application;

    ~ImGuiRenderer();
};
