#include "SourceModule.h"
#include <algorithm>
#include <stdexcept>
#include <thread>

void LidarSourceWrapper::setSource(std::shared_ptr<LidarSource> src) {
    source = src;
}

void LidarSourceWrapper::start() {
    if (source) source->start();
}

void LidarSourceWrapper::poll() {
    if (!source) return;

    while (source->hasNext()) {
        std::lock_guard<std::mutex> lock(queueMutex);
        packetQueue.push(source->next());
    }

    allDone = source->isDone();
}

bool LidarSourceWrapper::hasData() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !packetQueue.empty();
}

LidarPacket LidarSourceWrapper::getData() {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (packetQueue.empty()) {
        throw std::runtime_error("No data available");
    }
    LidarPacket pkt = packetQueue.front();
    packetQueue.pop();
    return pkt;
}

bool LidarSourceWrapper::isDone() const {
    return allDone;
}
