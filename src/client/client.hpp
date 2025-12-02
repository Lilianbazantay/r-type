#pragma once

#include <string>
#include "Asio.hpp"

class Client {
public:
    Client(const std::string &ip, int port);
    void start();
    void send_message(const std::string &msg);

private:
    std::string ip_;
    int port_;
    Asio_network network_;
};
