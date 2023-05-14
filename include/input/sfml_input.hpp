#pragma once

#include <SFML/Window.hpp>

#include "input/input_handler.hpp"

class SFMLWindowHandler : public InputHandler {
   private:
    sf::Window &window;

   public:
    SFMLWindowHandler(sf::Window &window) : window(window) {}

    void Handle();

    ~SFMLWindowHandler() {}
};
