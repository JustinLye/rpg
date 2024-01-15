#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <docopt.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include <cmath>
#include <cstdint>
#include <functional>
#include <map>
#include <numbers>
#include <optional>
#include <string>

using movement_speed_t = float;
#if defined(RPG_DEBUG)
movement_speed_t base_frontal_movement_speed = 500.0f;
movement_speed_t base_lateral_movement_speed = 150.0f;
movement_speed_t movement_speed_boost = 1.0f;
#elif defined(BASE_FRONTAL_MOVEMENT_SPEED)
constexpr movement_speed_t base_frontal_movement_speed =
    BASE_FRONTAL_MOVEMENT_SPEED;
constexpr auto base_lateral_movement_speed = 150.0f;
auto movement_speed_boost = 1.0f;
#else
constexpr movement_speed_t base_frontal_movement_speed = 500.0f;
constexpr auto base_lateral_movement_speed = 150.0f;
auto movement_speed_boost = 1.0f;
#endif

struct cli_args {
  std::uint32_t width;
  std::uint32_t height;
  double scale;
  std::uint32_t frame_limit;
};

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

static constexpr auto usage = R"(
  RPG Game

  Usage:
    rpg-game [options]

  Options:
    -h --help                  Show this message
    --width=WIDTH              Screen width in pixels [default: 1920]
    --height=HEIGHT            Screen height in pixels [default: 1080]
    --scale=SCALE              Scale [default: 2]
    --frame-limit=FRAME LIMIT  Frame limit [default: 60]
)";

[[nodiscard]] inline auto parse_cli_args(int argc, char **argv) -> cli_args {
  std::map<std::string, docopt::value> args =
      docopt::docopt(usage, {std::next(argv), std::next(argv, argc)}, true,
                     "RPG Game 0.0.0.0");
  return {
      .width = static_cast<std::uint32_t>(args["--width"].asLong()),
      .height = static_cast<std::uint32_t>(args["--height"].asLong()),
      .scale = static_cast<double>(args["--scale"].asLong()),
      .frame_limit = static_cast<std::uint32_t>(args["--frame-limit"].asLong()),
  };
}

[[nodiscard]] auto should_close(const sf::Event &event) {
  if (event.type == sf::Event::Closed) {
    return true;
  }

  return event.type == sf::Event::KeyPressed and
         event.key.code == sf::Keyboard::Key::Escape;
}

inline namespace detail {
struct speeds {
  movement_speed_t frontal{base_frontal_movement_speed};
  movement_speed_t lateral;
  movement_speed_t boost;
};

class speed_controller {
  detail::speeds speeds;

public:
  inline auto update(const auto &delta_time) {
#if defined(RPG_DEBUG)
    ImGui::Begin("Speed Dialog");
    ImGui::InputFloat("Base Frontal Movement Speed",
                      &base_frontal_movement_speed);
    ImGui::InputFloat("Base Lateral Movement Speed",
                      &base_lateral_movement_speed);
    ImGui::InputFloat("Movement Speed Boost", &movement_speed_boost);
    ImGui::End();
#endif
    speeds.boost = movement_speed_boost * delta_time.asSeconds();
    speeds.frontal = base_frontal_movement_speed * speeds.boost;
    speeds.lateral = base_lateral_movement_speed * speeds.boost;
  }

  [[nodiscard]] inline auto frontal() const noexcept { return speeds.frontal; }

  [[nodiscard]] inline auto lateral() const noexcept { return speeds.lateral; }
};

class movement_controller {
  detail::speed_controller speed_controller;
  sf::Vector2f direction{1.0f, 0.0f};
  std::optional<std::reference_wrapper<sf::Transformable>> optional_sprite;

public:
  inline auto attach(auto &transformable) {
    optional_sprite.emplace(std::ref(transformable));
  }

  inline auto update(const auto &delta_time) {
    if (not optional_sprite) {
      return;
    }

    speed_controller.update(delta_time);

    const auto delta_time_as_seconds = delta_time.asSeconds();
    auto &sprite = (*optional_sprite).get();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
      sprite.rotate(250.0f * delta_time_as_seconds);
      direction = rotate_vector(sprite.getRotation());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
      sprite.rotate(-250.0f * delta_time_as_seconds);
      direction = rotate_vector(sprite.getRotation());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      sprite.move(ortho(direction) * speed_controller.lateral());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      sprite.move(ortho(direction) * -speed_controller.lateral());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      sprite.move(direction * speed_controller.frontal());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      sprite.move(direction * -speed_controller.frontal());
    }
  }
};

} // namespace detail

int main(int argc, char **argv) {
  const auto args = parse_cli_args(argc, argv);
  sf::RenderWindow window(sf::VideoMode(args.width, args.height),
                          "ImGui + SFML = <3");
  window.setFramerateLimit(args.frame_limit);
  std::ignore = ImGui::SFML::Init(window);

  auto &style = ImGui::GetStyle();
  style.ScaleAllSizes(args.scale);
  ImGui::GetIO().FontGlobalScale = args.scale;

  sf::Clock deltaClock;
  sf::Texture texture;

  if (not texture.loadFromFile("textures/survivor-idle_shotgun_0.png")) {
    spdlog::error("Failed to load texture");
  }

  sf::Sprite sprite(texture);
  sprite.setOrigin(sprite.getTextureRect().width / 2.0,
                   sprite.getTextureRect().height / 2.0);
  spdlog::info(std::format("origin is {}, {}", sprite.getOrigin().x,
                           sprite.getOrigin().y));

  detail::movement_controller movement_controller;
  movement_controller.attach(sprite);

  while (window.isOpen()) {
    sf::Event event;
    const auto delta_time = deltaClock.restart();

    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (should_close(event)) {
        window.close();
      }
    }
    ImGui::SFML::Update(window, delta_time);
    movement_controller.update(delta_time);

    window.clear();
    window.draw(sprite);
    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}
