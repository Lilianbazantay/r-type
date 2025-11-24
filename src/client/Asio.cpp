#include "Asio.hpp"
#include <cstdio>
#include <iostream>
#include <ostream>

Asio_network::Asio_network(__uint16_t listen_port, ReceiveCallback on_receive)
    : socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port))
{
    this->receive_callback_ = on_receive;
}


Asio_network::~Asio_network() {
    stop();
}

void Asio_network::start() {
    if (running_)
        return;
    running_ = true;
    do_receive();
    io_thread_ = std::thread([this]() {
        try {
            io_ctx_.run();
        } catch (const std::exception& e) {
            std::cerr << "IO thread exception: " << e.what() << "\n";
        }
    });
}

void Asio_network::stop() {
    if (!running_)
        return;
    running_ = false;
    io_ctx_.stop();
    if (io_thread_.joinable())
        io_thread_.join();
    socket_.close();
}

void Asio_network::do_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](std::error_code error_code, std::size_t bytes_recvd) {
            if (!error_code && bytes_recvd > 0) {
                std::cout << "Received message:\n" << std::string(recv_buffer_.data(), bytes_recvd) << std::endl;
                gotText = true;
                if (receive_callback_) {
                    receive_callback_(recv_buffer_.data(),
                        remote_endpoint_);
                }
            }
            if (running_) {
                do_receive();
            }
        }
    );
}

void Asio_network::send(const std::string& msg, const std::string& host, __uint16_t port)
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
