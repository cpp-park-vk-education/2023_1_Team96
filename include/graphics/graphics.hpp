#pragma once

#include <memory>

class IModel;

enum ModelType
{
    UNIT,
    KING,
    STONE
};

class IMonitor
{
public:
    virtual void Prepare() = 0;
    virtual void Draw() = 0;
    virtual bool isEnd() = 0;
    virtual std::unique_ptr<IModel> getModel(ModelType type) = 0;
};

class IModel
{
public:
    virtual void draw() const = 0;
    virtual void move(int x, int y) const = 0;
    virtual void attack(int x, int y) const = 0;
    virtual void get_damage(int damage) const = 0;
};

class IModelFactory 
{
public:
    virtual std::unique_ptr<IModel> CreateModel() = 0;
    virtual ~IModelFactory() {}
};
