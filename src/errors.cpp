#include "errors.h"
#include <QDebug>

QString error_msg(ErrorType type)
{
    QString res = (ErrorCodes.contains(type)) ? ErrorCodes.value(type) : ErrorCodes.value(E_UNKNOWN_ERROR);
    return res;
}
