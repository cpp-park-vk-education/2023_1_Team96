#include "input/qt_input.hpp"

void QTWindowHandler::Handle() {
    sf::Event event;
    //QEvent qevent;
    while (window.pollEvent(event)) {
        
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    GameEvent ge{CELL};
                    ge.cords = sf::Vector2i{event.mouseButton.x / 63,
                                            event.mouseButton.y / 63};
                    bindings[CELL]->Execute(ge);
                }
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::BackSpace) {
                    GameEvent ge{CANCEL};
                    bindings[CANCEL]->Execute(ge);
                }
                if (event.key.code == sf::Keyboard::Num1) {
                    GameEvent ge{UNIT_NUM};
                    ge.unit_type = B;
                    if (bindings[UNIT_NUM]) bindings[UNIT_NUM]->Execute(ge);
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    GameEvent ge{SEND};
                    if (bindings[SEND]) bindings[SEND]->Execute(ge);
                }
                break;
        }
    }
};