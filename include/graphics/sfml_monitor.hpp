#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

#include "graphics/graphics.hpp"

using sf::Vector2u;
using std::string;

class SFMLModel : public IModel {
   protected:
    sf::RenderWindow &target;

   public:
    SFMLModel(sf::RenderWindow &_target) : target(_target) {}

    virtual void draw() override = 0;

    virtual ~SFMLModel() {}
};

class SFMLFieldModel : public IFieldModel, public SFMLModel {
   private:
    std::vector<string> tile_map;
    sf::Vector2u size;
    sf::Sprite s_map;
    sf::Texture tile_set;
    sf::Vector2i current;

   public:
    SFMLFieldModel(sf::RenderWindow &_window, uint cols, uint rows);

    void draw() override;

    void setCurrent(sf::Vector2i pos) override { current = pos; };

    void resetCurrent() override {
        current.x = -1;
        current.y = -1;
    }
};

class SFMLUnitModel : public SFMLModel, public IObjectModel {
   private:
    sf::Sprite sprite;
    sf::Texture tile_set;

   public:
    SFMLUnitModel(sf::RenderWindow &_window);

    void draw() override;

    void Move(sf::Vector2u pos) override;

    void Attack(sf::Vector2u pos) override {}
    void GetDamage(int damage) override {}
};

class SFMLWindow {
   public:
    SFMLWindow(const string &l_title, const sf::Vector2u &l_size, sf::WindowHandle winhandle, sf::WindowHandle mainwinhandle);
    
    ~SFMLWindow() { Destroy(); }

    void Prepare() { m_window.clear(); }

    void Draw() { m_window.display(); }

    bool IsOpen() { return m_window.isOpen(); }

    sf::Vector2u GetWindowSize() { return m_windowSize; }

    bool isEnd() { return !m_window.isOpen(); }

    std::unique_ptr<IObjectModel> getModel(ModelType type);

    std::unique_ptr<SFMLFieldModel> getFieldModel(uint cols, uint rows);

    sf::RenderWindow &getWindow() { return m_window; }

   private:
    void Destroy() { m_window.close(); }

    sf::RenderWindow m_window;
    sf::Vector2u m_windowSize;
    std::string m_windowTitle;
};
