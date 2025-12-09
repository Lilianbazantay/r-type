#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

struct Packet {
    std::array<uint8_t, 32> bytes;
    size_t size = 0;
};

struct ServerPacket {
    uint16_t packetId;
    uint8_t payloadSize;
    uint8_t actionType;

    uint8_t entityType;
    uint16_t entityId;

    uint16_t posX;
    uint16_t posY;
};
