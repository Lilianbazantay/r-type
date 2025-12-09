#include "server.hpp"
#include "protocol.hpp"
#include "server/encoder.hpp"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

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
                std::cout << arr.data();

                receiver.FillReceivedData(arr);
                packetDispatch();
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
void Server::send(size_t packetId, const std::string& host, __uint16_t port)
{
    asio::ip::udp::endpoint endpoint(
        asio::ip::address::from_string(host),
        port
    );
    std::vector<uint8_t> buffer = PacketEncoder::encodeStart(packetId);
    socket_.async_send_to(
        asio::buffer(buffer),
        endpoint,
        [](std::error_code, std::size_t) {}
    );
}
/**
 * @brief add the ip stored inside the receiver to the list of connected ips
 *
 * @return std::vector<size_t> ip received by the receiver
 */
std::vector<size_t> Server::addIp() {
    std::vector<size_t> IP;
    uint32_t binIP = receiver.getIP();
    IP.at(0) = binIP & 0xFFu;
    IP.at(1) = (binIP >> 8u)  & 0xFFu;
    IP.at(2) = (binIP >> 16u) & 0xFFu;
    IP.at(3) = (binIP >> 24u) & 0xFFu;
    for (size_t i = 0; i < list_ip.size(); i++)
        if (list_ip.at(i).empty())
            list_ip.at(i) = IP;
    return IP;
}
/**
 * @brief add the port stored inside the receiver to the list of ports
 *
 * @param tmpIP IP of the client linked to the port
 */
void Server::addPort(std::vector<size_t> tmpIP) {
    std::string stringIP = "";
    for (size_t i = 0; i < tmpIP.size(); i++)
        stringIP += std::format("{}{}", tmpIP[i], (i + 1 < tmpIP.size() ? "." : ""));
    for (size_t i = 0; i < list_port.size(); i++)
        if (list_port.at(i) == 0 || !list_port.at(i)) {
            list_port.at(i) = receiver.getPort();
            return;
        }
    send(currentID, stringIP, receiver.getPort());
}
/**
 * @brief dispatch the content of the receiver based on the payload (the amount of content inside of it)
 *
 */
void Server::packetDispatch() {
    if (receiver.getPayload() == 7) {
        std::vector<size_t> tmpIP = addIp();
        addPort(tmpIP);
    }
}