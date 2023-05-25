#include "graphics/sfml_monitor.hpp"

#include <iostream>

const char FIELD_BORDER = '0';
const char BROKEN_CELL = 'h';
const char CELL = ' ';

const string IMG_PATH = "../static/";

const unsigned int CELL_SIZE = 63;

const double ANIMATION_FRAME_TIME = 0.02;

SFMLFieldModel::SFMLFieldModel(sf::RenderWindow &window) : SFMLModel(window) {
    sf::Image map_image;
    map_image.loadFromFile(IMG_PATH + "map.png");
    tile_set_.loadFromImage(map_image);
    s_map_.setTexture(tile_set_);

    tile_map_ = {
        "000000000000000",
        "0          h  0",
        "0  h          0",
        "0             0",
        "0        h    0",
        "0             0",
        "0             0",
        "0    h        0",
        "000000000000000",
    };

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

SFMLUnitModel::SFMLUnitModel(sf::RenderWindow &window, bool is_mine)
    : SFMLModel(window),
      is_attack_(false),
      attack_frame_(0),
      is_mine_(is_mine) {
    sf::Image unit_image;
    unit_image.loadFromFile(IMG_PATH + "unit.png");
    tile_set_.loadFromImage(unit_image);
    sprite_.setTexture(tile_set_);

    if (is_mine_)
        sprite_.setTextureRect(sf::IntRect(13, 6, CELL_SIZE, CELL_SIZE));
    else
        sprite_.setTextureRect(sf::IntRect(13, 72, CELL_SIZE, CELL_SIZE));
}

void SFMLUnitModel::Draw() {
    if (is_attack_) {
        attack_frame_ += ANIMATION_FRAME_TIME;
        if (attack_frame_ > 3) {
            is_attack_ = false;
            attack_frame_ = 0;
        }
        if (is_mine_)
            sprite_.setTextureRect(sf::IntRect(((int)attack_frame_ * 63) + 13,
                                               6, CELL_SIZE, CELL_SIZE));
        else
            sprite_.setTextureRect(sf::IntRect(((int)attack_frame_ * 63) + 13,
                                               72, CELL_SIZE, CELL_SIZE));
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

std::unique_ptr<IObjectModel> SFMLWindow::GetModel(ModelType type,
                                                   bool is_mine) {
    switch (type) {
        case B_MODEL:
            return std::make_unique<SFMLUnitModel>(m_window_, is_mine);
    }
    return nullptr;
}

std::unique_ptr<SFMLFieldModel> SFMLWindow::GetFieldModel() {
    return std::make_unique<SFMLFieldModel>(m_window_);
};

SFMLWindow::SFMLWindow(const string &l_title, const sf::Vector2u &l_size,
                       sf::WindowHandle winhandle) {
    m_windowTitle_ = l_title;
    m_windowSize_ = l_size;
    m_window_.create(winhandle);
}
