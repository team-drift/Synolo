#include "SourceModule.h"
#include <algorithm>
#include <stdexcept>
#include <thread>

void LidarSourceWrapper::setSource(std::unique_ptr<LidarSource> src) {
    source = std::move(src);
}

void LidarSourceWrapper::start() {
    if (source) source->start();
}

void LidarSourceWrapper::poll() {
    if (!source) return;

    while (source->hasNext()) {
        packetQueue.push(source->next());
    }

    allDone = source->isDone();
}

bool LidarSourceWrapper::hasData() {
    return !packetQueue.empty();
}

LidarPacket LidarSourceWrapper::getData() {
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
