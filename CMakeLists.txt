cmake_minimum_required(VERSION 3.20)

project(vect LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "src/*.cpp")

add_library(vectlib INTERFACE)

target_include_directories(vectlib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE vectlib)