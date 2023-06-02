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

    sf::Vector2u my_king_pos_;
    sf::Vector2u enemy_king_pos_;

    uint index(Vector2u pos) { return pos.y * w_ + pos.x; }

   public:
    Field(uint h, uint w)
        : h_(h),
          w_(w),
          objects_(h * w),
          model_(std::move(f_model)),
          my_king_pos_({0, 0}),
          enemy_king_pos_({0, 0}) {}

    void setModel(unique_ptr<SFMLFieldModel>&& f_model)
    {
        model_ = std::move(f_model);
    }

    bool CreateUnit(UnitType type, bool is_mine, unique_ptr<IObjectModel> model,
                    sf::Vector2u pos);

    bool IsValidPosition(Vector2u pos);
    bool IsMyPart(Vector2u pos) { return pos.x < w_ / 2; }
    bool IsEmpty(Vector2u pos) { return objects_[index(pos)] == nullptr; }

    void Choose(const sf::Vector2u& pos) { model_->SetCurrent(pos); }
    void Reset() { model_->ResetCurrent(); }
    void ShowStat(Stats stat) { model_->SetStat(stat); }
    void HideStat() { model_->HideStat(); }

    shared_ptr<GameObject> GetObject(Vector2u pos) {
        return objects_[index(pos)];
    }

    bool MoveObject(Vector2u from, Vector2u to);
    void DeleteObject(Vector2u pos);

    shared_ptr<GameObject> GetMyKing() { return GetObject(my_king_pos_); }
    shared_ptr<GameObject> GetEnemyKing() { return GetObject(enemy_king_pos_); }

    void Draw();

    uint Width() const { return w_; }
    const vector<shared_ptr<GameObject>>& Objects() const { return objects_; }
};
