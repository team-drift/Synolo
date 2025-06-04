cmake_minimum_required(VERSION 3.25)

project(synolodemo)


add_executable(test_ros ../demos/test_ros.cpp)
target_link_libraries(test_ros
    synolo
    ${catkin_LIBRARIES}
    sf45b       
    lwNx
    platformLinux
    serialPortLinux
)
target_include_directories(test_ros PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include  
    ${catkin_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/lightwarelidar_forked/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/lightwarelidar_forked/src
)