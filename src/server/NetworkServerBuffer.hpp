
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

class NetworkClientBuffer {
    private:
        std::vector<std::vector<uint8_t>> packets;
        mutable std::mutex mtx;

    public:
        void pushPacket(std::vector<uint8_t>& pkt);
        void pushWholePacket(std::vector<std::vector<uint8_t>>& pkt);
        std::vector<uint8_t> popPacket();
        std::vector<std::vector<uint8_t>> popAllPackets();
        bool empty();
};
