#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace rpg::window {
struct keyboard_input {
  [[nodiscard]] static bool is_key_pressed(const sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
  }
};
} // namespace rpg::window