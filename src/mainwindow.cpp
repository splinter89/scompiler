#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui_(new Ui::MainWindow)
{
    int width = 1050, height = 650;

    ui_->setupUi(this);
    resize(width, height);
    QRect desktop = QApplication::desktop()->availableGeometry();
    if ((desktop.width() > width) && (desktop.height() > height + 10)) {
        move((desktop.width() - width) / 2, (desktop.height() - height) / 2 - 20);
    } else {
        resize(width / 2, height / 2);
    }
    //setFixedSize(width(), height());

    base_window_title_ = windowTitle();
    file_dialog_dir_ = QDir::currentPath();  // QCoreApplication::applicationDirPath()

    initInterface();  // >:[]
    initLogic();
    openFile();
    setStatusMsg("ok");
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::initInterface()
{
    // first width is for '#' column; second - 'index', 'start', 'length'
    int header_num_width = 50, header_index_width = 60;
    int header_grammar_width = 440;
    int tab_stop_width = 20;

    editor_ = new CodeEditor();
    editor_->setTabStopWidth(tab_stop_width);
    editor_->setCursor(Qt::IBeamCursor);

    tab_main_ = new QTabWidget();

    QWidget* tab_lex = new QWidget();
    tab_main_->addTab(tab_lex, trUtf8("Lexical analysis"));
    // ////
    QWidget* tab_lex_0 = new QWidget();
    QWidget* tab_lex_1 = new QWidget();
    QWidget* tab_lex_2 = new QWidget();
    QWidget* tab_lex_3 = new QWidget();
    QWidget* tab_lex_4 = new QWidget();
    tab_lex_main_ = new QTabWidget();
    tab_lex_main_->addTab(tab_lex_0, trUtf8("reduction"));
    // //// ////
    QGridLayout* grid_lex_0 = new QGridLayout();
    table_lex_0_ = new QTableWidget(0, 6);
    table_lex_0_->setColumnWidth(0, header_num_width);
    table_lex_0_->setColumnWidth(3, header_index_width);
    table_lex_0_->setColumnWidth(4, header_index_width);
    table_lex_0_->setColumnWidth(5, header_index_width);
    table_lex_0_->verticalHeader()->setVisible(false);
    grid_lex_0->addWidget(table_lex_0_);
    tab_lex_0->setLayout(grid_lex_0);
    // ////
    tab_lex_main_->addTab(tab_lex_1, trUtf8("IDs"));
    // //// ////
    QGridLayout* grid_lex_1 = new QGridLayout();
    table_lex_1_ = new QTableWidget(0, 2);
    table_lex_1_->setColumnWidth(0, header_num_width);
    table_lex_1_->verticalHeader()->setVisible(false);
    grid_lex_1->addWidget(table_lex_1_);
    tab_lex_1->setLayout(grid_lex_1);
    // ////
    tab_lex_main_->addTab(tab_lex_2, trUtf8("constants"));
    // //// ////
    QGridLayout* grid_lex_2 = new QGridLayout();
    table_lex_2_ = new QTableWidget(0, 3);
    table_lex_2_->setColumnWidth(0, header_num_width);
    table_lex_2_->verticalHeader()->setVisible(false);
    grid_lex_2->addWidget(table_lex_2_);
    tab_lex_2->setLayout(grid_lex_2);
    // ////
    tab_lex_main_->addTab(tab_lex_3, trUtf8("keywords"));
    // //// ////
    QGridLayout* grid_lex_3 = new QGridLayout();
    table_lex_3_ = new QTableWidget(0, 2);
    table_lex_3_->setColumnWidth(0, header_num_width);
    table_lex_3_->verticalHeader()->setVisible(false);
    grid_lex_3->addWidget(table_lex_3_);
    tab_lex_3->setLayout(grid_lex_3);
    // ////
    tab_lex_main_->addTab(tab_lex_4, trUtf8("separators"));
    // //// ////
    QGridLayout* grid_lex_4 = new QGridLayout();
    table_lex_4_ = new QTableWidget(0, 2);
    table_lex_4_->setColumnWidth(0, header_num_width);
    table_lex_4_->verticalHeader()->setVisible(false);
    grid_lex_4->addWidget(table_lex_4_);
    tab_lex_4->setLayout(grid_lex_4);
    // ////
    setLexTableHeaders();

    QGridLayout* grid_lex = new QGridLayout();
    grid_lex->addWidget(tab_lex_main_);
    tab_lex->setLayout(grid_lex);

    QWidget* tab_synt = new QWidget();
    tab_main_->addTab(tab_synt, trUtf8("Syntax"));
    // ////
    QWidget* tab_synt_0 = new QWidget();
    QWidget* tab_synt_1 = new QWidget();
    QWidget* tab_synt_2 = new QWidget();
    QWidget* tab_synt_3 = new QWidget();
    QWidget* tab_synt_4 = new QWidget();
    QWidget* tab_synt_5 = new QWidget();
    tab_synt_main_ = new QTabWidget();
    tab_synt_main_->addTab(tab_synt_0, trUtf8("rightmost derivation"));
    // //// ////
    QGridLayout* grid_synt_0 = new QGridLayout();
    edit_synt_0_ = new QPlainTextEdit();
    edit_synt_0_->setReadOnly(true);
    edit_synt_0_->setTabStopWidth(tab_stop_width);
    grid_synt_0->addWidget(edit_synt_0_);
    tab_synt_0->setLayout(grid_synt_0);
    // ////
    tab_synt_main_->addTab(tab_synt_1, trUtf8("grammar"));
    // //// ////
    QGridLayout* grid_synt_1 = new QGridLayout();
    table_synt_1_ = new QTableWidget(0, 2);
    table_synt_1_->setColumnWidth(0, header_num_width);
    table_synt_1_->setColumnWidth(1, header_grammar_width);
    table_synt_1_->verticalHeader()->setVisible(false);
    grid_synt_1->addWidget(table_synt_1_);
    tab_synt_1->setLayout(grid_synt_1);
    // ////
    tab_synt_main_->addTab(tab_synt_2, trUtf8("sets of situations"));
    // //// ////
    QGridLayout* grid_synt_2 = new QGridLayout();
    edit_synt_2_ = new QPlainTextEdit();
    edit_synt_2_->setReadOnly(true);
    edit_synt_2_->setTabStopWidth(tab_stop_width);
    grid_synt_2->addWidget(edit_synt_2_);
    tab_synt_2->setLayout(grid_synt_2);
    // ////
    tab_synt_main_->addTab(tab_synt_3, trUtf8("action table"));
    // //// ////
    QGridLayout* grid_synt_3 = new QGridLayout();
    table_synt_3_ = new QTableWidget(0, 1);
    table_synt_3_->setColumnWidth(0, header_num_width);
    table_synt_3_->verticalHeader()->setVisible(false);
    grid_synt_3->addWidget(table_synt_3_);
    tab_synt_3->setLayout(grid_synt_3);
    // ////
    tab_synt_main_->addTab(tab_synt_4, trUtf8("goto table"));
    // //// ////
    QGridLayout* grid_synt_4 = new QGridLayout();
    table_synt_4_ = new QTableWidget(0, 1);
    table_synt_4_->setColumnWidth(0, header_num_width);
    table_synt_4_->verticalHeader()->setVisible(false);
    grid_synt_4->addWidget(table_synt_4_);
    tab_synt_4->setLayout(grid_synt_4);
    // ////
    if (allow_custom_grammars_) {
        tab_synt_main_->addTab(tab_synt_5, trUtf8("set grammar"));
    }
    // //// ////
    QGridLayout* grid_synt_5 = new QGridLayout();
    table_synt_5_ = new QTableWidget(0, 2);
    table_synt_5_->setColumnWidth(0, header_num_width);
    table_synt_5_->setColumnWidth(1, header_grammar_width);
    table_synt_5_->verticalHeader()->setVisible(false);
    grid_synt_5->addWidget(table_synt_5_, 0, 0, 1, 5);

    // buttons
    QPushButton* b_load_rules = new QPushButton(trUtf8("load"));
    grid_synt_5->addWidget(b_load_rules, 1, 0);
    connect(b_load_rules, SIGNAL(clicked()), this, SLOT(loadActiveRules()));

    QPushButton* b_save_rules = new QPushButton(trUtf8("save"));
    grid_synt_5->addWidget(b_save_rules, 1, 1);
    connect(b_save_rules, SIGNAL(clicked()), this, SLOT(saveActiveRules()));

    QPushButton* b_check_all_rules = new QPushButton(trUtf8("all"));
    grid_synt_5->addWidget(b_check_all_rules, 1, 2);
    QPushButton* b_uncheck_all_rules = new QPushButton(trUtf8("none"));
    grid_synt_5->addWidget(b_uncheck_all_rules, 1, 3);

    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(b_check_all_rules, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(b_uncheck_all_rules, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(b_check_all_rules, 1);
    signalMapper->setMapping(b_uncheck_all_rules, 0);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setCheckedAllRules(int)));

    b_update_grammar = new QPushButton(trUtf8("apply"));
    grid_synt_5->addWidget(b_update_grammar, 1, 4);
    connect(b_update_grammar, SIGNAL(clicked()), this, SLOT(updateGrammar()));

    tab_synt_5->setLayout(grid_synt_5);

    QStringList table_synt_1_headers;
    table_synt_1_headers << trUtf8("#") << trUtf8("rule");
    table_synt_1_->setHorizontalHeaderLabels(table_synt_1_headers);

    QStringList table_synt_5_headers;
    table_synt_5_headers << trUtf8("#") << trUtf8("rule");
    table_synt_5_->setHorizontalHeaderLabels(table_synt_5_headers);

    QGridLayout* grid_synt = new QGridLayout();
    grid_synt->addWidget(tab_synt_main_);
    tab_synt->setLayout(grid_synt);

    QWidget* tab_sem = new QWidget();
    tab_main_->addTab(tab_sem, trUtf8("Semantics"));
    // ////
    QWidget* tab_sem_0 = new QWidget();
    QWidget* tab_sem_1 = new QWidget();
    tab_sem_main_ = new QTabWidget();
    tab_sem_main_->addTab(tab_sem_0, trUtf8("symbol table"));
    // //// ////
    QGridLayout* grid_sem_0 = new QGridLayout();
    edit_sem_0_ = new QPlainTextEdit();
    edit_sem_0_->setReadOnly(true);
    edit_sem_0_->setTabStopWidth(tab_stop_width);
    grid_sem_0->addWidget(edit_sem_0_);
    tab_sem_0->setLayout(grid_sem_0);
    // ////
    tab_sem_main_->addTab(tab_sem_1, trUtf8("block structure"));
    // //// ////
    QGridLayout* grid_sem_1 = new QGridLayout();
    edit_sem_1_ = new QPlainTextEdit();
    edit_sem_1_->setReadOnly(true);
    edit_sem_1_->setTabStopWidth(tab_stop_width);
    grid_sem_1->addWidget(edit_sem_1_);
    tab_sem_1->setLayout(grid_sem_1);

    QGridLayout* grid_sem = new QGridLayout();
    grid_sem->addWidget(tab_sem_main_);
    tab_sem->setLayout(grid_sem);

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(editor_);
    splitter->addWidget(tab_main_);
    setCentralWidget(splitter);
}

void MainWindow::initLogic()
{
    lexical_analyzer_ = new SLexicalAnalyzer();
    connect(lexical_analyzer_, SIGNAL(lexical_error(int, QString)), this, SLOT(displayError(int, QString)));
    syntactic_analyzer_ = new SSyntacticAnalyzer();
    connect(syntactic_analyzer_, SIGNAL(syntax_error(int, QString)), this, SLOT(displayError(int, QString)));
    connect(syntactic_analyzer_, SIGNAL(semantic_error(int, QString)), this, SLOT(displayError(int, QString)));

    grammar_active_rules_.clear();
    grammar_active_rules_ << 0;  // rule #0 must be always active
    //grammar_active_rules_ << 1 << 2 << 5 << 6 << 37 << 38 << 39 << 40 << 41 << 42 << 43 << 44 << 45 << 46 << 47 << 48
    //                      << 49 << 50 << 51 << 52 << 55 << 56 << 94 << 95 << 98 << 99 << 100 << 101 << 102 << 103 << 106
    //                      << 108 << 110 << 111 << 112 << 113 << 117 << 118;
    grammar_ = setGrammarRules(grammar_active_rules_);

    updateSyntTables();  // init checkboxes
    if (!allow_custom_grammars_) {
        setCheckedAllRules(1);
    }
    updateGrammar();  // generate stuff
}

void MainWindow::displayError(int pos, QString msg)
{
    // :TODO: calc (line, col)
    setStatusError(((pos > -1) ? "[pos " + QString::number(pos) + "] " : "") + msg);

    if (pos > -1) {
        // move cursor to position
        QTextCursor cursor(editor_->textCursor());
        cursor.setPosition(pos);
        editor_->setTextCursor(cursor);
    }
}

void MainWindow::setStatusMsg(const QString text)
{
    statusBar()->setStyleSheet("color: black;");
    statusBar()->showMessage(trUtf8("Status: ") + text);
}

void MainWindow::setStatusError(const QString text)
{
    statusBar()->setStyleSheet("color: red;");
    statusBar()->showMessage(trUtf8("Error: ") + text);
}

void MainWindow::setLexTableHeaders()
{
    QStringList table_lex_0_headers;
    table_lex_0_headers << trUtf8("#") << trUtf8("representation") << trUtf8("type") << trUtf8("ref") << trUtf8("start")
                        << trUtf8("length");
    table_lex_0_->setHorizontalHeaderLabels(table_lex_0_headers);

    QStringList table_lex_1_headers;
    table_lex_1_headers << trUtf8("#") << trUtf8("representation");
    table_lex_1_->setHorizontalHeaderLabels(table_lex_1_headers);

    QStringList table_lex_2_headers;
    table_lex_2_headers << trUtf8("#") << trUtf8("type") << trUtf8("value");
    table_lex_2_->setHorizontalHeaderLabels(table_lex_2_headers);

    QStringList table_lex_3_headers;
    table_lex_3_headers << trUtf8("#") << trUtf8("representation");
    table_lex_3_->setHorizontalHeaderLabels(table_lex_3_headers);

    QStringList table_lex_4_headers;
    table_lex_4_headers << trUtf8("#") << trUtf8("representation");
    table_lex_4_->setHorizontalHeaderLabels(table_lex_4_headers);
}

void MainWindow::clearLexTables()
{
    while (table_lex_0_->rowCount() > 0) {
        table_lex_0_->removeRow(0);
    }
    while (table_lex_1_->rowCount() > 0) {
        table_lex_1_->removeRow(0);
    }
    while (table_lex_2_->rowCount() > 0) {
        table_lex_2_->removeRow(0);
    }
    while (table_lex_3_->rowCount() > 0) {
        table_lex_3_->removeRow(0);
    }
    while (table_lex_4_->rowCount() > 0) {
        table_lex_4_->removeRow(0);
    }
}

void MainWindow::clearSyntTables()
{
    while (table_synt_1_->rowCount() > 0) {
        table_synt_1_->removeRow(0);
    }
    while (table_synt_3_->rowCount() > 0) {
        table_synt_3_->removeRow(0);
    }
    while (table_synt_4_->rowCount() > 0) {
        table_synt_4_->removeRow(0);
    }
    while (table_synt_5_->rowCount() > 0) {
        table_synt_5_->removeRow(0);
    }
}

void MainWindow::updateSyntTables()
{
    int i;
    QList<QSet<Situation>> ultimate_set = syntactic_analyzer_->getUltimateSetOfSituations();
    QList<QHash<Token, Action>> table_action = syntactic_analyzer_->getTableAction();
    QList<QHash<Token, int>> table_goto = syntactic_analyzer_->getTableGoto();

    clearSyntTables();
    for (i = 0; i < grammar_.length(); i++) {
        table_synt_1_->insertRow(i);

        QTableWidgetItem* item_0 = new QTableWidgetItem;
        QTableWidgetItem* item_1 = new QTableWidgetItem;

        item_0->setTextAlignment(Qt::AlignCenter);
        item_0->setText(QString::number(i));
        item_1->setText(grammar_.at(i).toString());
        table_synt_1_->setItem(i, 0, item_0);
        table_synt_1_->setItem(i, 1, item_1);
    }
    for (i = 0; i < Grammar_full.length(); i++) {
        table_synt_5_->insertRow(i);

        QCheckBox* chb = new QCheckBox(QString::number(i));
        QTableWidgetItem* item_1 = new QTableWidgetItem;

        if (i == 0) {
            chb->setEnabled(false);  // rule #0 must be always active
        }
        chb->setChecked(grammar_active_rules_.contains(i));
        item_1->setText(Grammar_full.at(i).toString());
        table_synt_5_->setCellWidget(i, 0, chb);
        table_synt_5_->setItem(i, 1, item_1);
    }

    QStringList sets;
    for (i = 0; i < ultimate_set.length(); i++) {
        QSet<Situation> one_set = ultimate_set.at(i);
        QStringList set;
        foreach (Situation situation, one_set) {
            set << ("\t" + situation.toString());
        }
        sets << ("// #" + QString::number(i) + "\n" + set.join(",\n"));
    }
    edit_synt_2_->setPlainText("(\n" + sets.join("\n), (\n") + "\n)");

    QSet<Token> terminals = getAllTerminalTokens();
    QStringList table_synt_3_headers;
    table_synt_3_headers << trUtf8("#");
    foreach (Token terminal, terminals) {
        table_synt_3_headers << tokenToString(terminal);
    }
    table_synt_3_->setColumnCount(terminals.count() + 1);
    table_synt_3_->setRowCount(table_action.length());
    table_synt_3_->setHorizontalHeaderLabels(table_synt_3_headers);
    for (i = 0; i < table_action.length(); i++) {
        QTableWidgetItem* item_0 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);
        item_0->setText(QString::number(i));
        table_synt_3_->setItem(i, 0, item_0);
    }
    int j = 1;
    foreach (Token terminal, terminals) {
        bool empty_col = true;
        for (i = 0; i < table_action.length(); i++) {
            if (table_action.at(i).contains(terminal)) {
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(table_action.at(i).value(terminal).toString());
                table_synt_3_->setItem(i, j, item);
                empty_col = false;
            }
        }
        if (empty_col) {
            table_synt_3_->removeColumn(j);
        } else {
            j++;
        }
    }

    QSet<Token> nonterminals = getAllNonTerminalTokens(grammar_);
    QStringList table_synt_4_headers;
    table_synt_4_headers << trUtf8("#");
    foreach (Token nonterminal, nonterminals) {
        table_synt_4_headers << tokenToString(nonterminal);
    }
    table_synt_4_->setColumnCount(nonterminals.count() + 1);
    table_synt_4_->setRowCount(table_goto.length());
    table_synt_4_->setHorizontalHeaderLabels(table_synt_4_headers);
    for (i = 0; i < table_goto.length(); i++) {
        QTableWidgetItem* item_0 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);
        item_0->setText(QString::number(i));
        table_synt_4_->setItem(i, 0, item_0);
    }
    j = 1;
    foreach (Token nonterminal, nonterminals) {
        bool empty_col = true;
        for (i = 0; i < table_goto.length(); i++) {
            if (table_goto.at(i).contains(nonterminal)) {
                QTableWidgetItem* item = new QTableWidgetItem;
                item->setText(QString::number(table_goto.at(i).value(nonterminal)));
                table_synt_4_->setItem(i, j, item);
                empty_col = false;
            }
        }
        if (empty_col) {
            table_synt_4_->removeColumn(j);
        } else {
            j++;
        }
    }
}

void MainWindow::openFile(const QString filename)
{
    if (filename.isEmpty()) {
        setWindowTitle(QString("Untitled - %1").arg(base_window_title_));
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStatusError(trUtf8("open error: %1").arg(file.errorString()));
    }
    QTextStream in(&file);
    editor_->setPlainText(in.readAll());  // \n,\r\n,\r -> \n (automatically)
    file.close();

    QFileInfo fi(filename);
    setStatusMsg(trUtf8("file \"%1\" loaded").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title_));
    file_dialog_dir_ = fi.absolutePath();
}

void MainWindow::saveFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setStatusError(trUtf8("save error: %1").arg(file.errorString()));
    }
    QTextStream out(&file);
    QString content = editor_->toPlainText();
    out << content;
    file.close();

    QFileInfo fi(filename);
    setStatusMsg(trUtf8("file \"%1\" saved successfully").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title_));
    file_dialog_dir_ = fi.absolutePath();
}

void MainWindow::run()
{
    if (editor_->toPlainText().trimmed().length() == 0) {
        setStatusError(trUtf8("type code or open a file (Ctrl+O)"));
        return;
    }

    int i;
    // --------------------------------------------------------------------------------
    // lexical analysis ---------------------------------------------------------------
    // --------------------------------------------------------------------------------
    if (lexical_analyzer_->process(editor_->toPlainText().trimmed())) {
        setStatusMsg("ok");
    }

    QList<TokenPointer> tokens = lexical_analyzer_->getAllTokens();
    QList<TokenId> table_ids = lexical_analyzer_->getTableIds();
    QList<TokenConst> table_consts = lexical_analyzer_->getTableConsts();
    QList<TokenKeyword> table_keywords = lexical_analyzer_->getTableKeywords();
    QList<TokenSeparator> table_separators = lexical_analyzer_->getTableSeparators();

    clearLexTables();
    for (i = 0; i < tokens.length(); i++) {
        table_lex_0_->insertRow(i);

        QString token_type_temp, const_type_temp, code_temp;
        switch (tokens.at(i).type) {
            case T_ID:
                token_type_temp = "id";
                code_temp = table_ids.at(tokens.at(i).index).name;
                break;

            case T_CONST:
                switch (table_consts.at(tokens.at(i).index).type) {
                    case TYPE_BOOL:
                        const_type_temp = "bool";
                        break;

                    case TYPE_INT:
                        const_type_temp = "int";
                        break;

                    case TYPE_DOUBLE:
                        const_type_temp = "double";
                        break;

                    case TYPE_CHAR:
                        const_type_temp = "char";
                        break;

                    case TYPE_STRING:
                        const_type_temp = "string";
                        break;

                    default:
                        const_type_temp = "???";
                }

                token_type_temp = "const (" + const_type_temp + ")";
                code_temp = table_consts.at(tokens.at(i).index).value.toString();
                break;

            case T_KEYWORD:
                token_type_temp = "keyword";
                code_temp = KeywordCodes.key(table_keywords.at(tokens.at(i).index).type);
                break;

            case T_SEPARATOR:
                token_type_temp = "separator";
                code_temp = (SeparatorCodes.key(table_separators.at(tokens.at(i).index).type) == " ")
                              ? "[space]"
                              : SeparatorCodes.key(table_separators.at(tokens.at(i).index).type);
                break;

            case EOF_TOKEN:
                token_type_temp = "eof";
                code_temp = "$";
                break;

            default:
                break;
        }

        QTableWidgetItem* item_0 = new QTableWidgetItem;
        QTableWidgetItem* item_1 = new QTableWidgetItem;
        QTableWidgetItem* item_2 = new QTableWidgetItem;
        QTableWidgetItem* item_3 = new QTableWidgetItem;
        QTableWidgetItem* item_4 = new QTableWidgetItem;
        QTableWidgetItem* item_5 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);
        item_3->setTextAlignment(Qt::AlignCenter);
        item_4->setTextAlignment(Qt::AlignCenter);
        item_5->setTextAlignment(Qt::AlignCenter);
        if (code_temp != "[space]") {
            QFont bold("Helvetica", 10, QFont::Bold);
            item_1->setFont(bold);
        }

        item_0->setText(QString::number(i));
        item_1->setText(code_temp);
        item_2->setText(token_type_temp);
        item_3->setText(tokens.at(i).toString());
        item_4->setText(QString::number(tokens.at(i).start));
        item_5->setText(QString::number(tokens.at(i).length));
        table_lex_0_->setItem(i, 0, item_0);
        table_lex_0_->setItem(i, 1, item_1);
        table_lex_0_->setItem(i, 2, item_2);
        table_lex_0_->setItem(i, 3, item_3);
        table_lex_0_->setItem(i, 4, item_4);
        table_lex_0_->setItem(i, 5, item_5);
    }
    for (i = 0; i < table_ids.length(); i++) {
        table_lex_1_->insertRow(i);

        QTableWidgetItem* item_0 = new QTableWidgetItem;
        QTableWidgetItem* item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText(table_ids.at(i).name);
        table_lex_1_->setItem(i, 0, item_0);
        table_lex_1_->setItem(i, 1, item_1);
    }
    for (i = 0; i < table_consts.length(); i++) {
        table_lex_2_->insertRow(i);

        QString const_type_temp;
        switch (table_consts.at(i).type) {
            case TYPE_BOOL:
                const_type_temp = "bool";
                break;

            case TYPE_INT:
                const_type_temp = "int";
                break;

            case TYPE_DOUBLE:
                const_type_temp = "double";
                break;

            case TYPE_CHAR:
                const_type_temp = "char";
                break;

            case TYPE_STRING:
                const_type_temp = "string";
                break;

            default:
                const_type_temp = "";
                break;
        }

        QTableWidgetItem* item_0 = new QTableWidgetItem;
        QTableWidgetItem* item_1 = new QTableWidgetItem;
        QTableWidgetItem* item_2 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText(const_type_temp);
        item_2->setText(table_consts.at(i).value.toString());

        table_lex_2_->setItem(i, 0, item_0);
        table_lex_2_->setItem(i, 1, item_1);
        table_lex_2_->setItem(i, 2, item_2);
    }
    for (i = 0; i < table_keywords.length(); i++) {
        table_lex_3_->insertRow(i);

        QTableWidgetItem* item_0 = new QTableWidgetItem;
        QTableWidgetItem* item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText(KeywordCodes.key(table_keywords.at(i).type));

        table_lex_3_->setItem(i, 0, item_0);
        table_lex_3_->setItem(i, 1, item_1);
    }
    for (i = 0; i < table_separators.length(); i++) {
        table_lex_4_->insertRow(i);

        QTableWidgetItem* item_0 = new QTableWidgetItem;
        QTableWidgetItem* item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText((SeparatorCodes.key(table_separators.at(i).type) == " ")
                          ? "[space]"
                          : SeparatorCodes.key(table_separators.at(i).type));

        table_lex_4_->setItem(i, 0, item_0);
        table_lex_4_->setItem(i, 1, item_1);
    }
    setLexTableHeaders();

    tab_lex_main_->setTabText(0, trUtf8("reduction (") + QString::number(tokens.length()) + ")");
    tab_lex_main_->setTabText(1, "IDs (" + QString::number(table_ids.length()) + ")");
    tab_lex_main_->setTabText(2, "constants (" + QString::number(table_consts.length()) + ")");
    tab_lex_main_->setTabText(3, "keywords (" + QString::number(table_keywords.length()) + ")");
    tab_lex_main_->setTabText(4, "separators (" + QString::number(table_separators.length()) + ")");

    // --------------------------------------------------------------------------------
    // syntax analysis ----------------------------------------------------------------
    // --------------------------------------------------------------------------------
    QList<int> parse_rules =
      syntactic_analyzer_->process(tokens, table_ids, table_consts, table_keywords, table_separators);
    // reverse the list
    for (int k = 0; k < (parse_rules.size() / 2); k++)
        parse_rules.swap(k, parse_rules.size() - (1 + k));

    QString text_rules, text_symbols, text_blocks;

    // output parse rules
    if (!parse_rules.isEmpty()) {
        QStringList text_rule_first, text_rule_second;
        foreach (int rule_index, parse_rules) {
            int rule_index_full =
              indexOfGrammarRule(grammar_.at(rule_index).left_token, grammar_.at(rule_index).right_side, Grammar_full);
            text_rule_first << QString::number(rule_index);
            text_rule_second << (allow_custom_grammars_ ? QString("#%1 (%2)\n\t").arg(rule_index).arg(rule_index_full)
                                                        : QString("#%1\n\t").arg(rule_index))
                                  + grammar_.at(rule_index).toString();
        }
        text_rules = text_rule_first.join(" ") + "\n\n"
                     + trUtf8("Details:\n=========================================\n")
                     + text_rule_second.join("\n\n=========================================\n");
    } else {
        text_rules = trUtf8("Failed to get the rightmost derivation for the specified grammar");
    }
    edit_synt_0_->setPlainText(text_rules);

    // --------------------------------------------------------------------------------
    // semantic analysis --------------------------------------------------------------
    // --------------------------------------------------------------------------------
    QList<Symbol> symbols_table = syntactic_analyzer_->getSymbolTable();
    QList<Block> blocks_table = syntactic_analyzer_->getBlockTable();
    QStringList text_symbols_list, text_blocks_list;
    for (i = 0; i < symbols_table.length(); i++) {
        text_symbols_list << QString("#%1: %2").arg(i).arg(symbols_table.at(i).toString());
    }
    for (i = 0; i < blocks_table.length(); i++) {
        text_blocks_list << QString("#%1: %2").arg(i).arg(blocks_table.at(i).toString());
    }
    text_symbols = text_symbols_list.join("\n");
    text_blocks = text_blocks_list.join("\n");
    edit_sem_0_->setPlainText(text_symbols);
    edit_sem_1_->setPlainText(text_blocks);

    tab_sem_main_->setTabText(0, "symbol table (" + QString::number(symbols_table.length()) + ")");
    tab_sem_main_->setTabText(1, "block structure (" + QString::number(blocks_table.length()) + ")");
}

void MainWindow::updateGrammar()
{
    b_update_grammar->setEnabled(false);

    grammar_active_rules_.clear();
    grammar_active_rules_ << 0;  // rule #0 must be always active
    for (int i = 1; i < table_synt_5_->rowCount(); i++) {
        QCheckBox* chb = qobject_cast<QCheckBox*>(table_synt_5_->cellWidget(i, 0));
        if (chb->isChecked()) {
            grammar_active_rules_ << i;
        }
    }
    grammar_ = setGrammarRules(grammar_active_rules_);

    syntactic_analyzer_->setGrammar(grammar_, cache_filename_);
    updateSyntTables();

    b_update_grammar->setEnabled(true);
}

void MainWindow::loadActiveRules()
{
    QString res = "";

    // read from file
    QString fileName = QFileDialog::getOpenFileName(this, trUtf8("Load template"), file_dialog_dir_,
                                                    trUtf8("Config file (*.cfg);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            setStatusError(trUtf8("load error: %1").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        res = in.readAll();  // \n,\r\n,\r -> \n (automatically)
        file.close();

        QFileInfo fi(fileName);
        file_dialog_dir_ = fi.absolutePath();

        if (!res.isEmpty()) {
            for (int i = 1; i < table_synt_5_->rowCount(); i++) {
                QCheckBox* chb = qobject_cast<QCheckBox*>(table_synt_5_->cellWidget(i, 0));
                if (i <= res.length()) {
                    chb->setChecked((res.at(i - 1) == '1') ? true : false);
                } else {
                    chb->setChecked(false);
                }
            }
        }
    }
}

void MainWindow::saveActiveRules()
{
    QString res = "";

    for (int i = 1; i < table_synt_5_->rowCount(); i++) {
        QCheckBox* chb = qobject_cast<QCheckBox*>(table_synt_5_->cellWidget(i, 0));
        res += (chb->isChecked()) ? "1" : "0";
    }

    // write to file
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Save template"), file_dialog_dir_,
                                                    trUtf8("Config file (*.cfg);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            setStatusError(trUtf8("save error: %1").arg(file.errorString()));
            return;
        }
        QTextStream out(&file);
        out << res;
        file.close();

        QFileInfo fi(fileName);
        file_dialog_dir_ = fi.absolutePath();
    }
}

void MainWindow::setCheckedAllRules(int is_checked)
{
    for (int i = 1; i < table_synt_5_->rowCount(); i++) {
        QCheckBox* chb = qobject_cast<QCheckBox*>(table_synt_5_->cellWidget(i, 0));
        chb->setChecked((bool)is_checked);
    }
}

void MainWindow::on_open_triggered()
{
    QString fileName =
      QFileDialog::getOpenFileName(this, trUtf8("Load code"), file_dialog_dir_,
                                   trUtf8("C++ sources (*.cpp *.cp *.cc *.cxx *.c++ *.c);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void MainWindow::on_save_triggered()
{
    QString fileName =
      QFileDialog::getSaveFileName(this, trUtf8("Save code"), file_dialog_dir_,
                                   trUtf8("C++ sources (*.cpp *.cp *.cc *.cxx *.c++ *.c);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        if (fileName.indexOf(".cpp") == -1) {
            fileName += ".cpp";
        }
        saveFile(fileName);
    }
}

void MainWindow::on_run_triggered()
{
    run();
}
