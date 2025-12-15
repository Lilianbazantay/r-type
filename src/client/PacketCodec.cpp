#include "PacketCodec.hpp"
#include "client/Packet.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

/**
 * @brief Encode a client-to-server network packet
 *
 * Packet layout:
 * - 2 bytes : packet ID
 * - 1 byte  : action type | payload size
 * - N bytes : payload data
 *
 * @param packetId Unique identifier of the packet
 * @param actionType Type of action encoded in the packet
 * @param payloadSize Size of the payload in bytes
 * @param payload Pointer to the payload data
 *
 * @return Packet encoded, ready to be sent to network
 */

Packet encodeClientPacket(
    uint16_t packetId,
    uint8_t actionType,
    uint8_t payloadSize,
    std::vector<uint8_t> payload
) {
    Packet p{};

    p.bytes[0] = (static_cast<uint32_t>(packetId) >> 8u) & 0xFFu;
    p.bytes[1] = packetId & 0xFFu;
    p.bytes[2] = ((actionType & 0x0Fu) << 4u) | (payloadSize & 0x0Fu);
    for (int i = 0; i < payloadSize; ++i) {
        p.bytes.at(3 + i) = payload[i];
    }
    p.size = 3 + payloadSize;
    return p;
}

/**
 * @brief Decode a server-to-client packet.
 *
 * Parse and extract from server:
 * - packet ID
 * - action type
 * - payload size
 * - entity type
 * - entity ID
 * - position (X, Y)
 *
 * @param b Pointer to the received bytes from server.
 * @param size Number of bytes received.
 * @return ServerPacket Decoded packet structure.
 */
NetworkPacket decodeNetworkPacket(std::vector<uint8_t>b, size_t size) {
    NetworkPacket p {};

    std::cout << "Decoding..." << std::endl;
    p.packetId = (static_cast<uint32_t>(b.at(0)) << 8u) | b[1];
    p.payloadSize = (static_cast<uint32_t>(b.at(2)) >> 4u) & 0x0Fu;
    p.actionType  =  b.at(2) & 0x0Fu;

    if (static_cast<int>(size) < 3 + p.payloadSize)
        return p;

    if (p.payloadSize >= 1) p.entityType = b[3];
    if (p.payloadSize >= 3) p.entityId = (static_cast<uint32_t>(b.at(4)) << 8u) | b[5];
    if (p.payloadSize >= 6) {
        uint32_t pos = (static_cast<uint32_t>(b.at(6)) << 16u) |
        (static_cast<uint32_t>(b.at(7)) << 8u) | b[8];
        p.posX = (pos >> 12u) & 0xFFFu;
        p.posY = pos & 0xFFFu;
    }
    return p;
}
