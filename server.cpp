#include <SFML/Network.hpp>
#include <iostream>
#include <string>

int main() {
    sf::TcpSocket first;
    sf::TcpSocket second;
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();

    sf::TcpListener listener;
    listener.listen(6000);

    if (listener.accept(first) != sf::Socket::Done) {
        std::cout << "error" << std::endl;
        return 1;
    }

    std::cout << "first!" << std::endl;
    sf::Packet first_packet;

    if (listener.accept(second) != sf::Socket::Done) {
        std::cout << "error" << std::endl;
        return 1;
    }

    sf::Packet second_packet;
    std::cout << "second" << std::endl;

    first.setBlocking(false);
    second.setBlocking(false);

    bool f = false;
    bool s = false;

    sf::Packet for_first;
    sf::Packet for_second;

    while (true) {
        while (!f || !s) {
            if (first.receive(first_packet) == sf::Socket::Done) {
                std::string m;

                first_packet >> m;

                std::cout << " 1: " << m << std::endl;

                for_second.clear();
                for_second << m;
                f = true;
            }

            if (second.receive(second_packet) == sf::Socket::Done) {
                std::string m;

                second_packet >> m;

                std::cout << " 2: " << m << std::endl;

                for_first.clear();
                for_first << m;
                s = true;
            }
        }

        first.send(for_first);
        second.send(for_second);

        s = f = false;
    }

    return 0;
}