#include "entranceform.h"
#include "ui_entranceform.h"

#include <QDebug>
#include <QDir>

EntranceForm::EntranceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntranceForm)
{
    QPalette Pal(palette());
    QPixmap bkgnd( QString(QCoreApplication::applicationDirPath() + "/../static/enterfon.png") );      //"/home/uliana/Pictures/enterfon.png"
    //qDebug() << QString(QDir::currentPath() + "/../enterfon.png");
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); //copy current, not create new
    p.setBrush(QPalette::Background, bkgnd);
    this->setAutoFillBackground(true);
    this->setPalette(p);


    //QFont font = ui->label->font();
    //font.setPointSize(7);
//    ui->label->setFont(font);
    //ui->label->setStyleSheet("QLabel { background-color : white ; color : blue; }");
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
    QPalette p = palette(); //copy current, not create new
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QWidget::resizeEvent(evt); //call base implementation
}
