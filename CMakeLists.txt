#### ROOT ####
cmake_minimum_required (VERSION 3.19)

project (ngn-engine LANGUAGES C CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

# Precompiled Headers

function(ngn_enable_pch target)
	target_precompile_headers(${target} PRIVATE
		"${CMAKE_SOURCE_DIR}/Core/src/ngnpch.h"
	)

	if (MSVC)
		target_compile_options(${target} PRIVATE /utf-8)
	endif()
endfunction()


# Project modules
add_subdirectory(Core)
add_subdirectory(Editor)
add_subdirectory(App/Sandbox)