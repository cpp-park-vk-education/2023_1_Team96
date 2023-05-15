#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <sstream>

#include "input/input_handler.hpp"

class NetworkHandler : public InputHandler {
   private:
    sf::TcpSocket socket;
    sf::Packet packet;
    std::stringstream received;

   public:
    NetworkHandler(const std::string& address, int port);

    void send(std::string message);

    void Handle() override;

    ~NetworkHandler(){};
};
