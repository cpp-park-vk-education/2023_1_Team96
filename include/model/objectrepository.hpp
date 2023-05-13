#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "gameobject.hpp"
#include "graphics/graphics.hpp"

struct IObjectRepository {
    using std::shared_ptr;

    virtual bool Add(shared_ptr<GameObject> obj) = 0;
    virtual shared_ptr<GameObject> GetByCords(sf::Vector2u cords) = 0;
    virtual shared_ptr<GameObject> GetByPlayer(Player player) = 0;
    virtual bool Delete(shared_ptr<GameObject> obj) = 0;
    virtual bool DeleteByCords(sf::Vector2u cords) = 0;

    virtual ~IObjectRepository() {}
};

class ObjectRepository : public IObjectRepository {
    using std::vector, std::shared_ptr;

   private:
    vector<vector<shared_ptr<GameObject>>> objects_;
    unsigned int h_, w_;

   public:
    ObjectRepository(int field_height, int field_width)
        : objects_(field_height), h_(field_height), w_(field_width) {
        for (int i = 0; i < field_height; ++i) objects_[i].resize(field_width);
    }

    bool Add(shared_ptr<GameObject> obj) override;
    shared_ptr<GameObject> GetByCords(sf::Vector2u cords) override;
    shared_ptr<GameObject> GetByPlayer(Player player) override;
    bool Delete(shared_ptr<GameObject> obj) override;
    bool DeleteByCords(sf::Vector2u cords) override;
};
