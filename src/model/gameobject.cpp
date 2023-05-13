#include "model/gameobject.hpp"

#include "gameobject.hpp"

IModel& GameObject::GetModel() { return *model_; }

void GameObject::Draw() { model_->Draw(); }

inline const std::shared_ptr<Player>& GameObject::GetPlayer() const {
    return player_;
}

inline sf::Vector2u GameObject::Pos() const { return pos_; }

inline inline void GameObject::SetPos(sf::Vector2u pos) { pos_ = pos; }

void GameObject::AddAction(ActionType name, std::unique_ptr<IAction> action) {
    actions_.insert(std::make_pair(name, std::move(action)));
}

void GameObject::DoAction(ActionType action, std::any params) {
    actions_[action].DoAction(params);
}

bool GameObject::CanDoAction(ActionType action, std::any params) const {
    bool has_action = actions_.find(action) != actions_.end();

    if (has_action)
        return actions_[action].CanDoAction(params);
    else
        return false;
}
