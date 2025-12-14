#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#define ACT_SIZE (uint64_t)4

/**
 * @brief define all the action types of the protocol
 *
 */
enum ActionType {
    INPUT_PRESSED = 0b0000,
    INPUT_RELEASED = 0b0001,
    PLAYER_CONNECT = 0b0010,
    START_GAME     = 0b0100,
    NEW_CONNECTION = 0b1000,
    NOT_RECEIVED   = 0b1100,
    OK           = 0b1111
};

/**
 * @brief store and parse a packet received data
 *
 */
class ServerPacket {
    private:
        uint16_t packetID;
        uint8_t actionType;
        uint8_t payloadSize;
        uint8_t actionValue;
        uint32_t IP;
        uint16_t port;

    public:
        ServerPacket() = default;
        ~ServerPacket() = default;


        void FillPacketID(std::vector<uint8_t>);
        void FillActionType(std::vector<uint8_t>);
        void FillPayloadSize(std::vector<uint8_t>);
        void FillActionValue(std::vector<uint8_t>);
        void FillIP(std::vector<uint8_t>);
        void FillPort(std::vector<uint8_t>, size_t offset);
        void FillReceivedData(std::vector<uint8_t>);

        inline uint16_t getID(void) { return packetID; };
        inline uint8_t getActionType(void) { return actionType; };
        inline uint8_t getPayload(void) { return payloadSize; };
        inline uint8_t getActionvalue(void) { return  actionValue; };
        inline uint32_t getIP(void) { return IP; };
        inline uint16_t getPort(void) { return port; };
};