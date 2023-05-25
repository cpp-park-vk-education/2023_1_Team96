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

class Client
{

private:
    bool is_connected;
    ip::tcp::endpoint ep_;
    io_context context_;
    boost::shared_ptr<Connection> connection_;
    std::string last_comand;    
    
public:

    Client(const std::string& address, const std::string& port)
    :   ep_(ip::tcp::endpoint(ip::address::from_string(address), std::stoi(port))),
        context_(),
        connection_(boost::make_shared<Connection>(context_)),
        is_connected(false)
    {
        std::cout << "Client has been created" << std::endl;
        
    };

    void connect();
    void handle_connect(const error_code& error);
    void start();
    void write(const std::string& message);
    void close();
    void read();
    void handle_read(const std::string& data, const error_code& error);
    io_context& get_context();
    bool get_connected()
    {
        return is_connected;
    }
};
