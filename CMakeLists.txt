cmake_minimum_required(VERSION 3.20)

project(rpg VERSION 0.0.0.0 DESCRIPTION "Top-down role playing game" LANGUAGES CXX)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

file(CREATE_LINK "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/build" SYMBOLIC)

add_subdirectory(apps)