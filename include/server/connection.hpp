#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <boost/system/error_code.hpp>
#include <functional>

using namespace boost::asio;
using namespace boost::placeholders;
using namespace boost::system;


class Connection: public boost::enable_shared_from_this<Connection>
{
    
private:

    using callback_t = std::function<void()>;
    boost::asio::streambuf b;
    ip::tcp::socket socket_;
    std::string read_buffer_;
    std::string write_buffer_;
    std::string last_accepted_str_;

public:
    Connection(io_context& context)
    : socket_(context)
    {
        std::cout << "Connection: has been created" << std::endl;
    };
    void start();
    void start(callback_t handler);
    void write(const std::string& message);
    void close();
    void handle_read(error_code error, size_t bytes_transferred);
    void handle_write(error_code& error, size_t bytes_transferred);
    std::string get_last_accepted_str();
    ip::tcp::socket& get_socket();
};

