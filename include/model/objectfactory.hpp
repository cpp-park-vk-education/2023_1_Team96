#pragma once

#include <memory>

#include "gameobject.hpp"

struct IObjectFactory {
    virtual std::unique_ptr<GameObject> CreateObject() = 0;

    virtual ~IObjectFactory() {}
};
