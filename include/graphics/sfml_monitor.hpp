#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "graphics/graphics.hpp"
#include "utility/common.h"

using sf::Vector2u;
using std::string;

enum class Animation {
    Hit,
    Hurt,
    Run,
    None,
    Blow,
    Open,
};

struct Stats {
    int hp, armor, attack;
};

class SFMLModel : public IModel {
   protected:
    sf::Sprite sprite;
    Animation anim;
    double anim_frame_;

    sf::RenderWindow &target_;

   public:
    virtual ~SFMLModel() {}

    SFMLModel(sf::RenderWindow &target) : target_(target) {}
    void SetOrigin(float x, float y) { sprite.setOrigin(x, y); }
    void SetTextRect(const sf::IntRect& rect) { sprite.setTextureRect(rect); }
    void Scale(float x, float y) { sprite.scale(x,y); }
};

class InfoBar : SFMLModel {
   private:
    sf::Text text;

   public:
    InfoBar(sf::RenderWindow &window, const sf::Font &font,
            const sf::Texture &texture, sf::IntRect tRect);

    void Draw() override;
    void UpdateInfo(Stats stats);
    void setPos(sf::Vector2f pos) {
        sprite.setPosition(pos);
        text.setPosition(pos.x + 20, pos.y + 50);
    }
    void open() {
        anim_frame_ = 0;
        anim = Animation::Open;
    }
    void close() { anim_frame_ = 0; }
};

class SFMLFieldModel : public IFieldModel, public SFMLModel {
   private:
    std::vector<string> tile_map_;
    sf::Vector2u size_;
    sf::Vector2u current_;
    InfoBar ibar;
    int is_info;

   public:
    SFMLFieldModel(sf::RenderWindow &window, const sf::Texture &texture,
                   const sf::Font &font, const std::vector<std::string> &map);

    void Draw() override;

    void SetCurrent(sf::Vector2u pos) override { current_ = pos; };
    void ResetCurrent() override {
        current_.x = -1;
        current_.y = -1;
    }
    void SetStat(Stats stat);
    void ShowStat() {
        if (is_info) ibar.Draw();
    };
    void HideStat() {
        is_info = false;
        ibar.close();
    };
};


class SFMLUnitModel : public SFMLModel, public IObjectModel {
   protected:
    sf::Vector2i mov_vec;
    sf::Vector2u pos;
    double mov_anim_speed;
    bool is_mine_;
    bool right;

    void setRight() {
        if (right) return;
        sprite.scale(-1, 1);
        sprite.setOrigin(0, 0);
        right = true;
    }
    void setLeft() {
        if (!right) return;
        sprite.scale(-1, 1);
        sprite.setOrigin(CELL_TEXTURE_SIZE, 0);
        right = false;
    }

   public:
    SFMLUnitModel(sf::RenderWindow &window, const sf::Texture &texture,
                  bool is_mine);

    virtual void Draw() = 0;
    void Move(sf::Vector2u to) override;
    void Attack(sf::Vector2u pos_) override;
    void GetDamage(int damage, sf::Vector2u pos_) override;
    void Blow() { anim = Animation::Blow; };
};

class SFMLWarriorModel : public SFMLUnitModel {
   public:
    SFMLWarriorModel(sf::RenderWindow &window, const sf::Texture &texture,
                     bool is_mine);

    void Draw() override;
};

class SFMLKingModel : public SFMLUnitModel {
   public:
    SFMLKingModel(sf::RenderWindow &window, const sf::Texture &texture,
                  bool is_mine);

    void Draw() override;
};


class SFMLEnvModel : public SFMLModel, public IObjectModel {
   public:
    SFMLEnvModel(sf::RenderWindow &window, const sf::Texture &texture);

    void Move(sf::Vector2u pos) override {
        sprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
    }
    void Draw() override { target_.draw(sprite); };
    void Attack(sf::Vector2u pos_) override {};
    void GetDamage(int damage, sf::Vector2u pos) override{};
    void Blow() override {};
};

class SFMLWindow : public IMonitor {
   private:
    sf::RenderWindow m_window_;
    sf::Vector2u m_windowSize_;
    std::string m_windowTitle_;
    sf::Texture tile_set_;
    sf::Texture units_set_;
    sf::Font font_;

    void Destroy() { m_window_.close(); }

   public:
    SFMLWindow(const string &l_title, const sf::Vector2u &l_size,
               sf::WindowHandle win_descriptor, const std::string& config_path);
    bool loadResources(const std::string& config_path);

    void Prepare() { m_window_.clear(); }
    void Draw() { m_window_.display(); }
    bool IsEnd() { return !m_window_.isOpen(); }

    sf::Vector2u GetWindowSize() { return m_windowSize_; }
    std::unique_ptr<IObjectModel> GetModel(ModelType type, bool is_mine);
    std::unique_ptr<IFieldModel> GetFieldModel(
        const std::vector<std::string> &map);
    sf::RenderWindow &GetWindow() { return m_window_; }

    ~SFMLWindow() { Destroy(); }
};
