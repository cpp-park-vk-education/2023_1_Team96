#include "model/field.hpp"

bool Field::CreateUnit(UnitType type, bool isMine,
                       unique_ptr<IObjectModel> model, sf::Vector2i pos) {
    uint i = index(pos);

    switch (type) {
        case B:
            objects[i] = std::make_shared<GameObject>(
                nullptr, isMine, std::move(model), Vector2u{pos.x, pos.y});

            UnitFactory uf(1, 1, 5, 1, 2);
            uf.AddObjectActions(objects[i]);

            return true;
    }
    return false;
};

void Field::DeleteObject(Vector2u pos) {
    objects[pos.y * w + pos.x] = nullptr;
};

void Field::Draw() {
    model->draw();
    for (std::shared_ptr<GameObject> object : objects) {
        if (object) object->getModel().draw();
    }
};

bool Field::MoveObject(Vector2i a, Vector2i b) {
    if (!IsValidPosition(a) || !IsValidPosition(b)) return false;

    uint ai = index(a);
    uint bi = index(b);

    if (objects[ai] == nullptr) return false;

    if (!(objects[ai]->is_mine())) return false;

    if (objects[bi] != nullptr) return false;

    std::swap(objects[ai], objects[bi]);
    return true;
};
