#pragma once

#include <array>
#include <cstdint>
#include <vector>

#define ACT_SIZE           (uint64_t)4


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

class PacketEncoder {
public:
    // Build full packet
    static std::vector<uint8_t> encode(
        uint16_t packetID,
        uint8_t payloadSize,
        uint8_t actionType,
        uint8_t entityType,
        uint16_t entityID,
        uint16_t posX,
        uint16_t posY) {
            std::vector<uint8_t> out(9);
            // --- HEADER ---
            uint32_t pid = packetID;
            out[0] = static_cast<uint8_t>((pid >> 8u) & 0xFFu);
            out[1] = static_cast<uint8_t>( packetID & 0xFFu);
            out[2] = static_cast<uint8_t>(((payloadSize & 0x0Fu) << 4u) |
                ( actionType & 0x0Fu));
            // --- BODY ---
            out[3] = entityType;
            uint32_t eid = entityID;
            out[4] = static_cast<uint8_t>((eid >> 8u) & 0xFFu);
            out[5] = static_cast<uint8_t>( entityID        & 0xFFu);
            // --- Position: pack X/Y into 24 bits ---
            posX &= 0x0FFFu;
            posY &= 0x0FFFu;
            uint32_t pos24 =
                ((uint32_t)posX << 12u) |
                ((uint32_t)posY);
            out[6] = static_cast<uint8_t>((pos24 >> 16u) & 0xFFu);
            out[7] = static_cast<uint8_t>((pos24 >> 8u)  & 0xFFu);
            out[8] = static_cast<uint8_t>( pos24         & 0xFFu);
            return out;
        }   

    static std::vector<uint8_t> encodeCreate(
        uint16_t packetID,
        uint8_t entityType,
        uint16_t entityID,
        uint16_t posX,
        uint16_t posY) {
            return encode(packetID,
                6,
                0x0u,
                entityType,
                entityID,
                posX,
                posY);
        }

    // entity moved
    static std::vector<uint8_t> encodeMove(
        uint16_t packetID,
        uint16_t entityID,
        uint16_t posX,
        uint16_t posY) {
            return encode(packetID,
                5,
                0x1u, // entity moved
                0u,   // no type included
                entityID,
                posX,
                posY);
        }

    // entity deleted
    static std::vector<uint8_t> encodeDelete(
        uint16_t packetID,
        uint16_t entityID) {
            return encode(packetID,
                2,
                0x2u, // entity deleted
                0u,
                entityID,
                0, 0);
        }
};
