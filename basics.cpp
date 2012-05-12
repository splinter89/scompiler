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
        case N_CLASS_BODY:
            s = QString::fromUtf8("<тело_класса>"); break;
        case N_CLASS_ELEMENT:
            s = QString::fromUtf8("<элемент_класса>"); break;
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
        case N_BLOCK_BODY:
            s = QString::fromUtf8("<тело_блока>"); break;
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
QString dataTypeToString(const DataType data_type, int class_index = 0) {
    QString s;

    switch (data_type) {
    case TYPE_BOOL:
        s = "bool"; break;
    case TYPE_INT:
        s = "int"; break;
    case TYPE_DOUBLE:
        s = "double"; break;
    case TYPE_CHAR:
        s = "char"; break;
    case TYPE_STRING:
        s = "string"; break;
    case TYPE_VOID:
        s = "void"; break;
    case TYPE_OBJECT:
        s = "#" + QString::number(class_index); break;
    default:
        s = "???"; break;
    }

    return s;
}
QString TokenPointer::toString() const {
    QString res, table;
    switch (type) {
    case T_ID:
        table = "i";
        break;
    case T_CONST:
        table = "c";
        break;
    case T_KEYWORD:
        table = "k";
        break;
    case T_SEPARATOR:
        table = "s";
        break;
    default:
        break;
    }

    res = ((type != EOF_TOKEN)
          ? QString("%1[%2]").arg(table).arg(index)
          : QString::number(index));
    return res;
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
    QString res, right;
    foreach (Token token, right_side) {
        right += tokenToString(token);
    }
    res = "[" + tokenToString(left_token) + " -> " + right + ", " + tokenToString(look_ahead_token) + "]";
    return res;
}
QString Symbol::toString() const {
    QString res;
    QStringList args;
    // :TODO: Symbol::toString
    switch (type) {
    case SYM_FUNCTION:
        foreach (int arg_index, args_indexes) {
            args << "#" + QString::number(arg_index);
        }
        res = QString("[FUNC] %1 %2(%3)").arg(dataTypeToString(data_type)).arg(name).arg(args.join(", "));
        break;

    case SYM_ARGUMENT:
        switch (arg_type) {
        case ARG_BY_VALUE:
            res = QString("%1 %2").arg(dataTypeToString(data_type, class_index)).arg(name);
            break;
        case ARG_BY_REFERENCE:
            res = QString("%1 &%2").arg(dataTypeToString(data_type, class_index)).arg(name);
            break;
        }
        if (is_const) {
            res = "const " + res;
        }
        res = "[ARG] " + res;
        break;

    case SYM_VARIABLE:
        res = QString("%1 %2").arg(dataTypeToString(data_type, class_index)).arg(name);
        if (is_const) {
            res = "const " + res;
        }
        res = "[VAR] " + res;
        break;

    default:
        res = "(todo)";
        break;
    }

    return res;
}


QDebug operator<<(QDebug d, const Token token) {
    d << tokenToString(token);
    return d;
}
QDebug operator<<(QDebug d, const TokenPointer token) {
    d << token.toString();
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
QDebug operator<<(QDebug d, const Symbol symbol) {
    d << symbol.toString();
    return d;
}


bool operator==(const Situation &e1, const Situation &e2) {
    return ((e1.left_token == e2.left_token)
        && (e1.right_side == e2.right_side)
        && (e1.look_ahead_token == e2.look_ahead_token));
}


QList<GrammarRule> getGrammarRulesByLeftToken(Token token, const QList<GrammarRule> &grammar) {
    QList<GrammarRule> result;
    for (int i = 0; i < grammar.length(); i++) {
        if (grammar.at(i).left_token == token) {
            result << grammar.at(i);
        }
    }
    return result;
}

int indexOfGrammarRule(Token left, QList<Token> right, const QList<GrammarRule> &grammar) {
    int result = -1;
    for (int i = 0; i < grammar.length(); i++) {
        if ((grammar.at(i).left_token == left) && (grammar.at(i).right_side == right)) {
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

bool isTokenNonTerminal(Token token, const QList<GrammarRule> &grammar) {
    return (getGrammarRulesByLeftToken(token, grammar).length() > 0);
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
QSet<Token> getAllNonTerminalTokens(const QList<GrammarRule> &grammar) {
    QSet<Token> result;
    for (int token = UNKNOWN; token <= EOF_TOKEN; token++) {
        if (isTokenNonTerminal((Token)token, grammar)) {
            result << (Token)token;
        }
    }

    return result;
}

QSet<Token> getAllGrammarTokens(const QList<GrammarRule> &grammar) {
    return getAllTerminalTokens() + getAllNonTerminalTokens(grammar);
}

QList<GrammarRule> setGrammarRules(QList<int> grammar_active_rules) {
    QList<GrammarRule> list;
    for (int i = 0; i < Grammar_full.length(); i++) {
        if (grammar_active_rules.contains(i)) {
            list << Grammar_full.at(i);
        }
    }
    return list;
}

