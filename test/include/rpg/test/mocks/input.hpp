#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <gmock/gmock.h>

namespace rpg::test::mocks {
struct input {
  MOCK_METHOD(bool, is_key_pressed, (const sf::Keyboard::Key),
              (const noexcept));
};
} // namespace rpg::test::mocks