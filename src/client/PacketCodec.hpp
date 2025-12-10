#pragma once
#include "Packet.hpp"
#include <string>

enum ActionTypeClient {
    INPUT_PRESSED = 0b0000,
    INPUT_RELEASED = 0b0001,
    PLAYER_CONNECT = 0b0010,
    START_GAME     = 0b0100,
    NEW_CONNECTION = 0b1000,
    NOT_RECEIVED   = 0b1100,
    OK_C           = 0b1111
};

Packet encodeClientPacket(
    uint16_t packetId,
    uint8_t actionType,
    uint8_t payloadSize,
    const uint8_t* payload
);

ServerPacket decodeServerPacket(const uint8_t* bytes, size_t size);
