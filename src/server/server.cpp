#include "server.hpp"
#include "protocol.hpp"

#include <cstdio>
#include <iostream>
#include <thread>

/**
 * @brief Construct a new Server:: Server object
 * 
 * @param listen_port port on which the server will listen
 * @param on_receive callback function used when receiving data
 */
Server::Server(__uint16_t listen_port, ReceiveCallback on_receive)
    : socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port))
{
    this->receive_callback_ = on_receive;
}

/**
 * @brief Destroy the Server:: Server object
 * 
 */
Server::~Server() {
    stop();
}

/**
 * @brief server's runner
 * 
 */
void Server::run() {
    try {
        io_ctx_.run();
    } catch (const std::exception& e) {
        std::cerr << "IO thread exception: " << e.what() << "\n";
    }
}

/**
 * @brief server's starter function
 * 
 */
void Server::start() {
    if (running_)
        return;
    running_ = true;
    do_receive();
    io_thread_ = std::jthread(&Server::run, this);
}

/**
 * @brief server stopping function
 * 
 */
void Server::stop() {
    if (!running_)
        return;
    running_ = false;
    io_ctx_.stop();
    socket_.close();
}

/**
 * @brief server's receiver function.
 * 
 */
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


/**
 * @brief server's sender function. It will send the msg contained to the host at a certain port
 * 
 * @param msg message to send
 * @param host host's IP
 * @param port host's port
 */
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
