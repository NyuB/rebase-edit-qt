cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppLearn LANGUAGES CXX)
enable_testing()
cmake_policy(SET CMP0135 NEW)

# Custom imports
include(cmake/qt.cmake)
include(cmake/testing.cmake)

# Compiler options
add_library(compiler_options INTERFACE)
target_compile_features(compiler_options INTERFACE cxx_std_20)

# Lib
add_library(Lib lib.cpp)
target_include_directories(Lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Lib PRIVATE compiler_options)

# Ui
qt_add_library(Ui panel.cpp)
target_link_qt_libraries(Ui)
target_include_directories(Ui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Ui PUBLIC Lib)
target_link_libraries(Ui PRIVATE compiler_options)
set(UI_AUTOGEN_INCLUDE ${CMAKE_BINARY_DIR}/Ui_autogen/include)

# Main
qt_add_executable(Main main.cpp)
target_include_directories(Main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                        ${UI_AUTOGEN_INCLUDE})
target_link_libraries(Main PRIVATE Lib Ui)
target_link_libraries(Main PRIVATE compiler_options)
install(TARGETS Main)

# Test
add_executable(Test test.cpp)
target_link_libraries(Test PRIVATE Lib)
target_link_libraries(Test PRIVATE compiler_options)
test_with_gtest(Test)

# AppTest
add_executable(AppTest app_test.cpp)
target_include_directories(AppTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                           ${UI_AUTOGEN_INCLUDE})
target_link_libraries(AppTest PRIVATE Lib Ui)
target_link_libraries(AppTest PRIVATE compiler_options)
test_with_gtest(AppTest)
