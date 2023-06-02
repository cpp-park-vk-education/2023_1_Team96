#include "model/field.hpp"

const int WARRIOR_ATTACK_POWER = 1;
const int WARRIOR_ATTACK_RANGE = 1;
const int WARRIOR_HP = 5;
const int WARRIOR_ARMOR = 1;
const int WARRIOR_MOVE_RANGE = 2;

const int KING_ATTACK_POWER = 2;
const int KING_ATTACK_RANGE = 1;
const int KING_HP = 7;
const int KING_ARMOR = 1;
const int KING_MOVE_RANGE = 1;

bool Field::CreateUnit(UnitType type, bool is_mine,
                       unique_ptr<IObjectModel> model, sf::Vector2u pos) {
    uint i = index(pos);

    switch (type) {
        case UnitType::B: {
            objects_[i] = std::make_shared<GameObject>(nullptr, is_mine,
                                                       std::move(model), pos);

            UnitFactory uf(WARRIOR_ATTACK_POWER, WARRIOR_ATTACK_RANGE,
                           WARRIOR_HP, WARRIOR_ARMOR, WARRIOR_MOVE_RANGE);
            uf.AddObjectActions(objects_[i]);

            return true;
        }
        case UnitType::K: {
            shared_ptr<GameObject> king = std::make_shared<GameObject>(
                nullptr, is_mine, std::move(model), pos);

            if (IsEmpty(my_king_pos_) && is_mine) {
                my_king_pos_ = pos;
            } else if (IsEmpty(enemy_king_pos_) && !is_mine) {
                enemy_king_pos_ = pos;
            } else {
                return false;
            }

            UnitFactory uf(KING_ATTACK_POWER, KING_ATTACK_RANGE, KING_HP,
                           KING_ARMOR, KING_MOVE_RANGE);
            uf.AddObjectActions(king);

            objects_[i] = king;

            return true;
        }
        case UnitType::T: {
            objects_[i] = std::make_shared<GameObject>(nullptr, is_mine,
                                                       std::move(model), pos);

            UnitFactory uf(1, 1, 5, 1, 2);
            uf.AddObjectActions(objects_[i]);

            return true;
        }
        case UnitType::H: {
            objects_[i] = std::make_shared<GameObject>(nullptr, is_mine,
                                                       std::move(model), pos);

            UnitFactory uf(1, 1, 5, 1, 2);
            uf.AddObjectActions(objects_[i]);

            return true;
        }
    }
    return false;
}

bool Field::IsValidPosition(Vector2u pos) {
    if (pos.x < 0 || pos.x >= w_ || pos.y < 0 || pos.y >= h_) return false;
    return true;
}

bool Field::MoveObject(Vector2u from, Vector2u to) {
    uint from_idx = index(from);
    uint to_idx = index(to);

    if (IsEmpty(from)) return false;
    if (!(objects_[from_idx]->IsMine())) return false;
    if (!IsEmpty(to)) return false;

    std::swap(objects_[from_idx], objects_[to_idx]);

    if (my_king_pos_ == from) my_king_pos_ = to;
    if (enemy_king_pos_ == from) enemy_king_pos_ = to;

    return true;
};

void Field::DeleteObject(Vector2u pos) {
    // objects_[pos.y * w_ + pos.x] = nullptr;
    objects_[pos.y * w_ + pos.x]->GetModel().Blow();
};

void Field::Draw() {
    model_->Draw();
    for (std::shared_ptr<GameObject> object : objects_) {
        if (object) object->GetModel().Draw();
    }
    model_->ShowStat();
};
