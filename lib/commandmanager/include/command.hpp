#pragma once

#include <any>

enum CommandType { START_GAME, END_GAME, ADD_OBJ, MOVE, ATTACK, SWITCH_TURN };

struct Command {
    CommandType type;
    std::any params;

    Command(CommandType type, std::any params) : type(type), params(params) {}
};

struct CommandPolicies {
    static int getEvent(const Command& e) { return e.type; }
};
