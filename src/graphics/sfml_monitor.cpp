#include "graphics/sfml_monitor.hpp"

#include <iostream>
#include <cmath>

const char FIELD_BORDER = '0';
const char BROKEN_CELL = 'h';
const char CELL = ' ';

const string IMG_PATH = "../static/";

const double ANIMATION_FRAME_TIME = 0.01;

SFMLWindow::SFMLWindow(const string &l_title, const sf::Vector2u &l_size,
                       sf::WindowHandle winhandle) {
    loadResources();
    m_windowTitle_ = l_title;
    m_windowSize_ = l_size;
    m_window_.create(winhandle);
}

std::unique_ptr<IObjectModel> SFMLWindow::GetModel(ModelType type,
                                                   bool is_mine) {
    switch (type) {
        case B_MODEL:
            return std::make_unique<SFMLUnitModel>(m_window_, units_set_,
                                                   is_mine);
        case K_MODEL:
            return std::make_unique<SFMLKingModel>(m_window_, units_set_,
                                                   is_mine);
        case T_MODEL:
            return std::make_unique<SFMLTModel>(m_window_, units_set_);

        case H_MODEL:
            return std::make_unique<SFMLHModel>(m_window_, units_set_, is_mine);
    }
    return nullptr;
}

std::unique_ptr<SFMLFieldModel> SFMLWindow::GetFieldModel(const std::vector<std::string>& map) {
    return std::make_unique<SFMLFieldModel>(m_window_, tile_set_, font_, map);
};

bool SFMLWindow::loadResources() {
    sf::Image unit_image;
    if (!unit_image.loadFromFile(IMG_PATH + "unit.png")) return false;
    if (!units_set_.loadFromImage(unit_image)) return false;

    sf::Image map_image;
    if (!map_image.loadFromFile(IMG_PATH + "map.png")) return false;
    if (!tile_set_.loadFromImage(map_image)) return false;

    sf::Font font;
    if (!font_.loadFromFile(IMG_PATH + "arkhip.ttf")) return false;
    return true;
};


InfoBar::InfoBar(const sf::Font &font, const sf::Texture &texture, sf::IntRect tRect) : 
                                        is_opening(false), anim_frame_(0) {
    layout.setTexture(texture);
    layout.setTextureRect(tRect);
    layout.setScale(sf::Vector2f(SCALE,SCALE));
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setScale(sf::Vector2f(SCALE,SCALE));
    text.setCharacterSize(7);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

void InfoBar::updateInfo(Stats stats) 
{
    std::stringstream ss;
    ss << " Warrior \n" << "hp: " << stats.hp << "\n" << "armor: " << stats.armor 
                 << "\n" << "attack: " << stats.attack << std::endl;
    text.setString(ss.str());
}

void InfoBar::draw(sf::RenderWindow& target)
{
    if (is_opening) {
        anim_frame_ += ANIMATION_FRAME_TIME*2;
        if (anim_frame_ > 4) {
            is_opening = false;
            anim_frame_ = 3;
        }
        layout.setTextureRect(sf::IntRect(((3 - (int)anim_frame_) * 75) + 24, 47, 74, 90));
    }
    target.draw(layout);
    if (!is_opening)
        target.draw(text);
}


SFMLFieldModel::SFMLFieldModel(sf::RenderWindow &window,
                               const sf::Texture &texture,
                               const sf::Font& font,
                               const std::vector<std::string>& map)
    : SFMLModel(window), ibar(font, texture, sf::IntRect(24,47,74,90)) {
    s_map_.setTexture(texture);
    s_map_.setScale(sf::Vector2f(SCALE,SCALE));
    is_info = false;
    
    tile_map_ = map;

    unsigned int w = tile_map_[0].length();
    unsigned int h = tile_map_.size();
    size_ = Vector2u{w, h};
    ResetCurrent();
};

void SFMLFieldModel::Draw() {
    for (int i = 0; i < size_.y; ++i) {
        for (int j = 0; j < size_.x; ++j) {
            char tile = tile_map_[i][j];

            switch (tile) {
                case CELL:
                    s_map_.setTextureRect(
                        sf::IntRect(26, 219, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;

                case FIELD_BORDER:
                    s_map_.setTextureRect(
                        sf::IntRect(26, 298, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;

                case BROKEN_CELL:
                    s_map_.setTextureRect(
                        sf::IntRect(106, 219, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;

                default:
                    s_map_.setTextureRect(
                        sf::IntRect(0, 0, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;
            }

            if (i == current_.y && j == current_.x)
                s_map_.setColor(sf::Color::Yellow);
            
            s_map_.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            target_.draw(s_map_);

            s_map_.setColor(sf::Color::White);
        }
    }
};

void SFMLFieldModel::SetStat(Stats stat) {
    ibar.updateInfo(stat);
    sf::Vector2f ibar_pos = sf::Vector2f{CELL_SIZE * (current_.x - 0.8), CELL_SIZE * (current_.y - 1)}; 
    ibar.setPos(ibar_pos);
    ibar.open();
    is_info = true;
};


SFMLUnitModel::SFMLUnitModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window),
      anim(NONE),
      anim_frame_(0),
      is_mine_(is_mine),
      right(true) {
    sprite_.setTexture(texture);
    sprite_.setScale(sf::Vector2f(SCALE,SCALE));

    if (is_mine_)
        sprite_.setTextureRect(sf::IntRect(1, 1, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    else {
        sprite_.setTextureRect(sf::IntRect(1, 65, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        setLeft();
        pos.x = 14;
        pos.y = 0;
    }
}

void SFMLUnitModel::Draw() {
    switch (anim)
    {
    case HIT:
        anim_frame_ += ANIMATION_FRAME_TIME;
        if (anim_frame_ > 3) {
            anim = NONE;
            anim_frame_ = 0;
        }
        if (is_mine_)
            sprite_.setTextureRect(sf::IntRect(((int)anim_frame_ * (CELL_TEXTURE_SIZE+1)) + 1,
                                               1, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        else
            sprite_.setTextureRect(sf::IntRect(((int)anim_frame_ * (CELL_TEXTURE_SIZE+1)) + 1,
                                               65, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        break;
    case RUN:
        anim_frame_ += mov_anim_speed;
        if (anim_frame_ > 60) {
            anim = NONE;
            anim_frame_ = 0;
            pos.x += mov_vec.x;
            pos.y += mov_vec.y;
            sprite_.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
        } else {
            float posx = (pos.x + (float)((int)anim_frame_ * mov_vec.x) / 60);
            float posy = (pos.y + (float)((int)anim_frame_ * mov_vec.y) / 60);
            sprite_.setPosition( posx * CELL_SIZE, posy * CELL_SIZE);
        }
        break;
    case HURT:
        anim_frame_ += ANIMATION_FRAME_TIME*20;
        if (anim_frame_ > 60) {
            anim = NONE;
            anim_frame_ = 0;
            sprite_.setColor(sf::Color::White);
        } else {
            sprite_.setColor(sf::Color((int)anim_frame_ * 200/60 + 55,0,0));
        }
        break;
    case BLOW:
        anim_frame_ += ANIMATION_FRAME_TIME;
        if (anim_frame_ > 9) {
            anim = NONE;
            anim_frame_ = 0;
            sprite_.setTextureRect(sf::IntRect((9 * (CELL_TEXTURE_SIZE+1)) + 1,
                                129, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        } else {
            sprite_.setTextureRect(sf::IntRect(((int)anim_frame_ * (CELL_TEXTURE_SIZE+1)) + 1,
                                129, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        }
        break;
    }
    target_.draw(sprite_);
};

void SFMLUnitModel::Move(sf::Vector2u pos_) {
    mov_vec = sf::Vector2i{pos_.x - pos.x, pos_.y - pos.y};
    float dist = std::sqrt(mov_vec.x*mov_vec.x + mov_vec.y*mov_vec.y);
    mov_anim_speed = 15 * 2 * ANIMATION_FRAME_TIME / dist;
    anim = RUN;
    if (mov_vec.x >= 0) setRight();
    else setLeft();
}

void SFMLUnitModel::Attack(sf::Vector2u pos_) {
    int dx = pos_.x - pos.x;
    if (dx >= 0) setRight();
    else setLeft();

    anim = HIT;
    anim_frame_ = 0;
}

void SFMLUnitModel::GetDamage(int damage, sf::Vector2u pos_) 
{
    int dx = pos_.x - pos.x;
    if (dx >= 0) setRight();
    else setLeft();

    anim = HURT;
};


SFMLKingModel::SFMLKingModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window),
      anim(NONE),
      anim_frame_(0),
      is_mine_(is_mine),
      right(true)  {
    sprite_.setTexture(texture);
    sprite_.setScale(sf::Vector2f(SCALE,SCALE));

    if (is_mine_)
        sprite_.setTextureRect(sf::IntRect(193, 1, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    else {
        sprite_.setTextureRect(sf::IntRect(193, 65, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        setLeft();
        pos.x = 15;
        pos.y = 0;
    }
}

void SFMLKingModel::Draw() {
    switch (anim)
    {
    case HIT:
        break;
    case RUN:
        anim_frame_ += mov_anim_speed;
        if (anim_frame_ > 60) {
            anim = NONE;
            anim_frame_ = 0;
            pos.x += mov_vec.x;
            pos.y += mov_vec.y;
            sprite_.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
        } else {
            float posx = (pos.x + (float)((int)anim_frame_ * mov_vec.x) / 60);
            float posy = (pos.y + (float)((int)anim_frame_ * mov_vec.y) / 60);
            sprite_.setPosition( posx * CELL_SIZE, posy * CELL_SIZE);
        }
        break;
    case HURT:
        anim_frame_ += ANIMATION_FRAME_TIME*20;
        if (anim_frame_ > 60) {
            anim = NONE;
            anim_frame_ = 0;
            sprite_.setColor(sf::Color::White);
        } else {
            sprite_.setColor(sf::Color((int)anim_frame_ * 200/60 + 55,0,0));
        }
        break;
    case BLOW:
        anim_frame_ += ANIMATION_FRAME_TIME;
        if (anim_frame_ > 9) {
            anim = NONE;
            anim_frame_ = 0;
            sprite_.setTextureRect(sf::IntRect((9 * (CELL_TEXTURE_SIZE+1)) + 1,
                                129, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        } else {
            sprite_.setTextureRect(sf::IntRect(((int)anim_frame_ * (CELL_TEXTURE_SIZE+1)) + 1,
                                129, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
        }
        break;
    }
    target_.draw(sprite_);
};

void SFMLKingModel::Move(sf::Vector2u pos_) {
    mov_vec = sf::Vector2i{pos_.x - pos.x, pos_.y - pos.y};
    float dist = std::sqrt(mov_vec.x*mov_vec.x + mov_vec.y*mov_vec.y);
    mov_anim_speed = 15 * 2 * ANIMATION_FRAME_TIME / dist;
    anim = RUN;
    if (mov_vec.x >= 0) setRight();
    else setLeft();
}

void SFMLKingModel::Attack(sf::Vector2u pos_) {
    int dx = pos_.x - pos.x;
    if (dx >= 0) setRight();
    else setLeft();

    anim = HIT;
    anim_frame_ = 0;
}

void SFMLKingModel::GetDamage(int damage, sf::Vector2u pos_) 
{
    int dx = pos_.x - pos.x;
    if (dx >= 0) setRight();
    else setLeft();

    anim = HURT;
};


SFMLTModel::SFMLTModel(sf::RenderWindow &window,
                             const sf::Texture &texture)
    : SFMLModel(window) {
    sprite_.setTexture(texture);
    sprite_.setScale(sf::Vector2f(SCALE,SCALE));
    sprite_.setOrigin(230,50);

    sprite_.setTextureRect(sf::IntRect(1, 257, 687, 193));
}

void SFMLTModel::Draw() {
    target_.draw(sprite_);
};

void SFMLTModel::Move(sf::Vector2u pos) {
    sprite_.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
}

SFMLHModel::SFMLHModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window) {
    sprite_.setTexture(texture);
    sprite_.setScale(sf::Vector2f(SCALE,SCALE));
    sprite_.setOrigin(150,122);
    sprite_.scale(sf::Vector2f(0.75,0.75));

    sprite_.setTextureRect(sf::IntRect(647, 4, 301, 244));
}

void SFMLHModel::Draw() {
    target_.draw(sprite_);
};

void SFMLHModel::Move(sf::Vector2u pos) {
    sprite_.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
}