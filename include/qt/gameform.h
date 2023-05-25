#ifndef GAMEFORM_H
#define GAMEFORM_H

#include <QFont>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <SFML/Graphics.hpp>
#include <string>

#include "graphics/sfml_monitor.hpp"
#include "input/input_handler.hpp"
#include "input/sfml_input.hpp"
#include "model/gamestate.hpp"

class QSFMLCanvas : public QWidget, public sf::RenderWindow {
   private:
    QTimer myTimer;
    bool myInitialized;
    std::vector<GameEvent> GameEvents;

    virtual void OnInit(){};

    virtual void OnUpdate(){};
    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);

    void mousePressEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);

   protected:
    void pushEvent(GameEvent& ev);

   public:
    QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size,
                unsigned int FrameTime = 0);

    virtual ~QSFMLCanvas(){};

    QWidget* parentwidget;

    bool pollEvent(GameEvent& ev);
};

class GameCanvas : public QSFMLCanvas {
   public:
    Game* mygame;

    GameCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size)
        : QSFMLCanvas(Parent, Position, Size) {}

   private:
    void OnInit() {
        sf::Texture texture;
        // Load the image
        texture.loadFromFile("../static/gamefon.jpg");

        // Setup the sprite
        mySprite.setTexture(texture);
        mySprite.setPosition(0, 0);
    }

    void OnUpdate();

    sf::Image myImage;
    sf::Sprite mySprite;
};

namespace Ui {
class GameForm;
}

class GameForm : public QWidget {
    Q_OBJECT

   public:
    explicit GameForm(QWidget* parent = nullptr);
    ~GameForm();

   protected:
    void resizeEvent(QResizeEvent* event) override;

   private slots:
    void onStartTimerClick();
    void onTimerTimeout();
    void stopTimer();

   public slots:
    void finish();

   signals:
    void CloseWindow();

   private:
    Ui::GameForm* ui;
    QTimer* timer;
    GameCanvas* mywidget;
};

#endif  // GAMEFORM_H
