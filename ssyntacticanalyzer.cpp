#include "SSyntacticAnalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject * parent)
{
    this->setParent(parent);

    ultimate_set_c_ = generateSetOfSituations();
}

SSyntacticAnalyzer::~SSyntacticAnalyzer()
{
    // bye
}

QSet<Token> SSyntacticAnalyzer::first(const Token token) {
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

QSet<Token> SSyntacticAnalyzer::first(const QList<Token> tokens) {
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


QSet<Situation> SSyntacticAnalyzer::closure(QSet<Situation> i) {
    int old_count = i.count();
    do {
        // для каждой ситуации [A -> alpha.Bbeta, a] из I
        foreach (const Situation &situation, i) {
            int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
            if ((dot_pos > -1) && (dot_pos < situation.right_side.length() - 1)) {
                Token b = situation.right_side.at(dot_pos + 1);
                QList<QList<Token> > rules = Grammar.values(b);
                if (!rules.isEmpty()) {
                    // для каждого правила вывода B -> gamma из G
                    foreach (const QList<Token> &rule, rules) {
                        QList<Token> test_right_side;
                        if (dot_pos < situation.right_side.length() - 2) {
                            test_right_side = situation.right_side.mid(dot_pos + 2);    // skip .B
                        }
                        test_right_side << situation.look_ahead_token;  // beta a
                        QSet<Token> first_test = first(test_right_side);
                        // для каждого терминала c из FIRST(beta a), такого, что [B ->.gamma, c] нет в I
                        foreach (const Token &c, first_test) {
                            QList<Token> new_right_rule = EmptyTokenList() << DOT_TOKEN;
                            new_right_rule += rule;     // .gamma
                            Situation new_situation = {b, new_right_rule, c};
                            // добавляем [B ->.gamma, c] к I;
                            i << new_situation;
                        }
                    }
                }
            }
        }

    } while (i.count() != old_count);
    return i;
}

QSet<Situation> SSyntacticAnalyzer::makeStep(const QSet<Situation> i, const Token x) {
    QSet<Situation> j;
    foreach (Situation situation, i) {
        int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
        if ((dot_pos > -1)
            && (dot_pos < situation.right_side.length() - 1)
            && (situation.right_side.at(situation.right_side.indexOf(DOT_TOKEN) + 1) == x)
        ) {
            situation.right_side.swap(dot_pos, dot_pos + 1);
            j << situation;
        }
    }
    return closure(j);
}

QSet<QSet<Situation> > SSyntacticAnalyzer::generateSetOfSituations() {
    Situation s = {
        N_S,
        EmptyTokenList() << DOT_TOKEN << N_E,
        EOF_TOKEN
    };
    QSet<Situation> i;
    QSet<QSet<Situation> > c;
    QSet<Token> all_tokens = getAllGrammarTokens();

    i << s;
    i = closure(i);
    c << i;

    int old_count = c.count();
    do {
        foreach (const QSet<Situation> &i, c) {
            foreach (const Token &x, all_tokens) {
                QSet<Situation> new_i = makeStep(i, x);
                if (!new_i.isEmpty()) {
                    c << new_i;
                }
            }
        }
    } while (c.count() != old_count);
    return c;
}
