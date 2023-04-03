#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <ranges>
#include <algorithm>
#include <concepts>
#include <type_traits>

namespace glstd {
using namespace glm;

constexpr vec3 UP    = vec3(0.0, 1.0, 0.0);
constexpr vec3 DOWN  = -UP;
constexpr vec3 FRONT = vec3(0.0, 0.0, -1.0);
constexpr vec3 BACK  = -FRONT;
constexpr vec3 RIGHT = vec3(1.0, 0.0, 0.0);
constexpr vec3 LEFT  = -RIGHT;

template<typename VertexType>
std::vector<float> flatten(const std::vector<VertexType>& vertex) {
    size_t m = sizeof(VertexType) * vertex.size() / sizeof(float);
    return std::vector<float>((float*)vertex.data(), (float*)vertex.data() + m);
}

}
