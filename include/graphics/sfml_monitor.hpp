#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "graphics/graphics.hpp"

using std::string;
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

    std::unique_ptr<IModel> getModel(ModelType type) override {}

    sf::RenderWindow& getWindow()
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

class SFMLSprite
{
private:
    sf::Shape* shape;
    sf::Texture* texture;
    sf::Vector2f position;
    sf::Color color;
public:
    void SetPosition(sf::Vector2f pos) {}
};
class SFMLModel : public IModel
{
private:
    sf::RenderWindow& target;
    SFMLSprite sprite;
public:
    void Draw() const override {}
    void Move(sf::Vector2u pos) const override {}
    void Attack(sf::Vector2u pos) const override {}
    void GetDamage(int damage) const override {}
};

class SFMLUnitModel : public SFMLModel
{
private:

};

class SFMLKingModel : public SFMLModel
{
private:
    
};

class SFMLStoneModel : public SFMLModel
{
private:
    
};

