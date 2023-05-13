#include "model/objectfactory.hpp"

void UnitFactory::AddObjectActions(std::shared_ptr<GameObject> obj) {
    std::unique_ptr<IAction> move_action =
        std::make_unique<MoveAction>(*obj, move_range_);
    obj->AddAction(ActionType::MOVE, std::move(move_action));

    std::unique_ptr<IAction> attack_action =
        std::make_unique<AttackAction>(*obj, attack_range_, attack_power_);
    obj->AddAction(ActionType::ATTACK, std::move(attack_action));

    std::unique_ptr<IAction> get_attacked_action =
        std::make_unique<GetAttackedAction>(*obj, hp_, armor_);
    obj->AddAction(ActionType::GET_ATTACKED, std::move(get_attacked_action));
}

UnitFactory::UnitFactory(int attack_power, int attack_range, int hp, int armor,
                         int move_range)
    : attack_power_(attack_power),
      attack_range_(attack_range),
      hp_(hp),
      armor_(armor),
      move_range_(move_range) {}