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

private:
    QList<QList<Situation> > ultimate_situations_set_;
    QList<QHash<Token, Action> > action_table_;
    QList<QHash<Token, int> > goto_table_;

    QList<int> states_stack_;
    QList<Token> tokens_stack_;

    QList<int> process(QList<TokenPointer> tokens);

    QSet<Token> first(const Token token);
    QSet<Token> first(const QList<Token> tokens);

    QList<Situation> closure(QList<Situation> i);
    QList<Situation> makeStep(const QList<Situation> i, const Token x);
    void generateSetOfSituations();
    void generateActionGotoTables();

signals:
    void syntax_error(int pos, QString msg);
};

#endif // SSYNTACTICANALYZER_H
