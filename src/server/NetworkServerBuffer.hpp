
#pragma once

#include <cstddef>
#include <vector>
#include "protocol.hpp"
#include <mutex>

/**
 * @brief buffer network
 *
 */
class NetworkServerBuffer {
    private:
        std::vector<ServerPacket> packets = {};
        mutable std::mutex mtx;

    public:
        NetworkServerBuffer() = default;
        ~NetworkServerBuffer() = default;
        void pushPacket(ServerPacket& pkt);
        ServerPacket popPacket();
        std::vector<ServerPacket> popAllPackets();
        bool empty();
};

class NetworkClientBuffer {
    private:
        std::vector<std::vector<uint8_t>> packets = {};
        std::vector<std::pair<std::pair<int, int>, size_t>> entities = {};
        mutable std::mutex mtx;

        int findEntity(std::pair<int, int>);
    public:
        NetworkClientBuffer() = default;
        ~NetworkClientBuffer() = default;
        void pushPacket(std::vector<uint8_t>&);
        void moveEntity(int, int, std::vector<uint8_t>&);
        void pushWholePacket(std::vector<std::vector<uint8_t>>& pkt);
        std::vector<uint8_t> popPacket();
        std::vector<std::vector<uint8_t>> popAllPackets();
        bool empty();
};

class NetworkContinuousBuffer {
    private:
        int serverCurrentId = 0;
        std::vector<std::vector<uint8_t>> packets = {};
        std::vector<std::pair<std::pair<int, int>, std::pair<std::vector<uint8_t>, std::vector<uint8_t>>>> entities = {};
        mutable std::mutex mtx;

        int findEntity(std::pair<int, int>);
    public:
        NetworkContinuousBuffer() = default;
        NetworkContinuousBuffer(int);
        ~NetworkContinuousBuffer() = default;
        void addEntity(int, int, std::vector<uint8_t>);
        void moveEntity(int, int, std::vector<uint8_t>);
        void deleteEntity(int, int);
        std::vector<std::vector<uint8_t>> getAllPackets();
        bool empty();
        void clear();
};
