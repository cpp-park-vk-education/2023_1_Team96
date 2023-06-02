#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

using std::vector;

class IModel;
class IFieldModel;

enum class ModelType { Warrior, King, Trees, Camp };

class IObjectModel;
class Stats;

class IMonitor {
   public:
    virtual ~IMonitor() {}

    virtual void Prepare() = 0;
    virtual void Draw() = 0;
    virtual bool IsEnd() = 0;
    virtual std::unique_ptr<IObjectModel> GetModel(ModelType type, bool is_mine) = 0;
    virtual std::unique_ptr<IFieldModel> GetFieldModel(
        const std::vector<std::string> &map) = 0;
};

class IModel {
   public:
    virtual ~IModel() {}

    virtual void Draw() = 0;
};

class IFieldModel : public IModel {
   public:
    virtual ~IFieldModel() {}
    
    virtual void SetCurrent(sf::Vector2u pos) = 0;
    virtual void ResetCurrent() = 0;
    virtual void SetStat(Stats stat) = 0;
    virtual void ShowStat() = 0;
    virtual void HideStat() = 0;
};

class IObjectModel : public IModel {
   public:
    virtual ~IObjectModel() {}

    virtual void Move(sf::Vector2u pos) = 0;
    virtual void Attack(sf::Vector2u pos) = 0;
    virtual void GetDamage(int damage, sf::Vector2u pos_) = 0;
    virtual void Blow() = 0;
};
