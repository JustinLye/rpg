add_library(rpglib INTERFACE)
configure_file(include/rpg/texture_paths.in.hpp "${CMAKE_BINARY_DIR}/include/rpg/texture_paths.hpp") # probably want to do something with install in the future. Could maybe use generators.
target_include_directories(rpglib INTERFACE include "${CMAKE_BINARY_DIR}/include")
if (${RPG_OS_IS_WINDOWS})
    target_link_libraries(rpglib INTERFACE ImGui-SFML::ImGui-SFML vcpkg::pkgs)
else()
    target_link_libraries(rpglib INTERFACE ImGui-SFML::ImGui-SFML Boost::boost)
endif()

add_library(rpg::lib ALIAS rpglib)


