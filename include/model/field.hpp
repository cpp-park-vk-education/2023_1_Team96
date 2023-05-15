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

    bool IsValidPosition(Vector2i pos) {
        if (pos.x < 0 || pos.x >= w || pos.y < 0 || pos.y >= h) return false;
        return true;
    }

    uint index(Vector2i pos) { return pos.y * w + pos.x; }

   public:
    Field(uint _h, uint _w, unique_ptr<SFMLFieldModel>&& f_model)
        : h(_h), w(_w), objects(_h * _w), model(std::move(f_model)) {}

    bool CreateUnit(UnitType type, bool isMine, unique_ptr<IObjectModel> model);

    GameObject* GetObject(Vector2i pos) { return objects[index(pos)].get(); };

    bool MoveObject(Vector2i from, Vector2i to);
    void DeleteObject(Vector2u pos);

    void SetCurrent(Vector2i pos);
    Vector2i GetCurrent() { return current; };
    void ResetCurrent();

    void Draw();
};
