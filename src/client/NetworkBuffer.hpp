#pragma once
#include "Packet.hpp"
#include <vector>
#include <mutex>

/**
 * @brief buffer network
 */
class NetworkBuffer {
    private:
        std::vector<NetworkPacket> packets;
        mutable std::mutex mtx;

    public:
        void pushPacket(NetworkPacket& pkt);
        NetworkPacket popPacket();
        std::vector<NetworkPacket> popAllPackets();
        bool empty();
};
