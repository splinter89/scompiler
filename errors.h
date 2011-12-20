#ifndef ERRORS_H
#define ERRORS_H

#include <QtCore>

enum ErrorType {E_UNKNOWN_ERROR,
                // lexical errors
                E_UNKNOWN_TERMINAL_ERROR, E_COMMENT_NOT_CLOSED,
                E_INVALID_DOUBLE, E_INVALID_INT, E_INVALID_CHAR,
                E_CHAR_NOT_CLOSED, E_STRING_NOT_CLOSED,
                // syntax errors
                E_NOT_LR1_GRAMMAR,
                E_INTERNAL_GENERATING_SITUATIONS, E_INTERNAL_GENERATING_TABLES,
                E_INTERNAL_GOTO_UNDEFINED, E_INTERNAL_ACTION_UNDEFINED,
                E_INTERNAL_STATES_STACK_EMPTY, E_CHAIN_REJECTED
               };

static QHash<ErrorType, QString> initErrorCodes() {
        QHash<ErrorType, QString> hash;
        hash.insert(E_UNKNOWN_ERROR,                "Unknown error occured...");

        hash.insert(E_UNKNOWN_TERMINAL_ERROR,       "Unknown terminal found");
        hash.insert(E_COMMENT_NOT_CLOSED,           "Multi-line comment is not closed");
        hash.insert(E_INVALID_DOUBLE,               "Double constant is out of range");
        hash.insert(E_INVALID_INT,                  "Integer constant is out of range");
        hash.insert(E_INVALID_CHAR,                 "Char constant must contain a single character");
        hash.insert(E_CHAR_NOT_CLOSED,              "Char constant is not closed");
        hash.insert(E_STRING_NOT_CLOSED,            "String constant is not closed");

//        hash.insert(E_NOT_LR1_GRAMMAR,              "The predefined grammar is not LR(1)");
        hash.insert(E_NOT_LR1_GRAMMAR,              "The chain rejected [it may be not LR(1)-grammar]");
        hash.insert(E_INTERNAL_GENERATING_SITUATIONS, "Grammar not loaded [failed to generate situations]");
        hash.insert(E_INTERNAL_GENERATING_TABLES,   "The chain rejected [failed to generate action/goto tables (internal)]");
        hash.insert(E_INTERNAL_GOTO_UNDEFINED,      "The chain rejected [goto-rule undefined (internal)]");
        hash.insert(E_INTERNAL_ACTION_UNDEFINED,    "The chain rejected [action-rule undefined (internal)]");
        hash.insert(E_INTERNAL_STATES_STACK_EMPTY,  "The chain rejected [stack of states got broken (internal)]");
        hash.insert(E_CHAIN_REJECTED,               "The chain rejected");

        return hash;
}
static const QHash<ErrorType, QString> ErrorCodes = initErrorCodes();

QString error_msg(ErrorType type);

#endif // ERRORS_H
