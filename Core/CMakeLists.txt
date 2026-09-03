#### CORE ####

set(SOURCES
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp

src/ngnpch.h
src/ngnpch.cpp
src/NGN.h

src/Containers/SlotMap.h

src/Core/Assert.h
src/Core/Application.h
src/Core/Application.cpp
src/Core/AssetManager.h
src/Core/AssetManager.cpp
src/Core/Base.h
src/Core/EntryPoint.h
src/Core/Enums.h
src/Core/GraphicsContext.h
src/Core/GraphicsContext.cpp
src/Core/Instrumentor.cpp
src/Core/Instrumentor.h
src/Core/KeyCodes.h
src/Core/Layer.h
src/Core/Layer.cpp
src/Core/Log.h
src/Core/Log.cpp
src/Core/MouseCodes.h
src/Core/String.h
src/Core/PlatformDetection.h
src/Core/Profile.h
src/Core/Timestep.h
src/Core/Types.h
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

src/Render/RenderBackend.h
src/Render/TextureTypes.h
src/Render/Frustum.h
src/Render/Frustum.cpp

src/Render/API/OpenGL/GL_Backend.cpp
src/Render/API/OpenGL/GL_Backend.h
src/Render/API/OpenGL/GL_Commands.cpp
src/Render/API/OpenGL/GL_Commands.h
src/Render/API/OpenGL/GL_ResourceManager.h
src/Render/API/OpenGL/GL_ResourceManager.cpp
src/Render/API/OpenGL/GL_Renderer.h
src/Render/API/OpenGL/GL_Utils.cpp 
src/Render/API/OpenGL/GL_Utils.h


 



src/Render/API/OpenGL/Types/GL_Texture.cpp
src/Render/API/OpenGL/Types/GL_Texture.h
src/Render/API/OpenGL/GL_TextureUploader.cpp
src/Render/API/OpenGL/GL_TextureUploader.h



src/Renderer/Renderer2D.cpp
src/Renderer/Renderer2D.h
src/Renderer/Renderer.cpp
src/Renderer/Renderer.h
src/Renderer/RenderGraph.cpp
src/Renderer/RenderGraph.h
src/Renderer/PassManager.cpp
src/Renderer/PassManager.h
src/Renderer/RenderPass.cpp
src/Renderer/RenderPass.h

src/Renderer/Backend/GraphicsContext.cpp
src/Renderer/Backend/GraphicsContext.h
src/Renderer/Backend/RenderCommand.cpp
src/Renderer/Backend/RenderCommand.h
src/Renderer/Backend/RendererAPI.cpp
src/Renderer/Backend/RendererAPI.h

src/Renderer/Passes/GeometryPass.cpp
src/Renderer/Passes/GeometryPass.h

src/Renderer/Resources/Buffer.cpp
src/Renderer/Resources/Buffer.h
src/Renderer/Resources/Font.cpp
src/Renderer/Resources/Font.h
src/Renderer/Resources/Framebuffer.cpp
src/Renderer/Resources/Framebuffer.h

src/Renderer/Resources/Material.h
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

src/ResourceManagement/CPU/Types/Resource.h
src/ResourceManagement/CPU/Types/Texture.cpp
src/ResourceManagement/CPU/Types/Texture.h
src/ResourceManagement/CPU/ResourceHandle.h
src/ResourceManagement/CPU/ResourceManager.cpp
src/ResourceManagement/CPU/ResourceManager.h

src/Scene/Components.h
src/Scene/Entity.cpp
src/Scene/Entity.h
src/Scene/Scene.cpp
src/Scene/Scene.h
src/Scene/Scenebvh.cpp
src/Scene/Scenebvh.h
src/Scene/SceneCamera.cpp
src/Scene/SceneCamera.h
src/Scene/SceneManager.cpp
src/Scene/SceneManager.h

src/Scene/Systems/System.h
src/Scene/Systems/PlayerControllerSystem.cpp
src/Scene/Systems/PlayerControllerSystem.h
src/Scene/Systems/RenderSystem.cpp
src/Scene/Systems/RenderSystem.h

src/Platform/OpenGL/Resources/GL_Buffer.cpp
src/Platform/OpenGL/Resources/GL_Buffer.h
src/Platform/OpenGL/GL_Context.cpp
src/Platform/OpenGL/GL_Context.h
src/Platform/OpenGL/Resources/GL_Framebuffer.h
src/Platform/OpenGL/Resources/GL_Framebuffer.cpp
src/Platform/OpenGL/Passes/GL_RasterizerState.cpp
src/Platform/OpenGL/Passes/GL_RasterizerState.h
src/Platform/OpenGL/GL_RendererAPI.cpp
src/Platform/OpenGL/GL_RendererAPI.h
src/Platform/OpenGL/Resources/GL_Shader.cpp
src/Platform/OpenGL/Resources/GL_Shader.h
src/Platform/OpenGL/Resources/GL_Texture.h
src/Platform/OpenGL/Resources/GL_Texture.cpp
src/Platform/OpenGL/Resources/GL_VertexArray.cpp
src/Platform/OpenGL/Resources/GL_VertexArray.h
src/Platform/OpenGL/GL_Utils.h
src/Platform/OpenGL/GL_Utils.cpp
src/Platform/OpenGL/Passes/GL_GeometryPass.cpp
src/Platform/OpenGL/Passes/GL_GeometryPass.h
src/Platform/Windows/WindowsInput.cpp
src/Platform/Windows/WindowsInput.h
src/Platform/Windows/WindowsPlatformUtils.cpp
src/Platform/Windows/WinWindow.cpp
src/Platform/Windows/WinWindow.h

src/Utils/PlatformUtils.h
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES} "src/ResourceManagement/CPU/ResourceId.h" "src/File/FileInfo.h" "src/File/File.h" "src/File/File.cpp" "src/Core/String.h"  "src/Render/BVH/Types.h" "src/Render/BVH/BVH.h" "src/Render/BVH/Types/SceneBvh.h"  "src/Render/SpriteExtraction.h" "src/Render/API/OpenGL/Types/GL_Texture_Array.h"   "src/Scene/Components.cpp" "src/Render/Camera.h" "src/Render/SpriteExtraction.cpp" "src/Render/API/OpenGL/GL_Renderer.cpp" "src/Render/Renderer.cpp")

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
