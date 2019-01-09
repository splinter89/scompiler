#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include "basics.h"
#include "codeeditor.h"
#include "errors.h"

#include "slexicalanalyzer.h"
#include "ssyntacticanalyzer.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void setStatusMsg(const QString text);
    void setStatusError(const QString text);

    void openFile(const QString filename = "");
    void saveFile(const QString filename);
    void run();

private:
    Ui::MainWindow* ui_;

    QList<GrammarRule> grammar_;
    QList<int> grammar_active_rules_;

    SLexicalAnalyzer* lexical_analyzer_;
    SSyntacticAnalyzer* syntactic_analyzer_;

    CodeEditor* editor_;
    QTabWidget* tab_main_;

    QTabWidget* tab_lex_main_;
    QTableWidget* table_lex_0_;  // reduction
    QTableWidget* table_lex_1_;  // IDs
    QTableWidget* table_lex_2_;  // constants
    QTableWidget* table_lex_3_;  // keywords
    QTableWidget* table_lex_4_;  // separators

    QTabWidget* tab_synt_main_;
    QPlainTextEdit* edit_synt_0_;  // parse rules
    QTableWidget* table_synt_1_;   // grammar
    QPlainTextEdit* edit_synt_2_;  // situations
    QTableWidget* table_synt_3_;   // action rules
    QTableWidget* table_synt_4_;   // goto rules
    QTableWidget* table_synt_5_;   // set grammar

    QTabWidget* tab_sem_main_;
    QPlainTextEdit* edit_sem_0_;  // symbols table
    QPlainTextEdit* edit_sem_1_;  // blocks table

    QPushButton* b_update_grammar;

    QString base_window_title_;
    QString file_dialog_dir_;
    QString cache_filename_ = "scompiler_cache.dat";

    void initInterface();
    void initLogic();
    void clearLexTables();
    void setLexTableHeaders();
    void clearSyntTables();
    void updateSyntTables();

public slots:
    void displayError(int pos, QString msg);

private slots:
    void on_open_triggered();
    void on_save_triggered();
    void on_run_triggered();
    void loadActiveRules();
    void saveActiveRules();
    void setCheckedAllRules(int is_checked);
    void updateGrammar();
};

#endif  // MAINWINDOW_H
