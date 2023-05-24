#include "graphics/sfml_monitor.hpp"

SFMLFieldModel::SFMLFieldModel(sf::RenderWindow &_window, uint cols, uint rows)
    : SFMLModel(_window) {
    sf::Image map_image;
    bool flag = map_image.loadFromFile("../images/map.png");
    tile_set.loadFromImage(map_image);
    s_map.setTexture(tile_set);

    tile_map = {
        "000000000000000", "0             0", "0  h          0",
        "0             0", "0        h    0", "0             0",
        "0             0", "0             0", "000000000000000",
    };

    unsigned int w = tile_map[0].length();
    unsigned int h = tile_map.size();
    size = Vector2u{w, h};
    resetCurrent();
};

void SFMLFieldModel::draw() {
    for (int i = 0; i < size.y; ++i) {
        for (int j = 0; j < size.x; ++j) {
            char tile = tile_map[i][j];

            switch (tile) {
                case ' ':
                    s_map.setTextureRect(sf::IntRect(26, 219, 63, 63));
                    break;

                case '0':
                    s_map.setTextureRect(sf::IntRect(26, 298, 63, 63));
                    break;

                case 'h':
                    s_map.setTextureRect(sf::IntRect(106, 219, 63, 63));
                    break;

                default:
                    s_map.setTextureRect(sf::IntRect(0, 0, 63, 63));
                    break;
            }

            if (i == current.y && j == current.x) {
                if (j < 7)
                    s_map.setColor(sf::Color::Green);
                else
                    s_map.setColor(sf::Color::Red);
            }

            s_map.setPosition(j * 63, i * 63);
            target.draw(s_map);

            s_map.setColor(sf::Color::White);
        }
    }
};

SFMLUnitModel::SFMLUnitModel(sf::RenderWindow &_window, bool isMine) : SFMLModel(_window) {
    sf::Image map_image;
    bool flag = map_image.loadFromFile("../images/unit.png");
    tile_set.loadFromImage(map_image);
    sprite.setTexture(tile_set);

    if (isMine)
    sprite.setTextureRect(sf::IntRect(296, 220, 63, 63));
    else
    sprite.setTextureRect(sf::IntRect(296+63, 220, -63, 63));
    // sprite.setScale(10);
}

void SFMLUnitModel::draw() { target.draw(sprite); };

void SFMLUnitModel::Move(sf::Vector2u pos) {
    sprite.setPosition(pos.x * 63, pos.y * 63);
}

std::unique_ptr<IObjectModel> SFMLWindow::getModel(ModelType type, bool isMine) {
    switch (type) {
        case B_MODEL:

            return std::make_unique<SFMLUnitModel>(m_window, isMine);

            break;
    }
    return nullptr;
}

std::unique_ptr<SFMLFieldModel> SFMLWindow::getFieldModel(uint cols,
                                                          uint rows) {
    return std::make_unique<SFMLFieldModel>(m_window, cols, rows);
};

SFMLWindow::SFMLWindow(const string &l_title, const sf::Vector2u &l_size, sf::WindowHandle winhandle, sf::WindowHandle mainwinhandle) {
    m_windowTitle = l_title;
    m_windowSize = l_size;
    m_window.create(winhandle);                               //sf::VideoMode(m_windowSize.x, m_windowSize.y),m_windowTitle
}