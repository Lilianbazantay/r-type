#pragma once

#include <array>
#include <cstdint>

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
class Packet {
    private:
        uint16_t packetID;
        uint8_t actionType;
        uint8_t payloadSize;
        uint8_t actionValue;
        uint32_t IP;
        uint16_t port;

    public:
        Packet() = default;
        ~Packet() = default;


        void FillPacketID(std::array<uint8_t, 9>);
        void FillActionType(std::array<uint8_t, 9>);
        void FillPayloadSize(std::array<uint8_t, 9>);
        void FillActionValue(std::array<uint8_t, 9>);
        void FillIP(std::array<uint8_t, 9>);
        void FillPort(std::array<uint8_t, 9>, uint8_t offset);
        void FillReceivedData(std::array<uint8_t, 9>);

        inline uint16_t getID(void) { return packetID; };
        inline uint8_t getActionType(void) { return actionType; };
        inline uint8_t getPayload(void) { return payloadSize; };
        inline uint8_t getActionvalue(void) { return  actionValue; };
        inline uint32_t getIP(void) { return IP; };
        inline uint16_t getPort(void) { return port; };
};