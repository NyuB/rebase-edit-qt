cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppLearn LANGUAGES CXX)
enable_testing()
cmake_policy(SET CMP0135 NEW)

add_library(compiler_options INTERFACE)
target_compile_features(compiler_options INTERFACE cxx_std_20)

include(cmake/qt.cmake)
qt_add_executable(Main main.cpp panel.cpp panel.ui)
target_link_qt_libraries(Main)

add_library(Lib lib.cpp)
target_include_directories(Lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Main PRIVATE Lib)

include(cmake/testing.cmake)
add_executable(Test test.cpp)
target_link_libraries(Test PRIVATE Lib)
test_with_gtest(Test)

install(TARGETS Main)

target_link_libraries(Main PUBLIC compiler_options)
target_link_libraries(Test PUBLIC compiler_options)
target_link_libraries(Lib PUBLIC compiler_options)
