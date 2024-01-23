#pragma once

#include <rpg/math.hpp>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#if defined(RPG_DEBUG) and not defined(RPG_TESTING)
#include <format>
#endif
#include <functional>
#include <optional>

namespace rpg::controllers {

template <class TInput, class TSpeed> class movement {

  std::reference_wrapper<const TInput> input_;
  std::reference_wrapper<const TSpeed> speed_;
  std::optional<std::reference_wrapper<sf::Transformable>> transformable_;
  sf::Vector2f direction_;

public:
  movement(const TInput &input, const TSpeed &speed)
      : input_(input), speed_(speed) {}

  auto attach(auto &transformable) {
    transformable_ = transformable;
    direction_.x = 1.0f;
    direction_.y = 0.0f;
  }

  auto detach() { transformable_.reset(); }
  [[nodiscard]] auto is_attached() const noexcept {
    return transformable_.has_value();
  }

  auto update(const auto &delta_time) {
    auto &transformable = (*transformable_).get();
    auto &input = input_.get();
    auto &speed = speed_.get();

    if (input.is_key_pressed(sf::Keyboard::Key::E)) {
      const auto rotational_movment_speed =
          speed.rotational_movement() * delta_time.asSeconds();
      transformable.rotate(rotational_movment_speed);
      direction_ = math::rotate_vector(transformable.getRotation());
    }

    if (input.is_key_pressed(sf::Keyboard::Key::Q)) {
      const auto rotational_movment_speed =
          speed.rotational_movement() * delta_time.asSeconds();
      transformable.rotate(-rotational_movment_speed);
      direction_ = math::rotate_vector(transformable.getRotation());
    }

    if (input.is_key_pressed(sf::Keyboard::Key::W)) {
      transformable.move(direction_ * speed.frontal_movement() *
                         delta_time.asSeconds());
    }

    if (input.is_key_pressed(sf::Keyboard::Key::S)) {
      transformable.move(direction_ * -speed.backward_movement() *
                         delta_time.asSeconds());
    }

    if (input.is_key_pressed(sf::Keyboard::Key::D)) {
      const auto movement_speed = direction_.y < 0 ? speed.lateral_movement()
                                                   : -speed.lateral_movement();
      transformable.move(math::ortho(direction_) * movement_speed *
                         delta_time.asSeconds());
    }

    if (input.is_key_pressed(sf::Keyboard::Key::A)) {
      const auto movement_speed = direction_.y < 0 ? -speed.lateral_movement()
                                                   : speed.lateral_movement();
      transformable.move(math::ortho(direction_) * movement_speed *
                         delta_time.asSeconds());
    }

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