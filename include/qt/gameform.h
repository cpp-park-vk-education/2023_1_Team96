#ifndef GAMEFORM_H
#define GAMEFORM_H

#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QTime>

namespace Ui {
class GameForm;
}

class GameForm : public QWidget
{
    Q_OBJECT

public:
    explicit GameForm(QWidget *parent = nullptr);
    ~GameForm();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onStartTimerClick();
    void onTimerTimeout();
    void stopTimer();

public slots:
    void finish();

signals:
    void CloseWindow();

private:
    Ui::GameForm *ui;
    QTimer *timer;
};

#endif // GAMEFORM_H
