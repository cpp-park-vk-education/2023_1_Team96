#pragma once

#include <SFML/Window.hpp>

#include "input/input_handler.hpp"

class QTWindowHandler : public InputHandler {
   private:
    sf::Window &window;

   public:
    QTWindowHandler(sf::Window &window) : window(window) {}

    void Handle();

    ~QTWindowHandler() {}
};
