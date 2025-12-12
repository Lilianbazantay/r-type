#include "client.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include "PacketCodec.hpp"

static uint16_t GLOBAL_PACKET_ID = 0;

static void on_receive_callback(const std::vector<uint8_t>data,
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

Client::Client(const std::string &ip, int port)
    : ip_(ip),
      port_(port),
      network_(0, on_receive_callback)
{
}

void Client::start()
{
    std::cout << "Starting client...\n";
    network_.start();
}

void Client::sendPacket(const Packet &p)
{
    network_.send(std::string((char*)p.bytes.data(), p.size), ip_, port_);

    std::cout << "[CLIENT] Sent packet (" << p.size << " bytes)\n";
}

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
