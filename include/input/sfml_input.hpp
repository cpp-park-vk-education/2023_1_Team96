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

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    GameEvent ge{CELL};
                    ge.cords = sf::Vector2u{event.mouseButton.x, event.mouseButton.y};
                    bindings[CELL]->Execute(ge);
                }
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::BackSpace)
                {
                    GameEvent ge{CANCEL};
                    bindings[CANCEL]->Execute(ge);
                }

                if (event.key.code == sf::Keyboard::Num1)
                {
                    GameEvent ge{UNIT_NUM};
                    ge.unit_type = B;
                    bindings[UNIT_NUM]->Execute(ge);
                }
                break;
            default:

                break;
            }
        }
    };

    ~SFMLWindowHandler() {}
};
