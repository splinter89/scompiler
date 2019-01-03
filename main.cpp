#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTime loadingTimer;
    loadingTimer.start();

    // init
    MainWindow w;
    w.show();

    qDebug() << QString("loading time: %1 sec").arg(QString::number(loadingTimer.elapsed() / 1000., 'f', 3));

    return a.exec();
}
