cmake_minimum_required(VERSION 3.20)

project(
  rpg
  VERSION 0.0.0.0
  DESCRIPTION "Top-down role playing game"
  LANGUAGES CXX)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Werror")

include(dependencies)

add_subdirectory(apps)

file(CREATE_LINK "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/build" SYMBOLIC)
