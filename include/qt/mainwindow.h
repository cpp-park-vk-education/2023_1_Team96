#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "server/client.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setupSecondWidget();
    void setupFirstWidget();
    void setupThirdWidget();




private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    Client client;
};
#endif // MAINWINDOW_H

