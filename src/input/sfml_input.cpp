#include "input/sfml_input.hpp"

#include <iostream>

GameEvent SFMLWindowHandler::Handle() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    GameEvent ge{EventType::CHOSE};
                    ge.cords = sf::Vector2u{event.mouseButton.x / 63,
                                            event.mouseButton.y / 63};
                    return ge;
                }
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::BackSpace) {
                    GameEvent ge{EventType::UNCHOSE};
                    return ge;
                }
                if (event.key.code == sf::Keyboard::Num1) {
                    GameEvent ge{EventType::CREATE_OBJECT};
                    ge.unit_type = B;
                    return ge;
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    GameEvent ge{EventType::FINISH};
                    return ge;
                }
                if (event.key.code == sf::Keyboard::Space) {
                    GameEvent ge{EventType::FINISH};
                    ge.cmds = "c b 2 3 c b 2 4 c b 2 5 e";
                    return ge;
                }
                if (event.key.code == sf::Keyboard::LShift) {
                    GameEvent ge{EventType::FINISH};
                    ge.cmds = "m 2 4 3 4 e";
                    return ge;
                } 
                break;
        }
    }
};