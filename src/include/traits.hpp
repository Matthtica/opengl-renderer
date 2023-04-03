#pragma once
#include <vector>
#include <stdint.h>

namespace fgl {

namespace traits {

template <typename... Types>
constexpr std::vector<uint32_t> layout_of() {
  return {sizeof(Types)...};
}

} // namespace traits

} // namespace fgb
