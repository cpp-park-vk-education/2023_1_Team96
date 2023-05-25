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
    sf::RenderWindow &target_;

   public:
    SFMLModel(sf::RenderWindow &target) : target_(target) {}

    virtual void Draw() override = 0;

    virtual ~SFMLModel() {}
};

class SFMLFieldModel : public IFieldModel, public SFMLModel {
   private:
    std::vector<string> tile_map_;
    sf::Vector2u size_;
    sf::Sprite s_map_;
    sf::Texture tile_set_;
    sf::Vector2u current_;

   public:
    SFMLFieldModel(sf::RenderWindow &window);

    void Draw() override;

    void SetCurrent(sf::Vector2u pos) override { current_ = pos; };

    void ResetCurrent() override {
        current_.x = -1;
        current_.y = -1;
    }
};

class SFMLUnitModel : public SFMLModel, public IObjectModel {
   private:
    sf::Sprite sprite_;
    sf::Texture tile_set_;
    bool is_attack_;
    bool is_mine_;
    double attack_frame_;

   public:
    SFMLUnitModel(sf::RenderWindow &window, bool is_mine);

    void Draw() override;

    void Move(sf::Vector2u pos) override;

    void Attack() override;
    void GetDamage(int damage) override {}
};

class SFMLWindow {
   private:
    sf::RenderWindow m_window_;
    sf::Vector2u m_windowSize_;
    std::string m_windowTitle_;

    void Destroy() { m_window_.close(); }

   public:
    SFMLWindow(const string &l_title, const sf::Vector2u &l_size,
               sf::WindowHandle winhandle);

    void Prepare() { m_window_.clear(); }
    void Draw() { m_window_.display(); }
    bool IsEnd() { return !m_window_.isOpen(); }

    sf::Vector2u GetWindowSize() { return m_windowSize_; }
    std::unique_ptr<IObjectModel> GetModel(ModelType type, bool is_mine);
    std::unique_ptr<SFMLFieldModel> GetFieldModel();
    sf::RenderWindow &GetWindow() { return m_window_; }

    ~SFMLWindow() { Destroy(); }
};
