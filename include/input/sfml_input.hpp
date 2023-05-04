#include "input_handler.hpp"
#include <SFML/Window.hpp>

class SfmlWindowHandler : public InputHandler
{
private:
    sf::Window &window;

public:
    SfmlWindowHandler(sf::Window &window) : window(window) {}

    void handle() override
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:

                break;
            }
        }
    };
    
    ~SfmlWindowHandler() {}
};
