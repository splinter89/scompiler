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
//    QString source_;

    QSet<Token> first(const Token token);
    QSet<Token> first(const QList<Token> tokens);

    QSet<Situation> closure(QSet<Situation> i);
    QSet<Situation> makeStep(const QSet<Situation> i, const Token x);
    QSet<QSet<Situation> > generateSetOfSituations();

signals:
    void syntax_error(int pos, QString msg);
};

#endif // SSYNTACTICANALYZER_H
