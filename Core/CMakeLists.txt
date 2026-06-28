#### CORE ####

set(SOURCES
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp

src/ngnpch.h
src/ngnpch.cpp
src/NGN.h

src/Core/Assert.h
src/Core/Application.h
src/Core/Application.cpp
src/Core/AssetManager.h
src/Core/AssetManager.cpp
src/Core/Base.h
src/Core/EntryPoint.h
src/Core/Instrumentor.cpp
src/Core/Instrumentor.h
src/Core/KeyCodes.h
src/Core/Layer.h
src/Core/Layer.cpp
src/Core/Log.h
src/Core/Log.cpp
src/Core/MouseCodes.h
src/Core/PlatformDetection.h
src/Core/Profile.h
src/Core/Timestep.h
src/Core/UUID.cpp
src/Core/UUID.h
src/Core/Window.h
src/Core/Window.cpp

src/Debug/ChromeProfiling.h

src/Events/Event.h
src/Events/ApplicationEvent.h
src/Events/KeyEvent.h
src/Events/MouseEvent.h

src/ImGui/ImGuiLayer.h
src/ImGui/ImGuiLayer.cpp

src/Input/Input.cpp
src/Input/Input.h
src/Input/InputBindings.cpp
src/Input/InputBindings.h
src/Input/InputMapper.cpp
src/Input/InputMapper.h
src/Input/InputState.h

src/Math/Math.h
src/Math/Math.cpp

src/Renderer/Renderer2D.cpp
src/Renderer/Renderer2D.h
src/Renderer/Renderer.cpp
src/Renderer/RenderPass.h
src/Renderer/Renderer.h

src/Renderer/Backend/GraphicsContext.cpp
src/Renderer/Backend/GraphicsContext.h
src/Renderer/Backend/RenderCommand.cpp
src/Renderer/Backend/RenderCommand.h
src/Renderer/Backend/RendererAPI.cpp
src/Renderer/Backend/RendererAPI.h

src/Renderer/Resources/Buffer.cpp
src/Renderer/Resources/Buffer.h
src/Renderer/Resources/Font.cpp
src/Renderer/Resources/Font.h
src/Renderer/Resources/Framebuffer.cpp
src/Renderer/Resources/Framebuffer.h
src/Renderer/Resources/Frustum.cpp
src/Renderer/Resources/Frustum.h
src/Renderer/Resources/Mesh.cpp
src/Renderer/Resources/Mesh.h
src/Renderer/Resources/OrthographicCamera.cpp
src/Renderer/Resources/OrthographicCamera.h
src/Renderer/Resources/OrthographicCameraController.cpp
src/Renderer/Resources/OrthographicCameraController.h
src/Renderer/Resources/PerspCameraController.cpp
src/Renderer/Resources/PerspCameraController.h
src/Renderer/Resources/PerspectiveCamera.cpp
src/Renderer/Resources/PerspectiveCamera.h
src/Renderer/Resources/Shader.h
src/Renderer/Resources/Shader.cpp
src/Renderer/Resources/SubTexture.cpp
src/Renderer/Resources/SubTexture.h
src/Renderer/Resources/Texture.cpp
src/Renderer/Resources/Texture.h
src/Renderer/Resources/VertexArray.cpp
src/Renderer/Resources/VertexArray.h

src/Scene/Components.h
src/Scene/Entity.cpp
src/Scene/Entity.h
src/Scene/Scene.cpp
src/Scene/Scene.h
src/Scene/SceneCamera.cpp
src/Scene/SceneCamera.h
src/Scene/SceneManager.cpp
src/Scene/SceneManager.h
src/Scene/Systems/MeshRenderSystem.cpp
src/Scene/Systems/MeshRenderSystem.h
src/Scene/Systems/SpriteRenderSystem.cpp
src/Scene/Systems/SpriteRenderSystem.h
src/Scene/Systems/PlayerControllerSystem.cpp
src/Scene/Systems/PlayerControllerSystem.h
src/Scene/Systems/System.h


src/Platform/OpenGL/OpenGLBuffer.cpp
src/Platform/OpenGL/OpenGLBuffer.h
src/Platform/OpenGL/OpenGLContext.cpp
src/Platform/OpenGL/OpenGLContext.h
src/Platform/OpenGL/OpenGLFramebuffer.h
src/Platform/OpenGL/OpenGLFramebuffer.cpp
src/Platform/OpenGL/OpenGLRendererAPI.cpp
src/Platform/OpenGL/OpenGLRendererAPI.h
src/Platform/OpenGL/OpenGLShader.cpp
src/Platform/OpenGL/OpenGLShader.h
src/Platform/OpenGL/OpenGLTexture.h
src/Platform/OpenGL/OpenGLTexture.cpp
src/Platform/OpenGL/OpenGLVertexArray.cpp
src/Platform/OpenGL/OpenGLVertexArray.h
src/Platform/OpenGL/GLUtils.h
src/Platform/OpenGL/GLUtils.cpp
src/Platform/Windows/WindowsInput.cpp
src/Platform/Windows/WindowsInput.h
src/Platform/Windows/WindowsPlatformUtils.cpp
src/Platform/Windows/WinWindow.cpp
src/Platform/Windows/WinWindow.h

src/Utils/PlatformUtils.h
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES} "src/Renderer/RenderContext.h" "src/Core/Types.h" "src/Renderer/RenderPass.cpp" "src/Renderer/RenderContext.cpp" "src/Core/Enums.h")

# enable precompile headers
ngn_enable_pch(Core)

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})


target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(Core PUBLIC SYSTEM 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/include
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
