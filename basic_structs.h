#ifndef BASIC_STRUCTS_H
#define BASIC_STRUCTS_H

#include <QtCore>

enum TokenType {T_ID, T_CONST, T_KEYWORD, T_SEPARATOR};
enum ConstType {CONST_BOOL, CONST_INT, CONST_DOUBLE, CONST_CHAR, CONST_STRING};

enum Keyword {K_IF, K_ELSE, K_FOR, K_WHILE, K_DO, K_RETURN, K_BREAK, K_CONTINUE,
               K_CHAR, K_INT, K_DOUBLE, K_BOOL, K_VOID, K_CONST, K_TRUE, K_FALSE,
               K_CLASS, K_PUBLIC, K_PRIVATE};
enum Separator {S_PLUS, S_MINUS, S_MULT, S_DIV, S_NOT, S_MOD, S_AMP, S_OR, S_VBAR,
                 S_LESS, S_GREATER, S_EQUAL, S_ROUND_OPEN, S_ROUND_CLOSE, S_CURLY_OPEN,
                 S_CURLY_CLOSE, S_SEMICOLON, S_COMMA, S_PERIOD, S_COLON, S_APOSTROPHE,
                 S_DOUBLE_QUOTE, S_TILDE};

static QHash<QString, Keyword> initKeywordValues() {
        QHash<QString, Keyword> hash;
        hash.insert("if", K_IF);
        hash.insert("else", K_ELSE);
        hash.insert("for", K_FOR);
        hash.insert("while", K_WHILE);
        hash.insert("do", K_DO);
        hash.insert("return", K_RETURN);
        hash.insert("break", K_BREAK);
        hash.insert("continue", K_CONTINUE);
        hash.insert("char", K_CHAR);
        hash.insert("int", K_INT);
        hash.insert("double", K_DOUBLE);
        hash.insert("bool", K_BOOL);
        hash.insert("void", K_VOID);
        hash.insert("const", K_CONST);
        hash.insert("true", K_TRUE);
        hash.insert("false", K_FALSE);
        hash.insert("class", K_CLASS);
        hash.insert("public", K_PUBLIC);
        hash.insert("private", K_PRIVATE);
        return hash;
}
static const QHash<QString, Keyword> KeywordCodes = initKeywordValues();

static QHash<QString, Separator> initSeparatorValues() {
    QHash<QString, Separator> hash;
    hash.insert("+", S_PLUS);
    hash.insert("-", S_MINUS);
    hash.insert("*", S_MULT);
    hash.insert("/", S_DIV);
    hash.insert("!", S_NOT);
    hash.insert("%", S_MOD);
    hash.insert("&", S_AMP);
    hash.insert("|", S_VBAR);
    hash.insert("<", S_LESS);
    hash.insert(">", S_GREATER);
    hash.insert("=", S_EQUAL);
    hash.insert("(", S_ROUND_OPEN);
    hash.insert(")", S_ROUND_CLOSE);
    hash.insert("{", S_CURLY_OPEN);
    hash.insert("}", S_CURLY_CLOSE);
    hash.insert(";", S_SEMICOLON);
    hash.insert(",", S_COMMA);
    hash.insert(".", S_PERIOD);
    hash.insert(":", S_COLON);
    hash.insert("'", S_APOSTROPHE);
    hash.insert("\"", S_DOUBLE_QUOTE);
    hash.insert("~", S_TILDE);
    return hash;
}
static const QHash<QString, Separator> SeparatorCodes = initSeparatorValues();

struct Table_item_id {
    int start, end;
    QString identifier;
};
struct Table_item_const {
    int start, end;
    ConstType type;
    QVariant value;
};
struct Table_item_keyword {
    int start, end;
    Keyword type;
};
struct Table_item_separator {
    int start, end;
    Separator type;
};

#endif // BASIC_STRUCTS_H
