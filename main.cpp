#include "model/gamemodel.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"

int main()
{
    std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{1000,600});

    std::unique_ptr<SFMLWindowHandler> handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());

    Game game(std::move(monitor), std::move(handler));

    game.StartGame();

    return 0;
}