#pragma once

#include <any>
#include <memory>

#include "gameobject.hpp"

class GameObject;

enum ActionType { ATTACK, GET_ATTACKED, MOVE };

class IAction {
   private:
    std::shared_ptr<GameObject> owner_;

   public:
    IAction(std::shared_ptr<GameObject> owner) : owner_(owner) {}

    virtual void DoAction(std::any params) = 0;
    virtual bool CanDoAction(std::any params) const = 0;

    virtual ~IAction() {}
};

class AttackAcion : public IAction {
   private:
    int range_;
    int power_;

   public:
    AttackAcion(std::shared_ptr<GameObject> owner, int range, int power)
        : IAction(owner), range_(range), power_(power) {}

    void DoAction(std::any params) override {}
    bool CanDoAction(std::any params) const override { return false; }
};

class GetAttackedAcion : public IAction {
   private:
    int hp_;
    int armor_;

   public:
    GetAttackedAcion(std::shared_ptr<GameObject> owner, int hp, int armor)
        : IAction(owner), hp_(hp), armor_(armor) {}

    void DoAction(std::any params) override {}
    bool CanDoAction(std::any params) const override { return false; }
};

class MoveAction : public IAction {
   private:
    int move_range_;

   public:
    MoveAction(std::shared_ptr<GameObject> owner, int move_range)
        : IAction(owner), move_range_(move_range) {}

    void DoAction(std::any params) override {}
    bool CanDoAction(std::any params) const override { return false; }
};
