#include "server/client.hpp"

void Client::connect()
{
    connection_->get_socket().async_connect(ep_, boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error));
}

void Client::handle_connect(const error_code& error)
{
    if (!error)
    {
        std::cout << "Connected to the server" << std::endl;

        start();
    }

    else{
        std::cout << "Failed to connect to the server" << std::endl;
    }
}

void Client::start()
{
    std::string msg = "Hello server!";
    write(msg);
}

void Client::write(const std::string& message)
{
    if (connection_)
        connection_->write(message);
    else
        std::cout << "No connection" << std::endl;
}

void Client::close()
{
    if (connection_)
    {
        connection_->close();
        connection_.reset();
    }
    else
        std::cout << "No active connections" << std::endl;
}

void Client::read()
{
    if (connection_)
    {
        connection_->start();
        last_comand = connection_->get_last_accepted_str();
    }
}

void Client::handle_read(const std::string& data, const error_code& error)
{
    if (!error)
    {
        std::cout << "Received data from server: " << data << std::endl;
        read(); // Продолжаем чтение данных
    }
    else
    {
        std::cout << "Error reading from server: " << error.message() << std::endl;
        close(); // Закрываем соединение при возникновении ошибки чтения
    }
}
