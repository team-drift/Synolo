#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <string>

struct LidarPacket {
    float angle;    // in degrees
    float distance; // normalized [0.0, 1.0]
    float strength; // currently 0.0

    // default constructor
    LidarPacket() = default; 

    LidarPacket(float a, float d, float s = 0.0f)
        : angle(a), distance(d), strength(s) {}
};

class LidarSource {
public:
    virtual ~LidarSource() = default;

    // opens a connection, loads data, starts a thread
    virtual void start() = 0;

    // tells us if the next thread is available 
    virtual bool hasNext() const = 0;

    // retrieves the next available packet 
    virtual LidarPacket next() = 0;

    // indicates the source has no more data to provide 
    virtual bool isDone() const = 0;
};

class LidarSourceWrapper {
private:
    // holds shared_pts to different LidarSource instances
    // shared_ptr allowes shared ownership of the LidarSource objects
    // Each LidarSource could be a simulator, live sensor, or any other implementation
    std::unique_ptr<LidarSource> source;

    std::queue<LidarPacket> packetQueue;

    bool allDone = false;

public:
    //adds new data source
    void setSource(std::unique_ptr<LidarSource> src);
    
    // Starts this collection
    void start();

    /** 
     * if the source has data, it calls next()
     * and pushes it into packetQueue. 
     * Sets allDone = true if all sources report isDone()
     *  */ 
    void poll();

    // checks whether the queue has any packets
    bool hasData();

    // retrieves the next LidarPacket from the queue
    LidarPacket getData();

    // Returns true if all sources are finished and queue is empty 
    bool isDone() const;
};