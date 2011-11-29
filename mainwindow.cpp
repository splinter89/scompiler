#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slexer.h"
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(width(), height());
    this->statusBar()->showMessage(trUtf8("Статус: ок"));
}

MainWindow::~MainWindow()
{
//    qDebug("MainWindow destructor");
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
//    int result = QMessageBox::question(0, trUtf8("Подтверждение"),
//                                       trUtf8("Вы действительно хотите выйти?"),
//                                       QMessageBox::Yes, QMessageBox::No);

//    if (result == QMessageBox::Yes) {
        event->accept();
//    } else {
//        event->ignore();
//    }
}


void MainWindow::openFile()
{
    qDebug()<<"openFile";
}

void MainWindow::saveFile()
{
    qDebug()<<"saveFile";
}

void MainWindow::run()
{
    qDebug()<<"run!";
}


void MainWindow::on_open_triggered()
{
    this->openFile();
}

void MainWindow::on_save_triggered()
{
    this->saveFile();
}

void MainWindow::on_run_triggered()
{
    this->run();
}
