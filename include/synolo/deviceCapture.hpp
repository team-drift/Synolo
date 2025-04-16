#include <iostream>
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control
#include <unistd.h>     // UNIX standard function definitions
#include <string>
#include <cstring>
#include <vector>

#include "../../extern/lightwarelidar_forked/src/sf45b.h"

class DeviceCapture {
private:
    std::string port = "/dev/ttyUSB0";
    int serial_fd = -1;

    SF45Communicate sensor;

public:
    // Constructors and Destructors
    DeviceCapture(int argc, char** argv) :  sensor(argc, argv){}

    DeviceCapture(int argc, char** argv, std::string serialPort) :  sensor(argc, argv) {
        port = serialPort;
        sensor.setPortName(port);
    }

    ~DeviceCapture() {}

    // Start sensor
    void start(){
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


};
