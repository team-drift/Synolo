/**
 * @file sim_test.cpp
 * @brief Simple test functionality for sim module
 * TODO: Comprehensive unit tests for each function
 * @version 0.1
 * 
 * This file allows us to test our Simulator Module
 */

#include "../include/sim_module.h"
#include <iostream>
#include <utility>

int main(){
    // Initilize object
    std::unique_ptr<SimulatedLidarSource> simulator = std::make_unique<SimulatedLidarSource>(
        160.0, -160.0, 1.0, 10.0, 10.0, 0.02, 0, 5);

    simulator->addObstacle(0.0, 0.0, 1.0, 1.0);

    // Use source module 
    LidarSourceWrapper sim_wrapper;
    sim_wrapper.setSource(std::move(simulator));

    sim_wrapper.start();
    
    // Loop until the simulator is done
    while (!sim_wrapper.isDone()) {
        sim_wrapper.poll();

        while (sim_wrapper.hasData()) {
            LidarPacket data = sim_wrapper.getData();
            std::cout << data.angle << " " << data.distance << std::endl;
        }
    }

    return 0;
}