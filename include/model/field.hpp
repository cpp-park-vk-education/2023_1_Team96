#pragma once

#include <memory>
#include <vector>

#include "model/gameobject.hpp"
#include "objectfactory.hpp"
#include "utility/common.h"

using sf::Vector2i;
using sf::Vector2u;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using uint = unsigned int;

class Field {
   private:
    vector<shared_ptr<GameObject>> objects;
    unsigned int h, w;
    unique_ptr<SFMLFieldModel> model;
    Vector2i current = {-1, -1};

    bool isValid(Vector2i pos) {
        if (pos.x < 0 || pos.y < 0) return false;
        if (pos.y*w + pos.x >= objects.size()) return false;
        return true;
    }

    uint index(Vector2i pos)
    {
        return pos.y*w + pos.x;
    }

   public:
    Field(uint _h, uint _w, unique_ptr<SFMLFieldModel>&& f_model)
        : h(_h), w(_w), objects(_h * _w), model(std::move(f_model)) {}

    bool createUnit(UnitType type, unique_ptr<IObjectModel> model);

    GameObject* getObject(Vector2i pos)
    {
        return objects[index(pos)].get();
    };

    bool change(Vector2i a, Vector2i b);

    void deleteUnit(Vector2u pos);

    void setCurrent(Vector2i pos);

    void resetCurrent();

    Vector2i getCurrent() { return current; };

    void draw();
};
