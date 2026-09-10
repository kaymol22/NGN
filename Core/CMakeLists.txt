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
src/Core/Base.h
src/Core/Constants.h
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

src/File/FileInfo.h
src/File/File.h
src/File/File.cpp

src/Geometry/PrimitiveMesh.h
src/Geometry/PrimitiveMesh.cpp

src/Math/Math.h
src/Math/Math.cpp

src/Render/RenderBackend.h
src/Render/TextureTypes.h
src/Render/Frustum.h
src/Render/Frustum.cpp
src/Render/Camera.h
src/Render/RenderDataManager.h
src/Render/RendererTypes.h
src/Render/Renderer.cpp
src/Render/VertexAttributes.h

src/Render/BVH/Types.h
src/Render/BVH/BVH.h
src/Render/BVH/Types/SceneBvh.h

src/Render/API/OpenGL/GL_Backend.cpp
src/Render/API/OpenGL/GL_Backend.h
src/Render/API/OpenGL/GL_Commands.cpp
src/Render/API/OpenGL/GL_Commands.h
src/Render/API/OpenGL/GL_ResourceManager.h
src/Render/API/OpenGL/GL_ResourceManager.cpp
src/Render/API/OpenGL/GL_Renderer.h
src/Render/API/OpenGL/Types/GL_ssbo.h
src/Render/API/OpenGL/Types/GL_ssbo.cpp
src/Render/API/OpenGL/Types/GL_Shader.h
src/Render/API/OpenGL/Types/GL_Shader.cpp
src/Render/API/OpenGL/Types/GL_FrameBuffer.cpp
src/Render/API/OpenGL/Types/GL_FrameBuffer.h
src/Render/API/OpenGL/Types/GL_Attachments.h
src/Render/API/OpenGL/Types/GL_GenericMesh.cpp
src/Render/API/OpenGL/Types/GL_GenericMesh.h
src/Render/API/OpenGL/GL_Utils.cpp 
src/Render/API/OpenGL/GL_Utils.h
src/Render/API/OpenGL/GL_Renderer.cpp
src/Render/API/OpenGL/Types/GL_Texture.cpp
src/Render/API/OpenGL/Types/GL_Texture.h
src/Render/API/OpenGL/GL_TextureUploader.cpp
src/Render/API/OpenGL/GL_TextureUploader.h
src/Render/API/OpenGL/Types/GL_Texture_Array.h
src/Render/API/OpenGL/Types/GL_RasterizerState.cpp
src/Render/API/OpenGL/Types/GL_RasterizerState.h
src/Render/API/OpenGL/GL_Rasterizer_State_Manager.h
src/Render/API/OpenGL/GL_Rasterizer_State_Manager.cpp

src/Render/API/OpenGL/Create/GL_Create_FrameBuffers.cpp
src/Render/API/OpenGL/Create/GL_Create_Shaders.cpp
src/Render/API/OpenGL/Create/GL_Create_SSBOs.cpp

src/ResourceManagement/CPU/Types/GenericMesh.cpp
src/ResourceManagement/CPU/Types/GenericMesh.h
src/ResourceManagement/CPU/Types/Resource.h
src/ResourceManagement/CPU/Types/Texture.cpp
src/ResourceManagement/CPU/Types/Texture.h
src/ResourceManagement/CPU/ResourceHandle.h
src/ResourceManagement/CPU/ResourceManager.cpp
src/ResourceManagement/CPU/ResourceManager.h
src/ResourceManagement/CPU/ResourceId.h

src/Scene/Components.cpp
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

src/Platform/Windows/WindowsInput.cpp
src/Platform/Windows/WindowsInput.h
src/Platform/Windows/WindowsPlatformUtils.cpp
src/Platform/Windows/WinWindow.cpp
src/Platform/Windows/WinWindow.h

src/Utils/PlatformUtils.h
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

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
