#include "server.hpp"
#include "protocol.hpp"
#include "encoder.hpp"
#include "utils.hpp"

#include <asio/steady_timer.hpp>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>

/**
 * @brief Construct a new Server:: Server object
 *
 * @param listen_port port on which the server will listen
 * @param on_receive callback function used when receiving data
 */
Server::Server(__uint16_t listen_port)
: io_ctx_(),
  socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port)),
  work_guard_(asio::make_work_guard(io_ctx_)),
  timer(io_ctx_),
  recv_buffer_{},
  list_ip{},
  list_port{},
  interval(std::chrono::milliseconds(10))
{}



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
    io_thread_ = std::jthread(&Server::run, this);
    timer.expires_after(interval);
    StartTimer();
    do_receive();
}

/**
 * @brief server stopping function
 *
 */
void Server::stop() {
    if (!running_)
        return;
    running_ = false;
    work_guard_.reset();
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
            if (!error_code && bytes_recvd >= 3) {

                std::array<uint8_t, 9> arr;
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
void Server::send(size_t actVal, const std::string& host, __uint16_t port) {
    asio::ip::udp::endpoint endpoint(
        asio::ip::address::from_string(host),
        port
    );
    std::vector<uint8_t> buffer;
    if (actVal == BEGIN_GAME)
        buffer = PacketEncoder::encodeStart(currentID);
    if (actVal == ENTITY_CREATED)
        buffer = PacketEncoder::encodeCreate(currentID, 0, 0, 0, 0);
    if (actVal == ENTITY_MOVED)
        buffer = PacketEncoder::encodeMove(currentID, 0, 0, 0, 0);
    if (actVal == ENTITY_DELETED)
        buffer = PacketEncoder::encodeDelete(currentID, 0);
    if (actVal == PACKAGE_NOT_RECEIVED)
        buffer = PacketEncoder::encodeNotReceived(currentID);
    if (actVal == VALIDATION)
        buffer = PacketEncoder::encodeOK(currentID);
    currentID++;
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
    std::vector<size_t> IP(4);
    uint32_t binIP = receiver.getIP();
    IP[0] = binIP & 0xFFu;
    IP[1] = (binIP >> 8u)  & 0xFFu;
    IP[2] = (binIP >> 16u) & 0xFFu;
    IP[3] = (binIP >> 24u) & 0xFFu;
    for (size_t i = 0; i < list_ip.size(); ++i)
        if (list_ip.at(i).empty()) {
            list_ip.at(i) = IP;
            break;
        }
    return IP;
}


/**
 * @brief add the port stored inside the receiver to the list of ports
 *
 * @param tmpIP IP of the client linked to the port
 */
void Server::addPort(std::vector<size_t> tmpIP) {
    for (size_t i = 0; i < list_port.size(); i++)
        if (list_port.at(i) == 0 || !list_port.at(i)) {
            list_port.at(i) = receiver.getPort();
            return;
        }
    send(0, ipToString(tmpIP), receiver.getPort());
}

/**
 * @brief dispatch the content of the receiver based on the payload (the amount of content inside of it)
 *
 */
void Server::packetDispatch() {
    if (receiver.getPayload() == 6) {
        addPort(addIp());
        send(15, remote_endpoint_.address().to_string(), remote_endpoint_.port());
    }
    else if (receiver.getPayload() == 1) {
        if (receiver.getActionType() == 0)
            input_pressed(receiver.getActionvalue());
        else if (receiver.getActionType() == 1)
            input_released(receiver.getActionvalue());
        else
            send(0, ipToString(list_ip.at(0)), list_port.at(0));
    } else if (receiver.getPayload() == 0) {
        switch (receiver.getActionType()) {
            case (4):
                return;
            case (15):
                return;
        }
    }
}

/**
 * @brief automatically send packets to all the clients in the list
 *
 */
void Server::RoutineSender() {
    for (size_t i = 0; i < list_ip.size(); i++) {
        if (list_ip.at(i).empty() || list_port.at(i) == 0)
            continue;
        send(0, ipToString(list_ip.at(i)), list_port.at(i));
    }
    currentID++;
}

/**
 * @brief start the timer for the packets to be sent
 *
 */
void Server::StartTimer() {
    timer.async_wait([this](std::error_code ec) {
        if (!ec && running_) {
            RoutineSender();
            OnTimer();
        }
    });
}

/**
 * @brief helper function to restart the timer
 *
 */
void Server::OnTimer() {
    timer.expires_after(interval);
    StartTimer();
}

/**
 * @brief function that will send an "input pressed" to the ECS
 *
 */
void Server::input_pressed(uint8_t /*action*/) {
}

/**
 * @brief function that will send an "input released" to the ECS
 *
 */
void Server::input_released(uint8_t /*action*/) {
}
