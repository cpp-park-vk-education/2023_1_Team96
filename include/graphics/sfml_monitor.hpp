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

class SFMLModel : public IModel {
   protected:
    sf::RenderWindow &target_;

   public:
    SFMLModel(sf::RenderWindow &target) : target_(target) {}

    virtual void Draw() override = 0;

    virtual ~SFMLModel() {}
};

struct Stats {
    int hp, armor, attack;
};

class InfoBar {
   private:
    sf::Sprite layout;
    sf::Text text;
    bool is_opening;
    double anim_frame_;

   public:
    InfoBar(const sf::Font &font, const sf::Texture &texture,
            sf::IntRect tRect);

    void updateInfo(Stats stats);
    void setPos(sf::Vector2f pos) {
        layout.setPosition(pos);
        text.setPosition(pos.x + 20, pos.y + 50);
    }
    void draw(sf::RenderWindow &target);
    void open() {
        anim_frame_ = 0;
        is_opening = true;
    }
    void close() {
        anim_frame_ = 0;
    }
};

class SFMLFieldModel : public IFieldModel, public SFMLModel {
   private:
    std::vector<string> tile_map_;
    sf::Vector2u size_;
    sf::Sprite s_map_;
    sf::Vector2u current_;
    int is_info;
    InfoBar ibar;

   public:
    SFMLFieldModel(sf::RenderWindow &window, const sf::Texture &texture,
                   const sf::Font &font, const std::vector<std::string>& map);

    void Draw() override;

    void SetCurrent(sf::Vector2u pos) override { current_ = pos; };
    void SetStat(Stats stat);
    void ShowStat() {
        if (is_info) ibar.draw(target_);
    };
    void HideStat() {
        is_info = false;
        ibar.close();
    };

    void ResetCurrent() override {
        current_.x = -1;
        current_.y = -1;
    }
};

enum Animation {
    HIT, HURT, RUN, NONE, BLOW,
};

class SFMLUnitModel : public SFMLModel, public IObjectModel {
   private:
    sf::Sprite sprite_;
    Animation anim;
    sf::Vector2i mov_vec;
    sf::Vector2u pos;
    double mov_anim_speed;
    double anim_frame_;
    bool is_mine_;
    bool right;

    void setRight()
    {
        if (right) return;
        sprite_.scale(-1,1);
        sprite_.setOrigin(0, 0);
        right = true;
    }

    void setLeft()
    {
        if (!right) return;
        sprite_.scale(-1,1);
        sprite_.setOrigin(CELL_TEXTURE_SIZE, 0);
        right = false;
    }

   public:
    SFMLUnitModel(sf::RenderWindow &window, const sf::Texture &texture,
                  bool is_mine);

    void Draw() override;
    void Move(sf::Vector2u to) override;
    void Attack(sf::Vector2u pos_) override;
    void GetDamage(int damage, sf::Vector2u pos_) override;
    void Blow() {
        anim = BLOW;
    };
};

class SFMLKingModel : public SFMLModel, public IObjectModel {
   private:
    sf::Sprite sprite_;
    Animation anim;
    sf::Vector2i mov_vec;
    sf::Vector2u pos;
    double mov_anim_speed;
    double anim_frame_;
    bool is_mine_;
    bool right;

    void setRight()
    {
        if (right) return;
        sprite_.scale(-1,1);
        sprite_.setOrigin(0, 0);
        right = true;
    }

    void setLeft()
    {
        if (!right) return;
        sprite_.scale(-1,1);
        sprite_.setOrigin(CELL_TEXTURE_SIZE, 0);
        right = false;
    }

   public:
    SFMLKingModel(sf::RenderWindow &window, const sf::Texture &texture,
                  bool is_mine);

    void Draw() override;
    void Move(sf::Vector2u pos) override;
    void Attack(sf::Vector2u pos_) override;
    void GetDamage(int damage, sf::Vector2u pos) override;
    void Blow() {
        anim = BLOW;
    };
};

class SFMLTModel : public SFMLModel, public IObjectModel {
   private:
    sf::Sprite sprite_;

   public:
    SFMLTModel(sf::RenderWindow &window, const sf::Texture &texture);

    void Draw() override;

    void Move(sf::Vector2u pos) override;

    void Attack(sf::Vector2u pos_) override {};
    void GetDamage(int damage, sf::Vector2u pos) override {};
    void Blow() {};
};

class SFMLHModel : public SFMLModel, public IObjectModel {
   private:
    sf::Sprite sprite_;

   public:
    SFMLHModel(sf::RenderWindow &window, const sf::Texture &texture, bool is_mine);

    void Draw() override;

    void Move(sf::Vector2u pos) override;

    void Attack(sf::Vector2u pos_) override {};
    void GetDamage(int damage, sf::Vector2u pos) override {};
    void Blow() {};
};

class SFMLWindow {
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
               sf::WindowHandle winhandle);
    bool loadResources();

    void Prepare() { m_window_.clear(); }
    void Draw() { m_window_.display(); }
    bool IsEnd() { return !m_window_.isOpen(); }

    sf::Vector2u GetWindowSize() { return m_windowSize_; }
    std::unique_ptr<IObjectModel> GetModel(ModelType type, bool is_mine);
    std::unique_ptr<SFMLFieldModel> GetFieldModel(const std::vector<std::string>& map);
    sf::RenderWindow &GetWindow() { return m_window_; }

    ~SFMLWindow() { Destroy(); }
};
