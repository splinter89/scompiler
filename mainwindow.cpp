#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slexer.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGridLayout>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int header_num_width = 40, main_width = 900, main_height = 650;

    ui->setupUi(this);
    resize(main_width, main_height);
    QRect desktop = QApplication::desktop()->availableGeometry();
    if ((desktop.width() > main_width) && (desktop.height() > main_height + 10)) {
        move((desktop.width() - main_width) / 2, (desktop.height() - main_height) / 2 - 20);
    } else {
        resize(main_width / 2, main_height / 2);
    }
//    setFixedSize(width(), height());
    statusBar()->showMessage(trUtf8("Статус: ок"));
    base_window_title = windowTitle();

    // now draw interface
    editor = new CodeEditor();
    editor->setTabStopWidth(20);
    editor->setCursor(Qt::IBeamCursor);

    QWidget *tab_lex = new QWidget();
    QWidget *tab_sint = new QWidget();
    tab_main = new QTabWidget();
    tab_main->addTab(tab_lex, trUtf8("Лексический анализ"));
        QWidget *tab_lex_1 = new QWidget();
        QWidget *tab_lex_2 = new QWidget();
        QWidget *tab_lex_3 = new QWidget();
        QWidget *tab_lex_4 = new QWidget();
        QWidget *tab_lex_5 = new QWidget();
        QTabWidget *tab_lex_main = new QTabWidget();
        tab_lex_main->addTab(tab_lex_1, trUtf8("свертка"));
            QGridLayout *grid_lex_1 = new QGridLayout();
            edit_lex = new QPlainTextEdit();
            edit_lex->setReadOnly(true);
            grid_lex_1->addWidget(edit_lex);
            tab_lex_1->setLayout(grid_lex_1);
        tab_lex_main->addTab(tab_lex_2, trUtf8("#1 id's"));
            QGridLayout *grid_lex_2 = new QGridLayout();
            table_lex_2 = new QTableWidget(0, 2);
            QStringList table_lex_2_headers;
            table_lex_2_headers << trUtf8("#") << trUtf8("представление");
            table_lex_2->setHorizontalHeaderLabels(table_lex_2_headers);
            table_lex_2->setColumnWidth(0, header_num_width);
            grid_lex_2->addWidget(table_lex_2);
            tab_lex_2->setLayout(grid_lex_2);
        tab_lex_main->addTab(tab_lex_3, trUtf8("#2 constants"));
            QGridLayout *grid_lex_3 = new QGridLayout();
            table_lex_3 = new QTableWidget(0, 3);
            QStringList table_lex_3_headers;
            table_lex_3_headers << trUtf8("#") << trUtf8("тип") << trUtf8("значение");
            table_lex_3->setHorizontalHeaderLabels(table_lex_3_headers);
            table_lex_3->setColumnWidth(0, header_num_width);
            grid_lex_3->addWidget(table_lex_3);
            tab_lex_3->setLayout(grid_lex_3);
        tab_lex_main->addTab(tab_lex_4, trUtf8("#3 keywords"));
            QGridLayout *grid_lex_4 = new QGridLayout();
            table_lex_4 = new QTableWidget(0, 2);
            QStringList table_lex_4_headers;
            table_lex_4_headers << trUtf8("#") << trUtf8("представление");
            table_lex_4->setHorizontalHeaderLabels(table_lex_4_headers);
            table_lex_4->setColumnWidth(0, header_num_width);
            grid_lex_4->addWidget(table_lex_4);
            tab_lex_4->setLayout(grid_lex_4);
        tab_lex_main->addTab(tab_lex_5, trUtf8("#4 separators"));
            QGridLayout *grid_lex_5 = new QGridLayout();
            table_lex_5 = new QTableWidget(0, 2);
            QStringList table_lex_5_headers;
            table_lex_5_headers << trUtf8("#") << trUtf8("представление");
            table_lex_5->setHorizontalHeaderLabels(table_lex_5_headers);
            table_lex_5->setColumnWidth(0, header_num_width);
            grid_lex_5->addWidget(table_lex_5);
            tab_lex_5->setLayout(grid_lex_5);

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


void MainWindow::setStatusMsg(const QString text) {
    statusBar()->setStyleSheet("color: black;");
    statusBar()->showMessage(trUtf8("Статус: ") + text);
}
void MainWindow::setStatusError(const QString text) {
    statusBar()->setStyleSheet("color: red;");
    statusBar()->showMessage(trUtf8("Ошибка: ") + text);
}


void MainWindow::openFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStatusError(trUtf8("при открытии: %1").arg(file.errorString()));
    }
    QTextStream in(&file);
    editor->setPlainText(in.readAll());     // \n,\r\n,\r -> \n (automatically)
    file.close();

    QFileInfo fi(filename);
    setStatusMsg(trUtf8("файл \"%1\" загружен").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title));
}

void MainWindow::saveFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setStatusError(trUtf8("при сохранении. %1").arg(file.errorString()));
    }
    QTextStream out(&file);
    QString content = editor->toPlainText();
    out << content;
    file.close();

    QFileInfo fi(filename);
    setStatusMsg(trUtf8("файл \"%1\" успешно сохранен").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title));
}

void MainWindow::run()
{
    if (editor->toPlainText().length() == 0) {
        setStatusError(trUtf8("введите код или загрузите его из файла (Ctrl+O)"));
        return;
    }

    SLexer *lex;
    QList<TokenPointer> tokens;

    switch (tab_main->currentIndex()) {
    case 0:
        // lexical analysis
        lex = new SLexer(editor->toPlainText());
        tokens = lex->getAllTokens();
        edit_lex->setPlainText(QString::number(tokens.count()));
        break;
    case 1:
        qDebug()<<"tab 2 active";
        break;
    }
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
