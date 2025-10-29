#### ROOT ####
cmake_minimum_required (VERSION 3.10)

project (ngn-engine LANGUAGES C CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

# Project modules
add_subdirectory(Core)
add_subdirectory(App/Sandbox)