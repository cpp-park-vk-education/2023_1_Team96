#include "graphics/sfml_monitor.hpp"
#include "input/sfml_input.hpp"
#include "mainwindow.h"
#include "qt/gameform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}