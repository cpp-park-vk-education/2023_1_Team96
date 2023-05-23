#include "model/field.hpp"

bool Field::CreateUnit(UnitType type, bool isMine,
                       unique_ptr<IObjectModel> model, sf::Vector2u pos) {
    uint i = index(pos);

    switch (type) {
        case B:
            objects_[i] = std::make_shared<GameObject>(
                nullptr, isMine, std::move(model), Vector2u{pos.x, pos.y});

            UnitFactory uf(1, 1, 5, 1, 2);
            uf.AddObjectActions(objects_[i]);

            return true;
    }
    return false;
}

bool Field::IsValidPosition(Vector2u pos) {
    if (pos.x < 0 || pos.x >= w_ || pos.y < 0 || pos.y >= h_) return false;
    return true;
}

bool Field::MoveObject(Vector2u from, Vector2u to) {
    uint from_idx = index(from);
    uint to_idx = index(to);

    if (IsEmpty(from)) return false;
    if (!(objects_[from_idx]->IsMine())) return false;
    if (!IsEmpty(to)) return false;

    std::swap(objects_[from_idx], objects_[to_idx]);
    return true;
};

void Field::DeleteObject(Vector2u pos) {
    objects_[pos.y * w_ + pos.x] = nullptr;
};

void Field::Draw() {
    model_->draw();
    for (std::shared_ptr<GameObject> object : objects_) {
        if (object) object->GetModel().draw();
    }
};
