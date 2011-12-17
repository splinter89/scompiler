#include "SSyntacticAnalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject * parent)
{
    this->setParent(parent);
    qDebug() << "ok";
}

SSyntacticAnalyzer::~SSyntacticAnalyzer()
{
    // bye
}

QList<Token> SSyntacticAnalyzer::first(Token token) {
    QList<Token> first;
    if (isTokenTerminal(token)) {
        first << token;


    } else {

    }
    return first;
}
