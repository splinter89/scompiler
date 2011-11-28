#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "euser.h"
#include "eclient.h"
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = 0;
}

MainWindow::~MainWindow()
{
    qDebug("MainWindow destructor");
    delete client;
    delete ui;
}

void MainWindow::startP(long user_id)
{
    EUser user(user_id);
    qDebug()<<">> User"<<user.getName()<<"has been connected.";

    if (user.getType() == EUser::CLIENT) {
        client = new EClient(user);
        this->setCentralWidget(client->window());
        this->setWindowTitle(QString("Client : \"%1 %2\"").arg(client->getName()).arg(client->getlastname()));
        this->resize(680, 270);
        qDebug()<<">> CLIENT has been connected.";

    }

    show();
}

//обработчик выхода
void MainWindow::closeEvent(QCloseEvent* event)
{
    //выдаём сообщение
    int result = QMessageBox::question(0, trUtf8("Предупреждение"),
                                       trUtf8("Вы действительно хотите выйти?"),
                                       QMessageBox::Yes, QMessageBox::No);

    //анализируем ответ
    if (result == QMessageBox::Yes){
        //выходим
        emit exitWorkplace();
        event->accept();
    }
    else{
        //остаёмся
        event->ignore();
    }
}
