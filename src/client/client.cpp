#include "client.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include "PacketCodec.hpp"

static uint16_t GLOBAL_PACKET_ID = 0;

Client::Client(const std::string &ip, int port, NetworkBuffer* buffer)
    : ip_(ip), port_(port), buffer_(buffer),
      network_(0, [this](const std::vector<uint8_t> data, size_t size, const asio::ip::udp::endpoint&) {
          ServerPacket pkt = decodeServerPacket(data, size);

          std::cout << "[CLIENT] Packet received id=" << pkt.packetId << "\n";

          if (buffer_)
              buffer_->pushPacket(pkt);
      })
{
}


/**
 * @brief Start the client network system.
 */
void Client::start()
{
    std::cout << "Starting client...\n";
    network_.start();
}

/**
 * @brief Send encoded packet to the server.
 *
 * @param p Packet structure
 */
void Client::sendPacket(const Packet &p)
{
    network_.send(std::string((char*)p.bytes.data(), p.size), ip_, port_);

    std::cout << "[CLIENT] Sent packet (" << p.size << " bytes)\n";
}

/**
 * @brief Send an input event to the server.
 *
 * @param pressed True if input is pressed, false if released
 * @param inputCode Input identifier (FIRE, UP, etc.)
 */
void Client::sendInput(bool pressed, uint8_t inputCode)
{
    std::vector<uint8_t> payload = {inputCode};

    Packet p = encodeClientPacket(
        GLOBAL_PACKET_ID++,
        pressed,
        1,
        payload
    );

    sendPacket(p);
}

/**
 * @brief Send a start game request to the server.
 */
void Client::sendStartGame()
{
    Packet p = encodeClientPacket(
        GLOBAL_PACKET_ID++,
        START_GAME,
        0,
        {}
    );

    sendPacket(p);
}

/**
 * @brief Send a connection request packet to the server.
 *
 * @param ipValue IPv4 address encoded as a 32-bit integer
 * @param portValue Client port number
 */
void Client::sendConnectionRequest(uint32_t ipValue, uint16_t portValue)
{
    std::vector<uint8_t> payload{(uint8_t)((ipValue >> 24u) & 0xFFu),
        (uint8_t)((ipValue >> 16u) & 0xFFu),
        (uint8_t)((ipValue >> 8u) & 0xFFu),
        (uint8_t)( ipValue       & 0xFFu),

        (uint8_t)((static_cast<uint32_t>(portValue) >> 8u) & 0xFFu),
        (uint8_t)( portValue       & 0xFFu)};

    Packet p = encodeClientPacket(
        GLOBAL_PACKET_ID++,
        PLAYER_CONNECT,
        6,
        payload
    );

    sendPacket(p);
}
