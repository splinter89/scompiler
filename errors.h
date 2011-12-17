#ifndef ERRORS_H
#define ERRORS_H

#include <QtCore>

enum ErrorType {E_UNKNOWN_ERROR,
                // lexical errors
                E_UNKNOWN_TERMINAL_ERROR, E_COMMENT_NOT_CLOSED,
                E_INVALID_DOUBLE, E_INVALID_INT, E_INVALID_CHAR,
                E_CHAR_NOT_CLOSED, E_STRING_NOT_CLOSED
                // syntactic errors
               };

static QHash<ErrorType, QString> initErrorCodes() {
        QHash<ErrorType, QString> hash;
        hash.insert(E_UNKNOWN_ERROR,        "Unknown error occured...");

        hash.insert(E_UNKNOWN_TERMINAL_ERROR,  "Unknown terminal found");
        hash.insert(E_COMMENT_NOT_CLOSED,   "Multi-line comment is not closed");
        hash.insert(E_INVALID_DOUBLE,       "Double constant is out of range");
        hash.insert(E_INVALID_INT,          "Integer constant is out of range");
        hash.insert(E_INVALID_CHAR,         "Char constant must contain a single character");
        hash.insert(E_CHAR_NOT_CLOSED,      "Char constant is not closed");
        hash.insert(E_STRING_NOT_CLOSED,    "String constant is not closed");

        return hash;
}
static const QHash<ErrorType, QString> ErrorCodes = initErrorCodes();

QString error_msg(ErrorType type);

#endif // ERRORS_H
