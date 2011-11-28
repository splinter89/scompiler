#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // init
    MainWindow w;

//    QObject::connect(&login, SIGNAL(loginning(long)), &w, SLOT(startP(long)));

//    test client
    EUser user(5);
    EClient client(user);
    client.show();

    int r_code = app.exec();
    return r_code;
}
