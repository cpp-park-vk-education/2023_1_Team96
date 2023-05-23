#pragma once

#include <any>
#include <cmath>
#include <map>
#include <memory>

#include "graphics/sfml_monitor.hpp"
#include "model/player.hpp"

enum ActionType { ATTACK, GET_ATTACKED, MOVE };

class IAction;

class GameObject {
   private:
    std::map<ActionType, std::unique_ptr<IAction>> actions_;
    sf::Vector2u pos_;
    std::shared_ptr<Player> player_;
    bool is_mine_;
    std::unique_ptr<IObjectModel> model_;

   public:
    GameObject();
    GameObject(std::shared_ptr<Player> player, bool isMine,
               std::unique_ptr<IObjectModel> model, sf::Vector2u pos);

    bool IsMine() { return is_mine_; }

    IObjectModel &GetModel() { return *model_; }

    inline const std::shared_ptr<Player> &GetPlayer() const { return player_; };

    inline sf::Vector2u Pos() const { return pos_; };

    inline void SetPos(sf::Vector2u pos) { pos_ = pos; }

    void AddAction(ActionType name, std::unique_ptr<IAction> action);

    void DoAction(ActionType action, std::any params);
    bool CanDoAction(ActionType action, std::any params);
};

class IAction {
   protected:
    GameObject &owner_;

   public:
    IAction(GameObject &owner) : owner_(owner) {}

    virtual void DoAction(std::any params) = 0;
    virtual bool CanDoAction(std::any params) const = 0;

    virtual ~IAction() {}
};

struct Attack {
    int damage;
    int range;
    sf::Vector2u attack_pos;
};

class AttackAction : public IAction {
   private:
    int attack_range_;
    int attack_power_;

   public:
    AttackAction(GameObject &owner, int range, int power)
        : IAction(owner), attack_range_(range), attack_power_(power) {}

    void DoAction(std::any params) override {
        Attack *attack = std::any_cast<Attack *>(params);
        attack->damage = attack_power_;
        attack->range = attack_range_;
        attack->attack_pos = owner_.Pos();
    }

    bool CanDoAction(std::any params) const override {
        if (params.type() != typeid(Attack *)) return false;

        return (attack_power_ > 0) && (attack_range_ > 0);
    }
};

class GetAttackedAction : public IAction {
   private:
    int hp_;
    int armor_;

   public:
    GetAttackedAction(GameObject &owner, int hp, int armor)
        : IAction(owner), hp_(hp), armor_(armor) {}

    void DoAction(std::any params) override {
        Attack *attack = std::any_cast<Attack *>(params);
        hp_ -= (attack->damage - 0.3 * armor_);
    }

    bool CanDoAction(std::any params) const override {
        if (params.type() != typeid(Attack *)) return false;

        Attack *attack = std::any_cast<Attack *>(params);
        sf::Vector2u attack_vector = owner_.Pos() - attack->attack_pos;
        int attack_abs = sqrt(attack_vector.x * attack_vector.x +
                              attack_vector.y * attack_vector.y);

        return attack_abs <= attack->range;
    }
};

class MoveAction : public IAction {
   private:
    int move_range_;

   public:
    MoveAction(GameObject &owner, int move_range)
        : IAction(owner), move_range_(move_range) {}

    void DoAction(std::any params) override {
        sf::Vector2u new_pos = std::any_cast<sf::Vector2u>(params);
        owner_.SetPos(new_pos);
        owner_.GetModel().Move(new_pos);
    }

    bool CanDoAction(std::any params) const override {
        if (params.type() != typeid(sf::Vector2u)) return false;

        sf::Vector2u new_pos = std::any_cast<sf::Vector2u>(params);
        sf::Vector2u move_vector = new_pos - owner_.Pos();
        int move_abs =
            sqrt(move_vector.x * move_vector.x + move_vector.y * move_vector.y);

        return move_abs <= move_range_;
    }
};
