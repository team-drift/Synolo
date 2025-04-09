cmake_minimum_required(VERSION 3.25)

project(test_lib VERSION 0.0.1 DESCRIPTION "Includes file to test Synolo Library in action")

add_executable(synolo_exec main.cpp)

target_link_libraries(synolo_exec PRIVATE synolo)