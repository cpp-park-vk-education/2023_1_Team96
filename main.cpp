#include "model/gamestate.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"
#include "mainwindow.h"
#include "qt/gameform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{1000,600});

    //std::unique_ptr<SFMLWindowHandler> handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());

    //Game game(std::move(monitor), std::move(handler));

    // game.StartGame();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Create the main frame
    // QFrame* MainFrame = new QFrame;
    // MainFrame->setWindowTitle("Qt SFML");
    // MainFrame->resize(400, 400);
    // MainFrame->show();

    // Create a SFML view inside the main frame
    // MyCanvas* SFMLWindow = new MyCanvas(MainFrame, QPoint(20, 20), QSize(360, 360));
    // SFMLWindow->show();

    return a.exec();


    //return App.exec();

    return 0;
}