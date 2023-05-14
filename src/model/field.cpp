#include "model/field.hpp"

bool Field::createUnit(UnitType type, unique_ptr<IObjectModel> model) {

    Vector2i pos = {current.x, current.y};

    if (!isValid(pos)) return false;

    uint i = index(pos);

    if (pos.x >= 7) return false;

    if (objects[i] != nullptr) return false;

    switch (type) {
        case B:
            objects[i] = std::make_shared<GameObject>(
                nullptr, std::move(model), Vector2u{pos.x, pos.y});
            objects[i]->AddAction(
                ActionType::MOVE,
                std::move(std::make_unique<MoveAction>(*(objects[i]), 1)));

            std::cout << "created!" << std::endl;

            return true;
    }
    return false;
};

void Field::deleteUnit(Vector2u pos) { objects[pos.y * w + pos.x] = nullptr; };

void Field::setCurrent(Vector2i pos) {
    current.x = pos.x;
    current.y = pos.y;
    model->setCurrent(current);
    std::cout << pos.x << std::endl;
};

void Field::resetCurrent() {
    sf::Vector2i res = current;
    current.x = -1;
    current.y = -1;
    model->resetCurrent();
};

void Field::draw() {
    model->draw();
    for (std::shared_ptr<GameObject> object : objects) {
        if (object) object->getModel().draw();
    }
};

bool Field::change(Vector2i a, Vector2i b)
{
    if (!isValid(a) || !isValid(b)) return false;

    uint ai = index(a);
    uint bi = index(b);

    if (objects[ai] == nullptr) return false;

    if (objects[bi] != nullptr) return false;

    std::swap(objects[ai], objects[bi]); 
    return true;
};
