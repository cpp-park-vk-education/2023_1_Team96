#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entranceform.h"
#include "chooseopponentform.h"
#include "gameform.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget;
    EntranceForm *entrance = new EntranceForm();
    ChooseOpponentForm *chooseopponent = new ChooseOpponentForm();
    GameForm *game = new GameForm();
    stackedWidget->addWidget(entrance);
    stackedWidget->addWidget(chooseopponent);
    stackedWidget->addWidget(game);
    this->setCentralWidget(stackedWidget);
    this->setWindowTitle("Tactics");
    this->setFixedSize(1000,500);

    connect(entrance, SIGNAL(WantToPlay()), this, SLOT(setupSecondWidget()));
    connect(chooseopponent, SIGNAL(WantToPlay()), this, SLOT(setupThirdWidget()));
    connect(game, SIGNAL(CloseWindow()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSecondWidget(){
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::setupFirstWidget(){
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::setupThirdWidget(){
    stackedWidget->setCurrentIndex(2);
}


