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
    PUBLIC vectlib)
    
enable_testing()
include(CTest)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-msse3" COMPILER_HAS_SSE3)
if(COMPILER_HAS_SSE3)
    target_compile_options(vectlib INTERFACE -msse3)
    target_compile_definitions(vectlib INTERFACE HAS_SSE3)
endif()

check_cxx_compiler_flag("-mavx2" COMPILER_HAS_AVX2)
if(COMPILER_HAS_AVX2)
    target_compile_options(vectlib INTERFACE -mavx2)
    target_compile_definitions(vectlib INTERFACE HAS_AVX2)
endif()