#include "server.hpp"
#include "protocol.hpp"
#include "encoder.hpp"

#include <asio/ip/udp.hpp>
#include <asio/registered_buffer.hpp>
#include <asio/steady_timer.hpp>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
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
Server::Server(__uint16_t listen_port, NetworkServerBuffer *newRBuffer, NetworkClientBuffer *newSBuffer, NetworkClientBuffer *newCBuffer)
: receivedBuffer(newRBuffer),
  sendBuffer(newSBuffer),
  io_ctx_(),
  work_guard_(asio::make_work_guard(io_ctx_)),
  socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port)),
  timer(io_ctx_),
  recv_buffer_{},
  list_ip{},
  list_port{},
  interval(std::chrono::milliseconds(100))
{
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
            if (!error_code && bytes_recvd > 0) {
                std::vector<uint8_t> arr(recv_buffer_.data(),
                    recv_buffer_.data() + bytes_recvd);
                receiver.FillReceivedData(arr);
                for (size_t i = 0; i < 4; i++)
                    if (list_ip.at(i) == remote_endpoint_.address().to_string() && list_port.at(i) == remote_endpoint_.port())
                        receiver.FillPlayerId(i);
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
    std::shared_ptr<asio::ip::udp::endpoint> endpoint =
        std::make_shared<asio::ip::udp::endpoint>(asio::ip::address::from_string(host), port);
    std::shared_ptr<std::vector<uint8_t>> buffer = std::make_shared<std::vector<uint8_t>>();
    if (actVal == BEGIN_GAME)
        *buffer = PacketEncoder::encodeStart(currentID);
    if (actVal == ENTITY_CREATED)
        *buffer = PacketEncoder::encodeCreate(currentID, 0, 0, 0, 0);
    if (actVal == ENTITY_MOVED)
        *buffer = PacketEncoder::encodeMove(currentID, 0, 0, 0, 0);
    if (actVal == ENTITY_DELETED)
        *buffer = PacketEncoder::encodeDelete(currentID, 0);
    if (actVal == PACKAGE_NOT_RECEIVED)
        *buffer = PacketEncoder::encodeNotReceived(currentID);
    if (actVal == VALIDATION)
        *buffer = PacketEncoder::encodeOK(currentID);
    currentID++;
    try {
        socket_.async_send_to(
            asio::buffer(*buffer),
            *endpoint,
            [](std::error_code, std::size_t) {}
        );
    } catch(std::exception &e) {
        std::cout << e.what() << "\n";
    }
}

void Server::send(const std::string& host, __uint16_t port, std::vector<uint8_t> packet) {
    std::shared_ptr<asio::ip::udp::endpoint> endpoint =
        std::make_shared<asio::ip::udp::endpoint>(asio::ip::address::from_string(host), port);
    try {
        currentID++;
        socket_.async_send_to(
            asio::buffer(packet),
            *endpoint,
            [](std::error_code, std::size_t) {}
        );
    } catch(std::exception &e) {
        std::cout << e.what() << "\n";
    }
}

/**
 * @brief add the ip stored inside the receiver to the list of connected ips
 *
 * @return std::vector<size_t> ip received by the receiver
 */
size_t Server::addIp() {
    std::vector<size_t> IP(4);
    uint32_t binIP = receiver.getIP();
    IP[0] = binIP & 0xFFu;
    IP[1] = (binIP >> 8u)  & 0xFFu;
    IP[2] = (binIP >> 16u) & 0xFFu;
    IP[3] = (binIP >> 24u) & 0xFFu;
    for (size_t i = 0; i < list_ip.size(); ++i)
        if (list_ip.at(i).empty()) {
            list_ip.at(i) = remote_endpoint_.address().to_string();
            std::cout << list_ip.at(i);
            return i;
        }
    return 5;
}


/**
 * @brief add the port stored inside the receiver to the list of ports
 *
 * @param tmpIP IP of the client linked to the port
 */
size_t Server::addPort(size_t id) {
    std::cout << id;
    if (id == 5)
        return id;
    list_port.at(id) = remote_endpoint_.port();
    return id;
}

/**
 * @brief dispatch the content of the receiver based on the payload (the amount of content inside of it)
 *
 */
void Server::packetDispatch() {
    if (receiver.getPayload() == 6) {
        addConnection(addPort(addIp()));
        send(15, remote_endpoint_.address().to_string(), remote_endpoint_.port());
    }
    if (receiver.getPayload() == 1 && is_connected.at(receiver.getPlayerId()) && has_started.at(receiver.getPlayerId())) {
        receivedBuffer->pushPacket(receiver);
    } else if (receiver.getPayload() == 0 && is_connected.at(receiver.getPlayerId())) {
        switch (receiver.getActionType()) {
            case (4):
                addStart(receiver.getPlayerId());
                send(4, remote_endpoint_.address().to_string(), remote_endpoint_.port());
                receivedBuffer->pushPacket(receiver);
                break;
            case (8):
                addStart(receiver.getPlayerId());
                send(4, remote_endpoint_.address().to_string(), remote_endpoint_.port());
                receivedBuffer->pushPacket(receiver);
                break;
            case (2):
                addStart(receiver.getPlayerId());
                send(4, remote_endpoint_.address().to_string(), remote_endpoint_.port());
                receivedBuffer->pushPacket(receiver);
                break;
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
    std::string ip;
    size_t port;
    std::vector<std::vector<uint8_t>> buff = sendBuffer->popAllPackets();
    for (size_t i = 0; i < buff.size(); i++) {
        for (size_t j = 0; j < list_ip.size(); j++) {
            ip = list_ip.at(j);
            port = list_port.at(j);
            if (ip.empty() || port == 0)
                continue;
            send(ip, port, buff[i]);
        }
    }
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

void Server::addConnection(size_t id) {
    if (id == 5)
        return;
    is_connected.at(id) = true;
}

void Server::addStart(size_t id) {
    if (id == 5)
        return;
    has_started.at(id) = true;
}