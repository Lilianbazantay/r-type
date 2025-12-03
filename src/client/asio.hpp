#ifndef ASIO_NETWORK_HPP
#define ASIO_NETWORK_HPP


#pragma once
#include <asio.hpp>
#include <thread>
#include <atomic>
#include <functional>
#include <string>

#define HEADER_PLAYER_ID_POSITION 60
#define HEADER_MESSAGE_ID_POSITION 44
#define BODY_MESSAGE_TYPE_POSITION 40

#define _4BITS_SIZE 4
#define _8BITS_SIZE 8
#define _16BITS_SIZE 16

class Asio_network {
    public:
        using ReceiveCallback = std::function<void(const std::string&, const asio::ip::udp::endpoint&)>;
        Asio_network(__uint16_t listen_port, ReceiveCallback on_receive = nullptr);
        ~Asio_network();

        void start();
        void stop();
        void send(const __uint64_t msg, const std::string& host, __uint16_t port);
        void set_data_sent(std::vector<__uint64_t>);
        void set_data_received(std::vector<__uint64_t>);
        bool gotText = false;
        int player_id = 0;

    private:
        void do_receive();
        void run();

        __uint64_t set_header(__uint64_t);
        void request_package(__uint16_t);
        void parse_header_binary(__uint64_t message);
    private:
        asio::io_context io_ctx_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;

        std::array<char, 2048> recv_buffer_{};

        std::thread io_thread_;
        std::atomic<bool> running_{false};

        ReceiveCallback receive_callback_;
        std::vector<__uint64_t>receive_data_list = {};
        std::vector<__uint64_t>send_data_list = {};
    private:
        std::string saved_ip_address = "";
        uint16_t saved_port = 0;
};

#endif
