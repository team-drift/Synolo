/**
 * @file main.cpp
 * @brief Test file for Synolo Library
 * @version 0.1
 * 
 * This file allows us to test our Synolo Library
 */

#include "filter.hpp"
#include "deviceCapture.hpp"
#include "../extern/lightwarelidar_forked/src/sf45b.h"


int main(int argc, char** argv) {
    DeviceCapture capture(argc, argv, "/dev/tty.usbmodem38S45_156941");

    capture.start();




    return 0;
}