#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slexer.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGridLayout>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(850, 650);
//    setFixedSize(width(), height());
    statusBar()->showMessage(trUtf8("Статус: ок"));
    base_window_title = windowTitle();

    // now draw interface
    editor = new CodeEditor();
    editor->setTabStopWidth(20);
    editor->setCursor(Qt::IBeamCursor);

    QWidget *tab_lex = new QWidget();
    QWidget *tab_sint = new QWidget();
    QTabWidget *tab_main = new QTabWidget();
    tab_main->addTab(tab_lex, trUtf8("Лексический анализ"));
        QWidget *tab_lex_1 = new QWidget();
        QWidget *tab_lex_2 = new QWidget();
        QTabWidget *tab_lex_main = new QTabWidget();
        tab_lex_main->addTab(tab_lex_1, trUtf8("Свертка"));
        tab_lex_main->addTab(tab_lex_2, trUtf8("..."));

        QGridLayout *grid_lex = new QGridLayout();
        grid_lex->addWidget(tab_lex_main);
        tab_lex->setLayout(grid_lex);
    tab_main->addTab(tab_sint, trUtf8("..."));

    QGridLayout *grid_main = new QGridLayout();
    grid_main->addWidget(editor, 0, 0);
    grid_main->addWidget(tab_main, 0, 1);


    QWidget *central_widget = new QWidget();
    central_widget->setLayout(grid_main);
    setCentralWidget(central_widget);
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
    editor->setPlainText(in.readAll());
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
    QString content = editor->toPlainText();
    out << content;
    file.close();

    QFileInfo fi(filename);
    statusBar()->showMessage(trUtf8("Статус: файл \"%1\" успешно сохранен.").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title));
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
                trUtf8("C++ source file (*.cpp *.cp *.cc *.cxx *.c++ *.C);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void MainWindow::on_save_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                trUtf8("Сохранить исходный код..."),
                QCoreApplication::applicationDirPath(),
                trUtf8("C++ source file (*.cpp *.cp *.cc *.cxx *.c++ *.C);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        saveFile(fileName);
    }
}

void MainWindow::on_run_triggered()
{
    run();
}
