#include "server/connection.hpp"


using namespace boost::asio;
using namespace boost::placeholders;
using namespace boost::system;


void Connection::close()
{
    error_code ec;
    socket_.close(ec);
 
    if (ec)
        std::cout << "Error with tryng to close connection" << std::endl;
}
 
void Connection::start()
{
 
    socket_.async_read_some(buffer(read_buffer_), [this](error_code error, size_t bytes_transferred)
    {
        handle_read(error, bytes_transferred);
    }); 
    if (read_buffer_.size() != 0)
        std::cout <<"Data has been read" << read_buffer_ << std::endl;
}
 
void Connection::handle_read(error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        std::string recived_data(read_buffer_, bytes_transferred);
        last_accepted_str_ = recived_data;
        //Обработка данных
        //std::cout << "Data has been read" << bytes_transferred << std::endl;
        start();
    }
    else
    {
        std::cout << "Connection: smth wrong with connection" << std::endl;
        close();
    }
}
 
 
 
void Connection::write(const std::string& message)
{
    std::string send_msg(message);
    std::cout << "Sent msg: " << send_msg << std::endl;
    async_write(socket_, buffer(send_msg), [this](error_code error, size_t bytes_transferred)
    {   
 
        handle_write(error, bytes_transferred);
    });
 
}
 
void Connection::handle_write(error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        std::cout << "Data has been sent successfully " << bytes_transferred << std::endl;
    }
    else{
        std::cout << "Connection: Error with write in socket" << std::endl;
    }
}
 
std::string Connection::get_last_accepted_str()
{
    return last_accepted_str_;
}
 
ip::tcp::socket& Connection::get_socket()
{
    return socket_;
}