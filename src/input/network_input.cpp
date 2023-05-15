#include "input/network_input.hpp"

NetworkHandler::NetworkHandler(const std::string& address, int port) {
    sf::IpAddress ip(address);
    if (socket.connect(ip, port) != sf::Socket::Done) {
        std::cout << "err" << std::endl;
        return;
    }

    socket.setBlocking(false);
};

void NetworkHandler::send(std::string message) {
    std::cout << message << std::endl;
    packet.clear();
    packet << message;
    socket.send(packet);
}

void NetworkHandler::Handle() {
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