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
    ui->setupUi(this);

    setFixedSize(width(), height());
    statusBar()->showMessage(trUtf8("Статус: ок"));
    ui->source_te->setTabStopWidth(20);

    base_window_title = windowTitle();

}

MainWindow::~MainWindow()
{
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


void MainWindow::openFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        statusBar()->showMessage(trUtf8("Ошибка при открытии: %1").arg(file.errorString()));
    }
    QTextStream in(&file);
    ui->source_te->setPlainText(in.readAll());
    file.close();

    QFileInfo fi(filename);
    statusBar()->showMessage(trUtf8("Статус: файл \"%1\" загружен.").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title));
}

void MainWindow::saveFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        statusBar()->showMessage(trUtf8("Ошибка при сохранении: %1").arg(file.errorString()));
    }
    QTextStream out(&file);
    QString content = ui->source_te->toPlainText();
    out << content;
    file.close();

    QFileInfo fi(filename);
    statusBar()->showMessage(trUtf8("Статус: файл \"%1\" успешно сохранен.").arg(fi.fileName()));
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
    openFile(fileName);
}

void MainWindow::on_save_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                trUtf8("Сохранить исходный код..."),
                QCoreApplication::applicationDirPath(),
                trUtf8("C++ sources (*.cpp *.cp *.cc *.cxx *.c++ *.C);;All files (*.*)"));
    saveFile(fileName);
}

void MainWindow::on_run_triggered()
{
    run();
}
