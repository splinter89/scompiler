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

    QList<Token> first(Token token);

signals:
    void syntax_error(int pos, QString msg);
};

#endif // SSYNTACTICANALYZER_H
