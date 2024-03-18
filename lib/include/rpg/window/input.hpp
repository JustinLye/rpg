#pragma once

#include <rpg/window/key_position.hpp>
#include <rpg/window/key_state.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <boost/container/flat_map.hpp>

#include <functional>
#include <tuple>

namespace rpg::window {
template <class TInput> class input {
  std::reference_wrapper<TInput> input_;
  boost::container::flat_map<sf::Keyboard::Key, key_state> key_states_;

  [[nodiscard]] inline bool
  update(boost::container::flat_map<sf::Keyboard::Key, key_state>::value_type
             &value) {
    auto &input = input_.get();
    if (input.is_key_pressed(value.first)) {
      switch (value.second.position) {
      case key_position::pressed:
        value.second.position = key_position::down;
        return false;
      case key_position::unknown:
      case key_position::released:
      case key_position::up:
        value.second.position = key_position::pressed;
        return true;
      case key_position::down:
        return false;
      }
    } else {
      switch (value.second.position) {
      case key_position::released:
        value.second.position = key_position::up;
        return false;
      case key_position::unknown:
      case key_position::pressed:
      case key_position::down:
        value.second.position = key_position::released;
        return true;
      case key_position::up:
        return false;
      }
    }
    return false;
  }

public:
  input(TInput &input, const auto... keys) : input_(input) {
    subscribe(keys...);
  };

  inline void subscribe(const auto key) {
    std::ignore = key_states_.insert({key, {}});
  }

  inline void unsubscribe(const auto key) {
    std::ignore = key_states_.erase(key);
  }

  inline void subscribe(const auto... keys) { (subscribe(keys), ...); }

  inline void update(const auto delta_time) {
    const auto seconds_elapsed_in_last_frame = delta_time.asSeconds();
    for (auto &value : key_states_) {
      if (update(value)) {
        value.second.seconds_in_current_position = 0;
      } else {
        value.second.seconds_in_current_position +=
            seconds_elapsed_in_last_frame;
      }
    }
  }

  inline const auto &get_key_state(const auto key) const {
    return key_states_.at(key);
  }
};

} // namespace rpg::window