#pragma once

#include <SFML/Network.hpp>
#include <sstream>

#include "input/input_handler.hpp"

class NetworkHandler : public InputHandler {
   private:
    sf::TcpSocket socket;
    sf::Packet packet;
    std::stringstream received;

   public:
    NetworkHandler(const std::string& address, int port) {
        sf::IpAddress ip(address);
        if (socket.connect(ip, port) != sf::Socket::Done) {
            std::cout << "err" << std::endl;
            return;
        }

        socket.setBlocking(false);
    };

    void send(std::string message) {
        std::cout << message << std::endl;
        packet.clear();
        packet << message;
        socket.send(packet);
    }

    void Handle() override {
        if (received.eof()) {
            if (socket.receive(packet) == sf::Socket::Done) {
                std::string msg;
                packet >> msg;

                received = std::stringstream(msg);

                std::cout << "in: " << msg << std::endl;
            }
            return;
        }

        char cmd;
        received >> cmd;

        switch (cmd) {
            case 'c': {
                char type = 0;
                received >> type;
                uint x = 0;
                uint y = 0;

                received >> x >> y;

                GameEvent click{CELL};
                click.cords = sf::Vector2i{x, y};
                bindings[CELL]->Execute(click);

                GameEvent create{UNIT_NUM};
                create.unit_type = B;
                if (bindings[UNIT_NUM]) bindings[UNIT_NUM]->Execute(create);

            } break;
            case 'e': {
                GameEvent end{END};
                if (bindings[END]) bindings[END]->Execute(end);

            } break;
        }
    };

    ~NetworkHandler(){};
};
