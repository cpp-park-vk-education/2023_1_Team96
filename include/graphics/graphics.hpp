#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

using std::vector;

class IModel;
class IFieldModel;

enum ModelType { B_MODEL, K_MODEL, S_MODEL };

class IMonitor {
   public:
    virtual void Prepare() = 0;
    virtual void Draw() = 0;
    virtual bool IsEnd() = 0;
    virtual std::unique_ptr<IModel> GetModel(ModelType type) = 0;
    virtual std::unique_ptr<IFieldModel> GetFieldModel() = 0;

    virtual ~IMonitor() {}
};

class IModel {
   public:
    virtual void Draw() = 0;

    virtual ~IModel() {}
};

class IFieldModel : public IModel {
   public:
    virtual void SetCurrent(sf::Vector2u pos) = 0;
    virtual void ResetCurrent() = 0;

    virtual ~IFieldModel() {}
};

class IObjectModel : public IModel {
   public:
    virtual void Move(sf::Vector2u pos) = 0;
    virtual void Attack() = 0;
    virtual void GetDamage(int damage) = 0;

    virtual ~IObjectModel() {}
};
