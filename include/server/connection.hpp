#pragma once

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <boost/thread.hpp>

using namespace boost::asio;
using namespace boost::system;


class Connection: public boost::enable_shared_from_this<Connection>
{
    
private:

    ip::tcp::socket socket_;
    std::string read_buffer_;
    std::string write_buffer_;
    std::string last_accepted_str_;

public:
    Connection(io_service service)
    : socket_(service)
    {};
    void start();
    void write(const std::string& message);
    void close();
    void handle_read(error_code& error, size_t bytes_transferred);
    void handle_write(error_code& error, size_t bytes_transferred);
    std::string get_last_accepted_str();
};

