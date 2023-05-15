#include "server/client.hpp"

void Client::connect()
{
    connection_.get_socket().async_connect(ep_, [this](const error_code& error) {
        if (!error) 
        {
            start();
        }
        else
        {
            std::cout << "Error with client connection" << std::endl;

        }
    });  
}

void Client::start()
{

}

void Client::write(const std::string& message)
{
    connection_.write(message);
}

void Client::close()
{
    connection_.close();
}
