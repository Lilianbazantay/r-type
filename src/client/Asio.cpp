#include "Asio.hpp"
#include <cstdio>
#include <iostream>
#include <ostream>
#include <thread>

/**
 * @brief Construct a UDP network handler.
 *
 * @param listen_port UDP port to bind the socket to
 * @param on_receive Callback function
 */
Asio_network::Asio_network(__uint16_t listen_port, ReceiveCallback on_receive)
    : work_guard_(asio::make_work_guard(io_ctx_)),
    socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port))
{
    this->receive_callback_ = on_receive;
}

/**
 * @brief Destroy the network handler.
 */
Asio_network::~Asio_network() {
    stop();
}

/**
 * @brief Run the ASIO I/O context loop.
 */
void Asio_network::run() {
    try {
        io_ctx_.run();
        std::cout << "io_context stopped\n";
    } catch (const std::exception& e) {
        std::cerr << "IO thread exception: " << e.what() << "\n";
    }
}

/**
 * @brief Start the network system.
 */
void Asio_network::start() {
    if (running_)
        return;
    running_ = true;
    do_receive();
    io_thread_ = std::jthread(&Asio_network::run, this);
    std::cout << "io_context stopped\n";
}

/**
 * @brief Stop the network system.
 */
void Asio_network::stop() {
    if (!running_)
        return;
    std::cout << "Stopping\n";
    running_ = false;
    work_guard_.reset();
    io_ctx_.stop();
    socket_.close();
}

/**
 * @brief receive UDP packets.
 */
void Asio_network::do_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](std::error_code error_code, std::size_t bytes_recvd) {
            std::cout << "listening\n";
            if (!error_code && bytes_recvd > 0) {
                std::cout << "Received message:\n" << std::string(recv_buffer_.data(), bytes_recvd) << std::endl;
                gotText = true;
                std::vector<uint8_t> arr(recv_buffer_.data(),
                    recv_buffer_.data() + bytes_recvd);
                if (receive_callback_) {
                    receive_callback_(
                        arr,
                        bytes_recvd,
                        remote_endpoint_
                    );
                }
            }
            if (error_code) {
                std::cerr << "Receive error: "
                    << error_code.message() << "\n";
            }

            if (running_)
                do_receive();
        }
    );
}

/**
 * @brief Send a UDP message to a remote host.
 *
 * @param msg Data to send
 * @param host Destination ip
 * @param port Destination port
 */
void Asio_network::send(const std::string& msg, const std::string& host, __uint16_t port)
{
    asio::ip::udp::endpoint endpoint(
        asio::ip::address::from_string(host),
        port
    );
    socket_.async_send_to(
        asio::buffer(msg, msg.size()),
        endpoint,
        [](std::error_code, std::size_t) {}
    );
}
