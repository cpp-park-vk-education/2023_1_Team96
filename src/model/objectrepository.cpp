#include "model/objectrepository.hpp"

#include "objectrepository.hpp"

bool ObjectRepository::Add(shared_ptr<GameObject> obj) {
    int x_cord = obj->Pos().x;
    int y_cord = obj->Pos().y;

    if (x_cord >= w_ || y_cord >= h_) return false;

    objects_[y_cord][x_cord] = obj;
    return true;
}

vector<shared_ptr<GameObject>>&& ObjectRepository::GetAll() {
    vector<shared_ptr<GameObject>> objects;
    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            if (objects_[y][x] != nullptr) objects.push_back(objects_[y][x]);
        }
    }

    return objects;
}

shared_ptr<GameObject> ObjectRepository::GetByCords(sf::Vector2u cords) {
    if (cords.x >= w_ || cords.y >= h_)
        return nullptr;
    else
        return objects_[cords.y][cords.x];
}

shared_ptr<GameObject> ObjectRepository::GetByPlayer(Player player) {
    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            if (*objects_[y][x].GetPlayer() == player) return objects_[y][x];
        }
    }

    return nullptr;
}

bool ObjectRepository::Delete(shared_ptr<GameObject> obj) {
    int x_cord = obj->Pos().x;
    int y_cord = obj->Pos().y;

    if (x_cord >= w_ || y_cord >= h_) return false;

    objects_[y_cord][x_cord] = nullptr;
    return true;
}

bool ObjectRepository::DeleteByCords(sf::Vector2u cords) {
    if (cords.x >= w_ || cords.y >= h_) {
        return false;
    } else {
        objects_[cords.y][cords.x] = nullptr;
        return true;
    }
}
