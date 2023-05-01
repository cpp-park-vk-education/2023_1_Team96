#pragma once

#include <functional>

#include "command.hpp"
#include "eventdispatcher.h"

struct ICommandManager {
    virtual void AddCommand(
        CommandType cmd, std::function<void(const Command& cmd)> callback) = 0;

    virtual void ProcessCommand(const Command& cmd) = 0;

    virtual ~ICommandManager() {}
};

class CommandManager : public ICommandManager {
   public:
    void AddCommand(CommandType cmd,
                    std::function<void(const Command& cmd)> callback) override {
        dispatcher.appendListener(cmd, callback);
    }

    void ProcessCommand(const Command& cmd) override {
        dispatcher.dispatch(cmd);
    }

   private:
    eventpp::EventDispatcher<int, void(const Command& cmd), CommandPolicies>
        dispatcher;
};
