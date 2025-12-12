#pragma once
#include <string>
#include "Asio.hpp"
#include "Packet.hpp"

class Client {
public:
    Client(const std::string &ip, int port);
    void start();
    void sendInput(bool pressed, uint8_t inputCode);
    void sendStartGame();
    void sendConnectionRequest(uint32_t ipValue, uint16_t portValue);

private:
    std::string ip_;
    int port_;
    Asio_network network_;
    void sendPacket(const Packet &p);
};
