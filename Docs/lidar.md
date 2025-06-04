#### LIDAR

#### SF-45b
SF-45b is the current LIDAR Sensor that DRIFT currently uses is the SF-45b from LightWare. This sensor is the "The world’s smallest and lightest scanning microLiDAR" according to LightWare. 

This sensor connects directly with the Lightware studio desktop application where we can see it scan and log/save data
    Plug in and connect

#### Receiving data for C++
Within the [Project Guide](https://support.lightware.co.za/sf45b/#/quickstart) found on the Lightware website, we can find the USB/Serial protocol reference documentation

To find code that complements the documentation, locate the [company's repository](https://github.com/LightWare-Optoelectronics/lightwarelidar).

To work with our project, the Lightware comapny's repository has been [forked and updated](https://github.com/team-drift/lightwarelidar_forked.git). This fork is included in the project as a submodule located in /extern

^Here you will find useful code for the ROS Driver, specifically for SF45b and SF40 lidar sensing models

#### ROS
The Robot Operating System(ROS) provides libraries and tools to build robotic applications. ROS acts as the middle framework between the software and hardware. 

We are using ROS 1 in this project.

Lightware uses ROS for it's tools that may directly work with the Lidar Sensors

ROS work directly on onboard computers such as Jetsons, Arduinos...


##### Setting up ROS Driver 
For MAC Users - Here is a [link](https://medium.com/@shubhjain_007/ros-on-mac-silicon-m1-m2-updated-method-e87086d84e45) for easy ROS setup.
For others, locate the documentation for installation on [ROS' Website](http://wiki.ros.org/Documentation).

Once in, the Catkin build system should come with ROS, Catkin's build system is automatically invoked in the Lightware forked repository cloned in /extern


#### Run
Open new terminal, open ROS here as well
run ```roscore```

Plug in Lidar sensor to computer, confirm port name is same as included in lightwarelidar_forked, include the DRIFT built class (SF45Communicate) in a project, build, and run