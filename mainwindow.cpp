#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slexer.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);

    setFixedSize(width(), height());
    this->statusBar()->showMessage(trUtf8("Статус: ок"));

    this->base_window_title = this->windowTitle();
}

MainWindow::~MainWindow()
{
//    qDebug("MainWindow destructor");
    delete this->ui;
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


void MainWindow::openFile(QString filename)
{
    qDebug()<<"openFile: "<<filename;

    QFile source(filename);
    if (!source.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Error", source.errorString());
    }
    ui->source_te->setPlainText(source.readAll());
    source.close();

    QFileInfo fi(filename);
    this->statusBar()->showMessage(trUtf8("Статус: файл \"%1\" загружен.").arg(fi.fileName()));
    this->setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(this->base_window_title));
}

void MainWindow::saveFile(QString filename)
{
    qDebug()<<"saveFile: "<<filename;

}

void MainWindow::run()
{
    qDebug()<<"run!";
}


void MainWindow::on_open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                trUtf8("Загрузить исходный код..."),
                QCoreApplication::applicationDirPath(),
                trUtf8("C++ sources (*.cpp *.cp *.cc *.cxx *.c++ *.C);;All files (*.*)"));
    this->openFile(fileName);
}

void MainWindow::on_save_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                trUtf8("Сохранить исходный код..."),
                QCoreApplication::applicationDirPath(),
                trUtf8("C++ sources (*.cpp *.cp *.cc *.cxx *.c++ *.C);;All files (*.*)"));
    this->saveFile(fileName);
}

void MainWindow::on_run_triggered()
{
    this->run();
}
