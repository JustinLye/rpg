#include <rpg/test/mocks/input.hpp>
#include <rpg/window/input.hpp>
#include <rpg/window/key_position.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <gtest/gtest.h>

TEST(window_input, key_state_is_pressed_the_first_time_its_pressed) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(true));

  rpg::window::input window_input{input};
  window_input.subscribe(sf::Keyboard::Key::A);
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  const auto &key_state = window_input.get_key_state(sf::Keyboard::Key::A);
  EXPECT_EQ(rpg::window::key_position::pressed, key_state.position);
}

TEST(window_input,
     key_state_is_down_after_key_is_pressed_on_subsequent_updates) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(2)
      .WillRepeatedly(::testing::Return(true));

  rpg::window::input window_input{input, sf::Keyboard::Key::A};
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  window_input.update(delta_time);
  const auto &key_state = window_input.get_key_state(sf::Keyboard::Key::A);
  EXPECT_EQ(rpg::window::key_position::down, key_state.position);
}

TEST(window_input,
     is_released_when_not_pressed_on_update_immediately_after_it_was_pressed) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(2)
      .WillOnce(::testing::Return(true))
      .WillOnce(::testing::Return(false));

  rpg::window::input window_input{input, sf::Keyboard::Key::A};
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  window_input.update(delta_time);
  const auto &key_state = window_input.get_key_state(sf::Keyboard::Key::A);
  EXPECT_EQ(rpg::window::key_position::released, key_state.position);
}

TEST(window_input, is_up_following_release) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(3)
      .WillOnce(::testing::Return(true))
      .WillOnce(::testing::Return(false))
      .WillOnce(::testing::Return(false));

  rpg::window::input window_input{input, sf::Keyboard::Key::A};
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  window_input.update(delta_time);
  window_input.update(delta_time);
  const auto &key_state = window_input.get_key_state(sf::Keyboard::Key::A);
  EXPECT_EQ(rpg::window::key_position::up, key_state.position);
}

TEST(window_input, position_remains_down) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(3)
      .WillRepeatedly(::testing::Return(true));

  rpg::window::input window_input{input, sf::Keyboard::Key::A};
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  EXPECT_EQ(rpg::window::key_position::pressed,
            window_input.get_key_state(sf::Keyboard::Key::A).position);
  window_input.update(delta_time);
  EXPECT_EQ(rpg::window::key_position::down,
            window_input.get_key_state(sf::Keyboard::Key::A).position);
  window_input.update(delta_time);
  EXPECT_EQ(rpg::window::key_position::down,
            window_input.get_key_state(sf::Keyboard::Key::A).position);
}

TEST(window_input, position_remains_up) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(3)
      .WillOnce(::testing::Return(true))
      .WillRepeatedly(::testing::Return(false));

  rpg::window::input window_input{input, sf::Keyboard::Key::A};
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  EXPECT_EQ(rpg::window::key_position::pressed,
            window_input.get_key_state(sf::Keyboard::Key::A).position);
  window_input.update(delta_time);
  EXPECT_EQ(rpg::window::key_position::released,
            window_input.get_key_state(sf::Keyboard::Key::A).position);
  window_input.update(delta_time);
  EXPECT_EQ(rpg::window::key_position::up,
            window_input.get_key_state(sf::Keyboard::Key::A).position);
}

TEST(window_input, track_time_in_state) {
  rpg::test::mocks::input input{};
  EXPECT_CALL(input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(8)
      .WillOnce(::testing::Return(true))
      .WillOnce(::testing::Return(true))
      .WillOnce(::testing::Return(true))
      .WillOnce(::testing::Return(false))
      .WillOnce(::testing::Return(false))
      .WillOnce(::testing::Return(false))
      .WillOnce(::testing::Return(true))
      .WillOnce(::testing::Return(true));

  rpg::window::input window_input{input, sf::Keyboard::Key::A};
  auto delta_time = sf::seconds(1.0f);
  window_input.update(delta_time);
  EXPECT_EQ(0.0f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);

  window_input.update(delta_time);

  EXPECT_EQ(1.0f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);

  delta_time = sf::seconds(0.5f);
  window_input.update(delta_time);

  EXPECT_EQ(1.5f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);

  window_input.update(delta_time);
  EXPECT_EQ(0.0f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);
  window_input.update(delta_time);
  EXPECT_EQ(0.5f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);
  window_input.update(delta_time);
  EXPECT_EQ(1.0f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);
  window_input.update(delta_time);
  EXPECT_EQ(0.0f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);
  window_input.update(delta_time);
  EXPECT_EQ(0.5f, window_input.get_key_state(sf::Keyboard::Key::A)
                      .seconds_in_current_position);
}