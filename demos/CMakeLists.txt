cmake_minimum_required(VERSION 3.25)

project(synolodemo)

# Create executable for fromFile.cpp and link synolo
add_executable( fromFile fromFile.cpp )
target_link_libraries( fromFile synolo )

