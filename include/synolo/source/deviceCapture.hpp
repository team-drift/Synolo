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
#include "SourceModule.h"



class DeviceCapture: public LidarSource{
private:
    // Serial Port in which device is connected
    std::string port = "/dev/ttyUSB0";

    // Object to set device communication
    SF45Communicate sensor;

    // Packet Structure
    LidarPacket packet;

public:
    //- Constructor
    DeviceCapture(int argc, char** argv) :  sensor(argc, argv){}

    // Constructor - When serial port needs to be set (preferred constructor)
    DeviceCapture(int argc, char** argv, std::string serialPort) :  sensor(argc, argv) {
        port = serialPort;
        sensor.setPortName(port);
    }

    // Destructor
    ~DeviceCapture() {}

    '''
    From pure virtual function in LidarSource

    Starts the sensor
    '''
    void start() override {
        sensor.start();
    }

    // Stop Sensor
    void stop(){
        sensor.stop();
    }

    // Retrieve latest data
    std::vector<float> getData(){
        return sensor.getData();
    }

    '''
    From pure virtual function in LidarSource

    Returns the next packet internally built within function
    '''
    LidarPacket next() override{
        // Retrieve latest data
        std::vector<float> data = getData();

        // Set packet member variables to latest data
        packet.distance = data[0];
        packet.angle = data[1];
        packet.strength = 0; // SF45 currently does not support strength

        return packet;
    }

    '''
    From pure virtual function in LidarSource

    Returns boolean depending on if data is flowing
    * Needs to be programmed when automatic exit when lidar is disconnected is developed...
    '''
    bool isDone() override {
        return false;
    }

    '''
    From pure virtual function in LidarSource

    tells us if the next thread is available 
    * Needs to be developed...
    '''
    bool hasNext() override {
        return true;
    }

};
