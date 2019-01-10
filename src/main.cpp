#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTime timer;
    timer.start();
    MainWindow window;
    window.show();
    qDebug() << QString("loaded: %1 sec").arg(QString::number(timer.elapsed() / 1000., 'f', 3));

    return app.exec();
}
