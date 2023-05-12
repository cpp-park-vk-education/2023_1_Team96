#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

using std::vector;

class IModel;
class IFieldModel;

enum ModelType
{
    B_MODEL,
    K_MODEL,
    S_MODEL,
};

class IMonitor
{
public:
    virtual void Prepare() = 0;
    virtual void Draw() = 0;
    virtual bool isEnd() = 0;
    virtual std::unique_ptr<IModel> getModel(ModelType type) = 0;
    virtual std::unique_ptr<IFieldModel> getFieldModel(sf::Vector2u size) = 0;

    virtual ~IMonitor() {}
};

class IModel
{
public:
    virtual void Draw() const = 0;
    virtual void Move(sf::Vector2u pos) = 0;
    virtual void Attack(sf::Vector2u pos) const = 0;
    virtual void GetDamage(int damage) const = 0;

    virtual ~IModel() {}
};

class IModelFactory
{
public:
    virtual std::unique_ptr<IModel> CreateModel() = 0;

    virtual ~IModelFactory() {}
};

class IFieldModel
{
public:

    virtual void render() = 0;

    virtual void setCurrent(sf::Vector2i pos) = 0;
    virtual void resetCurrent() = 0;
    
    virtual ~IFieldModel() {}
};