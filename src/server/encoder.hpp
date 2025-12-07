#pragma once

#include <cstdint>
#include <vector>

class PacketEncoder {
public:
    // Build full packet
    static std::vector<uint8_t> encode(
        uint16_t packetID, uint8_t payloadSize, uint8_t actionType,
        uint8_t entityType = 0, uint16_t entityID = 0,
        uint16_t posX = 0, uint16_t posY = 0) {
            std::vector<uint8_t> out(9);

            uint32_t pid = packetID;
            out[0] = static_cast<uint8_t>((pid >> 8u) & 0xFFu);
            out[1] = static_cast<uint8_t>( packetID & 0xFFu);
            out[2] = static_cast<uint8_t>(((payloadSize & 0x0Fu) << 4u) |
                ( actionType & 0x0Fu));

            out[3] = entityType;
            uint32_t eid = entityID;
            out[4] = static_cast<uint8_t>((eid >> 8u) & 0xFFu);
            out[5] = static_cast<uint8_t>( entityID        & 0xFFu);

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
        uint8_t entityType, uint16_t entityID,
        uint16_t posX, uint16_t posY) {
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
        uint16_t posX, uint16_t posY) {
            return encode(packetID,
                5,
                0x1u,
                0u,
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

    // start game
    static std::vector<uint8_t> encodeStart(
        uint16_t packetID) {
            return encode(packetID,
            0,
            0x4u);
        }

    // package not received
    static std::vector<uint8_t> encodeNotReceived(
        uint16_t packetID){
            return encode(packetID,
            0,
            0x12u);
        }
    
    // OK msg
    static std::vector<uint8_t> encodeOK(
        uint16_t packetID){
            return encode(packetID,
            0,
            0x15u);
        }
};
