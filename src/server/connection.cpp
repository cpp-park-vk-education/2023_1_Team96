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

void Connection::start(callback_t handler)
{
//std::cout << "Connection has started reading" << std::endl;
    auto self = shared_from_this();
    //std::cout << "last " << last_accepted_str_ << std::endl;
    async_read_until(socket_, b, "\r\n",
    [handler, self](error_code error, size_t bytes_transferred)
    {
        self->handle_read(error, bytes_transferred);
        handler();
    });
}

void Connection::start() {
    this->start([](){});
}

void Connection::handle_read(error_code error, size_t bytes_transferred)
{
    if (!error)
    {
    std::istream is(&b);
    std::getline(is, read_buffer_);
    b.consume(bytes_transferred);
    std::cout << "Вызван" << std::endl;

    if (bytes_transferred != 0)
        std::cout <<"Data has been read\n" << read_buffer_ << std::endl;
    
    
        std::string recived_data(read_buffer_);
        last_accepted_str_ = recived_data;
        
    //  write("Accepted");
        // start();
        //Обработка данных
        //std::cout << "Data has been read" << bytes_transferred << std::endl;
        //start();
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
    send_msg += "\r\n";
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