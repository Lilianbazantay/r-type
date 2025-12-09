#include <array>
#include <cstdint>

#include "protocol.hpp"

/**
 * @brief parse the received data
 *
 * @param received data that has been received
 */
void Packet::FillReceivedData(std::array<uint8_t, 8> received) {
    FillPacketID(received);
    FillActionType(received);
    FillPayloadSize(received);
    if (payloadSize == 1)
        FillActionValue(received);
    if (payloadSize == 2)
        FillPort(received, 3);
    if (payloadSize == 4)
        FillIP(received);
    if (payloadSize == 6) {
        FillIP(received);
        FillPort(received, 7);
    }
}

/**
 * @brief extract packetID from data
 *
 * @param received data that has been received
 */
void Packet::FillPacketID(std::array<uint8_t, 8> received) {
    uint32_t id1 = received[1];
    uint32_t id2 = received[2];
    packetID = (id1 << 8u) | id2;
}

/**
 * @brief extract the actionType from data
 *
 * @param received data that has been received
 */
void Packet::FillActionType(std::array<uint8_t, 8> received) {
    uint32_t act = received[2];
    actionType = (act >> ACT_SIZE) & 0x0Fu;
}

/**
 * @brief extract payload size from data
 *
 * @param received data that has been received
 */
void Packet::FillPayloadSize(std::array<uint8_t, 8> received) {
    uint32_t act = received[2];
    payloadSize = act & 0x0Fu;
}

/**
 * @brief extract action value from data
 *
 * @param received data that has been received
 */
void Packet::FillActionValue(std::array<uint8_t, 8> received) {
    uint32_t act = received[3];
    payloadSize = act & 0x0Fu;
}

/**
 * @brief extract the IP from data
 *
 * @param received data that has been received
 */
void Packet::FillIP(std::array<uint8_t, 8> received) {
    uint8_t offset = 3;
    IP =
        (static_cast<uint32_t>(received.at(offset + 0)) << 24u) |
        (static_cast<uint32_t>(received.at(offset + 1)) << 16u) |
        (static_cast<uint32_t>(received.at(offset + 2)) << 8u)  |
        (static_cast<uint32_t>(received.at(offset + 3)));
}

/**
 * @brief extract the Port from data
 *
 * @param received data that has been received
 */
void Packet::FillPort(std::array<uint8_t, 8> received, uint8_t offset) {
    port = (static_cast<uint32_t>(received.at(offset + 1)) << 8u) |
        received.at(offset + 1);
}
