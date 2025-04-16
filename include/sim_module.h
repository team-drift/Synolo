#pragma once
#include "SourceModule.h"
#include <vector>
#include <random>
#include <chrono>

struct ObstacleCluster{
    float start_angle;    
    float end_angle;      
    float base_distance;  
    float width;          
    int scans_remaining;             
    float noise_stddev;   // Distance variation   
};

class SimulatedLidarSource: public LidarSource{
private:
    // configure
    const float max_angle_;
    const float min_angle_;  
    const float angle_increment_;
    const float max_distance_;

    // environment
    const float room_width;  //meters
    const float room_height;  //meters 
    std::vector<std::tuple<float, float, float, float>> obstacles_; // (x, y, width, height)

    // State 
    float current_angle_;       // Current beam angle
    bool reverse_direction_;    // Scan direction flag (forward/backward)

    // Noise
    std::mt19937 gen;
    std::normal_distribution<float> noise_dist;

    std::vector<ObstacleCluster> active_clusters_;
    std::default_random_engine rng_;
    float cluster_spawn_prob_;

     /**
     * @brief Updates the list of active obstacle clusters.
     *
     * Removes expired clusters, ages existing ones,
     * and spawns a new cluster with some probability.
     */
    void updateClusters();

    bool shouldSpawnCluster() const;

    /**
     * @brief Spawns a new simulated obstacle cluster and adds it to the active clusters.
     *
     * This function generates a new ObstacleCluster with randomized properties,
     * including angular width, distance from the LiDAR, and duration (measured in scans).
     * It uses uniform distributions to determine:
     *   - The angular width of the cluster (in degrees)
     *   - The normalized distance of the cluster from the LiDAR
     */
    void spawnRandomCluster();
    
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

    void addObstacle(float x, float y, float width, float height);
};