#include "server.hpp"
#include "protocol.hpp"

#include <cstdio>
#include <iostream>
#include <thread>

Server::Server(__uint16_t listen_port, ReceiveCallback on_receive)
    : socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port))
{
    this->receive_callback_ = on_receive;
}


Server::~Server() {
    stop();
}

void Server::run() {
    try {
        io_ctx_.run();
    } catch (const std::exception& e) {
        std::cerr << "IO thread exception: " << e.what() << "\n";
    }
}

void Server::start() {
    if (running_)
        return;
    running_ = true;
    do_receive();
    io_thread_ = std::jthread(&Server::run, this);
}

void Server::stop() {
    if (!running_)
        return;
    running_ = false;
    io_ctx_.stop();
    socket_.close();
}

void Server::do_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](std::error_code error_code, std::size_t bytes_recvd) {
            if (!error_code && bytes_recvd >= 8) {

                std::array<uint8_t, 8> arr;
                std::memcpy(arr.data(), recv_buffer_.data(), 8);

                Packet pkt;
                pkt.getReceivedData(arr);
            }

            if (running_)
                do_receive();
        }
    );
}


void Server::send(const std::string& msg, const std::string& host, __uint16_t port)
{
    asio::ip::udp::endpoint endpoint(
        asio::ip::address::from_string(host),
        port
    );
    socket_.async_send_to(
        asio::buffer(msg),
        endpoint,
        [](std::error_code, std::size_t) {}
    );
}
