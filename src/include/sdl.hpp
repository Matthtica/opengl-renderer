#pragma once
#include <SDL2/SDL_video.h>
#include <vector>
#include <string>

namespace SDL {
	struct Mouse {
		int x, y;
		void refresh();
	};

    void flip(SDL_Surface* surface);

    std::pair<SDL_Window*, SDL_GLContext> initNload(int width, int height);

    unsigned int loadTexture(const char* path, bool flip = true);

    uint32_t loadCubemap(std::vector<std::string>& faces);
}
