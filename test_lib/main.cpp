/**
 * @file main.cpp
 * @brief Test file for Synolo Library
 * @version 0.1
 * 
 * This file allows us to test our Synolo Library
 */

#include "filter.hpp"
#include "deviceCapture.hpp"


int main() {
    std::cout << "starting" << std::endl;
    DeviceCapture capture;

    capture.getData();
    return 0;
}