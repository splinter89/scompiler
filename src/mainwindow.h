#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "codeeditor.h"
#include "errors.h"
#include "basics.h"
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>

#include "slexicalanalyzer.h"
#include "ssyntacticanalyzer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setStatusMsg(const QString text);
    void setStatusError(const QString text);

    void openFile(const QString filename);
    void saveFile(const QString filename);
    void run();

private:
    Ui::MainWindow *ui_;

    QList<GrammarRule> grammar_;
    QList<int> grammar_active_rules_;

    SLexicalAnalyzer *lexical_analyzer_;
    SSyntacticAnalyzer *syntactic_analyzer_;

    CodeEditor *editor_;
    QTabWidget *tab_main_;

    QTabWidget *tab_lex_main_;
    QTableWidget *table_lex_0_;     // convolution
    QTableWidget *table_lex_1_;     // id's
    QTableWidget *table_lex_2_;     // const's
    QTableWidget *table_lex_3_;     // keywords
    QTableWidget *table_lex_4_;     // separators

    QTabWidget *tab_synt_main_;
    QPlainTextEdit *edit_synt_0_;   // parse rules
    QTableWidget *table_synt_1_;    // grammar
    QPlainTextEdit *edit_synt_2_;   // situations
    QTableWidget *table_synt_3_;    // action rules
    QTableWidget *table_synt_4_;    // goto rules
    QTableWidget *table_synt_5_;    // set grammar

    QTabWidget *tab_sem_main_;
    QPlainTextEdit *edit_sem_0_;    // symbols table
    QPlainTextEdit *edit_sem_1_;    // blocks table

    QPushButton *b_update_grammar;

    QString base_window_title_;

    void clearLexTables();
    void setLexTableHeaders();
    void clearSyntTables();

public slots:
    void closeEvent(QCloseEvent *event);
    void displayError(int pos, QString msg);

private slots:
    void on_open_triggered();
    void on_save_triggered();
    void on_run_triggered();
    void loadActiveRules();
    void saveActiveRules();
    void uncheckAllRules();
    void updateGrammar();
};

#endif // MAINWINDOW_H
