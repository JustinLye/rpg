#pragma once

#include <cstdint>

namespace rpg::window {
enum class key_position : std::uint8_t { unknown, pressed, down, released, up };
}