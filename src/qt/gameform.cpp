#include "qt/gameform.h"

#include <QDebug>
#include <QMouseEvent>
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

    mywidget = new MyCanvas(this, QPoint(20, 20), QSize(945, 567));
    mywidget->show();

    ui->widget->resize(1000, 600);
    ui->widget->show();
}

GameForm::~GameForm() { delete ui; }

void GameForm::finish() { emit CloseWindow(); }

void GameForm::resizeEvent(QResizeEvent* evt) {
    QPixmap bkgnd(QString(QCoreApplication::applicationDirPath() +
                          "/../static/gamefon.jpg"));
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

    std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>(
        "Tactics", sf::Vector2u{945, 567}, mywidget->winId(), 9999292);

    std::unique_ptr<SFMLWindowHandler> handler =
        std::make_unique<SFMLWindowHandler>(monitor->GetWindow());

    mywidget->mygame = new Game(std::move(monitor), std::move(handler));
    mywidget->mygame->Render();
}

void GameForm::onTimerTimeout() { emit ui->switchButton->clicked(); }

void GameForm::stopTimer() { ui->lcd->setVisible(false); }

void QSFMLCanvas::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() == Qt::LeftButton) {
        GameEvent ge{EventType::CHOSE};
        ge.cords = sf::Vector2u{e->localPos().x() / 63, e->localPos().y() / 63};
        pushEvent(ge);
    }
}

void QSFMLCanvas::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Backspace) {
        GameEvent ge{EventType::UNCHOSE};
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_1) {
        GameEvent ge{EventType::CREATE_OBJECT};
        ge.unit_type = B;
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_Return) {
        GameEvent ge{EventType::FINISH};
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_Space) {
        GameEvent ge{EventType::FINISH};
        ge.cmds = "c b 2 3 c b 2 4 c b 2 5 e";
        pushEvent(ge);
    }

    if (e->key() == Qt::Key_Shift) {
        GameEvent ge{EventType::FINISH};
        ge.cmds = "a 2 4 3 4 a 2 4 3 4 a 2 4 3 4 a 2 4 3 4 a 2 4 3 4 e";
        pushEvent(ge);
    }
}

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position,
                         const QSize& Size, unsigned int FrameTime)
    : QWidget(Parent), myInitialized(false) {
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);

    move(Position);
    resize(Size);

    myTimer.setInterval(FrameTime);

    parentwidget = Parent;
}

void QSFMLCanvas::showEvent(QShowEvent*) {
    if (!myInitialized) {
        OnInit();

        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const { return 0; }

void QSFMLCanvas::paintEvent(QPaintEvent*) {
    OnUpdate();
    display();
}

void QSFMLCanvas::pushEvent(GameEvent& ev) { GameEvents.push_back(ev); }

bool QSFMLCanvas::pollEvent(GameEvent& ev) {
    if (GameEvents.size() == 0) return false;

    ev = GameEvents.back();
    GameEvents.pop_back();

    return true;
}

void MyCanvas::OnUpdate() {
    GameEvent event{EventType::FINISH};
    if (pollEvent(event)) mygame->HandleInput(event);
    mygame->Render();
}
