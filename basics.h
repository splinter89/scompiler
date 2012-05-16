#ifndef BASICS_H
#define BASICS_H

#include <QtCore>
#include <QDebug>

enum Token {
    UNKNOWN,        // MUST BE FIRST(!) - left tokens border
    LAMBDA,         // empty token
    DOT_TOKEN,      // separator in LR-situation

    // terminals
    T_ID, T_CONST, T_KEYWORD, T_SEPARATOR,

    // keywords
    K_IF, K_ELSE, K_FOR, K_WHILE, K_DO, K_RETURN,
    K_CHAR, K_INT, K_DOUBLE, K_BOOL, K_VOID, K_CONST, /*K_TRUE, K_FALSE,*/
    K_CLASS, K_PUBLIC, K_PRIVATE,

    // separators (1x)
    S_SPACE, S_PLUS, S_MINUS, S_MULT, S_DIV, S_NOT, S_MOD, S_AMP,
    S_LESS, S_GREATER, S_ASSIGN, S_ROUND_OPEN, S_ROUND_CLOSE, S_CURLY_OPEN,
    S_CURLY_CLOSE, S_SEMICOLON, S_COMMA, S_PERIOD, S_COLON, S_TILDE,
    // separators (2x)
    S_DECREMENT, S_INCREMENT, S_AND, S_OR, S_LE, S_GE, S_EQ, S_NOT_EQ,
    S_MULT_ASSIGN, S_DIV_ASSIGN, S_MOD_ASSIGN, S_ADD_ASSIGN, S_SUB_ASSIGN,
    S_ARROW, S_SCOPE,

    // non-terminals
    N_S,
    N_PROGRAM,
    N_PROG_ELEMENT,
    N_CLASS,
    N_CLASS_BODY,
    N_CLASS_ELEMENT,
    N_ACCESS_SPEC,
    N_CLASS_METHOD,
    N_CLASS_METHOD_IMPLEMENTATION,
    N_FUNCTION,
    N_ARGUMENTS_LIST,
    N_ARGUMENT,
    N_BLOCK,
    N_BLOCK_BODY,
    N_BLOCK_ELEMENT,
    N_RETURN,
    N_EXPRESSION,
    N_OP_1,
    N_OP_2,
    N_OP_3,
    N_OP_4,
    N_OP_5,
    N_OP_6,
    N_OP_7,
    N_OP_8,
    N_OP_9,
    N_FUNC_CALL,
    N_OBJECT,
    N_CLASS_PROPS_DECLARATION,
    N_VAR_DECLARATION,
    N_VAR_TYPE,
    N_CLASS_PROPS_LIST,
    N_VARS_LIST,
    N_LOOP,
    N_WHILE_LOOP,
    N_DO_WHILE_LOOP,
    N_FOR_LOOP,
    N_BRANCHING,

    EOF_TOKEN      // end of input string (MUST BE LAST(!)) - right tokens border
};
enum DataType {TYPE_BOOL, TYPE_INT, TYPE_DOUBLE, TYPE_CHAR, TYPE_STRING, TYPE_VOID, TYPE_OBJECT};
enum AccessSpecifier {ACCESS_PUBLIC, ACCESS_PRIVATE};
enum ActionType {A_SHIFT, A_REDUCE, A_ACCEPT};
enum SymbolType {SYM_CLASS, SYM_FUNCTION, SYM_ARGUMENT, SYM_VARIABLE};
enum ArgType {ARG_BY_VALUE, ARG_BY_REFERENCE};


// items of token tables
struct TokenId {
    QString name;
};
struct TokenConst {
    DataType type;
    QVariant value;
};
struct TokenKeyword {
    Token type;
};
struct TokenSeparator {
    Token type;
};

// lexical convolution item
struct TokenPointer {
    Token type;
    int index;
    int start, length;

    QString toString() const;
};

// [A -> B]
struct GrammarRule {
    Token left_token;
    QList<Token> right_side;

    GrammarRule() {}
    GrammarRule(Token left, QList<Token> right) { left_token = left; right_side = right; }
    QString toString() const;
};

// [A -> .B, a]
struct Situation {
    Token left_token;
    QList<Token> right_side;
    Token look_ahead_token;

    QString toString() const;
};

struct Action {
    ActionType type;
    int index;

    QString toString() const;
};

struct Symbol {
    QString name;
    SymbolType type;        // class/function/...
    DataType data_type;     // bool/int/...

    // for classes
    QList<int> properties_indexes;
    QList<int> methods_indexes;

    // for class members
    int class_index;        // also for objects, i.e. args & vars ("class_name var1;")
    AccessSpecifier access_type;

    // for functions
    QList<int> args_indexes;

    // for function arguments
    ArgType arg_type;
    bool is_const;

    // for variables --- mb Value?
//    bool is_defined;        // "a = ..." is definition

    QString toString() const;
};

struct Block {
    int parent_block_index;
    QList<int> declared_symbols_indexes;

    QString toString() const;
};


static QHash<QString, Token> initKeywordCodes() {
    QHash<QString, Token> hash;

    hash.insert("if", K_IF);
    hash.insert("else", K_ELSE);
    hash.insert("for", K_FOR);
    hash.insert("while", K_WHILE);
    hash.insert("do", K_DO);
    hash.insert("return", K_RETURN);
    hash.insert("char", K_CHAR);
    hash.insert("int", K_INT);
    hash.insert("double", K_DOUBLE);
    hash.insert("bool", K_BOOL);
    hash.insert("void", K_VOID);
    hash.insert("const", K_CONST);
//    hash.insert("true", K_TRUE);
//    hash.insert("false", K_FALSE);
    hash.insert("class", K_CLASS);
    hash.insert("public", K_PUBLIC);
    hash.insert("private", K_PRIVATE);

    return hash;
}
static const QHash<QString, Token> KeywordCodes = initKeywordCodes();

static QHash<QString, Token> initSeparatorCodes() {
    QHash<QString, Token> hash;

    hash.insert(" ", S_SPACE);
    hash.insert("+", S_PLUS);
    hash.insert("-", S_MINUS);
    hash.insert("*", S_MULT);
    hash.insert("/", S_DIV);
    hash.insert("!", S_NOT);
    hash.insert("%", S_MOD);
    hash.insert("&", S_AMP);
    hash.insert("<", S_LESS);
    hash.insert(">", S_GREATER);
    hash.insert("=", S_ASSIGN);
    hash.insert("(", S_ROUND_OPEN);
    hash.insert(")", S_ROUND_CLOSE);
    hash.insert("{", S_CURLY_OPEN);
    hash.insert("}", S_CURLY_CLOSE);
    hash.insert(";", S_SEMICOLON);
    hash.insert(",", S_COMMA);
    hash.insert(".", S_PERIOD);
    hash.insert(":", S_COLON);
    hash.insert("~", S_TILDE);

    hash.insert("--", S_DECREMENT);
    hash.insert("++", S_INCREMENT);
    hash.insert("&&", S_AND);
    hash.insert("||", S_OR);
    hash.insert("<=", S_LE);
    hash.insert(">=", S_GE);
    hash.insert("==", S_EQ);
    hash.insert("!=", S_NOT_EQ);
    hash.insert("*=", S_MULT_ASSIGN);
    hash.insert("/=", S_DIV_ASSIGN);
    hash.insert("%=", S_MOD_ASSIGN);
    hash.insert("+=", S_ADD_ASSIGN);
    hash.insert("-=", S_SUB_ASSIGN);
    hash.insert("->", S_ARROW);
    hash.insert("::", S_SCOPE);

    return hash;
}
static const QHash<QString, Token> SeparatorCodes = initSeparatorCodes();

static QList<Token> EmptyTokenList() {
    QList<Token> list;
    return list;
}
static QList<GrammarRule> initGrammarFullRules() {
    QList<GrammarRule> list;

    // grammar rules here
    list << GrammarRule(N_S, EmptyTokenList() << N_PROGRAM)   // additional rule

         << GrammarRule(N_PROGRAM, EmptyTokenList() << N_PROG_ELEMENT)
         << GrammarRule(N_PROGRAM, EmptyTokenList() << N_PROGRAM << N_PROG_ELEMENT)

         << GrammarRule(N_PROG_ELEMENT, EmptyTokenList() << N_CLASS)
         << GrammarRule(N_PROG_ELEMENT, EmptyTokenList() << N_CLASS_METHOD_IMPLEMENTATION)
         << GrammarRule(N_PROG_ELEMENT, EmptyTokenList() << N_VAR_DECLARATION)
         << GrammarRule(N_PROG_ELEMENT, EmptyTokenList() << N_FUNCTION)

         << GrammarRule(N_CLASS, EmptyTokenList() << K_CLASS << S_SPACE << T_ID << S_CURLY_OPEN << S_CURLY_CLOSE << S_SEMICOLON)
         << GrammarRule(N_CLASS, EmptyTokenList() << K_CLASS << S_SPACE << T_ID << S_CURLY_OPEN << N_CLASS_BODY << S_CURLY_CLOSE << S_SEMICOLON)

         << GrammarRule(N_CLASS_BODY, EmptyTokenList() << N_CLASS_ELEMENT)
         << GrammarRule(N_CLASS_BODY, EmptyTokenList() << N_CLASS_BODY << N_CLASS_ELEMENT)

         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_CLASS_PROPS_DECLARATION)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_CLASS_METHOD)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_CLASS_ELEMENT << N_CLASS_PROPS_DECLARATION)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_CLASS_ELEMENT << N_CLASS_METHOD)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_ACCESS_SPEC << S_COLON)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_ACCESS_SPEC << S_COLON << N_CLASS_PROPS_DECLARATION)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_ACCESS_SPEC << S_COLON << N_CLASS_METHOD)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_ACCESS_SPEC << S_COLON << N_CLASS_ELEMENT << N_CLASS_PROPS_DECLARATION)
         << GrammarRule(N_CLASS_ELEMENT, EmptyTokenList() << N_ACCESS_SPEC << S_COLON << N_CLASS_ELEMENT << N_CLASS_METHOD)

         << GrammarRule(N_ACCESS_SPEC, EmptyTokenList() << K_PUBLIC)
         << GrammarRule(N_ACCESS_SPEC, EmptyTokenList() << K_PRIVATE)

         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << K_VOID << S_SPACE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << K_VOID << S_SPACE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << S_TILDE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD, EmptyTokenList() << S_TILDE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)

         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << T_ID << S_SCOPE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << T_ID << S_SCOPE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID << S_SCOPE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID << S_SCOPE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << K_VOID << S_SPACE << T_ID << S_SCOPE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << K_VOID << S_SPACE << T_ID << S_SCOPE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << T_ID << S_SCOPE << S_TILDE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_CLASS_METHOD_IMPLEMENTATION, EmptyTokenList() << T_ID << S_SCOPE << S_TILDE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)

         << GrammarRule(N_FUNCTION, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_FUNCTION, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_FUNCTION, EmptyTokenList() << K_VOID << S_SPACE << T_ID << S_ROUND_OPEN << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_FUNCTION, EmptyTokenList() << K_VOID << S_SPACE << T_ID << S_ROUND_OPEN << N_ARGUMENTS_LIST << S_ROUND_CLOSE << N_BLOCK)

         << GrammarRule(N_ARGUMENTS_LIST, EmptyTokenList() << N_ARGUMENT)
         << GrammarRule(N_ARGUMENTS_LIST, EmptyTokenList() << N_ARGUMENTS_LIST << S_COMMA << N_ARGUMENT)

         << GrammarRule(N_ARGUMENT, EmptyTokenList() << N_VAR_TYPE << S_SPACE << T_ID)
         << GrammarRule(N_ARGUMENT, EmptyTokenList() << K_CONST << S_SPACE << N_VAR_TYPE << S_SPACE << T_ID)
         << GrammarRule(N_ARGUMENT, EmptyTokenList() << N_VAR_TYPE << S_AMP << T_ID)
         << GrammarRule(N_ARGUMENT, EmptyTokenList() << K_CONST << S_SPACE << N_VAR_TYPE << S_AMP << T_ID)

         << GrammarRule(N_BLOCK, EmptyTokenList() << S_CURLY_OPEN << S_CURLY_CLOSE)
         << GrammarRule(N_BLOCK, EmptyTokenList() << S_CURLY_OPEN << N_BLOCK_BODY << S_CURLY_CLOSE)

         << GrammarRule(N_BLOCK_BODY, EmptyTokenList() << N_BLOCK_ELEMENT)
         << GrammarRule(N_BLOCK_BODY, EmptyTokenList() << N_BLOCK_BODY << N_BLOCK_ELEMENT)

         << GrammarRule(N_BLOCK_ELEMENT, EmptyTokenList() << N_VAR_DECLARATION)
         << GrammarRule(N_BLOCK_ELEMENT, EmptyTokenList() << S_SEMICOLON)
         << GrammarRule(N_BLOCK_ELEMENT, EmptyTokenList() << N_EXPRESSION << S_SEMICOLON)
         << GrammarRule(N_BLOCK_ELEMENT, EmptyTokenList() << N_RETURN << S_SEMICOLON)
         << GrammarRule(N_BLOCK_ELEMENT, EmptyTokenList() << N_LOOP)
         << GrammarRule(N_BLOCK_ELEMENT, EmptyTokenList() << N_BRANCHING)

         << GrammarRule(N_RETURN, EmptyTokenList() << K_RETURN)
         << GrammarRule(N_RETURN, EmptyTokenList() << K_RETURN << S_SPACE << N_EXPRESSION)

         << GrammarRule(N_EXPRESSION, EmptyTokenList() << N_OP_1)
         << GrammarRule(N_EXPRESSION, EmptyTokenList() << N_EXPRESSION << S_COMMA << N_OP_1)

         << GrammarRule(N_OP_1, EmptyTokenList() << N_OP_2)
         << GrammarRule(N_OP_1, EmptyTokenList() << N_OBJECT << S_ASSIGN << N_OP_1)
         << GrammarRule(N_OP_1, EmptyTokenList() << N_OBJECT << S_ADD_ASSIGN << N_OP_1)
         << GrammarRule(N_OP_1, EmptyTokenList() << N_OBJECT << S_SUB_ASSIGN << N_OP_1)
         << GrammarRule(N_OP_1, EmptyTokenList() << N_OBJECT << S_MULT_ASSIGN << N_OP_1)
         << GrammarRule(N_OP_1, EmptyTokenList() << N_OBJECT << S_DIV_ASSIGN << N_OP_1)
         << GrammarRule(N_OP_1, EmptyTokenList() << N_OBJECT << S_MOD_ASSIGN << N_OP_1)

         << GrammarRule(N_OP_2, EmptyTokenList() << N_OP_3)
         << GrammarRule(N_OP_2, EmptyTokenList() << N_OP_2 << S_OR << N_OP_3)

         << GrammarRule(N_OP_3, EmptyTokenList() << N_OP_4)
         << GrammarRule(N_OP_3, EmptyTokenList() << N_OP_3 << S_AND << N_OP_4)

         << GrammarRule(N_OP_4, EmptyTokenList() << N_OP_5)
         << GrammarRule(N_OP_4, EmptyTokenList() << N_OP_4 << S_EQ << N_OP_5)
         << GrammarRule(N_OP_4, EmptyTokenList() << N_OP_4 << S_NOT_EQ << N_OP_5)

         << GrammarRule(N_OP_5, EmptyTokenList() << N_OP_6)
         << GrammarRule(N_OP_5, EmptyTokenList() << N_OP_5 << S_LESS << N_OP_6)
         << GrammarRule(N_OP_5, EmptyTokenList() << N_OP_5 << S_LE << N_OP_6)
         << GrammarRule(N_OP_5, EmptyTokenList() << N_OP_5 << S_GREATER << N_OP_6)
         << GrammarRule(N_OP_5, EmptyTokenList() << N_OP_5 << S_GE << N_OP_6)

         << GrammarRule(N_OP_6, EmptyTokenList() << N_OP_7)
         << GrammarRule(N_OP_6, EmptyTokenList() << N_OP_6 << S_PLUS << N_OP_7)
         << GrammarRule(N_OP_6, EmptyTokenList() << N_OP_6 << S_MINUS << N_OP_7)

         << GrammarRule(N_OP_7, EmptyTokenList() << N_OP_8)
         << GrammarRule(N_OP_7, EmptyTokenList() << N_OP_7 << S_MULT << N_OP_8)
         << GrammarRule(N_OP_7, EmptyTokenList() << N_OP_7 << S_DIV << N_OP_8)
         << GrammarRule(N_OP_7, EmptyTokenList() << N_OP_7 << S_MOD << N_OP_8)

         << GrammarRule(N_OP_8, EmptyTokenList() << N_OP_9)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_PLUS << N_OP_9)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_MINUS << N_OP_9)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_NOT << N_OP_9)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_MULT << N_OP_9)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_INCREMENT << N_OBJECT)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_DECREMENT << N_OBJECT)
         << GrammarRule(N_OP_8, EmptyTokenList() << S_AMP << N_OBJECT)

         << GrammarRule(N_OP_9, EmptyTokenList() << S_ROUND_OPEN << N_EXPRESSION << S_ROUND_CLOSE)
         << GrammarRule(N_OP_9, EmptyTokenList() << T_CONST)
         << GrammarRule(N_OP_9, EmptyTokenList() << N_OBJECT)
         << GrammarRule(N_OP_9, EmptyTokenList() << N_OBJECT << S_INCREMENT)
         << GrammarRule(N_OP_9, EmptyTokenList() << N_OBJECT << S_DECREMENT)
         << GrammarRule(N_OP_9, EmptyTokenList() << N_FUNC_CALL)

         << GrammarRule(N_FUNC_CALL, EmptyTokenList() << N_OBJECT << S_ROUND_OPEN << S_ROUND_CLOSE)
         << GrammarRule(N_FUNC_CALL, EmptyTokenList() << N_OBJECT << S_ROUND_OPEN << N_EXPRESSION << S_ROUND_CLOSE)

         << GrammarRule(N_OBJECT, EmptyTokenList() << T_ID)
         << GrammarRule(N_OBJECT, EmptyTokenList() << T_ID << S_PERIOD << T_ID)
         << GrammarRule(N_OBJECT, EmptyTokenList() << T_ID << S_ARROW << T_ID)

         << GrammarRule(N_CLASS_PROPS_DECLARATION, EmptyTokenList() << N_VAR_TYPE << S_SPACE << N_CLASS_PROPS_LIST << S_SEMICOLON)
         << GrammarRule(N_CLASS_PROPS_DECLARATION, EmptyTokenList() << K_CONST << S_SPACE << N_VAR_TYPE << S_SPACE << N_CLASS_PROPS_LIST << S_SEMICOLON)

         << GrammarRule(N_VAR_DECLARATION, EmptyTokenList() << N_VAR_TYPE << S_SPACE << N_VARS_LIST << S_SEMICOLON)
         << GrammarRule(N_VAR_DECLARATION, EmptyTokenList() << N_VAR_TYPE << S_MULT << N_VARS_LIST << S_SEMICOLON)
         << GrammarRule(N_VAR_DECLARATION, EmptyTokenList() << K_CONST << S_SPACE << N_VAR_TYPE << S_SPACE << N_VARS_LIST << S_SEMICOLON)
         << GrammarRule(N_VAR_DECLARATION, EmptyTokenList() << K_CONST << S_SPACE << N_VAR_TYPE << S_MULT << N_VARS_LIST << S_SEMICOLON)

         << GrammarRule(N_VAR_TYPE, EmptyTokenList() << K_INT)
         << GrammarRule(N_VAR_TYPE, EmptyTokenList() << K_DOUBLE)
         << GrammarRule(N_VAR_TYPE, EmptyTokenList() << K_CHAR)
         << GrammarRule(N_VAR_TYPE, EmptyTokenList() << K_BOOL)
         << GrammarRule(N_VAR_TYPE, EmptyTokenList() << T_ID)

         << GrammarRule(N_CLASS_PROPS_LIST, EmptyTokenList() << T_ID)
         << GrammarRule(N_CLASS_PROPS_LIST, EmptyTokenList() << N_CLASS_PROPS_LIST << S_COMMA << T_ID)

         << GrammarRule(N_VARS_LIST, EmptyTokenList() << T_ID)
         << GrammarRule(N_VARS_LIST, EmptyTokenList() << N_VARS_LIST << S_COMMA << T_ID)
         << GrammarRule(N_VARS_LIST, EmptyTokenList() << N_VARS_LIST << S_COMMA << S_MULT << T_ID)
         << GrammarRule(N_VARS_LIST, EmptyTokenList() << T_ID << S_ASSIGN << N_OP_1)
         << GrammarRule(N_VARS_LIST, EmptyTokenList() << N_VARS_LIST << S_COMMA << T_ID << S_ASSIGN << N_OP_1)
         << GrammarRule(N_VARS_LIST, EmptyTokenList() << N_VARS_LIST << S_COMMA << S_MULT << T_ID << S_ASSIGN << N_OP_1)

         << GrammarRule(N_LOOP, EmptyTokenList() << N_WHILE_LOOP)
         << GrammarRule(N_LOOP, EmptyTokenList() << N_DO_WHILE_LOOP)
         << GrammarRule(N_LOOP, EmptyTokenList() << N_FOR_LOOP)

         << GrammarRule(N_WHILE_LOOP, EmptyTokenList() << K_WHILE << S_ROUND_OPEN << N_EXPRESSION << S_ROUND_CLOSE << N_BLOCK)

         << GrammarRule(N_DO_WHILE_LOOP, EmptyTokenList() << K_DO << N_BLOCK << K_WHILE << S_ROUND_OPEN << N_EXPRESSION << S_ROUND_CLOSE << S_SEMICOLON)

         << GrammarRule(N_FOR_LOOP, EmptyTokenList() << K_FOR << S_ROUND_OPEN << N_EXPRESSION << S_SEMICOLON << N_EXPRESSION << S_SEMICOLON << N_EXPRESSION << S_ROUND_CLOSE << N_BLOCK)

         << GrammarRule(N_BRANCHING, EmptyTokenList() << K_IF << S_ROUND_OPEN << N_EXPRESSION << S_ROUND_CLOSE << N_BLOCK)
         << GrammarRule(N_BRANCHING, EmptyTokenList() << K_IF << S_ROUND_OPEN << N_EXPRESSION << S_ROUND_CLOSE << N_BLOCK << K_ELSE << N_BLOCK)

         << GrammarRule(N_EXPRESSION, EmptyTokenList() << N_OP_9)
    ;

    return list;
}
static const QList<GrammarRule> Grammar_full = initGrammarFullRules();
QList<GrammarRule> setGrammarRules(QList<int> grammar_active_rules);


// cool xD - in order to use QSet<Situation>
inline uint qHash(const Situation &e)
{
    QStringList res;
    res << QString::number(e.left_token) << "+";
    foreach (const Token tok, e.right_side) {
        res << QString::number(tok);
    }
    res << "+" << QString::number(e.look_ahead_token);

    return qHash(res.join("-"));
}

QString tokenToString(const Token token);
QString dataTypeToString(const DataType data_type, int class_index);
QDebug operator<<(QDebug d, const Token token);
QDebug operator<<(QDebug d, const TokenPointer token);
QDebug operator<<(QDebug d, const Action action);
QDebug operator<<(QDebug d, const QList<Token> tokens);
QDebug operator<<(QDebug d, const GrammarRule rule);
bool operator==(const Situation &e1, const Situation &e2);
QDebug operator<<(QDebug d, const Situation situation);
QDebug operator<<(QDebug d, const Symbol symbol);

QList<GrammarRule> getGrammarRulesByLeftToken(Token token, const QList<GrammarRule> &grammar);
int indexOfGrammarRule(Token left, QList<Token> right, const QList<GrammarRule> &grammar);
//GrammarRule getGrammarRule(Token left, QList<Token> right);

bool isTokenTerminal(Token token);
bool isTokenKeyword(Token token);
bool isTokenSeparator(Token token);
bool isTokenNonTerminal(Token token, const QList<GrammarRule> &grammar);

QSet<Token> getAllTerminalTokens();
QSet<Token> getAllNonTerminalTokens(const QList<GrammarRule> &grammar);
QSet<Token> getAllGrammarTokens(const QList<GrammarRule> &grammar);

#endif // BASICS_H
