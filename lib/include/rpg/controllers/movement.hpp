#pragma once

#include <rpg/action.hpp>
#include <rpg/math.hpp>
#include <rpg/window/key_position.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <boost/container/flat_map.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#if defined(RPG_DEBUG) and not defined(RPG_TESTING)
#include <format>
#endif
#include <functional>
#include <optional>

namespace rpg::controllers {

template <class TInput, class TSpeed> class movement {

  std::reference_wrapper<TInput> input_;
  std::reference_wrapper<const TSpeed> speed_;
  std::optional<std::reference_wrapper<sf::Transformable>> transformable_;
  sf::Vector2f direction_;
  boost::container::flat_map<rpg::action, sf::Keyboard::Key> action_map_;

  bool should_do_action(const auto action) const {
    if (const auto iter = action_map_.find(action);
        iter != std::cend(action_map_)) {
      return window::is_down(input_.get().get_key_state(iter->second).position);
    }
    return false;
  }

public:
  movement(TInput &input, const TSpeed &speed) : input_(input), speed_(speed) {}

  auto attach(auto &transformable) {
    transformable_ = transformable;
    direction_.x = 1.0f;
    direction_.y = 0.0f;
  }

  auto detach() { transformable_.reset(); }
  [[nodiscard]] auto is_attached() const noexcept {
    return transformable_.has_value();
  }

  auto map_action(const rpg::action action, const auto key) {
    action_map_[action] = key;
    auto &input = input_.get();
    input.subscribe(key);
  }

  auto update(const auto &delta_time) {
    if (not transformable_) {
      return;
    }
    auto &transformable = (*transformable_).get();
    auto &speed = speed_.get();

    bool lateral_movement_performed = false;

    if (should_do_action(action::move_right)) {
      transformable.move(math::ortho(direction_) * speed.lateral_movement() *
                         delta_time.asSeconds());
      lateral_movement_performed = true;
    }

    if (should_do_action(action::move_left)) {
      transformable.move(math::ortho(direction_) * -speed.lateral_movement() *
                         delta_time.asSeconds());
      lateral_movement_performed = true;
    }

    if (should_do_action(action::rotate_right)) {
      transformable.rotate(speed.rotational_movement() *
                           delta_time.asSeconds());
      direction_ = math::rotate_vector(transformable.getRotation());
    }

    if (should_do_action(action::rotate_left)) {
      transformable.rotate(-speed.rotational_movement() *
                           delta_time.asSeconds());
      direction_ = math::rotate_vector(transformable.getRotation());
    }

    if (not lateral_movement_performed and
        should_do_action(action::move_forward)) {
      transformable.move(direction_ * speed.frontal_movement() *
                         delta_time.asSeconds());
    }

    if (not lateral_movement_performed and
        should_do_action(action::move_backward)) {
      transformable.move(direction_ * -speed.backward_movement() *
                         delta_time.asSeconds());
    }

    // if (input.is_key_pressed(sf::Keyboard::Key::E)) {
    //   const auto rotational_movment_speed =
    //       speed.rotational_movement() * delta_time.asSeconds();
    //   transformable.rotate(rotational_movment_speed);
    //   direction_ = math::rotate_vector(transformable.getRotation());
    // }

    // if (input.is_key_pressed(sf::Keyboard::Key::Q)) {
    //   const auto rotational_movment_speed =
    //       speed.rotational_movement() * delta_time.asSeconds();
    //   transformable.rotate(-rotational_movment_speed);
    //   direction_ = math::rotate_vector(transformable.getRotation());
    // }

    // if (input.is_key_pressed(sf::Keyboard::Key::W)) {
    //   transformable.move(direction_ * speed.frontal_movement() *
    //                      delta_time.asSeconds());
    // }

    // if (input.is_key_pressed(sf::Keyboard::Key::S)) {
    //   transformable.move(direction_ * -speed.backward_movement() *
    //                      delta_time.asSeconds());
    // }

    // if (input.is_key_pressed(sf::Keyboard::Key::D)) {
    //   const auto movement_speed = direction_.y < 0 ? speed.lateral_movement()
    //                                                :
    //                                                -speed.lateral_movement();
    //   transformable.move(math::ortho(direction_) * movement_speed *
    //                      delta_time.asSeconds());
    // }

    // if (input.is_key_pressed(sf::Keyboard::Key::A)) {
    //   const auto movement_speed = direction_.y < 0 ?
    //   -speed.lateral_movement()
    //                                                :
    //                                                speed.lateral_movement();
    //   transformable.move(math::ortho(direction_) * movement_speed *
    //                      delta_time.asSeconds());
    // }

#if defined(RPG_DEBUG) and not defined(RPG_TESTING)
    ImGui::Begin("Movement");

    const auto text =
        std::format("direction is ({}, {})", direction_.x, direction_.y);
    ImGui::TextUnformatted(text.c_str());

    float rotation = transformable.getRotation();
    ImGui::InputFloat("Rotation", &rotation);
    transformable.setRotation(rotation);
    direction_ = math::rotate_vector(transformable.getRotation());

    {
      float vector[] = {transformable.getPosition().x,
                        transformable.getPosition().y};
      ImGui::InputFloat2("Position", vector);
      transformable.setPosition(vector[0], vector[1]);
    }

    {
      float vector[] = {transformable.getScale().x, transformable.getScale().y};
      ImGui::InputFloat2("Scale", vector);
      transformable.setScale(vector[0], vector[1]);
    }

    ImGui::End();
#endif
  }
};

} // namespace rpg::controllers