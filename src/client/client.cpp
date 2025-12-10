#include "client.hpp"
#include <iostream>
#include "PacketCodec.hpp"

static uint16_t GLOBAL_PACKET_ID = 0;

/**
 * @brief Callback triggered when a packet is received from the server.
 *
 * @param data Pointer to received data
 * @param size Size of packet in bytes
 * @param sender UDP endpoint of the server
 */
static void on_receive_callback(const uint8_t *data,
                                size_t size,
                                const asio::ip::udp::endpoint &)
{
    ServerPacket pkt = decodeServerPacket(data, size);

    std::cout << "\n--- SERVER PACKET ---\n";
    std::cout << "packetId = " << pkt.packetId << "\n";
    std::cout << "action   = " << (int)pkt.actionType << "\n";
    std::cout << "entity   = " << (int)pkt.entityType << "\n";
    std::cout << "entityId = " << pkt.entityId << "\n";
    std::cout << "pos      = (" << pkt.posX << ", " << pkt.posY << ")\n";
}

/**
 * @brief Construct a client instance.
 *
 * @param ip Server IP
 * @param port Server port
 */
Client::Client(const std::string &ip, int port)
    : ip_(ip),
      port_(port),
      network_(0, on_receive_callback)
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
    uint8_t action =
        pressed ? INPUT_PRESSED : INPUT_RELEASED;

    uint8_t payload[1] = { inputCode };

    Packet p = encodeClientPacket(
        GLOBAL_PACKET_ID++,
        action,
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
        nullptr
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
    uint8_t payload[6] = {
        (uint8_t)((ipValue >> 24) & 0xFF),
        (uint8_t)((ipValue >> 16) & 0xFF),
        (uint8_t)((ipValue >> 8) & 0xFF),
        (uint8_t)( ipValue       & 0xFF),

        (uint8_t)((portValue >> 8) & 0xFF),
        (uint8_t)( portValue       & 0xFF)
    };

    Packet p = encodeClientPacket(
        GLOBAL_PACKET_ID++,
        PLAYER_CONNECT,
        6,
        payload
    );

    sendPacket(p);
}
