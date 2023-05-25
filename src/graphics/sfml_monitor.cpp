#include "graphics/sfml_monitor.hpp"

#include <iostream>

const char FIELD_BORDER = '0';
const char BROKEN_CELL = 'h';
const char CELL = ' ';

const string IMG_PATH = "../static/";

const unsigned int CELL_SIZE = 63;

const double ANIMATION_FRAME_TIME = 0.02;

SFMLWindow::SFMLWindow(const string &l_title, const sf::Vector2u &l_size) {
    m_windowTitle_ = l_title;
    m_windowSize_ = l_size;
    m_window_.create(sf::VideoMode(m_windowSize_.x, m_windowSize_.y),
                     m_windowTitle_);
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
    }
    return nullptr;
}

std::unique_ptr<SFMLFieldModel> SFMLWindow::GetFieldModel() {
    return std::make_unique<SFMLFieldModel>(m_window_, tile_set_);
};

SFMLFieldModel::SFMLFieldModel(sf::RenderWindow &window,
                               const sf::Texture &texture)
    : SFMLModel(window) {
    s_map_.setTexture(texture);

    tile_map_ = {
        "000000000000000", "0          h  0", "0  h          0",
        "0             0", "0        h    0", "0             0",
        "0             0", "0    h        0", "000000000000000",
    };

    unsigned int w = tile_map_[0].length();
    unsigned int h = tile_map_.size();
    size_ = Vector2u{w, h};
    ResetCurrent();
};

bool SFMLWindow::loadResources() {
    sf::Image unit_image;
    if (!unit_image.loadFromFile(IMG_PATH + "unit.png")) return false;
    if (!units_set_.loadFromImage(unit_image)) return false;

    sf::Image map_image;
    if (!map_image.loadFromFile(IMG_PATH + "map.png")) return false;
    if (!tile_set_.loadFromImage(map_image)) return false;
    return true;
};

void SFMLFieldModel::Draw() {
    for (int i = 0; i < size_.y; ++i) {
        for (int j = 0; j < size_.x; ++j) {
            char tile = tile_map_[i][j];

            switch (tile) {
                case CELL:
                    s_map_.setTextureRect(
                        sf::IntRect(26, 219, CELL_SIZE, CELL_SIZE));
                    break;

                case FIELD_BORDER:
                    s_map_.setTextureRect(
                        sf::IntRect(26, 298, CELL_SIZE, CELL_SIZE));
                    break;

                case BROKEN_CELL:
                    s_map_.setTextureRect(
                        sf::IntRect(106, 219, CELL_SIZE, CELL_SIZE));
                    break;

                default:
                    s_map_.setTextureRect(
                        sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));
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

SFMLUnitModel::SFMLUnitModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window),
      is_attack_(false),
      attack_frame_(0),
      is_mine_(is_mine) {
    sprite_.setTexture(texture);

    if (is_mine_)
        sprite_.setTextureRect(sf::IntRect(1, 1, CELL_SIZE, CELL_SIZE));
    else
        sprite_.setTextureRect(sf::IntRect(1, 65, CELL_SIZE, CELL_SIZE));
}

void SFMLUnitModel::Draw() {
    if (is_attack_) {
        attack_frame_ += ANIMATION_FRAME_TIME;
        if (attack_frame_ > 3) {
            is_attack_ = false;
            attack_frame_ = 0;
        }
        if (is_mine_)
            sprite_.setTextureRect(sf::IntRect(((int)attack_frame_ * 64) + 1,
                                               1, CELL_SIZE, CELL_SIZE));
        else
            sprite_.setTextureRect(sf::IntRect(((int)attack_frame_ * 64) + 1,
                                               64, CELL_SIZE, CELL_SIZE));
    }
    target_.draw(sprite_);
};

void SFMLUnitModel::Move(sf::Vector2u pos) {
    sprite_.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
}

void SFMLUnitModel::Attack() {
    is_attack_ = true;
    attack_frame_ = 0;
}

SFMLKingModel::SFMLKingModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window),
      is_attack_(false),
      attack_frame_(1),
      is_mine_(is_mine) {
    sprite_.setTexture(texture);

    if (is_mine_)
        sprite_.setTextureRect(sf::IntRect(193, 1, CELL_SIZE, CELL_SIZE));
    else
        sprite_.setTextureRect(sf::IntRect(193, 65, CELL_SIZE, CELL_SIZE));
}

void SFMLKingModel::Draw() {
    target_.draw(sprite_);
};

void SFMLKingModel::Move(sf::Vector2u pos) {
    sprite_.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
}

void SFMLKingModel::Attack() {
    is_attack_ = true;
    attack_frame_ = 0;
}