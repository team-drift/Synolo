/**
 * @file sim_module.h
 * @brief Simulates a LiDAR source by generating synthetic scan data in a virtual 2D room.
 *
 * This module implements the LidarSource interface and provides a simulated LiDAR scanner that 
 * emits angular scan packets representing distances to walls, obstacles, and temporary clusters.
 * It supports static obstacles as well as dynamic, probabilistically spawned obstacle clusters.
 * 
 * The simulator is configurable with parameters such as room size, scan resolution, and noise level.
 * It serves as a testbed for developing and evaluating LiDAR filtering algorithms in a controlled, 
 * reproducible environment without requiring real sensor hardware.
 *
 * @TODO: Adapt this module to work directly with point clouds instead of
 * per-angle scan packets, to better match downstream filtering pipelines.
 *
 * @version 0.1
 */

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
    int scan_count_;
    int max_scans_;

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

    bool shouldSpawnCluster();

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

    void checkWallIntersection(float wall_pos, float dir, float orth_dir, float orth_half_extent, float& min_distance) const;
    
    float calculateDistance(float angle);

public:

    SimulatedLidarSource(float max_angle = 160.0f, 
    float min_angle = -160.0f, 
    float angle_increment = 0.0f, 
    float room_width = 10.0f, 
    float room_height = 10.0f, 
    float noise_dist = 0.02f,
    int scan_count = 0,
    int max_scans = 50);

    // Start the simulator module
    void start() override;

    // Checks if the next packet is available 
    bool hasNext() const override;


    LidarPacket next() override;

    // Checks if there is no more data coming 
    bool isDone() const override;

    void addObstacle(float x, float y, float width, float height);
};