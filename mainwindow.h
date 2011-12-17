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
    void run();

private:
    Ui::MainWindow *ui_;

    CodeEditor *editor_;
    QTabWidget *tab_main_;
    QTabWidget *tab_lex_main_;
    QTableWidget *table_lex_0_;  // convolution
    QTableWidget *table_lex_1_;  // id's
    QTableWidget *table_lex_2_;  // const's
    QTableWidget *table_lex_3_;  // keywords
    QTableWidget *table_lex_4_;  // separators

    QString base_window_title_;

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
