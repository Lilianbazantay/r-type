#ifndef ASIO_NETWORK_HPP
#define ASIO_NETWORK_HPP


#pragma once
#include <asio.hpp>
#include <thread>
#include <atomic>
#include <functional>
#include <string>

class Asio_network {
    public:
        using ReceiveCallback = std::function<void(const std::string&, const asio::ip::udp::endpoint&)>;
        Asio_network(__uint16_t listen_port, ReceiveCallback on_receive = nullptr);
        ~Asio_network();

        void start();
        void stop();
        void send(const std::string& msg, const std::string& host, __uint16_t port);
        bool gotText = false;

    private:
        void do_receive();
        void run();

    private:
        asio::io_context io_ctx_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;

        std::array<char, 2048> recv_buffer_{};

        std::jthread io_thread_;
        std::atomic<bool> running_{false};

        ReceiveCallback receive_callback_;
};

#endif
