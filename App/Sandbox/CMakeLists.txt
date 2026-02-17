#### SANDBOX APP ####
# Build the Sandbox executable
add_executable(Sandbox)

set(SOURCES
    Source/DebugLayer.h
    Source/GameLayer.cpp
    Source/GameLayer.h
    Source/SandboxApp.cpp
)

target_sources(Sandbox PRIVATE ${SOURCES})

# enable precompile headers
ngn_enable_pch(Sandbox)

# Match filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

# Link the engine library
target_link_libraries(Sandbox Core)

target_include_directories(Sandbox PRIVATE Source)

# Set working directory for VS debugger
set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App/Sandbox"
)

if (MSVC)
    target_compile_options(Sandbox PRIVATE /utf-8)
endif()
