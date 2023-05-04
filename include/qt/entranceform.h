#ifndef ENTRANCEFORM_H
#define ENTRANCEFORM_H

#include <QWidget>

namespace Ui {
class EntranceForm;
}

class EntranceForm : public QWidget
{
    Q_OBJECT

public:
    explicit EntranceForm(QWidget *parent = nullptr);
    ~EntranceForm();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void nextwidget();

signals:
    void WantToPlay();


private:
    Ui::EntranceForm *ui;
};

#endif // ENTRANCEFORM_H
