#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace fgl {

void init_sdl2_with_gl(const char* name,int width, int height);

void init_imgui_with_gl();

void destroy_sdl2_with_gl();

void destroy_imgui();


namespace T {
using u32 = unsigned int;
using i32 = int;

}

}
