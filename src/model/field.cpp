#include "model/field.hpp"

bool Field::createUnit(UnitType type, unique_ptr<IObjectModel> model) {
    if (current.x < 0) return false;

    Vector2u pos = {current.x, current.y};

    int index = pos.y * w + pos.x;

    if (index >= objects.size()) return false;

    if (pos.x >= 7) return false;

    if (objects[index] != nullptr) return false;

    switch (type) {
        case B:

            objects[index] = std::make_shared<GameObject>(
                nullptr, std::move(model), Vector2u{pos.x, pos.y});
            objects[index]->AddAction(
                ActionType::MOVE,
                std::move(std::make_unique<MoveAction>(*(objects[index]), 1)));

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