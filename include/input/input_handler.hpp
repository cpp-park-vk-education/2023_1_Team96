    #pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>

enum EventType
{
    CELL,
    CANCEL,
    UNIT_NUM,
};

struct GameEvent
{
    EventType type;

    union
    {
        sf::Vector2u cords;
        UnitType unit_type;
    };
};

class ICommand
{
public:
    virtual void Execute(GameEvent event) = 0;
    virtual void Undo() = 0;

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


class MoveCommand : public ICommand
{
private:
    /* data */
public:
    MoveCommand(/* args */) {}

    void Execute(GameEvent event) override {}
    void Undo() override {}

    ~MoveCommand() {}
};

class AttactCommand : public ICommand
{
private:
    /* data */
public:
    AttactCommand(/* args */) {}

    void Execute(GameEvent event) override {}
    void Undo() override {}

    ~AttactCommand() {}
};

class ChooseCommand : public ICommand
{
private:
    /* data */
public:
    ChooseCommand(/* args */) {}

    void Execute(GameEvent event) override {}
    void Undo() override {}

    ~ChooseCommand() {}
};

class CreateCommand : public ICommand
{
private:
    /* data */
public:
    CreateCommand(/* args */) {}

    void Execute(GameEvent event) override {}
    void Undo() override {}

    ~CreateCommand() {}
};

class StartCommand : public ICommand
{
private:
    /* data */
public:
    StartCommand (/* args */) {}

    void Execute(GameEvent event) override {}
    void Undo() override {}

    ~StartCommand () {}
};

class StepCommand : public ICommand
{
private:
    /* data */
public:
    StepCommand(/* args */) {}

    void Execute(GameEvent event) override {}
    void Undo() override {}

    ~StepCommand() {}
};
