#include "../include/sdl.hpp"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

namespace utils {
    /*
    void framebuffer_size_callback(SDL_Window* window, int width, int height) {
        glViewport(0, 0, width, height);
    }*/
}

namespace SDL {
    void Mouse::refresh() {
		SDL_GetMouseState(&x, &y);
    }

    void flip(SDL_Surface* surface) {
        SDL_LockSurface(surface);
        int pitch = surface->pitch;
        char* temp = new char[pitch];
        char* pixels = (char*) surface->pixels;

        for (int i = 0; i < surface->h / 2; ++i) {
            char* row1 = pixels + i * pitch;
            char* row2 = pixels + (surface->h - i - 1) * pitch;

            memcpy(temp, row1, pitch);
            memcpy(row1, row2, pitch);
            memcpy(row2, temp, pitch);
        }
        delete[] temp;
        SDL_UnlockSurface(surface);
    }

    uint32_t loadTexture(const char* path, bool flip) {
        uint32_t textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        } else std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return textureID;
    }

    uint32_t loadCubemap(std::vector<std::string>& faces) {
        uint32_t textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data;
        for (uint32_t i = 0; i < faces.size(); ++i) {
            data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
}
