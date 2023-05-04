#include "qt/chooseopponentform.h"
#include "ui_chooseopponentform.h"


ChooseOpponentForm::ChooseOpponentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseOpponentForm)
{
    QPalette Pal(palette());
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/enterfon.png") );
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); 
    p.setBrush(QPalette::Background, bkgnd);
    this->setAutoFillBackground(true);
    this->setPalette(p);

    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(nextwidget()));
}

ChooseOpponentForm::~ChooseOpponentForm()
{
    delete ui;
}

void ChooseOpponentForm::nextwidget(){
    emit WantToPlay();
}

void ChooseOpponentForm::resizeEvent(QResizeEvent *evt)
{
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/enterfon.png") );
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); 
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QWidget::resizeEvent(evt); 
}
