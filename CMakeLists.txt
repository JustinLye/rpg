cmake_minimum_required(VERSION 3.20)

project(
  rpg
  VERSION 0.0.0.0
  DESCRIPTION "Top-down role playing game"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(dependencies)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

if("$ENV{CMAKE_ENV}" STREQUAL "ASAN")
  add_compile_options("-fsanitize=address")
  add_link_options("-fsanitize=address")
endif()

add_subdirectory(apps)

file(CREATE_LINK "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/build" SYMBOLIC)
