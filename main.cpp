#include <QApplication>

#include "main.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setLocale(QLocale(QLocale::French, QLocale::France));
    w.showMaximized();

    return a.exec();
}
