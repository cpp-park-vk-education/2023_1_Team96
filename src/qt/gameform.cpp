#include "gameform.h"
#include "ui_gameform.h"
#include <QPushButton>

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
}

void GameForm::onTimerTimeout(){
    emit ui->switchButton->clicked();
}

void GameForm::stopTimer(){
    ui->lcd->setVisible(false);
}


