include(FetchContent)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x)

FetchContent_MakeAvailable(sfml)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.90.1)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG 2.6.x)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_MakeAvailable(imgui-sfml)

FetchContent_Declare(
  docopt
  GIT_REPOSITORY https://github.com/docopt/docopt.cpp
  GIT_TAG v0.6.3)

FetchContent_MakeAvailable(docopt)


FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v1.14.x)

FetchContent_MakeAvailable(googletest)

if (${RPG_OS_IS_WINDOWS})
  find_package(spdlog CONFIG REQUIRED)
  add_library(vcpkg_pkgs INTERFACE)
  target_include_directories(vcpkg_pkgs INTERFACE "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
  add_library(vcpkg::pkgs ALIAS vcpkg_pkgs)
else()
  find_package(spdlog REQUIRED)
  find_package(Boost 1.81.0 REQUIRED)
endif()

