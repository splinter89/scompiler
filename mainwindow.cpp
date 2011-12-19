#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "slexicalanalyzer.h"
#include "ssyntacticanalyzer.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSplitter>
#include <QGridLayout>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    // first width is for '#' column; second - 'index', 'start', 'length'
    int header_num_width = 40, header_index_width = 60;
    int header_grammar_width = 450;
    int main_width = 1050, main_height = 650;
    int tab_stop_width = 20;

    ui_->setupUi(this);
    resize(main_width, main_height);
    QRect desktop = QApplication::desktop()->availableGeometry();
    if ((desktop.width() > main_width) && (desktop.height() > main_height + 10)) {
        move((desktop.width() - main_width) / 2, (desktop.height() - main_height) / 2 - 20);
    } else {
        resize(main_width / 2, main_height / 2);
    }
//    setFixedSize(width(), height());
    statusBar()->showMessage(trUtf8("Статус: ok"));
    base_window_title_ = windowTitle();

    // now draw interface
    editor_ = new CodeEditor();
    editor_->setTabStopWidth(tab_stop_width);
    editor_->setCursor(Qt::IBeamCursor);

    tab_main_ = new QTabWidget();
    QWidget *tab_lex = new QWidget();
    tab_main_->addTab(tab_lex, trUtf8("Лексический анализ"));
        QWidget *tab_lex_0 = new QWidget();
        QWidget *tab_lex_1 = new QWidget();
        QWidget *tab_lex_2 = new QWidget();
        QWidget *tab_lex_3 = new QWidget();
        QWidget *tab_lex_4 = new QWidget();
        tab_lex_main_ = new QTabWidget();
        tab_lex_main_->addTab(tab_lex_0, trUtf8("свертка"));
            QGridLayout *grid_lex_0 = new QGridLayout();
            table_lex_0_ = new QTableWidget(0, 6);
            table_lex_0_->setColumnWidth(0, header_num_width);
            table_lex_0_->setColumnWidth(3, header_index_width);
            table_lex_0_->setColumnWidth(4, header_index_width);
            table_lex_0_->setColumnWidth(5, header_index_width);
            table_lex_0_->verticalHeader()->setVisible(false);
            grid_lex_0->addWidget(table_lex_0_);
            tab_lex_0->setLayout(grid_lex_0);
        tab_lex_main_->addTab(tab_lex_1, trUtf8("id's"));
            QGridLayout *grid_lex_1 = new QGridLayout();
            table_lex_1_ = new QTableWidget(0, 2);
            table_lex_1_->setColumnWidth(0, header_num_width);
            table_lex_1_->verticalHeader()->setVisible(false);
            grid_lex_1->addWidget(table_lex_1_);
            tab_lex_1->setLayout(grid_lex_1);
        tab_lex_main_->addTab(tab_lex_2, trUtf8("constants"));
            QGridLayout *grid_lex_2 = new QGridLayout();
            table_lex_2_ = new QTableWidget(0, 3);
            table_lex_2_->setColumnWidth(0, header_num_width);
            table_lex_2_->verticalHeader()->setVisible(false);
            grid_lex_2->addWidget(table_lex_2_);
            tab_lex_2->setLayout(grid_lex_2);
        tab_lex_main_->addTab(tab_lex_3, trUtf8("keywords"));
            QGridLayout *grid_lex_3 = new QGridLayout();
            table_lex_3_ = new QTableWidget(0, 2);
            table_lex_3_->setColumnWidth(0, header_num_width);
            table_lex_3_->verticalHeader()->setVisible(false);
            grid_lex_3->addWidget(table_lex_3_);
            tab_lex_3->setLayout(grid_lex_3);
        tab_lex_main_->addTab(tab_lex_4, trUtf8("separators"));
            QGridLayout *grid_lex_4 = new QGridLayout();
            table_lex_4_ = new QTableWidget(0, 2);
            table_lex_4_->setColumnWidth(0, header_num_width);
            table_lex_4_->verticalHeader()->setVisible(false);
            grid_lex_4->addWidget(table_lex_4_);
            tab_lex_4->setLayout(grid_lex_4);
        setLexTableHeaders();

        QGridLayout *grid_lex = new QGridLayout();
        grid_lex->addWidget(tab_lex_main_);
        tab_lex->setLayout(grid_lex);

    QWidget *tab_synt = new QWidget();
    tab_main_->addTab(tab_synt, trUtf8("Синтаксический анализ"));
        QWidget *tab_synt_0 = new QWidget();
        QWidget *tab_synt_1 = new QWidget();
        QWidget *tab_synt_2 = new QWidget();
        QWidget *tab_synt_3 = new QWidget();
        QWidget *tab_synt_4 = new QWidget();
        tab_synt_main_ = new QTabWidget();
        tab_synt_main_->addTab(tab_synt_0, trUtf8("правый вывод"));
            QGridLayout *grid_synt_0 = new QGridLayout();
            edit_synt_0_ = new QPlainTextEdit();
            edit_synt_0_->setReadOnly(true);
            edit_synt_0_->setTabStopWidth(tab_stop_width);
            grid_synt_0->addWidget(edit_synt_0_);
            tab_synt_0->setLayout(grid_synt_0);
        tab_synt_main_->addTab(tab_synt_1, trUtf8("grammar"));
            QGridLayout *grid_synt_1 = new QGridLayout();
            table_synt_1_ = new QTableWidget(0, 2);
            table_synt_1_->setColumnWidth(0, header_num_width);
            table_synt_1_->setColumnWidth(1, header_grammar_width);
            table_synt_1_->verticalHeader()->setVisible(false);
            grid_synt_1->addWidget(table_synt_1_);
            tab_synt_1->setLayout(grid_synt_1);
        tab_synt_main_->addTab(tab_synt_2, trUtf8("sets of situations"));
            QGridLayout *grid_synt_2 = new QGridLayout();
            edit_synt_2_ = new QPlainTextEdit();
            edit_synt_2_->setReadOnly(true);
            edit_synt_2_->setTabStopWidth(tab_stop_width);
            grid_synt_2->addWidget(edit_synt_2_);
            tab_synt_2->setLayout(grid_synt_2);
        tab_synt_main_->addTab(tab_synt_3, trUtf8("action table"));
            QGridLayout *grid_synt_3 = new QGridLayout();
            table_synt_3_ = new QTableWidget(0, 1);
            table_synt_3_->setColumnWidth(0, header_num_width);
            table_synt_3_->verticalHeader()->setVisible(false);
            grid_synt_3->addWidget(table_synt_3_);
            tab_synt_3->setLayout(grid_synt_3);
        tab_synt_main_->addTab(tab_synt_4, trUtf8("goto table"));
            QGridLayout *grid_synt_4 = new QGridLayout();
            table_synt_4_ = new QTableWidget(0, 1);
            table_synt_4_->setColumnWidth(0, header_num_width);
            table_synt_4_->verticalHeader()->setVisible(false);
            grid_synt_4->addWidget(table_synt_4_);
            tab_synt_4->setLayout(grid_synt_4);

        QStringList table_synt_1_headers;
        table_synt_1_headers << trUtf8("#") << trUtf8("правило");
        table_synt_1_->setHorizontalHeaderLabels(table_synt_1_headers);

        QStringList table_synt_3_headers;
        table_synt_3_headers << trUtf8("#") << trUtf8("");
        table_synt_3_->setHorizontalHeaderLabels(table_synt_3_headers);

        QStringList table_synt_4_headers;
        table_synt_4_headers << trUtf8("#") << trUtf8("");
        table_synt_4_->setHorizontalHeaderLabels(table_synt_4_headers);

        QGridLayout *grid_synt = new QGridLayout();
        grid_synt->addWidget(tab_synt_main_);
        tab_synt->setLayout(grid_synt);

    QWidget *tab_sem = new QWidget();
    tab_main_->addTab(tab_sem, trUtf8("..."));

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(editor_);
    splitter->addWidget(tab_main_);
    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui_;
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
    statusBar()->showMessage(trUtf8("Статус: ") + text);
}
void MainWindow::setStatusError(const QString text)
{
    statusBar()->setStyleSheet("color: red;");
    statusBar()->showMessage(trUtf8("Ошибка: ") + text);
}

void MainWindow::setLexTableHeaders()
{
    QStringList table_lex_0_headers;
    table_lex_0_headers << trUtf8("#") << trUtf8("представление") << trUtf8("тип")
                        << trUtf8("ссылка") << trUtf8("начало") << trUtf8("длина");
    table_lex_0_->setHorizontalHeaderLabels(table_lex_0_headers);

    QStringList table_lex_1_headers;
    table_lex_1_headers << trUtf8("#") << trUtf8("представление");
    table_lex_1_->setHorizontalHeaderLabels(table_lex_1_headers);

    QStringList table_lex_2_headers;
    table_lex_2_headers << trUtf8("#") << trUtf8("тип") << trUtf8("значение");
    table_lex_2_->setHorizontalHeaderLabels(table_lex_2_headers);

    QStringList table_lex_3_headers;
    table_lex_3_headers << trUtf8("#") << trUtf8("представление");
    table_lex_3_->setHorizontalHeaderLabels(table_lex_3_headers);

    QStringList table_lex_4_headers;
    table_lex_4_headers << trUtf8("#") << trUtf8("представление");
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
}


void MainWindow::openFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStatusError(trUtf8("при открытии: %1").arg(file.errorString()));
    }
    QTextStream in(&file);
    editor_->setPlainText(in.readAll());     // \n,\r\n,\r -> \n (automatically)
    file.close();

    QFileInfo fi(filename);
    setStatusMsg(trUtf8("файл \"%1\" загружен").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title_));
}

void MainWindow::saveFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setStatusError(trUtf8("при сохранении. %1").arg(file.errorString()));
    }
    QTextStream out(&file);
    QString content = editor_->toPlainText();
    out << content;
    file.close();

    QFileInfo fi(filename);
    setStatusMsg(trUtf8("файл \"%1\" успешно сохранен").arg(fi.fileName()));
    setWindowTitle(QString("%1 - %2").arg(fi.fileName()).arg(base_window_title_));
}

void MainWindow::run()
{
    if (editor_->toPlainText().length() == 0) {
        setStatusError(trUtf8("введите код или загрузите его из файла (Ctrl+O)"));
        return;
    }

    int i, j;

    // --------------------------------------------------------------------------------
    // lexical analysis ---------------------------------------------------------------
    // --------------------------------------------------------------------------------
    SLexicalAnalyzer *lexical_analyzer = new SLexicalAnalyzer();
    connect(lexical_analyzer, SIGNAL(lexical_error(int,QString)),
            this, SLOT(displayError(int,QString)));
    if (lexical_analyzer->process(editor_->toPlainText())) {
        setStatusMsg("ok");
    }

    QList<TokenPointer>   tokens           = lexical_analyzer->getAllTokens();
    QList<TokenId>        table_ids        = lexical_analyzer->getTableIds();
    QList<TokenConst>     table_consts     = lexical_analyzer->getTableConsts();
    QList<TokenKeyword>   table_keywords   = lexical_analyzer->getTableKeywords();
    QList<TokenSeparator> table_separators = lexical_analyzer->getTableSeparators();


    clearLexTables();
    for (i = 0; i < tokens.length(); i++) {
        table_lex_0_->insertRow(i);

        QString token_type_temp, const_type_temp, code_temp;
        switch (tokens.at(i).type) {
        case T_ID:
            token_type_temp = "id";
            code_temp = table_ids.at(tokens.at(i).index).identifier;
            break;

        case T_CONST:
            switch (table_consts.at(tokens.at(i).index).const_type) {
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

        QTableWidgetItem *item_0 = new QTableWidgetItem;
        QTableWidgetItem *item_1 = new QTableWidgetItem;
        QTableWidgetItem *item_2 = new QTableWidgetItem;
        QTableWidgetItem *item_3 = new QTableWidgetItem;
        QTableWidgetItem *item_4 = new QTableWidgetItem;
        QTableWidgetItem *item_5 = new QTableWidgetItem;
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
        item_3->setText(QString::number(tokens.at(i).index));
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

        QTableWidgetItem *item_0 = new QTableWidgetItem;
        QTableWidgetItem *item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText(table_ids.at(i).identifier);
        table_lex_1_->setItem(i, 0, item_0);
        table_lex_1_->setItem(i, 1, item_1);
    }
    for (i = 0; i < table_consts.length(); i++) {
        table_lex_2_->insertRow(i);

        QString const_type_temp;
        switch (table_consts.at(i).const_type) {
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

        QTableWidgetItem *item_0 = new QTableWidgetItem;
        QTableWidgetItem *item_1 = new QTableWidgetItem;
        QTableWidgetItem *item_2 = new QTableWidgetItem;
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

        QTableWidgetItem *item_0 = new QTableWidgetItem;
        QTableWidgetItem *item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText(KeywordCodes.key(table_keywords.at(i).type));

        table_lex_3_->setItem(i, 0, item_0);
        table_lex_3_->setItem(i, 1, item_1);
    }
    for (i = 0; i < table_separators.length(); i++) {
        table_lex_4_->insertRow(i);

        QTableWidgetItem *item_0 = new QTableWidgetItem;
        QTableWidgetItem *item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText((SeparatorCodes.key(table_separators.at(i).type) == " ")
                        ? "[space]"
                        : SeparatorCodes.key(table_separators.at(i).type));

        table_lex_4_->setItem(i, 0, item_0);
        table_lex_4_->setItem(i, 1, item_1);
    }
    setLexTableHeaders();

    tab_lex_main_->setTabText(0, trUtf8("свертка (") + QString::number(tokens.length()) + ")");
    tab_lex_main_->setTabText(1, "id's (" + QString::number(table_ids.length()) + ")");
    tab_lex_main_->setTabText(2, "constants (" + QString::number(table_consts.length()) + ")");
    tab_lex_main_->setTabText(3, "keywords (" + QString::number(table_keywords.length()) + ")");
    tab_lex_main_->setTabText(4, "separators (" + QString::number(table_separators.length()) + ")");

    // --------------------------------------------------------------------------------
    // syntax analysis ----------------------------------------------------------------
    // --------------------------------------------------------------------------------
    SSyntacticAnalyzer *syntactic_analyzer = new SSyntacticAnalyzer();
    connect(syntactic_analyzer, SIGNAL(syntax_error(int,QString)),
            this, SLOT(displayError(int,QString)));
    if (!syntactic_analyzer->generateSetOfSituations()) {
        displayError(-1, error_msg(E_INTERNAL_GENERATING_SITUATIONS));
        return;
    }
    if (!syntactic_analyzer->generateActionGotoTables()) {
//        displayError(-1, error_msg(E_INTERNAL_GENERATING_TABLES));
        return;
    }

    QList<int> parse_rules = syntactic_analyzer->process(tokens,table_ids, table_consts,
                                                         table_keywords, table_separators);
    QList<QSet<Situation> > ultimate_set = syntactic_analyzer->getUltimateSetOfSituations();
    QList<QHash<Token, Action> > table_action = syntactic_analyzer->getTableAction();
    QList<QHash<Token, int> > table_goto = syntactic_analyzer->getTableGoto();

    // reverse the list
    for(int k = 0; k < (parse_rules.size()/2); k++) parse_rules.swap(k, parse_rules.size()-(1+k));


    clearSyntTables();

    QString text_0;
    if (!parse_rules.isEmpty()) {
        QStringList text_0_first, text_0_second;
        foreach (int rule_num, parse_rules) {
            text_0_first << QString::number(rule_num);
            text_0_second << (QString("#%1\n\t").arg(rule_num) + Grammar.at(rule_num).toString());
        }
        text_0 = text_0_first.join(" ") + "\n\n"
                + trUtf8("Подробно:\n=========================================\n")
                + text_0_second.join("\n\n=========================================\n");
    } else {
        text_0 = trUtf8("Не удалось получить правый вывод в заданной грамматике");
    }
    edit_synt_0_->setPlainText(text_0);

    for (i = 0; i < Grammar.length(); i++) {
        table_synt_1_->insertRow(i);

        QTableWidgetItem *item_0 = new QTableWidgetItem;
        QTableWidgetItem *item_1 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);

        item_0->setText(QString::number(i));
        item_1->setText(Grammar.at(i).toString());
        table_synt_1_->setItem(i, 0, item_0);
        table_synt_1_->setItem(i, 1, item_1);
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
        QTableWidgetItem *item_0 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);
        item_0->setText(QString::number(i));
        table_synt_3_->setItem(i, 0, item_0);
    }
    j = 1;
    foreach (Token terminal, terminals) {
        bool empty_col = true;
        for (i = 0; i < table_action.length(); i++) {
            if (table_action.at(i).contains(terminal)) {
                QTableWidgetItem *item = new QTableWidgetItem;
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

    QSet<Token> nonterminals = getAllNonTerminalTokens();
    QStringList table_synt_4_headers;
    table_synt_4_headers << trUtf8("#");
    foreach (Token nonterminal, nonterminals) {
        table_synt_4_headers << tokenToString(nonterminal);
    }
    table_synt_4_->setColumnCount(nonterminals.count() + 1);
    table_synt_4_->setRowCount(table_goto.length());
    table_synt_4_->setHorizontalHeaderLabels(table_synt_4_headers);
    for (i = 0; i < table_goto.length(); i++) {
        QTableWidgetItem *item_0 = new QTableWidgetItem;
        item_0->setTextAlignment(Qt::AlignCenter);
        item_0->setText(QString::number(i));
        table_synt_4_->setItem(i, 0, item_0);
    }
    j = 1;
    foreach (Token nonterminal, nonterminals) {
        bool empty_col = true;
        for (i = 0; i < table_goto.length(); i++) {
            if (table_goto.at(i).contains(nonterminal)) {
                QTableWidgetItem *item = new QTableWidgetItem;
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
