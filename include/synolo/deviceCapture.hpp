#include <iostream>
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control
#include <unistd.h>     // UNIX standard function definitions
#include <string>
#include <cstring>

class DeviceCapture {
private:
    std::string port = "/dev/ttyUSB0";
    int serial_fd = -1;

public:
    DeviceCapture() {
        openPort(port);
    }

    DeviceCapture(std::string serialPort) {
        port = serialPort;
        openPort(port);
    }

    ~DeviceCapture() {
        if (serial_fd != -1) {
            close(serial_fd);
        }
    }

    bool openPort(const std::string& portName) {

        serial_fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);


        if (serial_fd < 0) {
            std::cerr << "Error opening serial port: " << strerror(errno) << std::endl;
            return false;
        }

        struct termios tty;
        memset(&tty, 0, sizeof tty);

        if (tcgetattr(serial_fd, &tty) != 0) {
            std::cerr << "Error from tcgetattr: " << strerror(errno) << std::endl;
            return false;
        }

        return true;
    }

    int getData() {
        if (serial_fd < 0) return -1;

        char buf;
        while (true){
            int n = read(serial_fd, &buf, 1);
        if (n > 0) {
            std::cout << "Received: " << buf << std::endl;
        } else {
            std::cout << "No data received or read failed" << std::endl;
        }
        }
        

        return 1;
    }

    int liveFeed();
};
