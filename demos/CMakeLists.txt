cmake_minimum_required(VERSION 3.25)

project(synolodemo)

# Create executable for test_ros.cpp and link synolo
add_executable(test_ros test_ros.cpp)

target_include_directories(test_ros PRIVATE
  ${PROJECT_SOURCE_DIR}/../extern/lightwarelidar_forked/src
)


target_link_libraries(test_ros synolo) 