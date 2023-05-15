#include "model/field.hpp"

bool Field::CreateUnit(UnitType type, bool isMine,
                       unique_ptr<IObjectModel> model) {
    if (!IsValidPosition(current)) return false;
    if (isMine && current.x >= w / 2) return false;

    uint i = index(current);
    if (objects[i] != nullptr) return false;

    switch (type) {
        case B:
            objects[i] =
                std::make_shared<GameObject>(nullptr, isMine, std::move(model),
                                             Vector2u{current.x, current.y});

            UnitFactory uf(1, 1, 5, 1, 2);
            uf.AddObjectActions(objects[i]);

            std::cout << "created!" << std::endl;

            return true;
    }
    return false;
};

void Field::DeleteObject(Vector2u pos) {
    objects[pos.y * w + pos.x] = nullptr;
};

void Field::SetCurrent(Vector2i pos) {
    current.x = pos.x;
    current.y = pos.y;
    model->setCurrent(current);
    std::cout << pos.x << std::endl;
};

void Field::ResetCurrent() {
    sf::Vector2i res = current;
    current.x = -1;
    current.y = -1;
    model->resetCurrent();
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
