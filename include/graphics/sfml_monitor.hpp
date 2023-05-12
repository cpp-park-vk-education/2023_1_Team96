#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "graphics/graphics.hpp"

using std::string;
using sf::Vector2u;

class SFMLFieldModel : public IFieldModel
{
private:
    sf::RenderWindow &window;
    vector<string> tile_map;
    sf::Vector2u size;
    sf::Sprite s_map;
    sf::Texture tile_set;
    sf::Vector2i current;

public:
    SFMLFieldModel(sf::RenderWindow &_window) : window(_window)
    {

        sf::Image map_image;
        bool flag = map_image.loadFromFile("../images/map.png");
        tile_set.loadFromImage(map_image);
        s_map.setTexture(tile_set);

        tile_map = {
            "000000000000000",
            "0             0",
            "0             0",
            "0             0",
            "0             0",
            "0             0",
            "0             0",
            "0             0",
            "000000000000000",
            };

        size = Vector2u{tile_map[0].length(), tile_map.size()};
        resetCurrent();
    }

    void render()
    {
        for (int i = 0; i < size.y; ++i)
        {
            for (int j = 0; j < size.x; ++j)
            {
                char tile = tile_map[i][j];

                switch (tile)
                {
                case ' ':
                    s_map.setTextureRect(sf::IntRect(26, 218, 63, 63));
                    break;

                case '0':
                    s_map.setTextureRect(sf::IntRect(26, 298, 63, 63));
                    break;

                default:
                    s_map.setTextureRect(sf::IntRect(0, 0, 63, 63));
                    break;
                }

                if (i == current.y && j == current.x)
                {
                    if (j < 7) 
                    s_map.setColor(sf::Color::Green);
                    else 
                    s_map.setColor(sf::Color::Red);
                }

                s_map.setPosition(j*63, i*63);
                window.draw(s_map);

                s_map.setColor(sf::Color::White);

            }
        }
    };

    void setCurrent(sf::Vector2i pos) override
    {
        current = pos;
    };

    void resetCurrent() override
    {
        current.x = -1;
        current.y = -1;
    }

};

class SFMLModel : public IModel
{
protected:
    sf::RenderWindow &target;

public:
    SFMLModel(sf::RenderWindow &_target) : target(_target){}
    void Draw() const override {}
    void Move(sf::Vector2u pos) override {}
    void Attack(sf::Vector2u pos) const override {}
    void GetDamage(int damage) const override {}
};

class SFMLUnitModel : public SFMLModel
{
private:
    sf::Sprite sprite;
    sf::Texture tile_set;

public:
    SFMLUnitModel(sf::RenderWindow &_window) : SFMLModel(_window)
    {
        sf::Image map_image;
        bool flag = map_image.loadFromFile("../images/unit.png");
        tile_set.loadFromImage(map_image);
        sprite.setTexture(tile_set);

        sprite.setTextureRect(sf::IntRect(296,220,63,63));
        // sprite.setScale(10);
    }

    void Draw() const override
    {
        std::cout << "draw!" << std::endl;
        target.draw(sprite);
    };

    void Move(sf::Vector2u pos) override 
    {
        sprite.setPosition(pos.x*63, pos.y*63);
    }


};

class SFMLWindow : public IMonitor
{
public:
    SFMLWindow(const string &l_title, const sf::Vector2u &l_size)
    {
        Setup(l_title, l_size);
    }

    ~SFMLWindow()
    {
        Destroy();
    }

    void Prepare() override
    {
        m_window.clear();
    }

    void Draw() override
    {
        m_window.display();
    }

    bool IsOpen()
    {
        return m_window.isOpen();
    }

    sf::Vector2u GetWindowSize()
    {
        return m_windowSize;
    }

    bool isEnd() override
    {
        return !m_window.isOpen();
    }

    std::unique_ptr<IModel> getModel(ModelType type) override 
    {
        switch (type)
        {
        case B_MODEL:
            
            return std::make_unique<SFMLUnitModel>(m_window);

            break;
        }
    }

    std::unique_ptr<IFieldModel> getFieldModel(sf::Vector2u size)
    {
        return std::make_unique<SFMLFieldModel>(m_window);
    };

    sf::RenderWindow &getWindow()
    {
        return m_window;
    }

private:
    void Setup(const std::string &l_title,
               const sf::Vector2u &l_size)
    {
        m_windowTitle = l_title;
        m_windowSize = l_size;
        Create();
    }

    void Create()
    {
        m_window.create(sf::VideoMode(m_windowSize.x, m_windowSize.y), m_windowTitle);
    }

    void Destroy()
    {
        m_window.close();
    }

    sf::RenderWindow m_window;
    sf::Vector2u m_windowSize;
    std::string m_windowTitle;
    bool m_isFullscreen;
};

class SFMLKingModel : public SFMLModel
{
private:
};

class SFMLStoneModel : public SFMLModel
{
private:
};
