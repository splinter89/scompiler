#include "basics.h"
#include <QDebug>

QString tokenToString(const Token token) {
    QString s;

    if (KeywordCodes.key(token).length() > 0) {
        s = KeywordCodes.key(token);
    } else if (SeparatorCodes.key(token).length() > 0) {
        s = SeparatorCodes.key(token);
    } else {
        switch (token) {
        case LAMBDA:
            s = "[e]"; break;
        case DOT_TOKEN:
            s = "[.]"; break;
        case EOF_TOKEN:
            s = "[$]"; break;

        case T_ID:
            s = "[id]"; break;
        case T_CONST:
            s = "[const]"; break;

        case N_S:
            s = "S"; break;
        case N_E:
            s = "E"; break;
        case N_T:
            s = "T"; break;
        case N_F:
            s = "F"; break;

        default:
            s = QString::number(token);
            break;
        }
    }
    return s;
}

QDebug operator<<(QDebug d, const Token token) {
    d << tokenToString(token);
    return d;
}
QDebug operator<<(QDebug d, const Action action) {
    QString res;
    switch (action.type) {
    case A_SHIFT:
        res = "S"; break;
    case A_REDUCE:
        res = "R"; break;
    case A_ACCEPT:
        res = "acc"; break;
    }
    if (res != "acc") {
        res += QString::number(action.index);
    }
    d << res;
    return d;
}
QDebug operator<<(QDebug d, const QList<Token> tokens) {
    QStringList s;
    foreach (const Token token, tokens) {
        s << tokenToString(token);
    }
    d << s.join(" ");
    return d;
}
QDebug operator<<(QDebug d, const QList<GrammarRule> grammar) {
    d << "Grammar:\n";
    foreach (const GrammarRule &rule, grammar) {
        d << rule.left_token << "->" << rule.right_side << "\n";
    }
    return d;
}
bool operator==(const Situation &e1, const Situation &e2) {
    return ((e1.left_token == e2.left_token)
        && (e1.right_side == e2.right_side)
        && (e1.look_ahead_token == e2.look_ahead_token));
}
QDebug operator<<(QDebug d, const Situation situation) {
    d << "[" << situation.left_token << "->" << situation.right_side << ", " << situation.look_ahead_token << "]";
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
    for (int i = 0; i < Grammar.length(); i++) {
        if ((Grammar.at(i).left_token == left) && (Grammar.at(i).right_side == right)) {
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


bool isTokenTerminal(Token token) {
    QSet<Token> list;
    list /*<< TERMINAL*/
         << T_ID << T_CONST << T_KEYWORD << T_SEPARATOR << EOF_TOKEN;
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
         << N_S << N_E << N_T << N_F;
    return list.contains(token);
}

QSet<Token> getAllTerminalTokens() {
    QSet<Token> result;
    for (int token = UNKNOWN; token <= EOF_TOKEN; token++) {
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
    for (int token = UNKNOWN; token <= EOF_TOKEN; token++) {
        if (isTokenNonTerminal((Token)token)) {
            result << (Token)token;
        }
    }

    return result;
}

QSet<Token> getAllGrammarTokens() {
    return getAllTerminalTokens() + getAllNonTerminalTokens();
}
