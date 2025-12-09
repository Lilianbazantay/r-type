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


        void FillPacketID(std::array<uint8_t, 8>);
        void FillActionType(std::array<uint8_t, 8>);
        void FillPayloadSize(std::array<uint8_t, 8>);
        void FillActionValue(std::array<uint8_t, 8>);
        void FillIP(std::array<uint8_t, 8>);
        void FillPort(std::array<uint8_t, 8>, uint8_t offset);
        void FillReceivedData(std::array<uint8_t, 8>);

        inline uint16_t getID(void) { return packetID; };
        inline uint8_t getActionType(void) { return actionType; };
        inline uint8_t getPayload(void) { return payloadSize; };
        inline uint8_t getActionvalue(void) { return  actionValue; };
        inline uint64_t getIP(void) { return IP; };
        inline uint16_t getPort(void) { return port; };
};