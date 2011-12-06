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

static QHash<Keyword, QString> initKeywordValues() {
        QHash<Keyword, QString> hash;
        hash.insert(K_IF, "if");
        hash.insert(K_ELSE, "else");
        hash.insert(K_FOR, "for");
        hash.insert(K_WHILE, "while");
        hash.insert(K_DO, "do");
        hash.insert(K_RETURN, "return");
        hash.insert(K_BREAK, "break");
        hash.insert(K_CONTINUE, "continue");
        hash.insert(K_CHAR, "char");
        hash.insert(K_INT, "int");
        hash.insert(K_DOUBLE, "double");
        hash.insert(K_BOOL, "bool");
        hash.insert(K_VOID, "void");
        hash.insert(K_CONST, "const");
        hash.insert(K_TRUE, "true");
        hash.insert(K_FALSE, "false");
        hash.insert(K_CLASS, "class");
        hash.insert(K_PUBLIC, "public");
        hash.insert(K_PRIVATE, "private");
        return hash;
}
static const QHash<Keyword, QString> KeywordCodes = initKeywordValues();

static QHash<Separator, QString> initSeparatorValues() {
    QHash<Separator, QString> hash;
    hash.insert(S_PLUS, "+");
    hash.insert(S_MINUS, "-");
    hash.insert(S_MULT, "*");
    hash.insert(S_DIV, "/");
    hash.insert(S_NOT, "!");
    hash.insert(S_MOD, "%");
    hash.insert(S_AMP, "&");
    hash.insert(S_VBAR, "|");
    hash.insert(S_LESS, "<");
    hash.insert(S_GREATER, ">");
    hash.insert(S_EQUAL, "=");
    hash.insert(S_ROUND_OPEN, "(");
    hash.insert(S_ROUND_CLOSE, ")");
    hash.insert(S_CURLY_OPEN, "{");
    hash.insert(S_CURLY_CLOSE, "}");
    hash.insert(S_SEMICOLON, ";");
    hash.insert(S_COMMA, ",");
    hash.insert(S_PERIOD, ".");
    hash.insert(S_COLON, ":");
    hash.insert(S_APOSTROPHE, "'");
    hash.insert(S_DOUBLE_QUOTE, "\"");
    hash.insert(S_TILDE, "~");
    return hash;
}
static const QHash<Separator, QString> SeparatorCodes = initSeparatorValues();

struct Table_item_id {
    int start, end;
    QString code;
};
struct Table_item_const {
    int start, end;
    QString code;
    ConstType type;
    QVariant value;
};
struct Table_item_keyword {
    int start, end;
    QString code;
};
struct Table_item_separator {
    int start, end;
    QString code;
};

#endif // BASIC_STRUCTS_H
