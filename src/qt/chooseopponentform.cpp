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

    QRect parentgeometry = this->geometry();
    ui->pushButton->setGeometry((parentgeometry.width() - parentgeometry.width() / 10) / 2, parentgeometry.height() * 3 / 6, 
                                    parentgeometry.width() / 10, parentgeometry.height() / 20);
    ui->label->setGeometry((parentgeometry.width() - ui->label->geometry().width()) / 2, parentgeometry.height() * 2 / 6,
                                    ui->label->geometry().width(), ui->label->geometry().height());

    QWidget::resizeEvent(evt); 
}
