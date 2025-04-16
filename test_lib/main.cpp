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

    

    for(int i = 0; i < 1000; i++){
        std::vector<float> data = capture.getData();
        
        for (size_t i = 0; i < data.size(); i += 2) {
            std::cout << "Distance: " << data[i] << ", Angle: " << data[i + 1] << std::endl;
        }
    }

    capture.stop();




    return 0;
}