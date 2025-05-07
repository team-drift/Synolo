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

add_executable(synolo_exec main.cpp)

target_link_libraries(synolo_exec PRIVATE synolo sf45b)

add_executable(sim_test sim_test.cpp)

target_link_libraries(sim_test PRIVATE synolo sf45b)

add_executable(deviceCapture deviceCapture.cpp)

target_link_libraries(deviceCapture PRIVATE synolo sf45b)

# Add Catch2 unit test
add_executable(kdtree_test kdtree_Utest.cpp)

target_link_libraries(kdtree_test
  PRIVATE
    synolo         # your main library
    sf45b          # if needed
    Catch2::Catch2WithMain
)
include(Catch)
catch_discover_tests(kdtree_test)

include_directories(
    ../extern/lightwarelidar/include
    ../extern/lightwarelidar/src
    ../extern/lightwarelidar/src/linux
)
