#ifndef BASICS_ERRORS_H
#define BASICS_ERRORS_H

#include <QtCore>

enum ErrorType {E_UNKNOWN_ERROR,
                // lexer errors
                E_UNKNOWN_TOKEN_ERROR, E_COMMENT_NOT_CLOSED,
                E_INVALID_DOUBLE, E_INVALID_INT, E_INVALID_CHAR
               };

static QHash<ErrorType, QString> initErrorCodes() {
        QHash<ErrorType, QString> hash;
        hash.insert(E_UNKNOWN_ERROR,        "Unknown error occured...");
        hash.insert(E_UNKNOWN_TOKEN_ERROR,  "Unknown token found");

        hash.insert(E_COMMENT_NOT_CLOSED,   "Multi-line comment not closed");
        hash.insert(E_INVALID_DOUBLE,       "Invalid Double! Constant is out of range");
        hash.insert(E_INVALID_INT,          "Invalid Integer! Constant is out of range");
        hash.insert(E_INVALID_CHAR,         "Invalid Char constant! It must be a single character");
        return hash;
}
static const QHash<ErrorType, QString> ErrorCodes = initErrorCodes();

inline QString error_msg(ErrorType type) {
    QString res = (ErrorCodes.contains(type))
            ? ErrorCodes.value(type)
            : ErrorCodes.value(E_UNKNOWN_ERROR);
    return res;
}

#endif // BASICS_ERRORS_H
