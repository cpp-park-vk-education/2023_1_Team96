#include "model/gamemodel.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"
#include "input/network_input.hpp"

int main()
{
    std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{1000,600});

    std::unique_ptr<SFMLWindowHandler> w_handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());

    std::unique_ptr<NetworkHandler> n_handler = std::make_unique<NetworkHandler>("localhost", 6000);

    Game game(std::move(monitor), std::move(w_handler), std::move(n_handler));

    game.StartGame();

    return 0;
}