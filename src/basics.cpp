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
            s = QString::fromUtf8("<program>"); break;
        case N_PROG_ELEMENT:
            s = QString::fromUtf8("<program_element>"); break;
        case N_CLASS:
            s = QString::fromUtf8("<class>"); break;
        case N_CLASS_BODY:
            s = QString::fromUtf8("<class_body>"); break;
        case N_CLASS_ELEMENT:
            s = QString::fromUtf8("<class_element>"); break;
        case N_ACCESS_SPEC:
            s = QString::fromUtf8("<access_spec>"); break;
        case N_CLASS_METHOD:
            s = QString::fromUtf8("<class_method>"); break;
        case N_CLASS_METHOD_IMPLEMENTATION:
            s = QString::fromUtf8("<class_method_impl>"); break;
        case N_FUNCTION:
            s = QString::fromUtf8("<function>"); break;
        case N_ARGUMENTS_LIST:
            s = QString::fromUtf8("<arguments_list>"); break;
        case N_ARGUMENT:
            s = QString::fromUtf8("<argument>"); break;
        case N_BLOCK:
            s = QString::fromUtf8("<block>"); break;
        case N_BLOCK_BODY:
            s = QString::fromUtf8("<block_body>"); break;
        case N_BLOCK_ELEMENT:
            s = QString::fromUtf8("<block_element>"); break;
        case N_RETURN:
            s = QString::fromUtf8("<return>"); break;
        case N_EXPRESSION:
            s = QString::fromUtf8("<expression>"); break;
        case N_OP_1:
            s = QString::fromUtf8("<operator_1>"); break;
        case N_OP_2:
            s = QString::fromUtf8("<operator_2>"); break;
        case N_OP_3:
            s = QString::fromUtf8("<operator_3>"); break;
        case N_OP_4:
            s = QString::fromUtf8("<operator_4>"); break;
        case N_OP_5:
            s = QString::fromUtf8("<operator_5>"); break;
        case N_OP_6:
            s = QString::fromUtf8("<operator_6>"); break;
        case N_OP_7:
            s = QString::fromUtf8("<operator_7>"); break;
        case N_OP_8:
            s = QString::fromUtf8("<operator_8>"); break;
        case N_OP_9:
            s = QString::fromUtf8("<operator_9>"); break;
        case N_FUNC_CALL:
            s = QString::fromUtf8("<function_call>"); break;
        case N_OBJECT:
            s = QString::fromUtf8("<object>"); break;
        case N_CLASS_PROPS_DECLARATION:
            s = QString::fromUtf8("<class_props_declaration>"); break;
        case N_VAR_DECLARATION:
            s = QString::fromUtf8("<vars_declaration>"); break;
        case N_VAR_TYPE:
            s = QString::fromUtf8("<var_type>"); break;
        case N_CLASS_PROPS_LIST:
            s = QString::fromUtf8("<class_props_list>"); break;
        case N_VARS_LIST:
            s = QString::fromUtf8("<vars_list>"); break;
        case N_LOOP:
            s = QString::fromUtf8("<loop>"); break;
        case N_WHILE_LOOP:
            s = QString::fromUtf8("<while-loop>"); break;
        case N_DO_WHILE_LOOP:
            s = QString::fromUtf8("<do-while-loop>"); break;
        case N_FOR_LOOP:
            s = QString::fromUtf8("<for-loop>"); break;
        case N_BRANCHING:
            s = QString::fromUtf8("<branching>"); break;
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
    QStringList list;

    switch (type) {
    case SYM_FUNCTION:
        foreach (int arg_index, args_indexes) {
            list << "#" + QString::number(arg_index);
        }
        res = "[FUNC]";
        if (access_type == ACCESS_PUBLIC) {
            res += " public";
        } else if (access_type == ACCESS_PRIVATE) {
            res += " private";
        }
        res += QString(" %1 %2(%3)").arg(dataTypeToString(data_type)).arg(name).arg(list.join(", "));
        break;

    case SYM_ARGUMENT:
        switch (arg_type) {
        case ARG_BY_VALUE:
            res = QString("%1 %2").arg(dataTypeToString(data_type, class_index)).arg(name);
            break;
        case ARG_BY_REFERENCE:
            res = QString("%1& %2").arg(dataTypeToString(data_type, class_index)).arg(name);
            break;
        }
        if (is_const) {
            res = "const " + res;
        }
        res = "[ARG] " + res;
        break;

    case SYM_VARIABLE:
        res = "[VAR]";
        if (access_type == ACCESS_PUBLIC) {
            res += " public";
        } else if (access_type == ACCESS_PRIVATE) {
            res += " private";
        }
        if (is_const) {
            res += " const";
        }
        res += QString(" %1 %2").arg(dataTypeToString(data_type, class_index)).arg(name);
        break;

    case SYM_CLASS:
        foreach (int members_index, members_indexes) {
            list << "#" + QString::number(members_index);
        }
        res = QString("[CLASS] %1 {%2 }").arg(name).arg(" " + list.join(", "));
        break;

    default:
        res = "(todo)";
        break;
    }

    return res;
}
QString Block::toString() const {
    QString res;
    QStringList declared_symbols;

    foreach (int declared_symbols_index, declared_symbols_indexes) {
        declared_symbols << "#" + QString::number(declared_symbols_index);
    }

    res = QString("{ parent: #%1, declared_symbols: [%2] }")
            .arg(parent_block_index)
            .arg(declared_symbols.join(", "));
    if (parent_block_index == -1) {
        res += " - global";
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

