#include <rpg/controllers/movement.hpp>
#include <rpg/test/comparison.hpp>
#include <rpg/test/mocks/input.hpp>
#include <rpg/test/mocks/speed.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class controllers_movement : public testing::Test {
protected:
  rpg::test::mocks::input test_input{};
  rpg::test::mocks::speed test_speed{};
  rpg::controllers::movement<rpg::test::mocks::input, rpg::test::mocks::speed>
      movement_controller{test_input, test_speed};
  sf::Transformable transformable{};
};

TEST_F(controllers_movement, can_attach_transform) {
  movement_controller.attach(transformable);
  EXPECT_TRUE(movement_controller.is_attached());
}

TEST_F(controllers_movement, is_not_attached_before_attach_is_called) {
  EXPECT_FALSE(movement_controller.is_attached());
}

TEST_F(controllers_movement, can_detach) {
  movement_controller.attach(transformable);
  movement_controller.detach();
  EXPECT_FALSE(movement_controller.is_attached());
}

TEST_F(controllers_movement, can_move_forward) {
  EXPECT_CALL(test_speed, frontal_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(1.0, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, can_vary_frontal_movement_speed) {
  EXPECT_CALL(test_speed, frontal_movement())
      .Times(1)
      .WillOnce(::testing::Return(2.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(2.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
}

TEST_F(controllers_movement, can_move_backward) {
  EXPECT_CALL(test_speed, backward_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(-1.0, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, can_vary_speed_backward) {
  EXPECT_CALL(test_speed, backward_movement())
      .Times(1)
      .WillOnce(::testing::Return(2.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(-2.0, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, move_lateral_right) {
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(0.0, transformable.getPosition().x);
  EXPECT_EQ(-1.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, move_lateral_right_vary_speed) {
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(2.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(0.0, transformable.getPosition().x);
  EXPECT_EQ(-2.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, move_lateral_left) {
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(0.0, transformable.getPosition().x);
  EXPECT_EQ(1.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, move_lateral_left_vary_speed) {
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(2.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(0.0, transformable.getPosition().x);
  EXPECT_EQ(2.0, transformable.getPosition().y);
}

TEST_F(controllers_movement, rotate_right) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(1.0f, transformable.getRotation());
}

TEST_F(controllers_movement, rotate_right_vary_speed) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(2.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(2.0f, transformable.getRotation());
}

TEST_F(controllers_movement, rotate_left) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(true));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(359.0f, transformable.getRotation());
}

TEST_F(controllers_movement, rotate_left_vary_speed) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(2.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(true));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(358.0f, transformable.getRotation());
}

TEST_F(controllers_movement, rotation_dictates_direction) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_speed, frontal_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(0.99984769502,
                                       transformable.getPosition().x);
  rpg::test::comparision::expect_equal(0.01745240576565266,
                                       transformable.getPosition().y);
}

TEST_F(controllers_movement, frontal_movement_accounts_for_delta_time) {
  EXPECT_CALL(test_speed, frontal_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(0.5f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(0.5f, transformable.getPosition().x);
  rpg::test::comparision::expect_equal(0.0f, transformable.getPosition().y);
}

TEST_F(controllers_movement, backward_movement_accounts_for_delta_time) {
  EXPECT_CALL(test_speed, backward_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(0.5f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(-0.5f, transformable.getPosition().x);
  rpg::test::comparision::expect_equal(0.0f, transformable.getPosition().y);
}

TEST_F(controllers_movement, lateral_movement_right_accounts_for_delta_time) {
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(0.5f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(0.0f, transformable.getPosition().x);
  rpg::test::comparision::expect_equal(-0.5f, transformable.getPosition().y);
}

TEST_F(controllers_movement, lateral_movement_left_accounts_for_delta_time) {
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(0.5f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(0.0f, transformable.getPosition().x);
  rpg::test::comparision::expect_equal(0.5f, transformable.getPosition().y);
}

TEST_F(controllers_movement,
       rotational_movement_right_accounts_for_delta_time) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(false));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(0.5f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(0.5f, transformable.getRotation());
}

TEST_F(controllers_movement, rotational_movement_left_accounts_for_delta_time) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(true));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(0.5f);
  movement_controller.update(delta_time);
  rpg::test::comparision::expect_equal(359.5f, transformable.getRotation());
}

TEST_F(controllers_movement, move_lateral_right_accounts_for_orientation) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(90.0f));
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(true));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(270.0f, transformable.getRotation());
  EXPECT_EQ(1.0f, transformable.getPosition().x);
  rpg::test::comparision::expect_equal(0.0f, transformable.getPosition().y);
}

TEST_F(controllers_movement, move_lateral_left_accounts_for_orientation) {
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(1)
      .WillOnce(::testing::Return(90.0f));
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(1)
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::W))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::S))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::D))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::A))
      .Times(1)
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::E))
      .Times(1)
      .WillOnce(::testing::Return(false));
  EXPECT_CALL(test_input, is_key_pressed(sf::Keyboard::Key::Q))
      .Times(1)
      .WillOnce(::testing::Return(true));

  movement_controller.attach(transformable);
  const auto delta_time = sf::seconds(1.0f);
  movement_controller.update(delta_time);
  EXPECT_EQ(270.0f, transformable.getRotation());
  EXPECT_EQ(-1.0f, transformable.getPosition().x);
  rpg::test::comparision::expect_equal(0.0f, transformable.getPosition().y);
}