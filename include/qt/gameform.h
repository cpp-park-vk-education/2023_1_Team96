#ifndef GAMEFORM_H
#define GAMEFORM_H

#include "model/gamemodel.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"

#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QTime>
#include <SFML/Graphics.hpp>
#include <string>


namespace Ui {
class GameForm;
}

class GameForm : public QWidget
{
    Q_OBJECT

public:
    explicit GameForm(QWidget *parent = nullptr);
    ~GameForm();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onStartTimerClick();
    void onTimerTimeout();
    void stopTimer();
    //void on_widget_clicked();

public slots:
    void finish();

signals:
    void CloseWindow();

private:
    Ui::GameForm *ui;
    QTimer *timer;
};

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
public :

    QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

    virtual ~QSFMLCanvas(){};

    QWidget* parentwidget;

private :

    virtual void OnInit(){};

    virtual void OnUpdate(){};

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);

    QTimer myTimer;
    bool   myInitialized;
    
};

class MyCanvas : public QSFMLCanvas
{
public :

    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size) :
    QSFMLCanvas(Parent, Position, Size)
    {
        
    }

private :

    void OnInit()
    {
        sf::Texture texture;
        // Load the image
        texture.loadFromFile("../static/gamefon.jpg");

        // std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{1000,600},(sf::WindowHandle)winId());
        // std::unique_ptr<SFMLWindowHandler> handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());
        // Game game(std::move(monitor), std::move(handler));
        // game.StartGame();

        

        // Setup the sprite
        mySprite.setTexture(texture);
        mySprite.setPosition(0,0);

        // std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{1000,600},parentwidget->winId());
        //         // qDebug() << "winId " << (sf::WindowHandle)winId();
        //         // qDebug() << parentwidget->winId();
        // std::unique_ptr<SFMLWindowHandler> handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());
        // Game game(std::move(monitor), std::move(handler));
        //game.StartGame();
        
    }

    void OnUpdate()
    {
        // Clear screen
        clear(sf::Color(0, 0, 128));
        
        //sf::Clock clock;
        // Rotate the sprite
        //mySprite.rotate(GetFrameTime() * 100.f);
        //clock.Restart();

        
        // Draw it
        draw(mySprite);
    
    }

    sf::Image  myImage;
    sf::Sprite mySprite;
};

#endif // GAMEFORM_H
