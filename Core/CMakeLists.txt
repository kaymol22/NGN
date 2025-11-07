#### CORE ####

set(SOURCES
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp

Source/Core/Base.h
Source/Core/Application.h
Source/Core/Application.cpp
Source/Core/Window.h
Source/Core/Window.cpp
Source/Core/Layer.h
Source/Core/Layer.cpp
Source/Core/KeyCodes.h
Source/Core/MouseCodes.h
Source/Core/Log.h

Source/Events/Event.h
Source/Events/ApplicationEvent.h
Source/Events/KeyEvent.h
Source/Events/MouseEvent.h

Source/Renderer/GLUtils.h
Source/Renderer/GLUtils.cpp
Source/Renderer/Renderer.h
Source/Renderer/Renderer.cpp
Source/Renderer/Shader.h
Source/Renderer/Shader.cpp
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES} "Source/Core/Log.cpp")

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})


target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
)

# Only add hardcoded External paths if they exist locally (for IntelliSense convenience)
if (EXISTS "${CMAKE_SOURCE_DIR}/External/glad/include")
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/External/glad/include")
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/External/glfw3/include")
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/External/glfw3/include")
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/External/glm")
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/External/glm")
endif()

# Set C++ standard for this library
set_target_properties(Core PROPERTIES CXX_STANDARD 23 )

# ensure GLFW doesn't include system gl headers
target_compile_definitions(Core PUBLIC GLFW_INCLUDE_NONE)

# Link GLFW & OpenGL
target_link_libraries(Core PUBLIC 
    glad
    glfw
    glm::glm
)

# UTF-8 source encoding  - required by spdlog/fmt
if (MSVC)
    target_compile_options(Core PRIVATE /utf-8)
endif()