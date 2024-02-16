#pragma once

#include <rpg/window/key_state.hpp>

#include <SFML/Window/Keyboard.hpp>

#include <gmock/gmock.h>

namespace rpg::test::mocks {
struct window_input {
  MOCK_METHOD(const window::key_state, get_key_state, (const sf::Keyboard::Key),
              (const ref(&)));
  MOCK_METHOD(void, subscribe, (const sf::Keyboard::Key));
};
} // namespace rpg::test::mocks