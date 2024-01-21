#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <functional>
#include <optional>

namespace rpg::controllers {

template <class TInput, class TSpeed> class movement {

  std::reference_wrapper<const TInput> input_;
  std::reference_wrapper<const TSpeed> speed_;
  std::optional<std::reference_wrapper<sf::Transformable>> transformable_;

public:
  movement(const TInput &input, const TSpeed &speed)
      : input_(input), speed_(speed) {}

  auto attach(auto &transformable) { transformable_ = transformable; }
  auto detach() { transformable_.reset(); }
  [[nodiscard]] auto is_attached() const noexcept {
    return transformable_.has_value();
  }

  auto update(const auto &) {
    auto &transformable = (*transformable_).get();
    auto &input = input_.get();
    auto &speed = speed_.get();

    if (input.is_key_pressed(sf::Keyboard::Key::W)) {
      const auto frontal_movement_speed = speed.frontal_movement();
      sf::Vector2f direction{frontal_movement_speed, 0.0f};
      transformable.move(direction);
    }

    if (input.is_key_pressed(sf::Keyboard::Key::S)) {
      const auto backward_movement_speed = speed.backward_movement();
      transformable.move({-backward_movement_speed, 0.0f});
    }

    if (input.is_key_pressed(sf::Keyboard::Key::D)) {
      const auto lateral_movement_speed = speed.lateral_movement();
      transformable.move({0.0f, -lateral_movement_speed});
    }

    if (input.is_key_pressed(sf::Keyboard::Key::A)) {
      const auto lateral_movement_speed = speed.lateral_movement();
      transformable.move({0.0f, lateral_movement_speed});
    }

    if (input.is_key_pressed(sf::Keyboard::Key::E)) {
      const auto rotational_movment_speed = speed.rotational_movement();
      transformable.rotate(rotational_movment_speed);
    }

    if (input.is_key_pressed(sf::Keyboard::Key::Q)) {
      [[maybe_unused]] const auto rotational_movment_speed =
          speed.rotational_movement();
      transformable.rotate(-rotational_movment_speed);
    }
  }
};

} // namespace rpg::controllers