#include "sim_module.h"
#include <cmath>

// Constructor: 
// Initilize scanning range and resolution
// Calculate maximum measurable distance 
// Seed random number generator for noise 
SimulatedLidarSource::SimulatedLidarSource(
    float max_angle, float min_angle, float angle_increment, 
    float room_width, float room_height, float noise_dist)
    : max_angle(max_angle),
      min_angle(min_angle),
      angle_increment(angle_increment),
      room_width(room_width),
      room_height(room_height),
      max_distance(std::sqrt(pow(room_width / 2, 2) + pow(room_height / 2, 2)))
{
    // Optionally seed RNG or set up noise distribution here
}


void SimulatedLidarSource::start(){

}

bool SimulatedLidarSource::hasNext() const{

}

LidarPacket SimulatedLidarSource::next(){

}

bool isDone(){

}

void SimulatedLidarSource::addObstacle(float x, float y, float radius){

}