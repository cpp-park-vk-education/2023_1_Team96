#include "graphics/sfml_monitor.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char FIELD_BORDER = '0';
const char CELL = ' ';
const char B_CELL = 'b';

double ANIMATION_FRAME_TIME;
const int SHORT_ANIMATION = 3;
const int NORMAL_ANIMATION = 9;
const int LONG_ANIMATION = 60;
const int HURT_ANIMATION_FACTOR = 20;
const int COLOR_FACTOR = 200;
const int COLOR_OFFSET = 50;
const int MY_UNITS_LINE = 1;
const int ENEMY_UNITS_LINE = 65;
const int BLOW_LINE = 129;

sf::IntRect INFO_BAR;
sf::IntRect BASIC_CELL;
sf::IntRect BORDER_CELL;
sf::IntRect BROKEN_CELL;
sf::IntRect MY_WARRIOR;
sf::IntRect ENEMY_WARRIOR;
sf::IntRect MY_KING;
sf::IntRect ENEMY_KING;

SFMLWindow::SFMLWindow(const string &l_title, const sf::Vector2u &l_size,
                       sf::WindowHandle win_descriptor,
                       const std::string &config_path) {
    loadResources(config_path);
    m_windowTitle_ = l_title;
    m_windowSize_ = l_size;
    m_window_.create(win_descriptor);
}

bool SFMLWindow::loadResources(const std::string &config_path) {
    std::ifstream config_fs(config_path);
    json config = json::parse(config_fs);
    config_fs.close();

    auto get_rect = [](const json& data, sf::IntRect& rect) {
        rect.left = data[0];
        rect.top = data[1];
        rect.width = CELL_TEXTURE_SIZE;
        rect.height = CELL_TEXTURE_SIZE;
    };

    std::string static_dir, units, map, font;
    config.at("static_dir").get_to(static_dir);
    config.at("units").get_to(units);
    config.at("map").get_to(map);
    config.at("font").get_to(font);
    config.at("animation_time").get_to(ANIMATION_FRAME_TIME);
    INFO_BAR.left = config.at("info_bar")[0];
    INFO_BAR.top = config.at("info_bar")[1];
    INFO_BAR.width = config.at("info_bar")[2];
    INFO_BAR.height = config.at("info_bar")[3];
    get_rect(config.at("basic_cell"), BASIC_CELL);
    get_rect(config.at("border_cell"), BORDER_CELL);
    get_rect(config.at("broken_cell"), BROKEN_CELL);
    get_rect(config.at("my_warrior"), MY_WARRIOR);
    get_rect(config.at("enemy_warrior"), ENEMY_WARRIOR);
    get_rect(config.at("my_king"), MY_KING);
    get_rect(config.at("enemy_king"), ENEMY_KING);

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
        {
                std::unique_ptr<SFMLEnvModel> model = std::make_unique<SFMLEnvModel>(m_window_, units_set_);
                model->SetOrigin(230, 50);
                model->SetTextRect(sf::IntRect(1, 257, 687, 193));
            return std::move(model);
        }
        case ModelType::Camp:
        {
                std::unique_ptr<SFMLEnvModel> model = std::make_unique<SFMLEnvModel>(m_window_, units_set_);
                model->SetOrigin(150, 122);
                model->Scale(0.75, 0.75);
                model->SetTextRect(sf::IntRect(647, 4, 301, 244));
            return std::move(model);
        }
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
    if (anim == Animation::Open) {
        anim_frame_ += ANIMATION_FRAME_TIME * 2;
        if (anim_frame_ > 4) {
            anim = Animation::None;
            anim_frame_ = SHORT_ANIMATION;
        }
        sprite.setTextureRect(
            sf::IntRect(((SHORT_ANIMATION - (int)anim_frame_) * 75) + 24, 47, 74, 90));
    }
    target_.draw(sprite);
    if (anim == Animation::None) target_.draw(text);
}

SFMLFieldModel::SFMLFieldModel(sf::RenderWindow &window,
                               const sf::Texture &texture, const sf::Font &font,
                               const std::vector<std::string> &map)
    : SFMLModel(window),
      ibar(target_, font, texture, INFO_BAR) {
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
                    sprite.setTextureRect(BASIC_CELL);
                    break;

                case FIELD_BORDER:
                    sprite.setTextureRect(BORDER_CELL);
                    break;

                case B_CELL:
                    sprite.setTextureRect(BROKEN_CELL);
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
    sf::Vector2f ibar_pos =
        sf::Vector2f{CELL_SIZE * static_cast<float>(current_.x - 0.8),
                     CELL_SIZE * static_cast<float>(current_.y - 1)};
    ibar.setPos(ibar_pos);
    ibar.open();
    is_info = true;
};

SFMLUnitModel::SFMLUnitModel(sf::RenderWindow &window,
                             const sf::Texture &texture, bool is_mine)
    : SFMLModel(window), is_mine_(is_mine), right(true) {
    anim = Animation::None;
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
    mov_vec = sf::Vector2i{static_cast<int>(pos_.x - pos.x),
                           static_cast<int>(pos_.y - pos.y)};
    float dist = std::sqrt(mov_vec.x * mov_vec.x + mov_vec.y * mov_vec.y);
    mov_anim_speed = 15 * 2 * ANIMATION_FRAME_TIME / dist;
    anim = Animation::Run;
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

    anim = Animation::Hit;
    anim_frame_ = 0;
}

void SFMLUnitModel::GetDamage(int damage, sf::Vector2u pos_) {
    int dx = pos_.x - pos.x;
    if (dx >= 0)
        setRight();
    else
        setLeft();

    anim = Animation::Hurt;
};

SFMLWarriorModel::SFMLWarriorModel(sf::RenderWindow &window,
                                   const sf::Texture &texture, bool is_mine)
    : SFMLUnitModel(window, texture, is_mine) {
    if (is_mine)
        sprite.setTextureRect(MY_WARRIOR);
    else {
        sprite.setTextureRect(ENEMY_WARRIOR);
    };
}

void SFMLWarriorModel::Draw() {
    switch (anim) {
        case Animation::Hit:
            anim_frame_ += ANIMATION_FRAME_TIME;
            if (anim_frame_ > SHORT_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
            }
            if (is_mine_)
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, MY_UNITS_LINE,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            else
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, ENEMY_UNITS_LINE,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            break;
        case Animation::Run:
            anim_frame_ += mov_anim_speed;
            if (anim_frame_ > LONG_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
                pos.x += mov_vec.x;
                pos.y += mov_vec.y;
                sprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
            } else {
                float posx =
                    (pos.x + (float)((int)anim_frame_ * mov_vec.x) / LONG_ANIMATION);
                float posy =
                    (pos.y + (float)((int)anim_frame_ * mov_vec.y) / LONG_ANIMATION);
                sprite.setPosition(posx * CELL_SIZE, posy * CELL_SIZE);
            }
            break;
        case Animation::Hurt:
            anim_frame_ += ANIMATION_FRAME_TIME * HURT_ANIMATION_FACTOR;
            if (anim_frame_ > LONG_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
                sprite.setColor(sf::Color::White);
            } else {
                sprite.setColor(
                    sf::Color((int)anim_frame_ *  COLOR_FACTOR / LONG_ANIMATION + COLOR_OFFSET, 0, 0));
            }
            break;
        case Animation::Blow:
            anim_frame_ += ANIMATION_FRAME_TIME;
            if (anim_frame_ > NORMAL_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
                sprite.setTextureRect(
                    sf::IntRect((NORMAL_ANIMATION * (CELL_TEXTURE_SIZE + 1)) + 1, BLOW_LINE,
                                CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            } else {
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, BLOW_LINE,
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
        sprite.setTextureRect(MY_KING);
    else
        sprite.setTextureRect(ENEMY_KING);
}

void SFMLKingModel::Draw() {
    switch (anim) {
        case Animation::Hit:
            break;
        case Animation::Run:
            anim_frame_ += mov_anim_speed;
            if (anim_frame_ > LONG_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
                pos.x += mov_vec.x;
                pos.y += mov_vec.y;
                sprite.setPosition(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
            } else {
                float posx =
                    (pos.x + (float)((int)anim_frame_ * mov_vec.x) / LONG_ANIMATION);
                float posy =
                    (pos.y + (float)((int)anim_frame_ * mov_vec.y) / LONG_ANIMATION);
                sprite.setPosition(posx * CELL_SIZE, posy * CELL_SIZE);
            }
            break;
        case Animation::Hurt:
            anim_frame_ += ANIMATION_FRAME_TIME * HURT_ANIMATION_FACTOR;
            if (anim_frame_ > LONG_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
                sprite.setColor(sf::Color::White);
            } else {
                sprite.setColor(
                    sf::Color((int)anim_frame_ *  COLOR_FACTOR / LONG_ANIMATION + COLOR_OFFSET, 0, 0));
            }
            break;
        case Animation::Blow:
            anim_frame_ += ANIMATION_FRAME_TIME;
            if (anim_frame_ > NORMAL_ANIMATION) {
                anim = Animation::None;
                anim_frame_ = 0;
                sprite.setTextureRect(
                    sf::IntRect((NORMAL_ANIMATION * (CELL_TEXTURE_SIZE + 1)) + 1, BLOW_LINE,
                                CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            } else {
                sprite.setTextureRect(sf::IntRect(
                    ((int)anim_frame_ * (CELL_TEXTURE_SIZE + 1)) + 1, BLOW_LINE,
                    CELL_TEXTURE_SIZE, CELL_TEXTURE_SIZE));
            }
            break;
    }
    target_.draw(sprite);
};

SFMLEnvModel::SFMLEnvModel(sf::RenderWindow &window, const sf::Texture &texture)
    : SFMLModel(window) {
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(SCALE, SCALE));
}
