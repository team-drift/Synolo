This file is used as an educational aid to understand Synolo, feel free to add to this

#### CMake's use in Synolo
CMake is a system that manages the build system for our project

For DRIFT, CMake has been the standard tool of choice for builds in projects such as DTS (Telemetry_stream) and our fork of the PTU-SDK repository

Synolo is using a similar build standard used within other repositories in the DRIFT lab

A key draw of CMake is it's cross platform capabilites and it's inclusion in many Open Source Projects written in C++
^Due to the cross platform capabilites, CMake has it's own scripting language (Many tutorials and guides available online)


#### Our Current system
In our root project directory, we include a CMakeLists.txt file whose main role is to compile the Synolo library

The Synolo Library brings in various information such as .cpp files, .hpp files, and external projects

Once the Synolo Library is compiled, we add subdirectories (listed in CMakeLists.txt) and these subdirectories hold their own CMakeLists.txt file
    The subdirectories are able to run using the Synolo Library

    Current subdirectories
        demos - Holds files to run a demo
        test_lib - Holds file meant to test the Synolo lib
    

Refer To CMakeLists.txt in root directory for comments explaining the file