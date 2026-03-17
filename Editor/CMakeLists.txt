#### EDITOR APP ####
# Build the Editor executable
add_executable(Editor)

set(SOURCES
	src/EditorApp.cpp
	src/EditorLayer.cpp
	src/EditorLayer.h
)

target_sources(Editor PRIVATE ${SOURCES})

ngn_enable_pch(Editor)

# Match filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

# Link engine library
target_link_libraries(Editor Core)

target_include_directories(Editor PRIVATE src)

# Set working directory for VS debugger
set_target_properties(Editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Editor"
)

if (MSVC)
    target_compile_options(Editor PRIVATE /utf-8)
endif()