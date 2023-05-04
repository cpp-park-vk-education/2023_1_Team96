#include "gameobject.hpp"

inline const std::shared_ptr<Player>& GameObject::GetPlayer() const {
    return player_;
}

inline void GameObject::AddAction(ActionType name,
                                  std::unique_ptr<IAction> action) {
    actions_.insert(std::make_pair(name, std::move(action)));
}

void GameObject::DoAction(ActionType action, std::any params) {}

bool GameObject::CanDoAction(ActionType action, std::any params) const {
    return false;
}
