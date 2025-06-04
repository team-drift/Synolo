/**
 * @file deviceCapture.hpp
 * @brief Creates source for capturing data directly from a LIDAR device (compatible with lightware LIDAR devices)
 * @version 0.1
 * 
 * This file allows us to connect our connected LIDAR device to the rest of our project
 * The main class within this file is DeviceCapture which is derived from the LidarSource parent classs
 */

#include <iostream>
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control
#include <unistd.h>     // UNIX standard function definitions
#include <string>
#include <cstring>
#include <vector>

#include "../../extern/lightwarelidar_forked/src/sf45b.h"
#include "../include/synolo/source/SourceModule.h"
#include "../include/synolo/source/dc.hpp"

    //DeviceCapture(int argc, char** argv) :  sensor(argc, argv){}
DeviceCapture::DeviceCapture(int argc, char** argv)
    : sensor(argc, argv), packet() {}

    // DeviceCapture(int argc, char** argv, std::string serialPort) :  sensor(argc, argv) {
    //     port = serialPort;
    //     sensor.setPortName(port);
    // }
DeviceCapture::DeviceCapture(int argc, char** argv, std::string serialPort)
    : sensor(argc, argv), packet() {
    port = serialPort;
    sensor.setPortName(port);
}


    // Destructor
DeviceCapture::~DeviceCapture() {}

    /**  
     * From pure virtual function in LidarSource
     * Starts the sensor
     */

void DeviceCapture::start() {
        sensor.start();
    }

    // Stop Sensor
void DeviceCapture::stop(){
        sensor.stop();
    }

    // Retrieve latest data
std::vector<float> DeviceCapture::getData(){
        return sensor.getData();
    }

    /**
     * 
     * From pure virtual function in LidarSource
     * Returns the next packet internally built within function
     */

LidarPacket DeviceCapture::next() {
        // Retrieve latest data
        std::vector<float> data = getData();

        // Set packet member variables to latest data
        packet.distance = data[0];
        packet.angle = data[1];
        packet.strength = 0; // SF45 currently does not support strength

        return packet;
    }

    /**
     * From pure virtual function in LidarSource
     * Returns boolean depending on if data is flowing
     * * Needs to be programmed when automatic exit when lidar is disconnected is developed...
     */
bool DeviceCapture::isDone() const {
        return false;
    }

    /**
     * From pure virtual function in LidarSource
     * tells us if the next thread is available 
     * * Needs to be developed...
     */
bool DeviceCapture::hasNext() const {
        return true;
    }
