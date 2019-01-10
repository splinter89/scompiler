#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QTime timer;
    timer.start();
    MainWindow w;
    w.show();
    qDebug() << QString("loaded: %1 sec").arg(QString::number(timer.elapsed() / 1000., 'f', 3));

    return a.exec();
}
