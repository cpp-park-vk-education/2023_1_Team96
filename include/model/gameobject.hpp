#pragma once

#include <any>
#include <map>
#include <memory>
#include <set>

#include "actions.hpp"
#include "graphics/graphics.hpp"
#include "player.hpp"

// struct Cell {
//     int q_cord;
//     int r_cord;
// };

enum ActionType { ATTACK, GET_ATTACKED, MOVE };

class IAction; 

class GameObject
{
private:
    // std::set<Cell> cells_;
    // std::shared_ptr<Player> player_;
    std::map<ActionType, std::unique_ptr<IAction>> actions_;
    std::unique_ptr<IModel> model;

public:
    // GameObject(std::set<Cell> cells, std::shared_ptr<Player> player)
    //     : cells_(cells), player_(player), actions_() {}

    GameObject(std::unique_ptr<IModel>&& _model) : actions_(), model(std::move(_model)) {}

    inline const std::shared_ptr<Player> &GetPlayer() const;

    inline void AddAction(ActionType name, std::unique_ptr<IAction> action)
    {
        actions_[name] = std::move(action);
    };

    IModel& getModel()
    {
        return *model;
    }

    void draw()
    {
        model->Draw();
    }

    void DoAction(ActionType action, std::any params);
    bool CanDoAction(ActionType action, std::any params) const;
};


class IAction {
   protected:
    GameObject& owner_;

   public:
    IAction(GameObject& owner) : owner_(owner) {}

    virtual void DoAction(std::any params) = 0;
    virtual bool CanDoAction(std::any params) const = 0;

    virtual ~IAction() {}
};

// class AttackAcion : public IAction {
//    private:
//     int attack_range_;
//     int attack_power_;

//    public:
//     AttackAcion(std::shared_ptr<GameObject> owner, int range, int power)
//         : IAction(owner), attack_range_(range), attack_power_(power) {}

//     void DoAction(std::any params) override {}
//     bool CanDoAction(std::any params) const override { return false; }
// };

// class GetAttackedAcion : public IAction {
//    private:
//     int hp_;
//     int armor_;

//    public:
//     GetAttackedAcion(std::shared_ptr<GameObject> owner, int hp, int armor)
//         : IAction(owner), hp_(hp), armor_(armor) {}

//     void DoAction(std::any params) override {}
//     bool CanDoAction(std::any params) const override { return false; }
// };

class MoveAction : public IAction {
   private:
    int move_range_;
    sf::Vector2u pos;

   public:
    MoveAction(GameObject& owner, sf::Vector2u _pos, int move_range)
        : IAction(owner), pos(_pos), move_range_(move_range) 
        {
            owner_.getModel().Move(pos);
        }

    void DoAction(std::any params) override 
    {
        owner_.getModel().Move(pos);
    }
    bool CanDoAction(std::any params) const override { return false; }
};
