#pragma once
#include "Packet.hpp"
#include <vector>
#include <mutex>

class NetworkBuffer {
private:
    std::vector<ServerPacket> packets;
    mutable std::mutex mtx;

public:
    void pushPacket(ServerPacket& pkt);
    ServerPacket popPacket();
    std::vector<ServerPacket> popAllPackets();
    bool empty();
};
