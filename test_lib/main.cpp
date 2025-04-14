/**
 * @file main.cpp
 * @brief Test file for Synolo Library
 * @version 0.1
 * 
 * This file allows us to test our Synolo Library
 */

#include "filter.hpp"
#include "deviceCapture.hpp"
#include "../extern/lightwarelidar/src/sf45b.h"


int main(int argc, char** argv) {
    // std::cout << "starting" << std::endl;
    DeviceCapture capture;

    SF45Communicate test(argc, argv);

    test.run();




    return 0;
}