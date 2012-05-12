#ifndef SSYNTACTICANALYZER_H
#define SSYNTACTICANALYZER_H

#include <QtCore>
#include <QObject>
#include "errors.h"
#include "basics.h"

class SSyntacticAnalyzer : public QObject
{
    Q_OBJECT

public:
    SSyntacticAnalyzer(QObject * parent = 0);
    ~SSyntacticAnalyzer();

    void setGrammar(QList<GrammarRule> grammar);

    bool generateSetOfSituations();
    bool generateActionGotoTables();

    QList<int> process(QList<TokenPointer> tokens,
                       QList<TokenId> table_ids,
                       QList<TokenConst> table_consts,
                       QList<TokenKeyword> table_keywords,
                       QList<TokenSeparator> table_separators);

    QList<QSet<Situation> > getUltimateSetOfSituations() { return ultimate_situations_set_; }
    QList<QHash<Token, Action> > getTableAction() { return action_table_; }
    QList<QHash<Token, int> > getTableGoto() { return goto_table_; }

private:
    QList<GrammarRule> grammar_;

    QList<QSet<Situation> > ultimate_situations_set_;
    QList<QHash<Token, Action> > action_table_;
    QList<QHash<Token, int> > goto_table_;

    QList<Symbol> symbol_table_;

    QSet<Token> first(const Token token);
    QSet<Token> first(const QList<Token> tokens);

    QSet<Situation> closure(QSet<Situation> i);
    QSet<Situation> makeStep(const QSet<Situation> i, const Token x);

    int addSymbolFunction(QString name, DataType data_type, QList<int> args_indexes);
    int addSymbolArgument(QString name, DataType data_type, ArgType arg_type, bool is_const);

signals:
    void syntax_error(int pos, QString msg);
};

#endif // SSYNTACTICANALYZER_H
