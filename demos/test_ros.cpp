/**
 * @brief this file tests the build system and if ROS and PCL
 * are working correctly
 */

#include "ros/ros.h"
#include "sensor_msgs/PointCloud2.h"
#include "../extern/lightwarelidar_forked/src/sf45b.h"

void pointCloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud) {
    ROS_INFO("Received PointCloud2 message");
    // Process the point cloud data here
}


int main(int argc, char** argv) {
    
    // Test build of SF45b driver
    SF45Communicate sensor(argc, argv);
    sensor.testBuildSystem();

    // Initialize the ROS node
    ros::init(argc, argv, "ros_test_node");
    ros::NodeHandle nh;

    // Create a publisher for PointCloud2 data
    ros::Publisher pub = nh.advertise<sensor_msgs::PointCloud2>("pointcloud", 10);

    ros::Subscriber sub = nh.subscribe("pointcloud", 10, pointCloudCallback);

    ros::Rate rate(1); // 1 Hz

    while (ros::ok()) {
        // Create a dummy point cloud message
        sensor_msgs::PointCloud2 cloud_msg;
        cloud_msg.header.stamp = ros::Time::now();
        cloud_msg.header.frame_id = "base_link";

        // Just publish an empty point cloud for now
        pub.publish(cloud_msg);

        ROS_INFO("Published dummy PointCloud2 message");

        

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
