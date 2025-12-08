#pragma once

#include <array>
#include <cstdint>

#define ACT_SIZE           (uint64_t)4

/**
 * @brief store and parse a packet received data
 *
 */
class Packet {
    private:
        uint16_t packetID;
        uint8_t actionType;
        uint8_t payloadSize;
        uint8_t actionValue;
        uint64_t IP;
        uint16_t port;

    public:
        Packet() = default;
        ~Packet() = default;


        void getPacketID(std::array<uint8_t, 8>);
        void getActionType(std::array<uint8_t, 8>);
        void getPayloadSize(std::array<uint8_t, 8>);
        void getActionValue(std::array<uint8_t, 8>);
        void getIP(std::array<uint8_t, 8>);
        void getPort(std::array<uint8_t, 8>);
        void getReceivedData(std::array<uint8_t, 8>);
};