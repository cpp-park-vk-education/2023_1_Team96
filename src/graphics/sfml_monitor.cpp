#include "graphics/sfml_monitor.hpp"

#include <cmath>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char FIELD_BORDER = '0';
const char CELL = ' ';
const char BROKEN_CELL = 'b';

const string CONFIG = "../config.json";

const double ANIMATION_FRAME_TIME = 0.01;

SFMLWindow::SFMLWindow(const string &l_title, const sf::Vector2u &l_size,
                       sf::WindowHandle win_descriptor) {
    loadResources();
    m_windowTitle_ = l_title;
    m_windowSize_ = l_size;
    m_window_.create(win_descriptor);
}

bool SFMLWindow::loadResources() {
    std::ifstream config_fs(CONFIG);
    json config = json::parse(config_fs);
    config_fs.close();

    std::string static_dir, units, map, font;
    config.at("static_dir").get_to(static_dir);
    config.at("units").get_to(units);
    config.at("map").get_to(map);
    config.at("font").get_to(font);

    sf::Image unit_image;
    if (!unit_image.loadFromFile(static_dir + units)) return false;
    if (!units_set_.loadFromImage(unit_image)) return false;

    sf::Image map_image;
    if (!map_image.loadFromFile(static_dir + map)) return false;
    if (!tile_set_.loadFromImage(map_image)) return false;

    if (!font_.loadFromFile(static_dir + font)) return false;
    return true;
};

std::unique_ptr<IObjectModel> SFMLWindow::GetModel(ModelType type,
                                                   bool is_mine) {
    switch (type) {
        case ModelType::Warrior:
            return std::make_unique<SFMLWarriorModel>(m_window_, units_set_,
                                                   is_mine);
        case ModelType::King:
            return std::make_unique<SFMLKingModel>(m_window_, units_set_,
                                                   is_mine);
        case ModelType::Trees:
            return std::make_unique<SFMLTreesModel>(m_window_, units_set_);

        case ModelType::Camp:
            return std::make_unique<SFMLCampModel>(m_window_, units_set_);
    }
    return nullptr;
}

std::unique_ptr<IFieldModel> SFMLWindow::GetFieldModel(
    const std::vector<std::string> &map) {
    return std::make_unique<SFMLFieldModel>(m_window_, tile_set_, font_, map);
};


InfoBar::InfoBar(sf::RenderWindow &window, const sf::Font &font,
                 const sf::Texture &texture, sf::IntRect tRect)
    : SFMLModel(window) {
    anim_frame_ = 0;
    sprite.setTexture(texture);
    sprite.setTextureRect(tRect);
    sprite.setScale(sf::Vector2f(SCALE, SCALE));
    text.setFont(font);
    text.setFillColor(sf::Color::Red);
    text.setScale(sf::Vector2f(SCALE, SCALE));
    text.setCharacterSize(7);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

void InfoBar::UpdateInfo(Stats stats) {
    std::stringstream ss;
    ss << " Warrior \n"
       << "hp: " << stats.hp << "\n"
       << "armor: " << stats.armor << "\n"
       << "attack: " << stats.attack << std::endl;
    text.setString(ss.str());
}

void InfoBar::Draw() {
    if (anim == Animation::OPEN) {
        anim_frame_ += ANIMATION_FRAME_TIME * 2;
        if (anim_frame_ > 4) {
            anim = NONE;
            anim_frame_ = 3;
        }
        sprite.setTextureRect(
            sf::IntRect(((3 - (int)anim_frame_) * 75) + 24, 47, 74, 90));
    }
    target_.draw(sprite);
    if (!anim) target_.draw(text);
}

SFMLFieldModel::SFMLFieldModel(sf::RenderWindow &window,
                               const sf::Texture &texture, const sf::Font &font,
                               const std::vector<std::string> &map)
    : SFMLModel(window),
      ibar(target_, font, texture, sf::IntRect(24, 47, 74, 90)) {
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(SCALE, SCALE));
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
                    sprite.setTextureRect(sf::IntRect(
                        26, 219, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;

                case FIELD_BORDER:
                    sprite.setTextureRect(sf::IntRect(
                        26, 298, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;

                case BROKEN_CELL:
                    sprite.setTextureRect(sf::IntRect(
                        106, 219, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
                    break;

                default:
                    sprite.setTextureRect(sf::IntRect(0, 0, CELL_TEXTURE_SIZE,
                                                      CELL_TEXTURE_SIZE));
                    break;
            }

            if (i == current_.y && j == current_.x)
                sprite.setColor(sf::Color::Yellow);

            sprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            target_.draw(sprite);

            sprite.setColor(sf::Color::White);
        }
    }
};

void SFMLFieldModel::SetStat(Stats stat) {
    ibar.UpdateInfo(stat);
    sf::Vector2f ibar_pos = sf::Vector2f{CELL_SIZE * static_cast<float>(current_.x - 0.8),
                                         CELL_SIZE * static_cast<float>(current_.y - 1)};
    ibar.setPos(ibar_pos);
    ibar.open();
    is_info = true;
};

SFMLUnitModel::SFMLUnitModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window), is_mine_(is_mine), right(true) {
    anim = Animation::NONE;
    anim_frame_ = 0;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(SCALE, SCALE));

    if (!is_mine_) {
        setLeft();
        pos.x = 14;
        pos.y = 0;
    }
}


void SFMLUnitModel::Move(sf::Vector2u pos_) {
    mov_vec = sf::Vector2i{static_cast<int>(pos_.x - pos.x), static_cast<int>(pos_.y - pos.y)};
    float dist = std::sqrt(mov_vec.x * mov_vec.x + mov_vec.y * mov_vec.y);
    mov_anim_speed = 15 * 2 * ANIMATION_FRAME_TIME / dist;
    anim = RUN;
    if (mov_vec.x >= 0)
        setRight();
    else
        setLeft();
}

void SFMLUnitModel::Attack(sf::Vector2u pos_) {
    int dx = pos_.x - pos.x;
    if (dx >= 0)
        setRight();
    else
        setLeft();

    anim = HIT;
    anim_frame_ = 0;
}

void SFMLUnitModel::GetDamage(int damage, sf::Vector2u pos_) {
    int dx = pos_.x - pos.x;
    if (dx >= 0)
        setRight();
    else
        setLeft();

    anim = HURT;
};


SFMLWarriorModel::SFMLWarriorModel(sf::RenderWindow &window,
                                   const sf::Texture &texture, bool is_mine)
    : SFMLUnitModel(window, texture, is_mine) {
    if (is_mine)
        sprite.setTextureRect(
            sf::IntRect(1, 1, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    else {
        sprite.setTextureRect(
            sf::IntRect(1, 65, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    };
}

void SFMLWarriorModel::Draw() {
    switch (anim) {
        case HIT:
            anim_frame_ += ANIMATION_FRAME_TIME;
            if (anim_frame_ > 3) {
                anim = NONE;
                anim_frame_ = 0;
            }
            if (is_mine_)
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, 1,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            else
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, 65,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            break;
        case RUN:
            anim_frame_ += mov_anim_speed;
            if (anim_frame_ > 60) {
                anim = NONE;
                anim_frame_ = 0;
                pos.x += mov_vec.x;
                pos.y += mov_vec.y;
                sprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
            } else {
                float posx =
                    (pos.x + (float)((int)anim_frame_ * mov_vec.x) / 60);
                float posy =
                    (pos.y + (float)((int)anim_frame_ * mov_vec.y) / 60);
                sprite.setPosition(posx * CELL_SIZE, posy * CELL_SIZE);
            }
            break;
        case HURT:
            anim_frame_ += ANIMATION_FRAME_TIME * 20;
            if (anim_frame_ > 60) {
                anim = NONE;
                anim_frame_ = 0;
                sprite.setColor(sf::Color::White);
            } else {
                sprite.setColor(
                    sf::Color((int)anim_frame_ * 200 / 60 + 55, 0, 0));
            }
            break;
        case BLOW:
            anim_frame_ += ANIMATION_FRAME_TIME;
            if (anim_frame_ > 9) {
                anim = NONE;
                anim_frame_ = 0;
                sprite.setTextureRect(
                    sf::IntRect((9 * (CELL_TEXTURE_SIZE + 1)) + 1, 129,
                                CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            } else {
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, 129,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            }
            break;
    }
    target_.draw(sprite);
};

SFMLKingModel::SFMLKingModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLUnitModel(window, texture, is_mine) {
    if (is_mine)
        sprite.setTextureRect(
            sf::IntRect(193, 1, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
    else 
        sprite.setTextureRect(
            sf::IntRect(193, 65, CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
}

void SFMLKingModel::Draw() {
    switch (anim) {
        case HIT:
            break;
        case RUN:
            anim_frame_ += mov_anim_speed;
            if (anim_frame_ > 60) {
                anim = NONE;
                anim_frame_ = 0;
                pos.x += mov_vec.x;
                pos.y += mov_vec.y;
                sprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
            } else {
                float posx =
                    (pos.x + (float)((int)anim_frame_ * mov_vec.x) / 60);
                float posy =
                    (pos.y + (float)((int)anim_frame_ * mov_vec.y) / 60);
                sprite.setPosition(posx * CELL_SIZE, posy * CELL_SIZE);
            }
            break;
        case HURT:
            anim_frame_ += ANIMATION_FRAME_TIME * 20;
            if (anim_frame_ > 60) {
                anim = NONE;
                anim_frame_ = 0;
                sprite.setColor(sf::Color::White);
            } else {
                sprite.setColor(
                    sf::Color((int)anim_frame_ * 200 / 60 + 55, 0, 0));
            }
            break;
        case BLOW:
            anim_frame_ += ANIMATION_FRAME_TIME;
            if (anim_frame_ > 9) {
                anim = NONE;
                anim_frame_ = 0;
                sprite.setTextureRect(
                    sf::IntRect((9 * (CELL_TEXTURE_SIZE + 1)) + 1, 129,
                                CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            } else {
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, 129,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            }
            break;
    }
    target_.draw(sprite);
};


SFMLEnvModel::SFMLEnvModel(sf::RenderWindow &window, const sf::Texture &texture) : SFMLModel(window)
{
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(SCALE, SCALE));
}

SFMLTreesModel::SFMLTreesModel(sf::RenderWindow &window, const sf::Texture &texture)
    : SFMLEnvModel(window, texture) {
    sprite.setOrigin(230, 50);

    sprite.setTextureRect(sf::IntRect(1, 257, 687, 193));
}

SFMLCampModel::SFMLCampModel(sf::RenderWindow &window, const sf::Texture &texture)
    : SFMLEnvModel(window, texture) {
    sprite.setOrigin(150, 122);
    sprite.scale(sf::Vector2f(0.75, 0.75));

    sprite.setTextureRect(sf::IntRect(647, 4, 301, 244));
}