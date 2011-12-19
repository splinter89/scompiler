#include "basics.h"
#include <QDebug>

QString tokenToString(const Token token) {
    QString s;

    if (token == S_SPACE) {
        s = QString::fromUtf8("[space]");
    } else if (KeywordCodes.key(token).length() > 0) {
        s = KeywordCodes.key(token);
    } else if (SeparatorCodes.key(token).length() > 0) {
        s = SeparatorCodes.key(token);
    } else {
        switch (token) {
        case LAMBDA:
            s = QString::fromUtf8("[e]"); break;
        case DOT_TOKEN:
            s = QString::fromUtf8("[.]"); break;
        case EOF_TOKEN:
            s = QString::fromUtf8("[$]"); break;

        case T_ID:
            s = QString::fromUtf8("[id]"); break;
        case T_CONST:
            s = QString::fromUtf8("[const]"); break;

        case N_S:
            s = QString::fromUtf8("S"); break;
        case N_PROGRAM:
            s = QString::fromUtf8("<программа>"); break;
        case N_PROG_ELEMENT:
            s = QString::fromUtf8("<элемент_программы>"); break;
        case N_CLASS:
            s = QString::fromUtf8("<класс>"); break;
        case N_ACCESS_SPEC:
            s = QString::fromUtf8("<спецификатор_доступа>"); break;
        case N_CLASS_METHOD:
            s = QString::fromUtf8("<метод_класса>"); break;
        case N_CLASS_METHOD_IMPLEMENTATION:
            s = QString::fromUtf8("<реализация_метода_класса>"); break;
        case N_FUNCTION:
            s = QString::fromUtf8("<функция>"); break;
        case N_ARGUMENTS_LIST:
            s = QString::fromUtf8("<список_аргументов>"); break;
        case N_ARGUMENT:
            s = QString::fromUtf8("<аргумент>"); break;
        case N_BLOCK:
            s = QString::fromUtf8("<блок>"); break;
        case N_BLOCK_ELEMENT:
            s = QString::fromUtf8("<элемент_блока>"); break;
        case N_RETURN:
            s = QString::fromUtf8("<возврат>"); break;
        case N_EXPRESSION:
            s = QString::fromUtf8("<выражение>"); break;
        case N_OP_1:
            s = QString::fromUtf8("<оператор_1>"); break;
        case N_OP_2:
            s = QString::fromUtf8("<оператор_2>"); break;
        case N_OP_3:
            s = QString::fromUtf8("<оператор_3>"); break;
        case N_OP_4:
            s = QString::fromUtf8("<оператор_4>"); break;
        case N_OP_5:
            s = QString::fromUtf8("<оператор_5>"); break;
        case N_OP_6:
            s = QString::fromUtf8("<оператор_6>"); break;
        case N_OP_7:
            s = QString::fromUtf8("<оператор_7>"); break;
        case N_OP_8:
            s = QString::fromUtf8("<оператор_8>"); break;
        case N_OP_9:
            s = QString::fromUtf8("<оператор_9>"); break;
        case N_FUNC_CALL:
            s = QString::fromUtf8("<вызов_функции>"); break;
        case N_OBJECT:
            s = QString::fromUtf8("<объект>"); break;
        case N_CLASS_PROPS_DECLARATION:
            s = QString::fromUtf8("<декларация_свойств_класса>"); break;
        case N_VAR_DECLARATION:
            s = QString::fromUtf8("<декларация_переменных>"); break;
        case N_VAR_TYPE:
            s = QString::fromUtf8("<тип_переменной>"); break;
        case N_CLASS_PROPS_LIST:
            s = QString::fromUtf8("<список_свойств_класса>"); break;
        case N_VARS_LIST:
            s = QString::fromUtf8("<список_переменных>"); break;
        case N_LOOP:
            s = QString::fromUtf8("<цикл>"); break;
        case N_WHILE_LOOP:
            s = QString::fromUtf8("<while-цикл>"); break;
        case N_DO_WHILE_LOOP:
            s = QString::fromUtf8("<do-while-цикл>"); break;
        case N_FOR_LOOP:
            s = QString::fromUtf8("<for-цикл>"); break;
        case N_BRANCHING:
            s = QString::fromUtf8("<ветвление>"); break;
        default:
            s = QString::number(token);
            break;
        }
    }
    return s;
}
QString GrammarRule::toString() const {
    QString res, right;
    foreach (Token token, right_side) {
        right += tokenToString(token);
    }
    res = tokenToString(left_token) + " ::= " + right;
    return res;
}
QString Action::toString() const {
    QString res;
    switch (type) {
    case A_SHIFT:
        res = "S"; break;
    case A_REDUCE:
        res = "R"; break;
    case A_ACCEPT:
        res = "acc"; break;
    }
    if (type != A_ACCEPT) {
        res += QString::number(index);
    }
    return res;
}
QString Situation::toString() const {
    QString right;
    foreach (Token token, right_side) {
        right += tokenToString(token);
    }
    return "[" + tokenToString(left_token) + " -> " + right + ", " + tokenToString(look_ahead_token) + "]";
}


QDebug operator<<(QDebug d, const Token token) {
    d << tokenToString(token);
    return d;
}
QDebug operator<<(QDebug d, const GrammarRule rule) {
    d << rule.toString();
    return d;
}
QDebug operator<<(QDebug d, const Action action) {
    d << action.toString();
    return d;
}
QDebug operator<<(QDebug d, const Situation situation) {
    d << situation.toString();
    return d;
}


bool operator==(const Situation &e1, const Situation &e2) {
    return ((e1.left_token == e2.left_token)
        && (e1.right_side == e2.right_side)
        && (e1.look_ahead_token == e2.look_ahead_token));
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


bool isTokenTerminal(Token token) {
    QSet<Token> list;
    list << T_ID << T_CONST << T_KEYWORD << T_SEPARATOR << EOF_TOKEN;
    return (list.contains(token) || isTokenKeyword(token) || isTokenSeparator(token));
}

bool isTokenKeyword(Token token) {
    return ((KeywordCodes.key(token).length() > 0)/* || (token == T_KEYWORD)*/);
}

bool isTokenSeparator(Token token) {
    return ((SeparatorCodes.key(token).length() > 0)/* || (token == T_SEPARATOR)*/);
}

bool isTokenNonTerminal(Token token) {
    return (getGrammarRulesByLeftToken(token).length() > 0);
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
