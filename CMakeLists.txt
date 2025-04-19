cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppLearn LANGUAGES CXX)
enable_testing()
cmake_policy(SET CMP0135 NEW)

add_executable(Main main.cpp)
add_library(Lib lib.cpp)
target_include_directories(Lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Main PRIVATE Lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(Test test.cpp)
target_link_libraries(Test PRIVATE Lib)
target_link_libraries(Test PRIVATE GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(Test)

install(TARGETS Main)
