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
    int header_num_width = 40, main_width = 1050, main_height = 650;

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
        tab_lex_main = new QTabWidget();
        tab_lex_main->addTab(tab_lex_1, trUtf8("свертка"));
            QGridLayout *grid_lex_1 = new QGridLayout();
            edit_lex = new QPlainTextEdit();
            edit_lex->setReadOnly(true);
            grid_lex_1->addWidget(edit_lex);
            tab_lex_1->setLayout(grid_lex_1);
        tab_lex_main->addTab(tab_lex_2, trUtf8("#1 id's"));
            QGridLayout *grid_lex_2 = new QGridLayout();
            table_lex_2 = new QTableWidget(0, 4);
            table_lex_2->setColumnWidth(0, header_num_width);
            table_lex_2->verticalHeader()->setVisible(false);
            grid_lex_2->addWidget(table_lex_2);
            tab_lex_2->setLayout(grid_lex_2);
        tab_lex_main->addTab(tab_lex_3, trUtf8("#2 constants"));
            QGridLayout *grid_lex_3 = new QGridLayout();
            table_lex_3 = new QTableWidget(0, 5);
            table_lex_3->setColumnWidth(0, header_num_width);
            table_lex_3->verticalHeader()->setVisible(false);
            grid_lex_3->addWidget(table_lex_3);
            tab_lex_3->setLayout(grid_lex_3);
        tab_lex_main->addTab(tab_lex_4, trUtf8("#3 keywords"));
            QGridLayout *grid_lex_4 = new QGridLayout();
            table_lex_4 = new QTableWidget(0, 4);
            table_lex_4->setColumnWidth(0, header_num_width);
            table_lex_4->verticalHeader()->setVisible(false);
            grid_lex_4->addWidget(table_lex_4);
            tab_lex_4->setLayout(grid_lex_4);
        tab_lex_main->addTab(tab_lex_5, trUtf8("#4 separators"));
            QGridLayout *grid_lex_5 = new QGridLayout();
            table_lex_5 = new QTableWidget(0, 4);
            table_lex_5->setColumnWidth(0, header_num_width);
            table_lex_5->verticalHeader()->setVisible(false);
            grid_lex_5->addWidget(table_lex_5);
            tab_lex_5->setLayout(grid_lex_5);
        setLexTableHeaders();

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

void MainWindow::setLexTableHeaders() {
    QStringList table_lex_2_headers;
    table_lex_2_headers << trUtf8("#") << trUtf8("представление") << trUtf8("начало") << trUtf8("длина");
    table_lex_2->setHorizontalHeaderLabels(table_lex_2_headers);

    QStringList table_lex_3_headers;
    table_lex_3_headers << trUtf8("#") << trUtf8("тип") << trUtf8("значение")
                        << trUtf8("начало") << trUtf8("длина");
    table_lex_3->setHorizontalHeaderLabels(table_lex_3_headers);

    QStringList table_lex_4_headers;
    table_lex_4_headers << trUtf8("#") << trUtf8("представление") << trUtf8("начало") << trUtf8("длина");
    table_lex_4->setHorizontalHeaderLabels(table_lex_4_headers);

    QStringList table_lex_5_headers;
    table_lex_5_headers << trUtf8("#") << trUtf8("представление") << trUtf8("начало") << trUtf8("длина");
    table_lex_5->setHorizontalHeaderLabels(table_lex_5_headers);
}

void MainWindow::clearLexTables() {
    table_lex_2->clear();
    table_lex_3->clear();
    table_lex_4->clear();
    table_lex_5->clear();
    int i;
    for (i = 0; i < table_lex_2->rowCount(); i++) {
        table_lex_2->removeRow(i);
    }
    for (i = 0; i < table_lex_3->rowCount(); i++) {
        table_lex_3->removeRow(i);
    }
    for (i = 0; i < table_lex_4->rowCount(); i++) {
        table_lex_4->removeRow(i);
    }
    for (i = 0; i < table_lex_5->rowCount(); i++) {
        table_lex_5->removeRow(i);
    }
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
    QList<TableItem_id> table_ids;
    QList<TableItem_const> table_consts;
    QList<TableItem_keyword> table_keywords;
    QList<TableItem_separator> table_separators;
    int i, row;

    switch (tab_main->currentIndex()) {
    case 0:
        // lexical analysis
        lex = new SLexer(editor->toPlainText());

        tokens = lex->getAllTokens();
        edit_lex->setPlainText(QString::number(tokens.count()));

        table_ids = lex->getTableIds();
        table_consts = lex->getTableConsts();
        table_keywords = lex->getTableKeywords();
        table_separators = lex->getTableSeparators();

        clearLexTables();
        for (i = 0; i < table_ids.length(); i++) {
            row = table_lex_2->rowCount();
            table_lex_2->insertRow(row);
            QTableWidgetItem* item_1 = new QTableWidgetItem;
            QTableWidgetItem* item_2 = new QTableWidgetItem;
            QTableWidgetItem* item_3 = new QTableWidgetItem;
            QTableWidgetItem* item_4 = new QTableWidgetItem;
            item_1->setText(QString::number(i));
            item_1->setTextAlignment(Qt::AlignCenter);
            item_2->setText(table_ids.at(i).identifier);
            item_3->setText(QString::number(table_ids.at(i).start));
            item_4->setText(QString::number(table_ids.at(i).length));
            table_lex_2->setItem(row, 0, item_1);
            table_lex_2->setItem(row, 1, item_2);
            table_lex_2->setItem(row, 2, item_3);
            table_lex_2->setItem(row, 3, item_4);
        }
        for (i = 0; i < table_consts.length(); i++) {
            row = table_lex_3->rowCount();
            table_lex_3->insertRow(row);
            QTableWidgetItem* item_1 = new QTableWidgetItem;
            QTableWidgetItem* item_2 = new QTableWidgetItem;
            QTableWidgetItem* item_3 = new QTableWidgetItem;
            QTableWidgetItem* item_4 = new QTableWidgetItem;
            QTableWidgetItem* item_5 = new QTableWidgetItem;
            item_1->setText(QString::number(i));
            item_1->setTextAlignment(Qt::AlignCenter);
            QString const_type_temp;
            switch (table_consts.at(i).type) {
            case CONST_BOOL:
                const_type_temp = "bool";
                break;

            case CONST_INT:
                const_type_temp = "int";
                break;

            case CONST_DOUBLE:
                const_type_temp = "double";
                break;

            case CONST_CHAR:
                const_type_temp = "char";
                break;

            case CONST_STRING:
                const_type_temp = "string";
                break;

            }
            item_2->setText(const_type_temp);
            item_3->setText(table_consts.at(i).value.toString());
            item_4->setText(QString::number(table_consts.at(i).start));
            item_5->setText(QString::number(table_consts.at(i).length));
            table_lex_3->setItem(row, 0, item_1);
            table_lex_3->setItem(row, 1, item_2);
            table_lex_3->setItem(row, 2, item_3);
            table_lex_3->setItem(row, 3, item_4);
            table_lex_3->setItem(row, 4, item_5);
        }
        for (i = 0; i < table_keywords.length(); i++) {
            row = table_lex_4->rowCount();
            table_lex_4->insertRow(row);
            QTableWidgetItem* item_1 = new QTableWidgetItem;
            QTableWidgetItem* item_2 = new QTableWidgetItem;
            QTableWidgetItem* item_3 = new QTableWidgetItem;
            QTableWidgetItem* item_4 = new QTableWidgetItem;
            item_1->setText(QString::number(i));
            item_1->setTextAlignment(Qt::AlignCenter);
            item_2->setText(KeywordCodes.key(table_keywords.at(i).type));
            item_3->setText(QString::number(table_keywords.at(i).start));
            item_4->setText(QString::number(table_keywords.at(i).length));
            table_lex_4->setItem(row, 0, item_1);
            table_lex_4->setItem(row, 1, item_2);
            table_lex_4->setItem(row, 2, item_3);
            table_lex_4->setItem(row, 3, item_4);
        }
        for (i = 0; i < table_separators.length(); i++) {
            row = table_lex_5->rowCount();
            table_lex_5->insertRow(row);
            QTableWidgetItem* item_1 = new QTableWidgetItem;
            QTableWidgetItem* item_2 = new QTableWidgetItem;
            QTableWidgetItem* item_3 = new QTableWidgetItem;
            QTableWidgetItem* item_4 = new QTableWidgetItem;
            item_1->setText(QString::number(i));
            item_1->setTextAlignment(Qt::AlignCenter);
            item_2->setText((SeparatorCodes.key(table_separators.at(i).type) == " ")
                            ? "[space]"
                            : SeparatorCodes.key(table_separators.at(i).type));
            item_3->setText(QString::number(table_separators.at(i).start));
            item_4->setText(QString::number(table_separators.at(i).length));
            table_lex_5->setItem(row, 0, item_1);
            table_lex_5->setItem(row, 1, item_2);
            table_lex_5->setItem(row, 2, item_3);
            table_lex_5->setItem(row, 3, item_4);
        }
        setLexTableHeaders();

        tab_lex_main->setTabText(1, "#1 id's (" + QString::number(table_ids.length()) + ")");
        tab_lex_main->setTabText(2, "#2 constants (" + QString::number(table_consts.length()) + ")");
        tab_lex_main->setTabText(3, "#3 keywords (" + QString::number(table_keywords.length()) + ")");
        tab_lex_main->setTabText(4, "#4 separators (" + QString::number(table_separators.length()) + ")");
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
