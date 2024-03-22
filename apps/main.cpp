#include <rpg/action.hpp>
#include <rpg/controllers/movement.hpp>
#include <rpg/texture_paths.hpp>
#include <rpg/window/input.hpp>
#include <rpg/window/keyboard_input.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <docopt.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <map>
#include <string>

struct cli_args {
  std::uint32_t width;
  std::uint32_t height;
  double scale;
  std::uint32_t frame_limit;
};

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

struct speed {
  [[nodiscard]] float frontal_movement() const noexcept { return 500.0f; }

  [[nodiscard]] float backward_movement() const noexcept { return 250.0f; }

  [[nodiscard]] float lateral_movement() const noexcept { return 150.0f; }

  [[nodiscard]] float rotational_movement() const noexcept { return 250.0f; }
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

  if (not texture.loadFromFile(rpg::texture_paths::survivor_idle_shotgun_0)) {
    spdlog::error("Failed to load texture: `{}`",
                  rpg::texture_paths::survivor_idle_shotgun_0);
  }

  sf::Sprite sprite(texture);
  sprite.setOrigin(sprite.getTextureRect().width / 2.0,
                   sprite.getTextureRect().height / 2.0);
  spdlog::info(std::format("origin is {}, {}", sprite.getOrigin().x,
                           sprite.getOrigin().y));

  rpg::window::keyboard_input keyboard_input{};
  rpg::window::input input{keyboard_input};
  detail::speed speed{};
  rpg::controllers::movement movement_controller{input, speed};
  movement_controller.attach(sprite);
  movement_controller.map_action(rpg::action::move_forward,
                                 sf::Keyboard::Key::W);
  movement_controller.map_action(rpg::action::move_backward,
                                 sf::Keyboard::Key::S);
  movement_controller.map_action(rpg::action::move_left, sf::Keyboard::Key::A);
  movement_controller.map_action(rpg::action::move_right, sf::Keyboard::Key::D);
  movement_controller.map_action(rpg::action::rotate_right,
                                 sf::Keyboard::Key::E);
  movement_controller.map_action(rpg::action::rotate_left,
                                 sf::Keyboard::Key::Q);

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
    input.update(delta_time);
    movement_controller.update(delta_time);

    window.clear();
    window.draw(sprite);
    ImGui::SFML::Render(window);

    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}
