#include "SSyntacticAnalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject * parent)
{
    this->setParent(parent);
    qDebug() << "ok";

    QSet<Token> some = first(EmptyTokenList() << N_E << S_MULT << N_T);
    Q_UNUSED(some)
}

SSyntacticAnalyzer::~SSyntacticAnalyzer()
{
    // bye
}

QSet<Token> SSyntacticAnalyzer::first(Token token) {
    QSet<Token> result;

    // step 1
    if (isTokenTerminal(token)) {
        result << token;
    } else if (isTokenNonTerminal(token)) {
        // step 2
        if (Grammar.values(token).contains(EmptyTokenList() << LAMBDA)) {
            result << LAMBDA;
        }

        // step 3
        int old_count = result.count();
        do {
            // для каждого правила X -> Y0Y1...Yk-1
            foreach (const QList<Token> &rule, Grammar.values(token)) {
                bool all_y_got_lambda = true;
                for (int i = 0; i < rule.length(); i++) {
                    QSet<Token> first_y = first(rule.at(i));
                    int old_first_y_count = first_y.count();
                    first_y.remove(LAMBDA);

                    // добавляем FIRST(Yi)\{e} к FIRST(X);
                    result += first_y;

                    if (old_first_y_count == first_y.count()) {
                        // в FIRST(Yi) не было e
                        all_y_got_lambda = false;
                        break;
                    }
                }
                if (all_y_got_lambda) {
                    result << LAMBDA;
                }
            }
        } while (result.count() != old_count);
    }

    return result;
}

QSet<Token> SSyntacticAnalyzer::first(QList<Token> tokens) {
    QSet<Token> result;

    // для каждого X0X1...Xk-1
    bool all_x_got_lambda = true;
    for (int i = 0; i < tokens.length(); i++) {
        QSet<Token> first_x = first(tokens.at(i));
        int old_first_x_count = first_x.count();
        first_x.remove(LAMBDA);

        // добавляем FIRST(Xi)\{e} к FIRST(X);
        result += first_x;

        if (old_first_x_count == first_x.count()) {
            // в FIRST(Xi) не было e
            all_x_got_lambda = false;
            break;
        }
    }
    if (all_x_got_lambda) {
        result << LAMBDA;
    }

    return result;
}
