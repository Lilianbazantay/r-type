#ifndef SERVER_HPP
#define SERVER_HPP


#include <vector>
#pragma once

#include <asio.hpp>
#include <thread>
#include <atomic>
#include <string>

#include "NetworkServerBuffer.hpp"
#include "protocol.hpp"
#include <array>
#include <asio/steady_timer.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>

/**
 * @brief server class, is the one communicating and parsing data with game and ECS
 *
 */
class Server {
    public:
        Server(std::uint16_t listen_port, NetworkServerBuffer *, NetworkClientBuffer *, NetworkContinuousBuffer *);
        ~Server();

        void start();
        void stop();
        void send(size_t actVal, const std::string& host, std::uint16_t port);
        void send(const std::string& host, uint16_t port, std::vector<uint8_t> pck);
        size_t currentID = 0;

    private:
        void do_receive();
        void run();
        void RoutineSender();

        void packetDispatch();
        void StartTimer();
        void OnTimer();
        std::array<bool, 4> is_connected = {false, false, false, false};
        std::array<bool, 4> has_started = {false, false, false, false};

        size_t addIp();
        size_t addPort(size_t id);
        void addConnection(size_t id);
        void addStart(size_t id);
        static void SigHandler(int signal);
        void handleShutdown();
        static inline std::atomic<bool> shutdown_requested{false};
        static inline Server* instance{nullptr};

    private:
        NetworkServerBuffer *receivedBuffer;
        NetworkClientBuffer *sendBuffer;
        NetworkContinuousBuffer *continuousBuffer;
        asio::io_context io_ctx_;
        asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        asio::steady_timer timer{io_ctx_};


        std::array<char, 2048> recv_buffer_{};
        std::array<std::string, 4> list_ip;
        std::array<size_t, 4> list_port;
        std::chrono::milliseconds interval{100};

        std::jthread io_thread_;
        std::jthread sender_thread;
        std::atomic<bool> running_{false};

        ServerPacket receiver;

};

#endif
