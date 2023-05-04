#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "graphics/graphics.hpp"

using std::string;

class ISfmlModelFactory : public IModelFactory
{
public:
    virtual ~ISfmlModelFactory() {}
};

class SFMLWindow : public IMonitor
{
public:
    SFMLWindow(const string &l_title, const sf::Vector2u &l_size)
    {
        Setup(l_title, l_size);
    };

    ~SFMLWindow()
    {
        Destroy();
    };

    void Prepare()
    {
        m_window.clear();
    };

    void Draw()
    {
        m_window.display();
    };

    bool IsOpen()
    {
        return m_window.isOpen();
    };

    sf::Vector2u GetWindowSize()
    {
        return m_windowSize;
    };

    bool isEnd()
    {
        return !m_window.isOpen();
    };

    std::unique_ptr<IModel> getModel(ModelType type) {};

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
    };

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

class SfmlModel
{
public:
    sf::RenderWindow& target;
};

class SfmlUnitModel
{
private:

};

class SfmlKingModel
{
private:
    
};

class SfmlStoneModel
{
private:
    
};

