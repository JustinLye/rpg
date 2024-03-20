add_library(rpglib INTERFACE)
target_include_directories(rpglib INTERFACE include)
if (${RPG_OS_IS_WINDOWS})
    target_link_libraries(rpglib INTERFACE ImGui-SFML::ImGui-SFML vcpkg::pkgs)
else()
    target_link_libraries(rpglib INTERFACE ImGui-SFML::ImGui-SFML Boost::boost)
endif()

add_library(rpg::lib ALIAS rpglib)
