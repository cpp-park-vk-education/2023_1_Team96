#pragma once

#include <memory>

#include "gameobject.hpp"

struct IObjectActionsFactory {
    virtual void AddObjectActions(std::shared_ptr<GameObject> obj) = 0;

    virtual ~IObjectActionsFactory() {}
};

class UnitFactory : public IObjectActionsFactory {
   private:
    int attack_power_, attack_range_;
    int hp_, armor_;
    int move_range_;

   public:
    UnitFactory(int attack_power, int attack_range, int hp, int armor,
                int move_range);

    void AddObjectActions(std::shared_ptr<GameObject> obj) override;
};
