#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "utility/common.h"

enum EventType
{
    CELL,
    CANCEL,
    UNIT_NUM,
    SEND,
    END,
};

struct GameEvent
{
    EventType type;

    union
    {
        sf::Vector2i cords;
        UnitType unit_type;
    };
};

class ICommand
{
public:
    virtual void Execute(GameEvent event) = 0;
    virtual void Undo() {};
    virtual std::string string() { return "";};

    ~ICommand() {}
};

class InputHandler
{
public:
    void AddBinding(EventType type, ICommand* cmd)
    {
        bindings[type] = cmd;
    };

    void ChangeBinding(EventType type, ICommand* cmd)
    {
        bindings[type] = cmd;
    };

    virtual void Handle() = 0;

    void DeleteBindings(EventType type) 
    {
        bindings[type] = nullptr;
    }

    void Execute(GameEvent event) 
    {
        bindings[event.type]->Execute(event);
    }

    virtual ~InputHandler() {}

protected:
    std::unordered_map<EventType, ICommand*> bindings;
};