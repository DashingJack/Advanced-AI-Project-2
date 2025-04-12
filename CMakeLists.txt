cmake_minimum_required(VERSION 4.0.0)

project(
    AI_Project-2
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE -D_UNICODE)


include_directories(include)

file(GLOB SOURCES "src/*.cpp")

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master 
)

FetchContent_MakeAvailable(imgui glfw glad)

include_directories(
    ${GLFW_SOURCE_DIR}/include
    ${GLAD_SOURCE_DIR}/include
)

set(imgui_SOURCES 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)

add_executable(AI_Project-2 ${SOURCES} ${imgui_SOURCES})

target_include_directories(AI_Project-2 PRIVATE
    ${imgui_SOURCE_DIR}
)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

find_package(OpenGL REQUIRED)
target_link_libraries(AI_Project-2 OpenGL::GL glfw glad)