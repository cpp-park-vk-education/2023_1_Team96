#include "model/gameobject.hpp"

sf::Vector2u GameObject::Pos() const { return pos_; }

void GameObject::AddAction(ActionType name,
                                  std::unique_ptr<IAction> action) {
    actions_.insert(std::make_pair(name, std::move(action)));
}

void GameObject::DoAction(ActionType action, std::any params) {
    actions_[action]->DoAction(params);
}

bool GameObject::CanDoAction(ActionType action, std::any params) {
    bool has_action = actions_.find(action) != actions_.end();

    if (has_action)
        return actions_[action]->CanDoAction(params);
    else
        return false;
}
