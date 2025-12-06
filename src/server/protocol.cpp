#include <array>
#include <cstdint>

#include "protocol.hpp"

void Packet::getReceivedData(std::array<uint8_t, 8> received) {
    getPacketID(received);
    getActionType(received);
    getPayloadSize(received);
    if (payloadSize == 1)
        getActionValue(received);
    if (payloadSize == 2)
        getPort(received);
    if (payloadSize == 4)
        getIP(received);
}

void Packet::getPacketID(std::array<uint8_t, 8> received) {
    uint32_t id1 = received[1];
    uint32_t id2 = received[2];
    packetID = (id1 << 8u) | id2;
}

void Packet::getActionType(std::array<uint8_t, 8> received) {
    uint32_t act = received[2];
    actionType = (act >> ACT_SIZE) & 0x0Fu;
}

void Packet::getPayloadSize(std::array<uint8_t, 8> received) {
    uint32_t act = received[2];
    payloadSize = act & 0x0Fu;
}

void Packet::getActionValue(std::array<uint8_t, 8> received) {
    uint32_t act = received[3];
    payloadSize = act & 0x0Fu;
}

void Packet::getIP(std::array<uint8_t, 8> received) {
    uint8_t offset = 3;
    IP =
        (static_cast<uint32_t>(received.at(offset + 1)) << 24u) |
        (static_cast<uint32_t>(received.at(offset + 1)) << 16u) |
        (static_cast<uint32_t>(received.at(offset + 2)) << 8u)  |
        (static_cast<uint32_t>(received.at(offset + 3)));
}

void Packet::getPort(std::array<uint8_t, 8> received) {
    uint8_t offset = 3;
    port = (static_cast<uint32_t>(received.at(offset + 1)) << 8u) |
        received.at(offset + 1);
}
