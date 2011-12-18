#include "SSyntacticAnalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject * parent)
{
    this->setParent(parent);
    generateSetOfSituations();
    generateActionGotoTables();
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
        if (indexOfGrammarRule(token, EmptyTokenList() << LAMBDA) > -1) {
            result << LAMBDA;
        }

        // step 3
        int old_count = result.count();
        do {
            // для каждого правила X -> Y0Y1...Yk-1
            foreach (const GrammarRule &rule, getGrammarRulesByLeftToken(token)) {
                bool all_y_got_lambda = true;
                for (int i = 0; i < rule.right_side.length(); i++) {
                    QSet<Token> first_y = first(rule.right_side.at(i));
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
                QList<GrammarRule> rules = getGrammarRulesByLeftToken(b);
                if (!rules.isEmpty()) {
                    // для каждого правила вывода B -> gamma из G
                    foreach (const GrammarRule &rule, rules) {
                        QList<Token> test_right_side;
                        if (dot_pos < situation.right_side.length() - 2) {
                            test_right_side = situation.right_side.mid(dot_pos + 2);    // skip .B
                        }
                        test_right_side << situation.look_ahead_token;  // beta a
                        QSet<Token> first_test = first(test_right_side);
                        // для каждого терминала c из FIRST(beta a), такого, что [B ->.gamma, c] нет в I
                        foreach (const Token &c, first_test) {
                            QList<Token> new_right_rule = EmptyTokenList() << DOT_TOKEN;
                            new_right_rule += rule.right_side;     // .gamma
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
            && (situation.right_side.at(dot_pos + 1) == x)
        ) {
            situation.right_side.swap(dot_pos, dot_pos + 1);
            j << situation;
        }
    }
    return closure(j);
}

void SSyntacticAnalyzer::generateSetOfSituations() {
    if (Grammar.isEmpty()) return;

    // initial situation (in I0)
    Situation s = {
        N_S,
        EmptyTokenList() << DOT_TOKEN << N_E,
        EOF_TOKEN
    };
    QSet<Situation> i;
    QList<QSet<Situation> > c;
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
    ultimate_situations_set_ = c;
}

void SSyntacticAnalyzer::generateActionGotoTables() {
    if (ultimate_situations_set_.isEmpty()) return;

    QSet<Token> terminals = getAllTerminalTokens();
    QSet<Token> non_terminals = getAllNonTerminalTokens();

    foreach (const QSet<Situation> &i, ultimate_situations_set_) {
        QHash<Token, Action> action_row;
        QHash<Token, int> goto_row;

        foreach (const Situation &situation, i) {
            // action
            foreach (const Token &terminal, terminals) {
                int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
                // step 1
                if ((dot_pos > -1)
                    && (dot_pos < situation.right_side.length() - 1)
                    && (situation.right_side.at(dot_pos + 1) == terminal)
                ) {
                    QSet<Situation> test_i = makeStep(i, terminal);
                    int j = ultimate_situations_set_.indexOf(test_i);
                    if (j > -1) {
                        Action new_action = {A_SHIFT, j};
                        if (action_row.contains(terminal)) {
                            // error - not LR(1)
                        } else {
                            action_row.insert(terminal, new_action);
                        }
                    }
                }
                // step 2
                if ((dot_pos == situation.right_side.length() - 1)
                    && (situation.left_token != N_S)
                    && (situation.look_ahead_token == terminal)
                ) {
                    int j = indexOfGrammarRule(
                                situation.left_token,
                                situation.right_side.mid(0, situation.right_side.length() - 1)
                            );
                    if (j > -1) {
                        Action new_action = {A_REDUCE, j};
                        if (action_row.contains(terminal)) {
                            // error - not LR(1)
                        } else {
                            action_row.insert(terminal, new_action);
                        }
                    } else {
                        // error - grammar rule not found
                    }
                }
            }
            // step 3
            if ((situation.left_token == N_S)
                && (situation.right_side == (EmptyTokenList() << N_E << DOT_TOKEN))
                && (situation.look_ahead_token == EOF_TOKEN)
            ) {
                Action new_action = {A_ACCEPT, 0};
                if (action_row.contains(EOF_TOKEN)) {
                    // error
                } else {
                    action_row.insert(EOF_TOKEN, new_action);
                }
            }

            // goto
            foreach (const Token &non_terminal, non_terminals) {
                QSet<Situation> test_i = makeStep(i, non_terminal);
                int j = ultimate_situations_set_.indexOf(test_i);
                if (j > -1) {
                    int new_goto = j;
                    if (goto_row.contains(non_terminal)) {
                        // error
                    } else {
                        goto_row.insert(non_terminal, new_goto);
                    }
                }
            }
        }
        action_table_ << action_row;
        goto_table_ << goto_row;
    }
}

QList<int> SSyntacticAnalyzer::process(QList<TokenPointer> tokens) {
    QList<int> result;
    if (Grammar.isEmpty() || action_table_.isEmpty() || goto_table_.isEmpty()) return result;

    int i = 0;
    bool tokens_accepted = false;
    while (!tokens_accepted && (i < tokens.length())) {
        int state = states_stack_.last();
        Token token = tokens.at(i).type;

        if (action_table_.at(state).contains(token)) {
            Action action = action_table_.at(state).value(token);
            GrammarRule rule;

            switch (action.type) {
            case A_SHIFT:
                states_stack_ << action.index;
                tokens_stack_ << token;
                i++;
                break;

            case A_REDUCE:
                rule = Grammar.at(action.index);
                if ((states_stack_.length() > rule.right_side.length())
                     && (tokens_stack_.length() >= rule.right_side.length())
                ) {
                    for (int num = 0; num < rule.right_side.length(); num++) {
                        states_stack_.removeLast();
                        tokens_stack_.removeLast();
                    }
                    if (goto_table_.at(states_stack_.last()).contains(rule.left_token)) {
                        int goto_index = goto_table_.at(states_stack_.last()).value(rule.left_token);
                        states_stack_ << goto_index;
                        tokens_stack_ << rule.left_token;
                    } else {
                        // error - goto not defined
                    }
                } else {
                    // error - stacks have too few items
                }
                break;

            case A_ACCEPT:
                tokens_accepted = true;
                break;
            }
        } else {
            // error - action not defined
        }
    }
    if (i >= tokens.length()) {
        // error - tokens not accepted
    }

    return result;
}
