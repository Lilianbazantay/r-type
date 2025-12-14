
#pragma once

#include <vector>
#include "protocol.hpp"
#include <mutex>

/**
 * @brief buffer network
 * 
 */
class NetworkServerBuffer {
    private:
        std::vector<ServerPacket> packets;
        mutable std::mutex mtx;

    public:
        void pushPacket(ServerPacket& pkt);
        ServerPacket popPacket();
        std::vector<ServerPacket> popAllPackets();
        bool empty();
};
