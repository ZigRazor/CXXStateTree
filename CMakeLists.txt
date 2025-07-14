cmake_minimum_required(VERSION 3.16)
project(CXXStateTree VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file (GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(CXXStateTree SHARED ${SRC_FILES})
target_include_directories(CXXStateTree INTERFACE include ${CMAKE_CURRENT_SOURCE_DIR}/include )

set_target_properties(CXXStateTree PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

option(ENABLE_TEST "Enable Test" OFF)

if(ENABLE_TEST)
# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
add_executable(state_tree_test tests/state_tree_test.cpp)
target_link_libraries(state_tree_test PRIVATE CXXStateTree gtest_main)

include(GoogleTest)
gtest_discover_tests(state_tree_test)

endif()


option(ENABLE_EXAMPLE "Enable Example" OFF)

if(ENABLE_EXAMPLE)
add_executable(basic examples/basic.cpp)
target_include_directories(basic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(basic PRIVATE CXXStateTree)

add_executable(nested examples/nested.cpp)
target_include_directories(basic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nested PRIVATE CXXStateTree)

add_executable(export_dot_example examples/export_dot.cpp)
target_include_directories(export_dot_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(export_dot_example PRIVATE CXXStateTree)
add_executable(export_dot_nested_example examples/export_dot_nested.cpp)
target_include_directories(export_dot_nested_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(export_dot_nested_example PRIVATE CXXStateTree)
add_executable(export_dot_context_example examples/export_dot_context.cpp)
target_include_directories(export_dot_context_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(export_dot_context_example PRIVATE CXXStateTree)

add_executable(context_example examples/context_example.cpp)
target_include_directories(context_example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(context_example PRIVATE CXXStateTree)

endif()

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
  message(STATUS "Building with coverage flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

option(ENABLE_SINGLE_HEADER "Enable Single Header Generation" OFF)

if(ENABLE_SINGLE_HEADER)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
  include(FetchContent)

  # ----- Download the script (configure‑time, once, cached in build dir) -----
  FetchContent_Declare(
    edlund_amalgamate
    GIT_REPOSITORY https://github.com/edlund/amalgamate.git
    GIT_TAG        master          # ↔ pin a commit / tag for reproducible builds
  )
  FetchContent_MakeAvailable(edlund_amalgamate)   # populates edlund_amalgamate_SOURCE_DIR

  set(AMALGAMATE_PY  "${edlund_amalgamate_SOURCE_DIR}/amalgamate.py")  # :contentReference[oaicite:0]{index=0}

  set(AMALGAMATE_CFG   "${CMAKE_CURRENT_SOURCE_DIR}/config_CXXStateTree.json")
  set(AMALGAMATE_PRO   "${CMAKE_CURRENT_SOURCE_DIR}/config_CXXStateTree.prologue")
  set(AMALGAMATE_OUT   "${CMAKE_CURRENT_SOURCE_DIR}/single_include/CXXStateTree.hpp")

  add_custom_command(
    OUTPUT  ${AMALGAMATE_OUT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/single_include  # ensure /dist
    COMMAND python3 ${AMALGAMATE_PY} -c ${AMALGAMATE_CFG} -s ${CMAKE_CURRENT_SOURCE_DIR} -p ${AMALGAMATE_PRO} --verbose yes
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${AMALGAMATE_PY} ${AMALGAMATE_CFG}
    COMMENT "Generating single‑header CXXStateTree.hpp with edlund/amalgamate"
    VERBATIM
  )

  add_custom_target(amalgamate ALL DEPENDS ${AMALGAMATE_OUT})

  if(ENABLE_TEST)
    # GoogleTest setup
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/heads/main.zip
      DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)


    enable_testing()
    add_executable(state_tree_singleheader_test tests/state_tree_singleheader_test.cpp)
    target_include_directories(state_tree_singleheader_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/single_include)
    target_link_libraries(state_tree_singleheader_test PRIVATE CXXStateTree gtest_main)

    include(GoogleTest)
    gtest_discover_tests(state_tree_singleheader_test)

  endif()

endif()