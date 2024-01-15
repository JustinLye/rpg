add_executable(rpg-game main.cpp)

target_link_libraries(rpg-game PRIVATE ImGui-SFML::ImGui-SFML spdlog::spdlog
                                       docopt)
