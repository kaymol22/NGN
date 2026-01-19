#### CORE ####

set(SOURCES
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp

Source/Core/Assert.h
Source/Core/Application.h
Source/Core/Application.cpp
Source/Core/Base.h
Source/Core/Window.h
Source/Core/Window.cpp
Source/Core/Layer.h
Source/Core/Layer.cpp
Source/Core/KeyCodes.h
Source/Core/MouseCodes.h
Source/Core/Log.h
Source/Core/Log.cpp
Source/Core/ngnpch.h
Source/Core/ngnpch.cpp
Source/Core/PlatformDetection.h

Source/Events/Event.h
Source/Events/ApplicationEvent.h
Source/Events/KeyEvent.h
Source/Events/MouseEvent.h

Source/ImGui/ImGuiLayer.h
Source/ImGui/ImGuiLayer.cpp

Source/Math/Math.h
Source/Math/Math.cpp

Source/Renderer/Framebuffer.cpp
Source/Renderer/GLUtils.h
Source/Renderer/GLUtils.cpp
Source/Renderer/OrthographicCamera.cpp
Source/Renderer/OrthographicCamera.h
Source/Renderer/Renderer.h
Source/Renderer/RendererAPI.h
Source/Renderer/Renderer2D.h
Source/Renderer/Renderer.cpp
Source/Renderer/Shader.h
Source/Renderer/Shader.cpp

Source/Scene/ComponentPool.h
Source/Scene/ComponentPoolBase.h
Source/Scene/Components.h
Source/Scene/Entity.h
Source/Scene/Scene.h
Source/Scene/Scene.cpp
Source/Scene/SpriteRenderSystem.cpp
Source/Scene/SpriteRenderSystem.h
Source/Scene/System.h
Source/Scene/TransformDebugSystem.h
Source/Scene/World.h

Source/Platform/OpenGL/OpenGLFramebuffer.h
Source/Platform/OpenGL/OpenGLFramebuffer.cpp
Source/Platform/OpenGL/OpenGLTexture.h
Source/Platform/OpenGL/OpenGLTexture.cpp
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

# enable precompile headers
ngn_enable_pch(Core)

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})


target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_include_directories(Core PUBLIC SYSTEM 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
)

# Only add hardcoded External paths if they exist locally (for IntelliSense convenience)
if (EXISTS "${CMAKE_SOURCE_DIR}/External/glad/include")
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/External/glad/include")
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/External/glfw/include")
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/External/glfw/include")
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/External/glm")
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/External/glm")
endif()

# Set C++ standard for this library
set_target_properties(Core PROPERTIES CXX_STANDARD 23 )

# ensure GLFW doesn't include system gl headers
target_compile_definitions(Core PUBLIC GLFW_INCLUDE_NONE)

target_compile_definitions(Core PRIVATE $<$<CONFIG:Debug>:NGN_DEBUG>)

# Link GLFW & OpenGL
target_link_libraries(Core PUBLIC 
    glad
    glfw
    glm::glm
    imgui
)
