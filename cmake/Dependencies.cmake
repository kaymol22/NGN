#
# Dependencies.cmake
#

include(FetchContent)

# -------------------------
# GLFW
# -------------------------
set(GLFW_LOCAL_PATH "${CMAKE_SOURCE_DIR}/External/glfw")

if (EXISTS "${GLFW_LOCAL_PATH}/CMakeLists.txt")
    message(STATUS "Using local GLFW at ${GLFW_LOCAL_PATH}")
    add_subdirectory(${GLFW_LOCAL_PATH})
    set_target_properties(glfw PROPERTIES OUTPUT_NAME "glfw3")
else()
    message(STATUS "Fetching GLFW from GitHub...")
    FetchContent_Declare(
        glfw
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
    )
    FetchContent_MakeAvailable(glfw)
endif()

# -------------------------
# GLAD
# -------------------------
set(GLAD_LOCAL_PATH "${CMAKE_SOURCE_DIR}/External/glad")

# Pre-seeded local GLAD (no CMakeLists.txt file)
if (EXISTS "${GLAD_LOCAL_PATH}/include/glad/gl.h")
    message(STATUS "Using local GLAD at ${GLAD_LOCAL_PATH}")
    # Manually create library for glad (header-only or prebuilt)
    add_library(glad STATIC
        ${GLAD_LOCAL_PATH}/src/gl.c
     )

    target_include_directories(glad PUBLIC
        ${GLAD_LOCAL_PATH}/include
    )
else()
    message(STATUS "Fetching GLAD from GitHub...")
    FetchContent_Declare(
        glad
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/Dav1dde/glad/archive/refs/tags/v2.0.8.zip
    )
    FetchContent_Populate(glad)

    # Make sure GLAD’s CMake functions are loaded
    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6)
endif()

set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# -------------------------
# GLM
# -------------------------
set(GLM_LOCAL_PATH "${CMAKE_SOURCE_DIR}/External/glm/glm")

if (EXISTS "${GLM_LOCAL_PATH}/CMakeLists.txt")
    message(STATUS "Using local GLM at ${GLM_LOCAL_PATH}")
    add_subdirectory(${GLM_LOCAL_PATH})
else()
    message(STATUS "Fetching GLM from GitHub...")
    FetchContent_Declare(
        glm
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    )
    FetchContent_MakeAvailable(glm)
    set(GLM_REAL_TARGET glm)
endif()

# Make sure include directories correct
if (TARGET ${GLM_REAL_TARGET})
    get_target_property(GLM_INC ${GLM_REAL_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    if (NOT GLM_INC)
        message(STATUS "Patching include directories for ${GLM_REAL_TARGET}")
        target_include_directories(${GLM_REAL_TARGET} INTERFACE "${GLM_LOCAL_PATH}")
    endif()
endif()

# -------------------------
# IMGUI
# -------------------------
set(IMGUI_LOCAL_PATH "${CMAKE_SOURCE_DIR}/External/imgui")

if (EXISTS "${IMGUI_LOCAL_PATH}/imgui.cpp")
    message(STATUS "Using local ImGui at ${IMGUI_LOCAL_PATH}")

    add_library(imgui STATIC
        ${IMGUI_LOCAL_PATH}/imgui.cpp
        ${IMGUI_LOCAL_PATH}/imgui_draw.cpp
        ${IMGUI_LOCAL_PATH}/imgui_widgets.cpp
        ${IMGUI_LOCAL_PATH}/imgui_tables.cpp
        ${IMGUI_LOCAL_PATH}/imgui_demo.cpp
        ${IMGUI_LOCAL_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_LOCAL_PATH}/backends/imgui_impl_opengl3.cpp
     )

    target_include_directories(imgui PUBLIC
        ${IMGUI_LOCAL_PATH}
        ${IMGUI_LOCAL_PATH}/backends
    )

    # Backend dependencies for ImGui
    target_link_libraries(imgui PUBLIC glfw glad)

else()
    message(STATUS "Fetching ImGui from Github...")
    FetchContent_Declare(
        imgui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/ocornut/imgui/archive/refs/heads/master.zip
    )
    FetchContent_Populate(imgui)

    add_library(imgui STATIC
        ${IMGUI_LOCAL_PATH}/imgui.cpp
        ${IMGUI_LOCAL_PATH}/imgui_draw.cpp
        ${IMGUI_LOCAL_PATH}/imgui_widgets.cpp
        ${IMGUI_LOCAL_PATH}/imgui_tables.cpp
        ${IMGUI_LOCAL_PATH}/imgui_impl_glfw.cpp
        ${IMGUI_LOCAL_PATH}/imgui_impl_opengl3.cpp
    )
    
    target_include_directories(imgui PUBLIC
        ${IMGUI_LOCAL_PATH}
        ${IMGUI_LOCAL_PATH}/backends
    )

    # Backend dependencies for ImGui
    target_link_libraries(imgui PUBLIC glfw glad)
endif()

set_target_properties(glm PROPERTIES FOLDER "Dependencies")
