#pragma once

#include <any>
#include <map>
#include <memory>
#include <set>

#include "actions.hpp"
#include "player.hpp"

struct Cell {
    int q_cord;
    int r_cord;
};

class GameObject {
   private:
    std::set<Cell> cells_;
    std::shared_ptr<Player> player_;
    std::map<ActionType, std::unique_ptr<IAction>> actions_;

   public:
    GameObject(std::set<Cell> cells, std::shared_ptr<Player> player)
        : cells_(cells), player_(player), actions_() {}

    inline const std::shared_ptr<Player>& GetPlayer() const;

    inline void AddAction(ActionType name, std::unique_ptr<IAction> action);

    void DoAction(ActionType action, std::any params);
    bool CanDoAction(ActionType action, std::any params) const;
};
