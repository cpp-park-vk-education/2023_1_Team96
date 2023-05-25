#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QWidget>
#include <SFML/Graphics.hpp>

#include "graphics/sfml_monitor.hpp"
#include "input/input_handler.hpp"
#include "input/sfml_input.hpp"
#include "model/gamestate.hpp"

class QSFMLCanvas : public QWidget, public sf::RenderWindow {
   private:
    QTimer myTimer;
    bool myInitialized;
    std::vector<GameEvent> GameEvents;

    sf::Image myImage;
    sf::Sprite mySprite;

    virtual void OnInit();
    virtual void OnUpdate();

    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);

    void mousePressEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);

   protected:
    void pushEvent(GameEvent& ev);

   public:
    QWidget* parentwidget;
    Game* game;

    QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size,
                unsigned int FrameTime = 0);

    bool pollEvent(GameEvent& ev);
    virtual ~QSFMLCanvas();
};

#endif  // QSFMLCANVAS_H
