#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace glstd {
using namespace glm;

constexpr vec3 UP    = vec3(0.0, 1.0, 0.0);
constexpr vec3 DOWN  = -UP;
constexpr vec3 FRONT = vec3(0.0, 0.0, -1.0);
constexpr vec3 BACK  = -FRONT;
constexpr vec3 RIGHT = vec3(1.0, 0.0, 0.0);
constexpr vec3 LEFT  = -RIGHT;

constexpr vec3 cube_vertices[8] = {
    UP + LEFT + FRONT, UP + RIGHT + FRONT, // 0, 1
    DOWN + LEFT + FRONT, DOWN + RIGHT + FRONT, // 2, 3
    UP + LEFT + BACK, UP + RIGHT + BACK, // 4, 5
    DOWN + LEFT + BACK, UP + RIGHT + BACK // 6, 7
};

const std::vector<uint32_t> cube_indices = {
    6, 5, 4,
};

constexpr vec3 plane_vertices[4] = {
    UP + LEFT, UP + RIGHT,
    DOWN + LEFT, DOWN + RIGHT
};
}
