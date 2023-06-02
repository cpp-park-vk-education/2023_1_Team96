#include "qt/entranceform.h"
#include "ui_entranceform.h"

#include <QDebug>
#include <QDir>

EntranceForm::EntranceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntranceForm)
{
    QPalette Pal(palette());
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/enterfon.png") );      
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); //copy current, not create new
    p.setBrush(QPalette::Background, bkgnd);
    this->setAutoFillBackground(true);
    this->setPalette(p);

    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(nextwidget()));

}

EntranceForm::~EntranceForm()
{
    delete ui;
}

void EntranceForm::nextwidget(){
    emit WantToPlay();
}

void EntranceForm::resizeEvent(QResizeEvent *evt)
{
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/enterfon.png") );
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); 
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QRect parentgeometry = this->geometry();
    ui->pushButton->setGeometry((parentgeometry.width() - parentgeometry.width() / 10) / 2, parentgeometry.height() * 3 / 6, 
                                    parentgeometry.width() / 10, parentgeometry.height() / 20);
    ui->label->setGeometry((parentgeometry.width() - ui->label->geometry().width()) / 2, parentgeometry.height() * 2 / 6,
                                    ui->label->geometry().width(), ui->label->geometry().height());

    QWidget::resizeEvent(evt); 
}
