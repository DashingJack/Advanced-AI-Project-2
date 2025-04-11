cmake_minimum_required(VERSION 4.0.0)

project(
    AI_Project-2
    VERSION 0.0.1
    LANGUAGES CXX
)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(AI_Project-2 ${SOURCES})