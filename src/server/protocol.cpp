#include <cstddef>
#include <cstdint>

#include "protocol.hpp"

/**
 * @brief parse the received data
 *
 * @param received data that has been received
 */
void Packet::FillReceivedData(std::vector<uint8_t> received) {
    FillPacketID(received);
    FillActionType(received);
    FillPayloadSize(received);
    if (actionType == OK || actionType == START_GAME)
        return;
    else if (actionType == PLAYER_CONNECT || actionType == NEW_CONNECTION) {
        FillIP(received);
        FillPort(received, 7);
    } else if (actionType == NOT_RECEIVED)
        FillPort(received, 3);
    else
        FillActionValue(received);
}

/**
 * @brief extract packetID from data
 *
 * @param received data that has been received
 */
void Packet::FillPacketID(std::vector<uint8_t> received) {
    uint32_t id1 = received[1];
    uint32_t id2 = received[2];
    packetID = (id1 << 8u) | id2;
}

/**
 * @brief extract the actionType from data
 *
 * @param received data that has been received
 */
void Packet::FillActionType(std::vector<uint8_t> received) {
    uint32_t act = received[2];
    actionType = (act >> ACT_SIZE) & 0x0Fu;
}

/**
 * @brief extract payload size from data
 *
 * @param received data that has been received
 */
void Packet::FillPayloadSize(std::vector<uint8_t> received) {
    uint32_t act = received[2];
    payloadSize = act & 0x0Fu;
}

/**
 * @brief extract action value from data
 *
 * @param received data that has been received
 */
void Packet::FillActionValue(std::vector<uint8_t> received) {
    uint32_t act = received[3];
    payloadSize = act & 0x0Fu;
}

/**
 * @brief extract the IP from data
 *
 * @param received data that has been received
 */
void Packet::FillIP(std::vector<uint8_t> received) {
    uint8_t offset = 3;
    IP =
        (static_cast<uint32_t>(received.at(offset + 0))) |
        (static_cast<uint32_t>(received.at(offset + 1)) << 8u) |
        (static_cast<uint32_t>(received.at(offset + 2)) << 16u) |
        (static_cast<uint32_t>(received.at(offset + 3)) << 24u);
}

/**
 * @brief extract the Port from data
 *
 * @param received data that has been received
 */
void Packet::FillPort(std::vector<uint8_t> received, size_t offset) {
    port = (static_cast<uint32_t>(received.at(offset)) << 8u) |
        received.at(offset + 1);
}
