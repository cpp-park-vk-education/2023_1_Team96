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
    vector<shared_ptr<GameObject>> objects_;
    unsigned int h_, w_;
    unique_ptr<SFMLFieldModel> model_;

    uint index(Vector2u pos) { return pos.y * w_ + pos.x; }

   public:
    Field(uint h, uint w, unique_ptr<SFMLFieldModel>&& f_model)
        : h_(h), w_(w), objects_(h * w), model_(std::move(f_model)) {}

    bool CreateUnit(UnitType type, bool is_mine, unique_ptr<IObjectModel> model,
                    sf::Vector2u pos);

    bool IsValidPosition(Vector2u pos);
    bool IsMyPart(Vector2u pos) { return pos.x < w_ / 2; }
    bool IsEmpty(Vector2u pos) { return objects_[index(pos)] == nullptr; }

    void Choose(const sf::Vector2u& pos) { model_->SetCurrent(pos); }
    void Reset() { model_->ResetCurrent(); }

    shared_ptr<GameObject> GetObject(Vector2u pos) {
        return objects_[index(pos)];
    }

    bool MoveObject(Vector2u from, Vector2u to);
    void DeleteObject(Vector2u pos);

    void Draw();

    uint Width() const { return w_; }
    const vector<shared_ptr<GameObject>>& Objects() const { return objects_; }
};
