add_library(rpglib INTERFACE)
target_include_directories(rpglib INTERFACE include)
target_link_libraries(rpglib INTERFACE ImGui-SFML::ImGui-SFML Boost::boost)
add_library(rpg::lib ALIAS rpglib)
