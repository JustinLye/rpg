#include <rpg/action.hpp>
#include <rpg/controllers/movement.hpp>
#include <rpg/window/key_position.hpp>
#include <rpg/window/key_state.hpp>

#include <rpg/test/comparison.hpp>
#include <rpg/test/mocks/speed.hpp>
#include <rpg/test/mocks/window_input.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

template <template <class> class TMock>
class controllers_movement_impl : public testing::Test {
protected:
  TMock<rpg::test::mocks::window_input> test_input{};
  rpg::test::mocks::speed test_speed{};
  rpg::controllers::movement<rpg::test::mocks::window_input,
                             rpg::test::mocks::speed>
      movement_controller{test_input, test_speed};
  sf::Transformable transformable{};
};

using strick_controllers_movement =
    controllers_movement_impl<testing::StrictMock>;

using nice_controllers_movement = controllers_movement_impl<testing::NiceMock>;

TEST_F(strick_controllers_movement, can_attach_transform) {
  movement_controller.attach(transformable);
  EXPECT_TRUE(movement_controller.is_attached());
}

TEST_F(strick_controllers_movement, is_not_attached_before_attach_is_called) {
  EXPECT_FALSE(movement_controller.is_attached());
}

TEST_F(strick_controllers_movement, can_detach) {
  movement_controller.attach(transformable);
  movement_controller.detach();
  EXPECT_FALSE(movement_controller.is_attached());
}

TEST_F(strick_controllers_movement, can_move_forward) {
  rpg::window::key_state key_state{
      .position = rpg::window::key_position::pressed,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::W)).Times(1);
  EXPECT_CALL(test_speed, frontal_movement())
      .Times(2)
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(4.0f));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::W))
      .Times(2)
      .WillOnce(::testing::Return(key_state))
      .WillOnce(::testing::Return(key_state));

  movement_controller.map_action(rpg::action::move_forward,
                                 sf::Keyboard::Key::W);
  movement_controller.attach(transformable);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(1.0, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);

  movement_controller.update(sf::seconds(1.5f));
  EXPECT_EQ(7.0, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, can_move_backward) {
  rpg::window::key_state key_state{
      .position = rpg::window::key_position::pressed,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::S)).Times(1);
  EXPECT_CALL(test_speed, backward_movement())
      .Times(2)
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(4.0f));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::S))
      .Times(2)
      .WillRepeatedly(::testing::Return(key_state));

  movement_controller.map_action(rpg::action::move_backward,
                                 sf::Keyboard::Key::S);
  movement_controller.attach(transformable);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(-1.0, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);

  movement_controller.update(sf::seconds(2.5f));
  EXPECT_EQ(-11.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, move_lateral_right) {
  rpg::window::key_state key_state{
      .position = rpg::window::key_position::pressed,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::D)).Times(1);
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(2)
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(4.0f));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::D))
      .Times(2)
      .WillRepeatedly(::testing::Return(key_state));

  movement_controller.map_action(rpg::action::move_right, sf::Keyboard::Key::D);
  movement_controller.attach(transformable);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(1.0f, transformable.getPosition().y);

  movement_controller.update(sf::seconds(0.75f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(4.0f, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, move_lateral_left) {
  rpg::window::key_state key_state{
      .position = rpg::window::key_position::pressed,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::A)).Times(1);
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(2)
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(4.0f));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::A))
      .Times(2)
      .WillRepeatedly(::testing::Return(key_state));

  movement_controller.map_action(rpg::action::move_left, sf::Keyboard::Key::A);
  movement_controller.attach(transformable);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(-1.0f, transformable.getPosition().y);

  movement_controller.update(sf::seconds(0.75f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(-4.0f, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, lateral_movment_takes_precedence) {
  rpg::window::key_state key_state_down{
      .position = rpg::window::key_position::down,
      .seconds_in_current_position = 0,
  };

  rpg::window::key_state key_state_up{
      .position = rpg::window::key_position::up,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::W)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::S)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::A)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::D)).Times(1);

  EXPECT_CALL(test_speed, lateral_movement())
      .Times(4)
      .WillRepeatedly(::testing::Return(1.0f));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::A))
      .Times(4)
      .WillOnce(::testing::Return(key_state_up))
      .WillOnce(::testing::Return(key_state_down))
      .WillOnce(::testing::Return(key_state_up))
      .WillOnce(::testing::Return(key_state_down));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::D))
      .Times(4)
      .WillOnce(::testing::Return(key_state_down))
      .WillOnce(::testing::Return(key_state_up))
      .WillOnce(::testing::Return(key_state_down))
      .WillOnce(::testing::Return(key_state_up));

  movement_controller.map_action(rpg::action::move_forward,
                                 sf::Keyboard::Key::W);
  movement_controller.map_action(rpg::action::move_backward,
                                 sf::Keyboard::Key::S);
  movement_controller.map_action(rpg::action::move_left, sf::Keyboard::Key::A);
  movement_controller.map_action(rpg::action::move_right, sf::Keyboard::Key::D);

  movement_controller.attach(transformable);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(1.0f, transformable.getPosition().y);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(1.0f, transformable.getPosition().y);

  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, rotate_right) {
  rpg::window::key_state key_state_down{
      .position = rpg::window::key_position::down,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::E)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::W)).Times(1);
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(3)
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(2.0f))
      .WillOnce(::testing::Return(1.0f));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::E))
      .Times(3)
      .WillRepeatedly(::testing::Return(key_state_down));

  movement_controller.map_action(rpg::action::rotate_right,
                                 sf::Keyboard::Key::E);
  movement_controller.attach(transformable);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(1.0f, transformable.getRotation());
  movement_controller.update(sf::seconds(2.0f));
  EXPECT_EQ(5.0f, transformable.getRotation());
  movement_controller.map_action(rpg::action::move_forward,
                                 sf::Keyboard::Key::W);
  movement_controller.update(sf::seconds(85.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, rotate_left) {
  rpg::window::key_state key_down{
      .position = rpg::window::key_position::down,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::Q)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::W)).Times(1);
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::Q))
      .Times(4)
      .WillRepeatedly(::testing::Return(key_down));
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(4)
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(1.0f))
      .WillOnce(::testing::Return(2.0f))
      .WillOnce(::testing::Return(1.0f));
  movement_controller.map_action(rpg::action::rotate_left,
                                 sf::Keyboard::Key::Q);
  movement_controller.attach(transformable);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(359.0f, transformable.getRotation());
  movement_controller.update(sf::seconds(2.0f));
  EXPECT_EQ(357.0f, transformable.getRotation());
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(355.0f, transformable.getRotation());
  movement_controller.map_action(rpg::action::move_forward,
                                 sf::Keyboard::Key::W);
  movement_controller.update(sf::seconds(85.0f));
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
}

TEST_F(strick_controllers_movement, lateral_movement_considers_orientation) {
  rpg::window::key_state key_down{
      .position = rpg::window::key_position::down,
      .seconds_in_current_position = 0,
  };
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::Q)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::D)).Times(1);
  EXPECT_CALL(test_input, subscribe(sf::Keyboard::Key::A)).Times(1);
  EXPECT_CALL(test_input, unsubscribe(sf::Keyboard::Key::D)).Times(1);
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::Q))
      .Times(4)
      .WillRepeatedly(::testing::Return(key_down));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::D))
      .Times(3)
      .WillRepeatedly(::testing::Return(key_down));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::A))
      .Times(1)
      .WillRepeatedly(::testing::Return(key_down));
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(4)
      .WillRepeatedly(::testing::Return(90.0f));
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(4)
      .WillRepeatedly(::testing::Return(1.0f));
  movement_controller.attach(transformable);
  movement_controller.map_action(rpg::action::rotate_left,
                                 sf::Keyboard::Key::Q);
  movement_controller.map_action(rpg::action::move_right, sf::Keyboard::Key::D);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(270.0f, transformable.getRotation());
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(270.0f, transformable.getRotation());
  EXPECT_EQ(1.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(180.0f, transformable.getRotation());
  EXPECT_EQ(1.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(180.0f, transformable.getRotation());
  EXPECT_EQ(1.0f, transformable.getPosition().x);
  EXPECT_EQ(-1.0f, transformable.getPosition().y);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(90.0f, transformable.getRotation());
  EXPECT_EQ(1.0f, transformable.getPosition().x);
  EXPECT_EQ(-1.0f, transformable.getPosition().y);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getRotation());
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(-1.0f, transformable.getPosition().y);
  movement_controller.clear_action(rpg::action::move_right);
  movement_controller.map_action(rpg::action::move_left, sf::Keyboard::Key::A);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getRotation());
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(-2.0f, transformable.getPosition().y);
}

TEST_F(nice_controllers_movement, cannot_move_laterally_if_rotating) {
  constexpr rpg::window::key_state key_down{
      .position = rpg::window::key_position::down,
      .seconds_in_current_position = 0,
  };
  constexpr rpg::window::key_state key_up{
      .position = rpg::window::key_position::up,
      .seconds_in_current_position = 0,
  };

  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::Q))
      .Times(2)
      .WillOnce(::testing::Return(key_down))
      .WillOnce(::testing::Return(key_up));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::D))
      .Times(2)
      .WillOnce(::testing::Return(key_down))
      .WillOnce(::testing::Return(key_up));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::E))
      .Times(2)
      .WillOnce(::testing::Return(key_up))
      .WillOnce(::testing::Return(key_down));
  EXPECT_CALL(test_input, get_key_state(sf::Keyboard::Key::A))
      .Times(2)
      .WillOnce(::testing::Return(key_up))
      .WillOnce(::testing::Return(key_down));
  EXPECT_CALL(test_speed, rotational_movement())
      .Times(2)
      .WillRepeatedly(::testing::Return(1.0f));
  EXPECT_CALL(test_speed, lateral_movement())
      .Times(2)
      .WillRepeatedly(::testing::Return(1.0f));
  movement_controller.attach(transformable);
  movement_controller.map_action(rpg::action::rotate_left,
                                 sf::Keyboard::Key::Q);
  movement_controller.map_action(rpg::action::rotate_right,
                                 sf::Keyboard::Key::E);
  movement_controller.map_action(rpg::action::move_right, sf::Keyboard::Key::D);
  movement_controller.map_action(rpg::action::move_left, sf::Keyboard::Key::A);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(359.0f, transformable.getRotation());
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
  movement_controller.update(sf::seconds(1.0f));
  EXPECT_EQ(0.0f, transformable.getRotation());
  EXPECT_EQ(0.0f, transformable.getPosition().x);
  EXPECT_EQ(0.0f, transformable.getPosition().y);
}

#if defined(RPG_OS_IS_WINDOWS)

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif