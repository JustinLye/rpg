#pragma once

#include <rpg/window/key_position.hpp>

namespace rpg::window {
struct key_state {
  key_position position;
  float seconds_in_current_position;
};
} // namespace rpg::window