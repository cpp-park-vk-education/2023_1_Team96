#include "qt/gameform.h"
#include "ui_gameform.h"
#include <QPushButton>
#include <QDebug>

#include "model/gamemodel.hpp"
#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

GameForm::GameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameForm)
{
    QPalette Pal(palette());
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/gamefon.jpg") );
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); 
    p.setBrush(QPalette::Background, bkgnd);
    this->setAutoFillBackground(true);
    this->setPalette(p);
    ui->setupUi(this);
    connect(ui->finishButton, SIGNAL(clicked()), this, SLOT(finish()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStartTimerClick()));
    connect(ui->switchButton, SIGNAL(clicked()), this, SLOT(stopTimer()));

    ui->widget->resize(1000,600);
    ui->widget->show();

    
    //this->setVisible(false);

    //SFMLWindow->show();
}

GameForm::~GameForm()
{
    delete ui;
}

void GameForm::finish()
{
    emit CloseWindow();
}



void GameForm::resizeEvent(QResizeEvent *evt)
{
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/gamefon.jpg") );
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); 
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QRect parentgeometry = this->geometry();
    ui->startButton->setGeometry({parentgeometry.width() - ui->startButton->geometry().width() - 25,
                                   25,
                                   ui->startButton->geometry().width(),
                                   ui->startButton->geometry().height()});
    
    ui->switchButton->setGeometry({parentgeometry.width() - ui->switchButton->geometry().width() - 25,
                                   75,
                                   ui->switchButton->geometry().width(),
                                   ui->switchButton->geometry().height()});

    ui->lcd->setGeometry({parentgeometry.width() - ui->lcd->geometry().width() - 25,
                                   125,
                                   ui->lcd->geometry().width(),
                                   ui->lcd->geometry().height()});

    ui->finishButton->setGeometry({parentgeometry.width() - ui->finishButton->geometry().width() - 25,
                                   175,
                                   ui->finishButton->geometry().width(),
                                   ui->finishButton->geometry().height()});

    

    QWidget::resizeEvent(evt); 
}


void GameForm::onStartTimerClick() {
  QTimer * timer = new QTimer(this);
  QTimer * updater = new QTimer(this);
  updater->setSingleShot(false);
  connect(updater, &QTimer::timeout, this, [this, timer](){
      ui->lcd->display(QTime::fromMSecsSinceStartOfDay(timer->remainingTime()).toString("h.mm.ss"));

  });
  timer->setSingleShot(true);
  connect(timer, &QTimer::timeout, this, &GameForm::onTimerTimeout);
  connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
  connect(timer, &QTimer::timeout, updater, &QObject::deleteLater);
  updater->start(1000);
  timer->start(60000);

//   std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{700,700},(sf::WindowHandle)winId());
//   std::unique_ptr<SFMLWindowHandler> handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());
//   Game game(std::move(monitor), std::move(handler));
//   game.StartGame();

    MyCanvas* SFMLWindow = new MyCanvas(ui->widget, QPoint(20, 20), QSize(1000, 360));
    SFMLWindow->show();
    
}

void GameForm::onTimerTimeout(){
    emit ui->switchButton->clicked();
}

void GameForm::stopTimer(){
    ui->lcd->setVisible(false);
}

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) :
QWidget       (Parent),
myInitialized (false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the widget geometry
    move(Position);
    resize(Size);

    // Setup the timer
    myTimer.setInterval(FrameTime);

    parentwidget = Parent;
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        std::unique_ptr<SFMLWindow> monitor = std::make_unique<SFMLWindow>("Tactics", sf::Vector2u{1000,600},parentwidget->winId(), 9999292);
                // qDebug() << "winId " << (sf::WindowHandle)winId();
                // qDebug() << parentwidget->winId();
        std::unique_ptr<SFMLWindowHandler> handler = std::make_unique<SFMLWindowHandler>(monitor->getWindow());
        Game game(std::move(monitor), std::move(handler));
        game.StartGame();
        // Create the SFML window with the widget handle
        //sf::RenderWindow::create((sf::WindowHandle)winId());

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
    OnUpdate();

    // Display on screen
    display();
}


