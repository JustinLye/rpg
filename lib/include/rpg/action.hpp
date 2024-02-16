#pragma once
#include <cstdint>
namespace rpg {
enum class action : std::uint8_t {
  move_forward,
  move_backward,
  move_right,
  move_left,
  rotate_right,
  rotate_left
};
}