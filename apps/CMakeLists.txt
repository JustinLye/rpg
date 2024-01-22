add_executable(rpg-game main.cpp)

target_link_libraries(rpg-game PRIVATE rpg::lib spdlog::spdlog docopt)
