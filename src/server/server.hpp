#ifndef SERVER_HPP
#define SERVER_HPP


#include "server/protocol.hpp"
#include <array>
#include <cstddef>
#include <vector>
#pragma once

#include <asio.hpp>
#include <thread>
#include <atomic>
#include <functional>
#include <string>

/**
 * @brief server class, is the one communicating and parsing data with game and ECS
 *
 */
class Server {
    public:
        using ReceiveCallback = std::function<void(const std::string&, const asio::ip::udp::endpoint&)>;
        Server(__uint16_t listen_port, ReceiveCallback on_receive = nullptr);
        ~Server();

        void start();
        void stop();
        void send(size_t packetId, const std::string& host, __uint16_t port);
        bool gotText = false;
        size_t currentID;

    private:
        void do_receive();
        void run();

        void packetDispatch();

        std::vector<size_t> addIp();
        void addPort(std::vector <size_t> IP);

    private:
        asio::io_context io_ctx_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;

        std::array<char, 2048> recv_buffer_{};
        Packet receiver;
        std::array<std::vector<size_t>, 4> list_ip;
        std::array<size_t, 4> list_port;

        std::jthread io_thread_;
        std::atomic<bool> running_{false};

        ReceiveCallback receive_callback_;
};

#endif
