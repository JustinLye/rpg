#pragma once

#include <rpg/guid.hpp>

#include <algorithm>
#include <cstring>

namespace rpg::operators {
inline guid operator""_guid(const char *cs, std::size_t n) {
  rpg::guid guid{};
  std::memcpy(std::data(guid), cs, std::min(n, sizeof(guid)));
  return guid;
}
} // namespace rpg::operators