include(FetchContent)

cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES "*.cpp")

add_executable(run_unit_tests ${SOURCES})

target_link_libraries(run_unit_tests 
    PRIVATE 
        GTest::gtest_main 
        vectlib
)

include(GoogleTest)
gtest_discover_tests(run_unit_tests)