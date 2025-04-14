#### LIDAR


#### SF-45b
The current LIDAR Sensor that DRIFT currently uses is the SF-45b from Lightware

This connects directly with Lightware studio where we can see it scan and log/save data

#### Receiving data for C++
Within the Project Guide found on the Lightware website, we can find the USB/Serial protocol reference documentation

To find code that complements the documentation, locate the [company's repository](https://github.com/LightWare-Optoelectronics/lightwarelidar)
^Here you will find useful code for the ROS Driver, specifically for SF45b and SF40 lidar sensing models


##### Setting up ROS Driver 
For MAC Users
    Here is a [link](https://medium.com/@shubhjain_007/ros-on-mac-silicon-m1-m2-updated-method-e87086d84e45) for easy ROS setup

Once in, the Catkin build system should automatically be installed
within the lightware directory, downloaded from the repo previously, add this to the CMakeLists.txt

```
target_link_libraries(lwNx
  platformLinux
  serialPortLinux
)

target_link_libraries(platformLinux
  serialPortLinux
)
```

#### Run
Open new terminal, open ROS here as well
run ```roscore```

then make the project, run executable


#### Communicating with the sensor