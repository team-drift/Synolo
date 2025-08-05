cmake_minimum_required(VERSION 3.25)

project(synolo VERSION 0.0.1 DESCRIPTION "Synolo, a LiDar filtration system")

# Only build static libraries
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

## Find C++ catkin library dependencies
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  CATKIN_DEPENDS roscpp sensor_msgs
)

# Before compiling, collect all the header paths
include_directories(
  include
  ${catkin_INCLUDE_DIRS}    
)

###########
## Build ##
###########


# Add in specific files
add_library(${PROJECT_NAME}
    src/kdtree.cpp
    src/SourceModule.cpp
)

# Bring in external CMake projects, refer to extern directory
add_subdirectory(extern) 

# Link ROS libraries
target_link_libraries(synolo
  ${catkin_LIBRARIES}
  sf45b   
)

# Bring in projects from /include, which holds header files assisting /src
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add subdirectories below, within the subdirectory make sure to add a CMakeLists.txt file
add_subdirectory("test_lib")
add_subdirectory("demos")