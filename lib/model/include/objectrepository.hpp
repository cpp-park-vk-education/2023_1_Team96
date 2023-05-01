#pragma once

#include <memory>

#include "gameobject.hpp"

struct IObjectRepository {
    virtual void Add(std::unique_ptr<GameObject> obj) = 0;
    virtual std::shared_ptr<GameObject> GetByCell(Cell cell) = 0;
    virtual std::shared_ptr<GameObject> GetByPlayer(Player player) = 0;
    virtual void Delete(std::shared_ptr<GameObject> obj) = 0;
    virtual void DeleteByCell(Cell cell) = 0;

    virtual ~IObjectRepository() {}
};
