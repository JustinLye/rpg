#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <numbers>

namespace rpg::inline math {
constexpr auto _180_over_pi = 180.0 / std::numbers::pi_v<double>;

[[nodiscard]] inline auto degrees_to_radians(const auto degrees) {
  return degrees * std::numbers::pi_v<double> / 180.0;
}

[[nodiscard]] inline auto radians_to_degrees(const auto radians) {
  return radians * _180_over_pi;
}

[[nodiscard]] inline auto rotate_vector(const auto degrees) -> sf::Vector2f {
  const auto radians = static_cast<float>(degrees_to_radians(degrees));
  return {std::cos(radians), std::sin(radians)};
}

[[nodiscard]] inline auto ortho(const auto &vector) {
  auto result = vector;
  result.x = -vector.y;
  result.y = vector.x;
  return result;
}

} // namespace rpg::inline math