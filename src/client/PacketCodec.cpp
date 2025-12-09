#include "PacketCodec.hpp"
#include <cstring>

Packet encodeClientPacket(
    uint16_t packetId,
    uint8_t actionType,
    uint8_t payloadSize,
    const uint8_t* payload
) {
    Packet p;

    p.bytes[0] = (packetId >> 8) & 0xFF;
    p.bytes[1] = packetId & 0xFF;
    p.bytes[2] = ((actionType & 0x0F) << 4) | (payloadSize & 0x0F);
    for (int i = 0; i < payloadSize; ++i) {
        p.bytes[3 + i] = payload[i];
    }
    p.size = 3 + payloadSize;
    return p;
}

ServerPacket decodeServerPacket(const uint8_t* b, size_t size) {
    ServerPacket p {};

    p.packetId = (b[0] << 8) | b[1];
    p.payloadSize = (b[2] >> 4) & 0x0F;
    p.actionType  =  b[2] & 0x0F;

    if (size < 3 + p.payloadSize)
        return p;

    if (p.payloadSize >= 1) p.entityType = b[3];
    if (p.payloadSize >= 3) p.entityId = (b[4] << 8) | b[5];
    if (p.payloadSize >= 6) {
        uint32_t pos = (b[6] << 16) | (b[7] << 8) | b[8];
        p.posX = (pos >> 12) & 0xFFF;
        p.posY = pos & 0xFFF;
    }

    return p;
}
