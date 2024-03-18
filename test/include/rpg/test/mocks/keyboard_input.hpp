#pragma once

#include <rpg/window/key_state.hpp>

#include <SFML/Window/Keyboard.hpp>

#include <gmock/gmock.h>

namespace rpg::test::mocks {
struct keyboard_input {
  MOCK_METHOD(bool, is_key_pressed, (const sf::Keyboard::Key), (const));
  MOCK_METHOD(void, subscribe, (const sf::Keyboard::Key));
  MOCK_METHOD(void, unsubscribe, (const sf::Keyboard::Key));
};
} // namespace rpg::test::mocks