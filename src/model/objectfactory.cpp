#include "model/objectfactory.hpp"

void UnitFactory::AddObjectActions(std::shared_ptr<GameObject> obj) {
    std::unique_ptr<IAction> move_action =
        std::make_unique<MoveAction>(*obj, move_range_);
    obj->AddAction(ActionType::MOVE, move_action);

    std::unique_ptr<IAction> attack_action =
        std::make_unique<AttackAcion>(*obj, attack_range_, attack_power_);
    obj->AddAction(ActionType::ATTACK, attack_action);

    std::unique_ptr<IAction> get_attacked_action =
        std::make_unique<GetAttackedAcion>(*obj, hp_, armor_);
    obj->AddAction(ActionType::GET_ATTACKED, get_attacked_action);
}
