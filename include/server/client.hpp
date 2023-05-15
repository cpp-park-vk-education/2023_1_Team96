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

    ip::tcp::endpoint ep_;
    io_service service_;
    Connection connection_;
    
public:

    Client(const std::string& address, const std::string& port);
    void connect();
    void start();
    void write(const std::string& message);
    void close();

};
