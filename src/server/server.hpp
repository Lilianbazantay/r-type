#ifndef SERVER_HPP
#define SERVER_HPP


#include "server/protocol.hpp"
#include <array>
#include <asio/steady_timer.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <vector>
#pragma once

#include <asio.hpp>
#include <thread>
#include <atomic>
#include <string>

/**
 * @brief server class, is the one communicating and parsing data with game and ECS
 *
 */
class Server {
    public:
        Server(__uint16_t listen_port);
        ~Server();

        void start();
        void stop();
        void send(size_t packetId, const std::string& host, __uint16_t port);
        bool gotText = false;
        size_t currentID;
        void input_pressed(uint8_t action);
        void input_released(uint8_t action);

    private:
        void do_receive();
        void run();
        void RoutineSender();

        void packetDispatch();
        void StartTimer();
        void OnTimer();

        std::vector<size_t> addIp();
        void addPort(std::vector <size_t> IP);

    private:
        asio::io_context io_ctx_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        asio::steady_timer timer{io_ctx_};

        std::array<char, 2048> recv_buffer_{};
        std::array<std::vector<size_t>, 4> list_ip;
        std::array<size_t, 4> list_port;
        std::chrono::milliseconds interval{10};

        std::jthread io_thread_;
        std::jthread sender_thread;
        std::atomic<bool> running_{false};

        Packet receiver;

};

#endif
