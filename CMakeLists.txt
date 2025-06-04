cmake_minimum_required(VERSION 3.25)

project(synolo VERSION 0.0.1 DESCRIPTION "Synolo, a LiDar filtration system")

if(APPLE)
    add_compile_options(-Wno-enum-constexpr-conversion)
endif()

## Find ROS package
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
)

## Declare a catkin package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES synolo
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs
)

## Add include paths
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  extern/lightwarelidar_forked/include 
  extern/lightwarelidar_forked/src      
)
# Only build static libraries
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Add in specific files
add_library(${PROJECT_NAME}
    src/kdtree.cpp
    src/SourceModule.cpp
    src/sim_module.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17) 

## Link ROS libraries
target_link_libraries(synolo
  ${catkin_LIBRARIES}
)

# Bring in external CMake projects, refer to extern directory
add_subdirectory(extern)


# Bring in projects from /include, which holds header files assisting /src
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/lightwarelidar_forked/include


    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add subdirectories below, within the subdirectory make sure to add a CMakeLists.txt file
add_subdirectory("test_lib")
add_subdirectory("demos")