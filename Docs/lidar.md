#### LIDAR


#### SF-45b
SF-45b is the current LIDAR Sensor that DRIFT currently uses is the SF-45b from LightWare. This sensor is the "The world’s smallest and lightest scanning microLiDAR" according to LightWare. 

This sensor connects directly with Lightware studio desktop application where we can see it scan and log/save data
    Plug in and connect

#### Receiving data for C++
Within the [Project Guide](https://support.lightware.co.za/sf45b/#/quickstart) found on the Lightware website, we can find the USB/Serial protocol reference documentation

To find code that complements the documentation, locate the [company's repository](https://github.com/LightWare-Optoelectronics/lightwarelidar)

^Here you will find useful code for the ROS Driver, specifically for SF45b and SF40 lidar sensing models


##### Setting up ROS Driver 
For MAC Users
    Here is a [link](https://medium.com/@shubhjain_007/ros-on-mac-silicon-m1-m2-updated-method-e87086d84e45) for easy ROS setup
For others, locate documentation on [ROS](http://wiki.ros.org/Documentation)

Once in, the Catkin build system should come with ROS

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

Plug in Lidar sensor to computer, make the project, run executable
