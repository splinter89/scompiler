#include "basics.h"
#include <QDebug>

QDebug operator<<(QDebug d, const QList<Token> tokens) {
    QString s;
    foreach (const Token token, tokens) {
        s += (!s.isEmpty()?", ":"") + QString::number(token);
    }
    d << s;
    return d;
}
QDebug operator<<(QDebug d, const QList<GrammarRule> grammar) {
    qDebug() << "Grammar:";
    foreach (const GrammarRule &rule, grammar) {
        qDebug() << rule.left_token << "->" << rule.right_side;
    }
    return d;
}

QList<GrammarRule> getGrammarRulesByLeftToken(Token token) {
    QList<GrammarRule> result;
    for (int i = 0; i < Grammar.length(); i++) {
        if (Grammar.at(i).left_token == token) {
            result << Grammar.at(i);
        }
    }
    return result;
}

int indexOfGrammarRule(Token left, QList<Token> right) {
    int result = -1;
    QList<GrammarRule> rules = getGrammarRulesByLeftToken(left);
    for (int i = 0; i < rules.length(); i++) {
        if (rules.at(i).right_side == right) {
            result = i;
            break;
        }
    }
    return result;
}

GrammarRule getGrammarRule(Token left, QList<Token> right) {
    GrammarRule result;
    int index = indexOfGrammarRule(left, right);
    if (index > -1) {
        result = Grammar.at(index);
    }
    return result;
}

int indexOfSituation(Situation situation, QList<Situation> i) {
    int result = -1;
    for (int num = 0; num < i.length(); num++) {
        if (i.at(num) == situation) {
            result = num;
            break;
        }
    }
    return result;
}

int indexOfSetOfSituations(QList<Situation> i, QList<QList<Situation> > c) {
    int result = -1;
    for (int num = 0; num < c.length(); num++) {
        if (c.at(num) == i) {
            result = num;
            break;
        }
    }
    return result;
}


bool isTokenTerminal(Token token) {
    QSet<Token> list;
    list /*<< TERMINAL*/
         << T_ID << T_CONST << T_KEYWORD << T_SEPARATOR;
    return (list.contains(token) || isTokenKeyword(token) || isTokenSeparator(token));
}

bool isTokenKeyword(Token token) {
    QSet<Token> list;
    list << T_KEYWORD
         << K_IF << K_ELSE << K_FOR << K_WHILE << K_DO << K_RETURN << K_BREAK
         << K_CONTINUE << K_CHAR << K_INT << K_DOUBLE << K_BOOL << K_VOID
         << K_CONST << K_TRUE << K_FALSE << K_CLASS << K_PUBLIC << K_PRIVATE;
    return list.contains(token);
}

bool isTokenSeparator(Token token) {
    QSet<Token> list;
    list << T_SEPARATOR
         << S_SPACE << S_PLUS << S_MINUS << S_MULT << S_DIV << S_NOT
         << S_MOD << S_AMP << S_LESS << S_GREATER << S_ASSIGN
         << S_ROUND_OPEN << S_ROUND_CLOSE << S_CURLY_OPEN << S_CURLY_CLOSE
         << S_SEMICOLON << S_COMMA << S_PERIOD << S_COLON << S_TILDE
         << S_DECREMENT << S_INCREMENT << S_AND << S_OR << S_LE << S_GE
         << S_EQ << S_NOT_EQ << S_MULT_ASSIGN << S_DIV_ASSIGN << S_MOD_ASSIGN
         << S_ADD_ASSIGN << S_SUB_ASSIGN << S_ARROW << S_SCOPE;
    return list.contains(token);
}

bool isTokenNonTerminal(Token token) {
    QSet<Token> list;
    list /*<< NON_TERMINAL*/
//         << N_S << N_E << N_T << N_F;
         << N_S << N_E << N_E1 << N_T << N_T1 << N_F;
    return list.contains(token);
}

QSet<Token> getAllTerminalTokens() {
    QSet<Token> result;
    for (int token = UNKNOWN; token != EOF_TOKEN; token++) {
        if ((token == T_ID) || (token == T_CONST) || (token == EOF_TOKEN)
            || isTokenKeyword((Token)token)
            || isTokenSeparator((Token)token)
        ) {
            result << (Token)token;
        }
    }

    return result;
}
QSet<Token> getAllNonTerminalTokens() {
    QSet<Token> result;
    for (int token = UNKNOWN; token != EOF_TOKEN; token++) {
        if (isTokenNonTerminal((Token)token)) {
            result << (Token)token;
        }
    }

    return result;
}

QSet<Token> getAllGrammarTokens() {
    return getAllTerminalTokens() + getAllNonTerminalTokens();
}
