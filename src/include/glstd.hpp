#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <ranges>
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <gl.hpp>

template<typename T>
constexpr std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
    std::vector<T> res;
    res.reserve(a.size() + b.size());
    res.insert(res.end(), a.begin(), a.end());
    res.insert(res.end(), b.begin(), b.end());
    return res;
}

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

constexpr std::vector<uint32_t> plane_index(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    return {a, c, b, c, d, b};
}

constexpr std::vector<uint32_t> cube_index() {
    return plane_index(0, 1, 2, 3) + plane_index(5, 4, 7, 6) +
        plane_index(1, 5, 3, 7) + plane_index(4, 0, 6, 2) +
        plane_index(4, 5, 0, 1) + plane_index(2, 3, 6, 7);
}

}

