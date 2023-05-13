#pragma once

#include "objectfactory.hpp"

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

    bool createUnit(UnitType type, unique_ptr<IObjectModel> model);

    void deleteUnit(Vector2u pos);

    void setCurrent(Vector2i pos);

    void resetCurrent();

    Vector2i getCurrent() { return current; };

    void draw();
};
