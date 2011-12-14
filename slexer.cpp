#include "slexer.h"
#include <QDebug>

SLexer::SLexer(QObject * parent)
{
    this->setParent(parent);
}

SLexer::~SLexer()
{
    // bye
}

bool SLexer::processSource(const QString code)
{
//    QList<QString> space_chars;
//    space_chars << " " << "\n" << "\t" << "\r";

    TokenType type;
    ConstType const_type;

    QString identifier;
    QVariant const_value;
    Keyword keyword;
    Separator separator;
    int i, start;

    // main loop
    for (i = 0; i < code.length(); i++) {
        start = i;
        type = T_UNKNOWN;

        // :TODO: calculate lines (do we really need this?)
        if (code.at(i) == '\n') {
            type = T_SEPARATOR;
            separator = S_SPACE;
        } else if ((i <= code.length() - 2) && (code.mid(i, 2) == "//")) {
            // single-line comment
            i++;
            while ((i < code.length() - 1) && (code.at(i + 1) != '\n')) {
                i++;
            }
            continue;
        } else if ((i <= code.length() - 2) && (code.mid(i, 2) == "/*")) {
            // multi-line comment
            // TODO: it may break token into 2 parts - is it ok?
            i++;
            while ((i < code.length() - 2) && (code.mid(i + 1, 2) != "*/")) {
                i++;
            }
            if (i >= code.length() - 2) {
                // error (comment not closed by the end of code)
                tokens.clear();
                emit lex_error(code.length(), error_msg(E_COMMENT_NOT_CLOSED));
                return false;
            } else {
                i += 2; // */
            }
            continue;
        } else if ((code.at(i) == ' ') || (code.at(i) == '\t')) {
            // space (have special behavior)
            while ((i < code.length() - 1)
                   && ((code.at(i + 1) == ' ') || (code.at(i + 1) == '\t'))) {
                i++;
            }

            type = T_SEPARATOR;
            separator = S_SPACE;
        } else if (code.at(i).isLetter()) {

            while ((i < code.length() - 1)
                    &&(code.at(i + 1).isLetterOrNumber() || (code.at(i + 1) == '_'))) {
                i++;
            }
            identifier = code.mid(start, i - start + 1);
            if (KeywordCodes.contains(identifier)) {
                // keyword
                type = T_KEYWORD;
                keyword = KeywordCodes.value(identifier);
            } else if ((identifier == "true") || (identifier == "false")) {
                // boolean const
                type = T_CONST;
                const_type = CONST_BOOL;
                const_value = (identifier == "true") ? true : false;
            } else {
                // identifier
                type = T_ID;
            }
        } else if (code.at(i).isDigit() || code.at(i) == '.') {
            // number const
            type = T_CONST;

            QString str = code.mid(i);
            bool ok;   // error flag while converting string to int/double

            QRegExp rx_double("^(\\d+)?\\.(\\d+)?(?:[eE]([+-]?\\d+))?");
            QRegExp rx_int("^((?:0x[\\dabcdef]+)|(?:0[01234567]+)|(?:\\d+))");
            rx_int.setCaseSensitivity(Qt::CaseInsensitive);

            if (rx_double.indexIn(str) > -1) {
                // double const
                const_value = rx_double.cap(1) + "." + rx_double.cap(2);
                if (const_value == ".") {
                    type = T_SEPARATOR;
                    separator = S_PERIOD;
                } else {
                    // correct double
                    if (!rx_double.cap(3).isEmpty()) {
                        // got an exponent
                        const_value = const_value.toString() + "e" + rx_double.cap(3);
                    }
                    i += const_value.toString().length() - 1;
                    const_value = const_value.toDouble(&ok);
                    const_type = CONST_DOUBLE;
                }
            } else if (rx_int.indexIn(str) > -1) {
                // int (hex/oct/dec) const
                i += rx_int.cap(1).length() - 1;
                const_value = rx_int.cap(1).toInt(&ok, 0);
                const_type = CONST_INT;
            }

            // check value for min/max range
            if (!ok && (const_type == CONST_DOUBLE)) {
                tokens.clear();
                emit lex_error(i + 1, error_msg(E_INVALID_DOUBLE));
                return false;
            } else if (!ok && (const_type == CONST_INT)) {
                tokens.clear();
                emit lex_error(i + 1, error_msg(E_INVALID_INT));
                return false;
            }
        } else if ((i <= code.length() - 2) && (SeparatorCodes.contains(code.mid(i, 2)))) {
            type = T_SEPARATOR;
            separator = SeparatorCodes.value(code.mid(i, 2));
            i++;
        } else if (SeparatorCodes.contains(code.at(i))) {
            type = T_SEPARATOR;
            separator = SeparatorCodes.value(code.at(i));
        } else if (code.at(i) == '\'') {
            // char const
            while ((i < code.length() - 1)
                   && ((code.at(i + 1) != '\'')
                       || ((code.at(i - 1) != '\\')
                           && (code.at(i) == '\\')
                           )
                       )
            ) {
                i++;
            }
            if (i == code.length() - 1) {
                tokens.clear();
                emit lex_error(i + 1, error_msg(E_CHAR_NOT_CLOSED));
                return false;
            }

            identifier = code.mid(start + 1, i - start);    // +1 because of opening apostrophe
            if (identifier == "\\'") {
                const_value = '\'';
            } else if (identifier == "\\\"") {
                const_value = '\"';
            } else if (identifier == "\\\\") {
                const_value = '\\';
            } else if (identifier == "\\n") {
                const_value = '\n';
            } else if (identifier == "\\t") {
                const_value = '\t';
            } else if (identifier.length() == 1) {
                const_value = identifier.at(0);
            } else {
                tokens.clear();
                emit lex_error(i + 2, error_msg(E_INVALID_CHAR));
                return false;
            }
            i++;    // closing apostrophe
            type = T_CONST;
            const_type = CONST_CHAR;
        } else if (code.at(i) == '"') {
            // string const
            while ((i < code.length() - 1)
                   && ((code.at(i + 1) != '\"')
                       || ((code.at(i - 1) != '\\')
                           && (code.at(i) == '\\')
                           )
                       )
            ) {
                i++;
            }
            if (i == code.length() - 1) {
                tokens.clear();
                emit lex_error(i + 1, error_msg(E_STRING_NOT_CLOSED));
                return false;
            }

            const_value = code.mid(start + 1, i - start)    // +1 because of opening double quotes
                    .replace("\\'", "'")
                    .replace("\\\"", "\"")
                    .replace("\\\\", "\\")
                    .replace("\\n", "\n")
                    .replace("\\t", "\t");
            i++;    // closing double quotes
            type = T_CONST;
            const_type = CONST_STRING;
        } else {
            // error (unknown token, e.g. after "3_" (or "123?") "_" (or "?") is unknown)
        }

        switch (type) {
        case T_ID:
            addIdToken(start, i - start + 1, identifier);
            break;

        case T_CONST:
            addConstToken(start, i - start + 1, const_type, const_value);
            break;

        case T_KEYWORD:
            addKeywordToken(start, i - start + 1, keyword);
            break;

        case T_SEPARATOR:
            addSeparatorToken(start, i - start + 1, separator);
            break;

        case T_UNKNOWN:
            tokens.clear();
            emit lex_error(i, error_msg(E_UNKNOWN_TOKEN_ERROR));
            return false;
            break;
        }
    }

    // trim spaces
    while ((tokens.first().type == T_SEPARATOR)
           && Table_separators.at(tokens.first().index).type == S_SPACE) {
        removeToken(0);
    }
    while ((tokens.last().type == T_SEPARATOR)
           && Table_separators.at(tokens.last().index).type == S_SPACE) {
        removeToken(tokens.length() - 1);
    }

    return true;
}


void SLexer::addIdToken(int start, int length, QString identifier)
{
    TableItem_id new_item = {identifier};
    int index = indexOfIdToken(new_item);
    if (index == -1) {
        Table_ids << new_item;
        index = Table_ids.length() - 1;
    }

    TokenPointer new_token = {T_ID, index, start, length};
    tokens << new_token;
}

void SLexer::addConstToken(int start, int length, ConstType type, QVariant value)
{
    if (((type == CONST_INT) || (type == CONST_DOUBLE))
        && (tokens.length() >= 1)
        && (tokens.last().type == T_SEPARATOR)
        && ((Table_separators.at(tokens.last().index).type == S_PLUS)
            || (Table_separators.at(tokens.last().index).type == S_MINUS))
        && ((tokens.length() == 1)
            || ((tokens.length() >= 2)
                && (tokens.at(tokens.length() - 2).type == T_SEPARATOR)
                && (Table_separators.at(tokens.at(tokens.length() - 2).index).type != S_ROUND_CLOSE)
            )
        )
    ) {
        // (some separator (excluding ")") -> unary plus/minus -> number) => (separator -> signed number)
        if (Table_separators.at(tokens.last().index).type == S_MINUS) {
            if (type == CONST_INT) {
                value = -value.toInt();
            } else {
                value = -value.toDouble();
            }
        }
        removeToken(tokens.length() - 1);   // remove that unary +/-
    }

    TableItem_const new_item = {type, value};
    int index = indexOfConstToken(new_item);
    if (index == -1) {
        Table_consts << new_item;
        index = Table_consts.length() - 1;
    }

    TokenPointer new_token = {T_CONST, index, start, length};
    tokens << new_token;
}

void SLexer::addKeywordToken(int start, int length, Keyword type)
{
    TableItem_keyword new_item = {type};
    int index = indexOfKeywordToken(new_item);
    if (index == -1) {
        Table_keywords << new_item;
        index = Table_keywords.length() - 1;
    }

    TokenPointer new_token = {T_KEYWORD, index, start, length};
    tokens << new_token;
}

void SLexer::addSeparatorToken(int start, int length, Separator type)
{
    // don't add space after separator
    if (type == S_SPACE
        && !tokens.isEmpty()
        && (tokens.last().type == T_SEPARATOR)
    ) {
        return;
    }

    // remove space before separator
    if (!tokens.isEmpty()
        && (tokens.last().type == T_SEPARATOR)
        && (Table_separators.at(tokens.last().index).type == S_SPACE)
    ) {
        removeToken(tokens.length() - 1);
    }

    TableItem_separator new_item = {type};
    int index = indexOfSeparatorToken(new_item);
    if (index == -1) {
        Table_separators << new_item;
        index = Table_separators.length() - 1;
    }

    TokenPointer new_token = {T_SEPARATOR, index, start, length};
    tokens << new_token;
}


int SLexer::indexOfIdToken(TableItem_id item)
{
    int i, res = -1;
    for (i = 0; i < Table_ids.length(); i++) {
        if (item.identifier == Table_ids.at(i).identifier) {
            res = i;
            break;
        }
    }
    return res;
}

int SLexer::indexOfConstToken(TableItem_const item)
{
    int i, res = -1;
    for (i = 0; i < Table_consts.length(); i++) {
        if ((item.type == Table_consts.at(i).type)
                && (item.value.toString() == Table_consts.at(i).value.toString())) {
            res = i;
            break;
        }
    }
    return res;
}

int SLexer::indexOfKeywordToken(TableItem_keyword item)
{
    int i, res = -1;
    for (i = 0; i < Table_keywords.length(); i++) {
        if (item.type == Table_keywords.at(i).type) {
            res = i;
            break;
        }
    }
    return res;
}

int SLexer::indexOfSeparatorToken(TableItem_separator item)
{
    int i, res = -1;
    for (i = 0; i < Table_separators.length(); i++) {
        if (item.type == Table_separators.at(i).type) {
            res = i;
            break;
        }
    }
    return res;
}


void SLexer::removeToken(int list_index)
{
    TokenPointer token = tokens.at(list_index);

    bool got_other_links = false;
    for (int i = 0; i < tokens.length(); i++) {
        if ((tokens.at(i).type == token.type) && (tokens.at(i).index == token.index)) {
            // we can't remove token from table
            got_other_links = true;
            break;
        }
    }
    if (!got_other_links) {
        switch (token.type) {
        case T_ID:
            Table_ids.removeAt(token.index);
            break;

        case T_CONST:
            Table_consts.removeAt(token.index);
            break;

        case T_KEYWORD:
            Table_keywords.removeAt(token.index);
            break;

        case T_SEPARATOR:
            Table_separators.removeAt(token.index);
            break;
        default:
            break;
        }
    }

    tokens.removeAt(list_index);
}
