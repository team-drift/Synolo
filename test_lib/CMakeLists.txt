cmake_minimum_required(VERSION 3.25)

project(test_lib VERSION 0.0.1 DESCRIPTION "Includes file to test Synolo Library in action")

add_executable(synolo_exec main.cpp)

target_link_libraries(synolo_exec PRIVATE synolo sf45b)

add_executable(sim_test sim_test.cpp)

target_link_libraries(sim_test PRIVATE synolo sf45b)


include_directories(
    ../extern/lightwarelidar/include
    ../extern/lightwarelidar/src
    ../extern/lightwarelidar/src/linux
)
