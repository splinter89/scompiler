#ifndef BASICS_H
#define BASICS_H

#include <QtCore>
#include <QDebug>

enum Token {
    UNKNOWN,        // MUST BE FIRST(!)
    /*TERMINAL, NON_TERMINAL,*/
    LAMBDA,         // empty token
    DOT_TOKEN,      // separator in LR-situation

    // terminals
    T_ID, T_CONST, T_KEYWORD, T_SEPARATOR,

    // keywords
    K_IF, K_ELSE, K_FOR, K_WHILE, K_DO, K_RETURN, K_BREAK, K_CONTINUE,
    K_CHAR, K_INT, K_DOUBLE, K_BOOL, K_VOID, K_CONST, K_TRUE, K_FALSE,
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
//    N_S, N_E, N_T, N_F,
    N_S,
    N_E, N_E1, N_T, N_T1, N_F,
    EOF_TOKEN      // end of input string (MUST BE LAST(!))
};
enum ConstType {CONST_BOOL, CONST_INT, CONST_DOUBLE, CONST_CHAR, CONST_STRING};
enum ActionType {A_SHIFT, A_REDUCE, A_ACCEPT};


// items of token tables
struct TokenId {
    QString identifier;
};
struct TokenConst {
    ConstType const_type;
    QVariant value;
};
struct TokenKeyword {
    Token type;
};
struct TokenSeparator {
    Token type;
};
/*struct TokenNonTerminal {
    Token type;
};*/

// lexical convolution item
struct TokenPointer {
    Token type;
    int index;
    int start, length;
};

// [A -> B]
struct GrammarRule {
    Token left_token;
    QList<Token> right_side;
    GrammarRule() {}
    GrammarRule(Token left, QList<Token> right) { left_token = left; right_side = right; }
};
// [A -> .B, a]
struct Situation {
    Token left_token;
    QList<Token> right_side;
    Token look_ahead_token;
};

// crazy stuff...
inline bool operator==(const Situation &e1, const Situation &e2) {
    return ((e1.left_token == e2.left_token)
        && (e1.right_side == e2.right_side)
        && (e1.look_ahead_token == e2.look_ahead_token));
}
/*inline uint qHash(const QList<Token> &key)
{
    uint result = 0;
    foreach (const Token &token, key) {
        result ^= qHash(token);
    }
    return result;
}
inline uint qHash(const Situation &key)
{
    return key.left_token ^ qHash(key.right_side) ^ key.look_ahead_token;
}
inline uint qHash(const QSet<Situation> &key)
{
    uint result = 0;
    foreach (const Situation &situation, key) {
        result ^= qHash(situation);
    }
    return result;
}*/

struct Action {
    ActionType type;
    int index;
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
    hash.insert("true", K_TRUE);
    hash.insert("false", K_FALSE);
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
static QList<GrammarRule> initGrammarRules() {
    QList<GrammarRule> list;

    // grammar rules here
//    list << GrammarRule(N_S, EmptyTokenList() << N_E)
//         << GrammarRule(N_E, EmptyTokenList() << N_E << S_PLUS << N_T)
//         << GrammarRule(N_E, EmptyTokenList() << N_T)
//         << GrammarRule(N_T, EmptyTokenList() << N_T << S_MULT << N_F)
//         << GrammarRule(N_T, EmptyTokenList() << N_F)
//         << GrammarRule(N_F, EmptyTokenList() << T_ID);
    list << GrammarRule(N_E,  EmptyTokenList() << N_T << N_E1)
         << GrammarRule(N_E1, EmptyTokenList() << S_PLUS << N_T << N_E1)
         << GrammarRule(N_E1, EmptyTokenList() << LAMBDA)
         << GrammarRule(N_T,  EmptyTokenList() << N_F << N_T1)
         << GrammarRule(N_T1, EmptyTokenList() << S_MULT << N_F << N_T1)
         << GrammarRule(N_T1, EmptyTokenList() << LAMBDA)
         << GrammarRule(N_F,  EmptyTokenList() << S_ROUND_OPEN << N_E << S_ROUND_CLOSE)
         << GrammarRule(N_F,  EmptyTokenList() << T_ID);

    return list;
}
static const QList<GrammarRule> Grammar = initGrammarRules();


QDebug operator<<(QDebug d, const QList<Token> tokens);
QDebug operator<<(QDebug d, const QList<GrammarRule> grammar);

QList<GrammarRule> getGrammarRulesByLeftToken(Token token);
int indexOfGrammarRule(Token left, QList<Token> right);
GrammarRule getGrammarRule(Token left, QList<Token> right);
int indexOfSituation(Situation situation, QList<Situation> i);
int indexOfSetOfSituations(QList<Situation> i, QList<QList<Situation> > c);

bool isTokenTerminal(Token token);
bool isTokenKeyword(Token token);
bool isTokenSeparator(Token token);
bool isTokenNonTerminal(Token token);

QSet<Token> getAllTerminalTokens();
QSet<Token> getAllNonTerminalTokens();
QSet<Token> getAllGrammarTokens();

#endif // BASICS_H
