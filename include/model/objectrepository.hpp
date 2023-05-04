#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "gameobject.hpp"

struct IObjectRepository {
    virtual void Add(std::unique_ptr<GameObject> obj) = 0;
    virtual std::shared_ptr<GameObject> GetByCords(sf::Vector2u cords) = 0;
    virtual std::shared_ptr<GameObject> GetByPlayer(Player player) = 0;
    virtual void Delete(std::shared_ptr<GameObject> obj) = 0;
    virtual void DeleteByCords(sf::Vector2u cords) = 0;

    virtual ~IObjectRepository() {}
};
