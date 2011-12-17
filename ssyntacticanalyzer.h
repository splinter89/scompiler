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

    QSet<Token> first(Token token);
    QSet<Token> first(QList<Token> tokens);

signals:
    void syntax_error(int pos, QString msg);
};

#endif // SSYNTACTICANALYZER_H
