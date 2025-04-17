#include "sim_module.h"
#include <cmath>
#include <algorithm>
#include <limits>


void SimulatedLidarSource::updateClusters() {
    // Remove expired clusters
    active_clusters_.erase(
        std::remove_if(active_clusters_.begin(), active_clusters_.end(),
            [](const ObstacleCluster& c) { return c.scans_remaining <= 0; }),
        active_clusters_.end()
    );

    // Age existing clusters
    for(auto& cluster : active_clusters_) {
        cluster.scans_remaining--;
    }

    // Spawn new clusters
    if(shouldSpawnCluster()) {
        spawnRandomCluster();
    }
}

bool SimulatedLidarSource::shouldSpawnCluster() const {
    std::bernoulli_distribution dist(cluster_spawn_prob_);
    return dist(rng_);
}

void SimulatedLidarSource::spawnRandomCluster() {
    std::uniform_real_distribution<float> width_dist(2.0f, 15.0f);
    std::uniform_real_distribution<float> dist_dist(0.2f, 0.8f);
    
    ObstacleCluster new_cluster {
        .start_angle = current_angle_,
        .end_angle = current_angle_ + width_dist(rng_),
        .base_distance = max_distance_ * dist_dist(rng_),
        .width = width_dist(rng_),
        .scans_remaining = 50,  
        .noise_stddev = 0.05f
    };
    
    active_clusters_.push_back(new_cluster);
}


// Constructor: 
// Initilize scanning range and resolution
// Calculate maximum measurable distance 
// Seed random number generator for noise 
SimulatedLidarSource::SimulatedLidarSource(
    float max_angle, float min_angle, float angle_increment, 
    float room_width, float room_height, float noise_dist, int scan_count, int max_scans)
    : max_angle_(max_angle),
      min_angle_(min_angle),
      angle_increment_(angle_increment),
      room_width(room_width),
      room_height(room_height),
      max_distance_(std::sqrt(pow(room_width / 2, 2) + pow(room_height / 2, 2))),
      scan_count_(scan_count),
      max_scans_(max_scans)
{
    gen.seed(std::random_device{}());
}


void SimulatedLidarSource::start(){
    // will start with from the left
    current_angle_ = min_angle_;
    reverse_direction_ = false;
}

bool SimulatedLidarSource::hasNext() const{
    return scan_count_ < max_scans_;
}

LidarPacket SimulatedLidarSource::next(){
    updateClusters(); 
    // Calculate distance 
    float distance = calculateDistance(current_angle_);
    //distance = std::clamp(distance, 0.0f, max_distance);

    // convert to normalized distance 
    float normalized_distance = distance/max_distance_;

    // Create Lidar Packet
    LidarPacket packet(current_angle_, normalized_distance);

    // Update angle
    if(!reverse_direction_){
        current_angle_ += angle_increment_;
        if(current_angle_ > max_angle_){
            reverse_direction_ = true;
            current_angle_ = max_angle_;
        }
    }
    else{
        current_angle_ -= angle_increment_;
        if(current_angle_ < min_angle_){
            reverse_direction_ = false;
            current_angle_ = min_angle_;
        }
    }

    scan_count_++;
    return packet;

}

bool SimulatedLidarSource::isDone() const {
    return scan_count_ >= max_scans_;
}

// Helper 
// wall_pos: Position of wall along primary axis
// dir: Beam direction component along primary axis
// orth_dir: Beam direction component along orthogonal axis
// orth_half_extent: Room/obstacle extent along orthogonal axis
void SimulatedLidarSource::checkWallIntersection(float wall_pos, float dir, float orth_dir, float orth_half_extent, float& min_distance) const{
    if (dir != 0) {
        float t = wall_pos / dir;
        float orth_pos = t * orth_dir;
        if (t > 0 && fabsf(orth_pos) <= orth_half_extent) {
            min_distance = std::min(min_distance, t);
        }
    }
}


float SimulatedLidarSource::calculateDistance(float angle) const{
    //conversions
    const float rad = angle * M_PI/ 180.0f;
    const float dir_x = cosf(rad);
    const float dir_y = sinf(rad);

    // We set min_distance to a large value to ensure the intercection will always be smaller
    float min_distance = std::numeric_limits<float>::max(); 
    float half_width = room_width/2;
    float half_height = room_height/2;
    
    // Check room walls 
    checkWallIntersection(half_width, dir_x, dir_y, half_height, min_distance);   // Right
    checkWallIntersection(-half_width, dir_x, dir_y, half_height, min_distance);  // Left
    checkWallIntersection(half_height, dir_y, dir_x, half_width, min_distance);   // Top
    checkWallIntersection(-half_height, dir_y, dir_x, half_width, min_distance);  // Bottom
  
    
    for (const auto& [x, y, w, h] : obstacles_) {
        const float half_w = w / 2;
        const float half_h = h / 2;
    
        checkWallIntersection(x - half_w, dir_x, dir_y, half_h, min_distance); // Left wall
        checkWallIntersection(x + half_w, dir_x, dir_y, half_h, min_distance); // Right wall
        checkWallIntersection(y + half_h, dir_y, dir_x, half_w, min_distance); // Top wall
        checkWallIntersection(y - half_h, dir_y, dir_x, half_w, min_distance); // Bottom wall
    }

     // Check dynamic clusters with noise
     for(const auto& cluster : active_clusters_) {
        if(angle >= cluster.start_angle && angle <= cluster.end_angle) {
            // Calculate base distance with noise
            std::normal_distribution<float> dist(cluster.base_distance, 
                                                cluster.noise_stddev);
            const float noisy_distance = dist(rng_);
            
            // Add angular-dependent variation
            const float angle_progress = (angle - cluster.start_angle) / 
                                       (cluster.end_angle - cluster.start_angle);
            const float shape_noise = 0.1f * sinf(angle_progress * 2 * M_PI);
            
            float cluster_distance = noisy_distance + shape_noise;
            if (cluster_distance < 0.1f * max_distance_)
                cluster_distance = 0.1f * max_distance_;
            else if (cluster_distance > 0.9f * max_distance_)
                cluster_distance = 0.9f * max_distance_;


            // Only occlude if closer than current minimum
            if(cluster_distance < min_distance) {
                min_distance = cluster_distance;
            }
        }
    }

    return min_distance != std::numeric_limits<float>::max() ? min_distance: max_distance_;
}

void SimulatedLidarSource::addObstacle(float x, float y, float width, float height){
    obstacles_.emplace_back(x, y, width, height);
}