#include "model/gameobject.hpp"

void GameObject::AddAction(ActionType name, std::unique_ptr<IAction> action) {
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

GameObject::GameObject()
    : player_(), model_(nullptr), pos_(-1, -1), actions_() {}
GameObject::GameObject(std::shared_ptr<Player> player, bool _isMine,
                       std::unique_ptr<IObjectModel> model, sf::Vector2u pos)
    : player_(player), isMine(_isMine) , model_(std::move(model)), pos_(pos), actions_() {
    model_->Move(pos_);
}

