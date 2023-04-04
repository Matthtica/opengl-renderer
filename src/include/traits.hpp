#pragma once
#include <vector>
#include <stdint.h>

namespace fgl {

namespace traits {

template <typename... Types>
constexpr std::vector<uint32_t> layout_of() {
  return {sizeof(Types)...};
}

template<typename T>
constexpr std::vector<uint32_t> layout_by(const std::vector<uint32_t>& in) {
    std::vector<uint32_t> res = in;
    for (uint32_t& i : res) i *= sizeof(T);
    return res;
}

} // namespace traits

} // namespace fgb
