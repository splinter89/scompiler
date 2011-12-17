#include "basics.h"
#include <QDebug>

bool isTokenTerminal(Token token) {
    QList<Token> list;
    list << TERMINAL
         << T_ID << T_CONST << T_KEYWORD << T_SEPARATOR;
    return (list.contains(token) || isTokenKeyword(token) || isTokenSeparator(token));
}

bool isTokenKeyword(Token token) {
    QList<Token> list;
    list << T_KEYWORD
         << K_IF << K_ELSE << K_FOR << K_WHILE << K_DO << K_RETURN << K_BREAK
         << K_CONTINUE << K_CHAR << K_INT << K_DOUBLE << K_BOOL << K_VOID
         << K_CONST << K_TRUE << K_FALSE << K_CLASS << K_PUBLIC << K_PRIVATE;
    return list.contains(token);
}

bool isTokenSeparator(Token token) {
    QList<Token> list;
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
    QList<Token> list;
    list << NON_TERMINAL
         << N_S << N_E << N_T << N_F;
    return list.contains(token);
}
