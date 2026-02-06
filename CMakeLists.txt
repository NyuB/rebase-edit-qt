cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
project(CppLearn LANGUAGES CXX)
cmake_policy(SET CMP0135 NEW)

# Custom imports
include(cmake/qt.cmake)
include(cmake/testing.cmake)

# Compiler options
add_library(compiler_options INTERFACE)
target_compile_features(compiler_options INTERFACE cxx_std_20)
# All warnings, warnings as errors
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
  compiler_options
  INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Werror>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3;-WX>>")

# Lib
add_library(Lib lib.cpp)
target_include_directories(Lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Lib PRIVATE compiler_options)

# Wren
file(GLOB WREN_SRC "wren/src/vm/*.c")
set_source_files_properties(${WREN_SRC} PROPERTIES LANGUAGE CXX)
add_library(Wren ${WREN_SRC})
target_include_directories(Wren PRIVATE wren/src/vm wren/src/optional)
target_include_directories(Wren PUBLIC wren/src/include)
target_compile_definitions(
  Wren
  PRIVATE WREN_OPT_META=0
          WREN_OPT_RANDOM=0 # Skip optional wren features
          register= # Not a C++ keyworkd anymore since C++17, this MUST remain
                    # private not to mess with the stdlib. TODO compile Wren as
                    # C or replace register in wren files manually
)

# Ui
add_library(Ui panel.cpp panel.ui wren_scripted.cpp)
target_link_qt_libraries(Ui)
target_include_directories(Ui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Ui PUBLIC Lib)
target_link_libraries(Ui PRIVATE compiler_options)
target_link_libraries(Ui PUBLIC Wren)
set(UI_AUTOGEN_INCLUDE ${CMAKE_BINARY_DIR}/Ui_autogen/include)

# Main
add_executable(Main main.cpp)
target_include_directories(Main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                        ${UI_AUTOGEN_INCLUDE})
target_link_libraries(Main PRIVATE Lib Ui)
target_link_libraries(Main PRIVATE compiler_options)

install_qt_executable_with_dependencies(Main)

install(FILES ${CMAKE_SOURCE_DIR}/scripts/newbase.bat DESTINATION ..)

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
