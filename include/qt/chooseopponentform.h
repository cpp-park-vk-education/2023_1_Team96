#ifndef CHOOSEOPPONENTFORM_H
#define CHOOSEOPPONENTFORM_H

#include <QWidget>

namespace Ui {
class ChooseOpponentForm;
}

class ChooseOpponentForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseOpponentForm(QWidget *parent = nullptr);
    ~ChooseOpponentForm();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void nextwidget();

signals:
    void WantToPlay();


private:
    Ui::ChooseOpponentForm *ui;
};

#endif // CHOOSEOPPONENTFORM_H
