#include "input_handler.hpp"
#include <SFML/Window.hpp>

class SFMLWindowHandler : public InputHandler
{
private:
    sf::Window &window;

public:
    SFMLWindowHandler(sf::Window &window) : window(window) {}

    void Handle() override
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
    
    ~SFMLWindowHandler() {}
};
