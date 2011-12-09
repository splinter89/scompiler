#ifndef BASICS_ERRORS_H
#define BASICS_ERRORS_H

#include <QtCore>

enum ErrorType {E_UNKNOWN_ERROR,
                // tokenizer errors
                E_UNKNOWN_TOKEN_ERROR};

static QHash<ErrorType, QString> initErrorCodes() {
        QHash<ErrorType, QString> hash;
        hash.insert(E_UNKNOWN_ERROR,        "Unknown error");
        hash.insert(E_UNKNOWN_TOKEN_ERROR,  "Unknown token found");
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
