#pragma once
#include "SourceModule.h"
#include <vector>
#include <random>

class SimulatedLidarSource: public LidarSource{
private:
    // configure
    const float max_angle;
    const float min_angle;  
    const float angle_increment;
    const float max_distance;

    // environment
    const float room_width;  //meters
    const float room_height;  //meters 
    std::vector<std::tuple<float, float, float>> obstacles; // (x, y, radius)

    // State 
    float current_angle_;       // Current beam angle
    bool reverse_direction_;    // Scan direction flag (forward/backward)

    // Noise
    std::mt19937 gen;
    std::normal_distribution<float> noise_dist;
    
    float calculateDistance(float angle) const;

public:

    SimulatedLidarSource(float max_angle = 160.0f, 
    float min_angle = -160.0f, 
    float angle_increment = 0.0f, 
    float room_width = 10.0f, 
    float room_height = 10.0f, 
    float noise_dist = 0.02f);

    void start() override;

    bool hasNext() const override;

    LidarPacket next() override;

    bool isDone() const override;

    void addObstacle(float x, float y, float radius);
};