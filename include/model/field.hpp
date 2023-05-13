#pragma once

// #include "objectrepository.hpp"

#include <memory>
#include <vector>

#include "utility/common.h"
#include "model/gameobject.hpp"

using sf::Vector2i;
using sf::Vector2u;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using uint = unsigned int;

class Field
{
private:
    vector<shared_ptr<GameObject>> objects;
    unsigned int h, w;
    unique_ptr<SFMLFieldModel> model;
    Vector2i current = {-1, -1};

public:
    Field(uint _h, uint _w, unique_ptr<SFMLFieldModel> &&f_model)
        : h(_h), w(_w), objects(_h * _w), model(std::move(f_model))
    {}

    bool createUnit(UnitType type, unique_ptr<IObjectModel> model)
    {

        if (current.x < 0) return false;

        Vector2u pos = {current.x, current.y};

        int index = pos.y * w + pos.x;

        if (index >= objects.size())
            return false;

        if (pos.x >= 7)
            return false;

        if (objects[index] != nullptr)
            return false;

        switch (type)
        {
        case B:

            objects[index] = std::make_shared<GameObject>(std::move(model), Vector2u{pos.x, pos.y});
            objects[index]->AddAction(
                ActionType::MOVE,
                std::move(std::make_unique<MoveAction>(
                    *(objects[index]), 1)));

            std::cout << "created!" << std::endl;

            return true;
        }
        return false;
    };

    void deleteUnit(Vector2u pos)
    {
        objects[pos.y * w + pos.x] = nullptr;
    };

    void Add(unique_ptr<GameObject> obj){

    };

    void setCurrent(Vector2i pos)
    {
        current.x = pos.x;
        current.y = pos.y;
        model->setCurrent(current);
        std::cout << pos.x << std::endl;
    };

    void resetCurrent()
    {
        sf::Vector2i res = current;
        current.x = -1;
        current.y = -1;
        model->resetCurrent();
    };

    sf::Vector2i getCurrent() { return current; };

    void Delete(std::shared_ptr<GameObject> obj){};
    void DeleteByCords(sf::Vector2u cords){};

    void draw()
    {
        model->draw();
        for (std::shared_ptr<GameObject> object : objects)
        {
            if (object)
                object->getModel().draw();
        }
    };
};
