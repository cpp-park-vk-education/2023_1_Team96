#include "qt/mainwindow.h"
#include "ui_mainwindow.h"
#include "qt/entranceform.h"
#include "qt/chooseopponentform.h"
#include "qt/gameform.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client("127.0.0.1", "1234")
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
    this->resize(1400,800);

    connect(entrance, SIGNAL(WantToPlay()), this, SLOT(setupSecondWidget()));
    connect(chooseopponent, SIGNAL(WantToPlay()), this, SLOT(setupThirdWidget()));
    connect(game, SIGNAL(CloseWindow()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSecondWidget(){
    client.connect();
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::setupFirstWidget(){
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::setupThirdWidget(){
    stackedWidget->setCurrentIndex(2);
    //this->setVisible(false);
}


