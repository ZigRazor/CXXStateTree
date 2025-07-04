cmake_minimum_required(VERSION 3.16)
project(CXXStateTree VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CXXStateTree INTERFACE)
target_include_directories(CXXStateTree INTERFACE include)

add_executable(basic examples/basic.cpp)
target_link_libraries(basic PRIVATE CXXStateTree)


add_executable(nested examples/nested.cpp)
target_link_libraries(nested PRIVATE CXXStateTree)

add_executable(export_dot_example examples/export_dot.cpp)
target_include_directories(export_dot_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(export_dot_nested_example examples/export_dot_nested.cpp)
target_include_directories(export_dot_nested_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(export_dot_context_example examples/export_dot_context.cpp)
target_include_directories(export_dot_context_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(context_example examples/context_example.cpp)
target_include_directories(context_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
add_executable(state_tree_test tests/state_tree_test.cpp)
target_link_libraries(state_tree_test PRIVATE CXXStateTree gtest_main)

include(GoogleTest)
gtest_discover_tests(state_tree_test)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
  message(STATUS "Building with coverage flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()