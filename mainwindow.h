#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basic_structs.h"
#include <QMainWindow>
#include "codeeditor.h"
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
    QPlainTextEdit *edit_lex;
    QTableWidget *table_lex_2;
    QTableWidget *table_lex_3;
    QTableWidget *table_lex_4;
    QTableWidget *table_lex_5;

    QString base_window_title;

public slots:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_open_triggered();
    void on_save_triggered();
    void on_run_triggered();
};

#endif // MAINWINDOW_H
