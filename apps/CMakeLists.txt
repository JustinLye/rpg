add_executable(rpg-game main.cpp)

target_link_libraries(rpg-game PRIVATE rpg::lib spdlog::spdlog docopt)


add_custom_target(run-rpg-game COMMAND "${CMAKE_BINARY_DIR}/bin/rpg-game" DEPENDS rpg-game)
