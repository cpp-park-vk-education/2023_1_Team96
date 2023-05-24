#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "utility/common.h"

enum EventType { CHOSE, UNCHOSE, CREATE_OBJECT, MOVE_CMD, ATTACK_CMD, FINISH };

struct GameEvent
{
    EventType type;

    union
    {
        sf::Vector2u cords;
        UnitType unit_type;
        char* cmds;
    };
};

struct InputHandler {
    virtual GameEvent Handle() = 0;
};