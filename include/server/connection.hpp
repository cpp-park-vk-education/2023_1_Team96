#pragma once

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include "connection.hpp"

using namespace boost::asio;
using namespace boost::system;


class Connection: public boost::enable_shared_from_this<Connection>, boost::noncopyable 
{
    
private:

    ip::tcp::socket socket_;
    std::string read_buffer_;
    std::string write_buffer_;

public:
    Connection(io_service service);
    void start();
    void write(std::string message);
    void close();
    void handle_read(error_code& error, size_t bytes_transferred);
    void handle_write(error_code& error, size_t bytes_transferred);
    ip::tcp::socket get_socket();
};

