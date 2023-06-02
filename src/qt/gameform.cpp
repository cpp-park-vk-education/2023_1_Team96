#include "qt/gameform.h"

#include <QPushButton>
#include <iostream>

#include "graphics/sfml_monitor.hpp"
#include "input/input_handler.hpp"
#include "input/sfml_input.hpp"
#include "model/gamestate.hpp"
#include "ui_gameform.h"

GameForm::GameForm(QWidget* parent) : QWidget(parent), ui(new Ui::GameForm) {
    QPalette Pal(palette());
    QPixmap bkgnd(QString(QCoreApplication::applicationDirPath() +
                          "/../static/gamefon.jpg"));
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette();
    p.setBrush(QPalette::Background, bkgnd);
    this->setAutoFillBackground(true);
    this->setPalette(p);
    ui->setupUi(this);
    connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(finish()));
    connect(ui->startButton, SIGNAL(clicked()), this,
            SLOT(onStartTimerClick()));
    connect(ui->switchButton, SIGNAL(clicked()), this, SLOT(stopTimer()));

    ui->lcd->setVisible(false);

    ui->widget->resize(945, 567);
    ui->widget->show();
}

GameForm::~GameForm() { delete ui; }

void GameForm::finish() { emit CloseWindow(); }

void GameForm::resizeEvent(QResizeEvent* evt) {
    QPixmap bkgnd(QString(QCoreApplication::applicationDirPath() +
                          "/../static/gamefon2.jpg"));
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette();
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QRect parentgeometry = this->geometry();
    ui->startButton->setGeometry(
        {parentgeometry.width() - ui->startButton->geometry().width() - 25, 25,
         ui->startButton->geometry().width(),
         ui->startButton->geometry().height()});

    ui->switchButton->setGeometry(
        {parentgeometry.width() - ui->switchButton->geometry().width() - 25, 75,
         ui->switchButton->geometry().width(),
         ui->switchButton->geometry().height()});

    ui->lcd->setGeometry(
        {parentgeometry.width() - ui->lcd->geometry().width() - 25, 125,
         ui->lcd->geometry().width(), ui->lcd->geometry().height()});

    ui->finishButton->setGeometry(
        {parentgeometry.width() - ui->finishButton->geometry().width() - 25,
         175, ui->finishButton->geometry().width(),
         ui->finishButton->geometry().height()});

    ui->widget->setGeometry(
        {(parentgeometry.width() - 25 - ui->startButton->geometry().width()- ui->widget->geometry().width()) / 2,
        25, ui->widget->geometry().width(),
        ui->widget->geometry().height()});

    // if ( game_widget != nullptr ){

    //     game_widget->setGeometry(
    //     {(parentgeometry.width() - 25 - ui->startButton->geometry().width()- game_widget->geometry().width()) / 2,
    //     25, game_widget->geometry().width(),
    //     game_widget->geometry().height()});
    // }

    QWidget::resizeEvent(evt);
}

void GameForm::onStartTimerClick() {
    QTimer* timer = new QTimer(this);
    QTimer* updater = new QTimer(this);
    updater->setSingleShot(false);
    connect(updater, &QTimer::timeout, this, [this, timer]() {
        ui->lcd->display(QTime::fromMSecsSinceStartOfDay(timer->remainingTime())
                             .toString("h.mm.ss"));
    });
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &GameForm::onTimerTimeout);
    connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
    connect(timer, &QTimer::timeout, updater, &QObject::deleteLater);
    updater->start(1000);
    timer->start(60000);

    game_widget = new QSFMLCanvas(this, QPoint(20, 20), QSize(1414, 851));
    game_widget->show();

    std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>(
        "Tactics", sf::Vector2u{945, 567}, game_widget->winId(), CONFIG);

    std::unique_ptr<SFMLWindowHandler> handler =
        std::make_unique<SFMLWindowHandler>(monitor->GetWindow());

    game_widget->game = new Game(std::move(monitor), std::move(handler));
    game_widget->game->Render();
}

void GameForm::onTimerTimeout() { emit ui->switchButton->clicked(); }

void GameForm::stopTimer() { ui->lcd->setVisible(false); }
