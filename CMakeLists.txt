cmake_minimum_required(VERSION 3.20)

project(
  rpg
  VERSION 0.0.0.0
  DESCRIPTION "Top-down role playing game"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(dependencies)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_CPPCHECK
    "cppcheck;--suppress=preprocessorErrorDirective:${CMAKE_BINARY_DIR}/_deps/sfml-src/include/SFML/Config.hpp"
)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use")

add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=gnu++2b)

if("$ENV{RPG_GAME_ASAN}" STREQUAL "ON")
  add_compile_options("-fsanitize=address")
  add_link_options("-fsanitize=address")
endif()

if("$ENV{RPG_GAME_UBSAN}" STREQUAL "ON")
  add_compile_options("-fsanitize=undefined")
  add_link_options("-fsanitize=undefined")
endif()

if("$ENV{RPG_DEBUG}" STREQUAL "ON")
  add_compile_definitions(RPG_DEBUG=1)
endif()

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(apps)

file(CREATE_LINK "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/build" SYMBOLIC)
