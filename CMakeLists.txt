cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppLearn LANGUAGES CXX)
enable_testing()
cmake_policy(SET CMP0135 NEW)

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
