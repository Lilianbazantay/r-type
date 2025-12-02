#include "client.hpp"
#include <iostream>

static void on_receive_callback(const std::string &text, const asio::ip::udp::endpoint &sender)
{
    std::cout << "[SERVER] " << text << "\n";
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

void Client::send_message(const std::string &msg)
{
    network_.send(msg, ip_, port_);
}
