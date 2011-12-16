#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include "errors.h"
#include "basics.h"
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setStatusMsg(const QString text);
    void setStatusError(const QString text);

    void openFile(const QString filename);
    void saveFile(const QString filename);
    void run(void);

private:
    Ui::MainWindow *ui;

    CodeEditor *editor;
    QTabWidget *tab_main;
    QTabWidget *tab_lex_main;
    QTableWidget *table_lex_0;  // convolution
    QTableWidget *table_lex_1;  // id's
    QTableWidget *table_lex_2;  // const's
    QTableWidget *table_lex_3;  // keywords
    QTableWidget *table_lex_4;  // separators

    QString base_window_title;

    void clearLexTables();
    void setLexTableHeaders();

public slots:
    void closeEvent(QCloseEvent *event);
    void displayError(int pos, QString msg);

private slots:
    void on_open_triggered();
    void on_save_triggered();
    void on_run_triggered();
};

#endif // MAINWINDOW_H
