#include "asio.hpp"
#include <bitset>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <thread>


/* PARSE_HEADER_BINARY
**
** parse the header of the received *message*
** automatically checks if a package is lost and asks for a resend
*/
void Asio_network::parse_header_binary(__uint64_t message) {
    std::cout << "trying to parse" << std::endl;
    __uint16_t player = message >> HEADER_PLAYER_ID_POSITION;
    __uint16_t packageId = message >> HEADER_MESSAGE_ID_POSITION;
    std::cout << "Player: " << player << ", Package ID: " << std::bitset<16>(packageId) << std::endl;
//    uint64_t result = (message >> 40) & 0xF;
//    std::cout << "Message Identifier: " << result << std::endl;
    if (packageId < this->receive_data_list.size() && this->receive_data_list[packageId] == 0) {
        std::cout << "ID Smaller" << std::endl;
        this->receive_data_list[packageId] = message;
        return;
    }
    for (; packageId > this->receive_data_list.size(); this->receive_data_list.push_back(0)) {
        std::cout << "ID Bigger" << std::endl;
        request_package(this->receive_data_list.size());
    }
    if (packageId == this->receive_data_list.size()) {
        this->receive_data_list.push_back(message);
        std::cout << "Perfect size" << std::endl;
        return;
    }
}

/* REQUEST
**
** send a request for a specific package to a specific player
*/
void Asio_network::request_package(__uint16_t MissedpackageId) {
    if (saved_ip_address == "")
        return;
    __uint64_t msg = 0;
    msg |= (__uint64_t)12 << BODY_MESSAGE_TYPE_POSITION; // 12 is the code for missed package
    msg |= (__uint64_t)MissedpackageId << BODY_MESSAGE_TYPE_POSITION - _16BITS_SIZE;
    send(msg, saved_ip_address, saved_port);
    return;
}

/* HEADER
**
** automatically sets the header of any message, and returns it
*/
__uint64_t Asio_network::set_header(__uint64_t msg) {
    msg |= (__uint64_t)player_id << HEADER_PLAYER_ID_POSITION;
    msg |= (__uint64_t)((__uint16_t)send_data_list.size()) << HEADER_MESSAGE_ID_POSITION;

    return msg;
}

/* CONSTRUCTOR
**
** Links the chosen port with Asio system.
** Initializes the function ReceiveCallback
*/
Asio_network::Asio_network(__uint16_t listen_port, ReceiveCallback on_receive)
    : socket_(io_ctx_, asio::ip::udp::endpoint(asio::ip::udp::v4(), listen_port))
{
    this->receive_callback_ = on_receive;
}


/* DESTRUCTOR
**
** stops Asio
*/
Asio_network::~Asio_network() {
    stop();
}

void Asio_network::run() {
    try {
        io_ctx_.run();
    } catch (const std::exception& e) {
        std::cerr << "IO thread exception: " << e.what() << "\n";
    }
}

void Asio_network::start() {
    if (running_)
        return;
    running_ = true;
    do_receive();
    io_thread_ = std::thread(&Asio_network::run, this);
}

void Asio_network::stop() {
    if (!running_)
        return;
    running_ = false;
    if (io_thread_.joinable())
        io_thread_.join();
    io_ctx_.stop();
    socket_.close();
}

void Asio_network::do_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](std::error_code error_code, std::size_t bytes_recvd) {
            if (!error_code && bytes_recvd == sizeof(uint64_t)) {
                uint64_t value_be = 0;
                std::memcpy(&value_be, recv_buffer_.data(), sizeof(uint64_t));
                uint64_t value = be64toh(value_be);
                receive_data_list.push_back(value);
                std::cout << "Received 64-bit value:\n";
                std::cout << std::bitset<64>(value) << std::endl;
                parse_header_binary(value);
                gotText = true;
                if (receive_callback_) {
                    receive_callback_(recv_buffer_.data(), remote_endpoint_);
                }
            }

            if (running_)
                do_receive();
        }
    );
}

void Asio_network::send(const __uint64_t msg, const std::string& host, __uint16_t port)
{
    asio::ip::udp::endpoint endpoint(
        asio::ip::address::from_string(host),
        port
    );
    if (saved_ip_address == "") {
        saved_ip_address = host;
        saved_port = port;
    }
    __uint64_t msg_big_endian = htobe64(set_header(msg));
    std::cout << "Sending:\n" << std::bitset<64>(msg) << std::endl;
    send_data_list.push_back(msg_big_endian);
    socket_.async_send_to(
        asio::buffer(&msg_big_endian, sizeof(msg_big_endian)),
        endpoint,
        [](std::error_code, std::size_t) {}
    );
}

void Asio_network::set_data_sent(std::vector<__uint64_t> new_list) {
    this->send_data_list = new_list;
}

void Asio_network::set_data_received(std::vector<__uint64_t> new_list) {
    this->receive_data_list = new_list;
}
