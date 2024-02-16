#pragma once

#include <cstdint>

namespace rpg::window {
enum class key_position : std::uint8_t { unknown, pressed, down, released, up };

[[nodiscard]] inline bool
is_down(const rpg::window::key_position key_position) {
  return key_position == key_position::pressed or
         key_position == key_position::down;
}
[[nodiscard]] inline bool is_up(const rpg::window::key_position key_position) {
  return key_position == key_position::released or
         key_position == key_position::up;
}

} // namespace rpg::window