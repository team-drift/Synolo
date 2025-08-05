cmake_minimum_required(VERSION 3.25)

project(test_lib VERSION 0.0.1 DESCRIPTION "Includes file to test Synolo Library in action")

# include Catch2
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(synolo_exec main.cpp ../src/deviceCapture.cpp)

target_link_libraries(synolo_exec PRIVATE synolo sf45b)

# Unit test files using catch2

set(TEST_FILES
    createkdtree_test.cpp
    insert_test.cpp
)

# Adding the tests target
add_executable(synolo_tests ${TEST_FILES})

# Linking test with the Catch2 libraries
target_link_libraries(synolo_tests PRIVATE synolo Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(synolo_tests)


include_directories(
    ../extern/lightwarelidar/include
    ../extern/lightwarelidar/src
    ../extern/lightwarelidar/src/linux
)
