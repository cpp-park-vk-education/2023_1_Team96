#pragma once

#include <unordered_map>

enum CommandType 
{
};

enum EventType
{
};

class Event
{
public:

    EventType type;

    union
    {
        /*event data*/
    };
};

class ICommand
{
public:
    virtual void execute(Event) = 0;
    virtual void undo() = 0;
    ~ICommand() {}
};

class InputHandler
{
public:
    void AddBinding(EventType type, ICommand* cmd)
    {
        bindings[type] = cmd;
    };

    virtual void handle() = 0;

    void deleteBIndings(EventType type);

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
    ~MoveCommand() {}
};

class AttactCommand : public ICommand
{
private:
    /* data */
public:
    AttactCommand(/* args */) {}
    ~AttactCommand() {}
};

class ChooseCommand : public ICommand
{
private:
    /* data */
public:
    ChooseCommand(/* args */) {}
    ~ChooseCommand() {}
};

class CreateCommand : public ICommand
{
private:
    /* data */
public:
    CreateCommand(/* args */) {}
    ~CreateCommand() {}
};

class StartCommand : public ICommand
{
private:
    /* data */
public:
    StartCommand (/* args */) {}
    ~StartCommand () {}
};

class StepCommand : public ICommand
{
private:
    /* data */
public:
    StepCommand(/* args */) {}
    ~StepCommand() {}
};
