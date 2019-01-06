#include "slexicalanalyzer.h"
#include <QDebug>

SLexicalAnalyzer::SLexicalAnalyzer(QObject* parent)
{
    this->setParent(parent);
}

SLexicalAnalyzer::~SLexicalAnalyzer()
{
    // bye
}

bool SLexicalAnalyzer::process(const QString code)
{
    tokens_.clear();
    table_ids_.clear();
    table_consts_.clear();
    table_keywords_.clear();
    table_separators_.clear();

    Token token_type;
    DataType const_type;

    QString identifier;
    QVariant const_value;
    Token token;  // keyword/separator

    int i, start;

    // main loop
    for (i = 0; i < code.length(); i++) {
        start = i;
        token_type = UNKNOWN;

        // :TODO: calculate lines (do we really need this?)
        if (code.at(i) == '\n') {
            token_type = T_SEPARATOR;
            token = S_SPACE;
        } else if ((i <= code.length() - 2) && (code.mid(i, 2) == "//")) {
            // single-line comment
            i++;
            while ((i < code.length() - 1) && (code.at(i + 1) != '\n')) {
                i++;
            }
            continue;
        } else if ((i <= code.length() - 2) && (code.mid(i, 2) == "/*")) {
            // multi-line comment
            // TODO: it may break token into 2 parts - is it ok? --- insert Space token
            i++;
            while ((i < code.length() - 2) && (code.mid(i + 1, 2) != "*/")) {
                i++;
            }
            if (i >= code.length() - 2) {
                // error (comment not closed by the end of code)
                tokens_.clear();
                emit lexical_error(code.length(), error_msg(E_COMMENT_NOT_CLOSED));
                return false;
            } else {
                i += 2;  // */
            }
            continue;
        } else if ((code.at(i) == ' ') || (code.at(i) == '\t')) {
            // space (have special behavior)
            while ((i < code.length() - 1) && ((code.at(i + 1) == ' ') || (code.at(i + 1) == '\t'))) {
                i++;
            }

            token_type = T_SEPARATOR;
            token = S_SPACE;
        } else if (code.at(i).isLetter()) {
            while ((i < code.length() - 1) && (code.at(i + 1).isLetterOrNumber() || (code.at(i + 1) == '_'))) {
                i++;
            }
            identifier = code.mid(start, i - start + 1);
            if ((identifier == "true") || (identifier == "false")) {
                // boolean const
                token_type = T_CONST;
                const_type = TYPE_BOOL;
                const_value = (identifier == "true") ? true : false;
            } else if (KeywordCodes.contains(identifier)) {
                // keyword
                token_type = T_KEYWORD;
                token = KeywordCodes.value(identifier);
            } else {
                // identifier
                token_type = T_ID;
            }
        } else if (code.at(i).isDigit() || code.at(i) == '.') {
            // number const
            token_type = T_CONST;

            QString str = code.mid(i);
            bool ok;  // error flag while converting string to int/double

            QRegExp rx_double("^(\\d+)?\\.(\\d+)?(?:[eE]([+-]?\\d+))?");
            QRegExp rx_int("^((?:0x[\\dabcdef]+)|(?:0[01234567]+)|(?:\\d+))");
            rx_int.setCaseSensitivity(Qt::CaseInsensitive);

            if (rx_double.indexIn(str) > -1) {
                // double const
                const_value = rx_double.cap(1) + "." + rx_double.cap(2);
                if (const_value == ".") {
                    token_type = T_SEPARATOR;
                    token = S_PERIOD;
                } else {
                    // correct double
                    if (!rx_double.cap(3).isEmpty()) {
                        // got an exponent
                        const_value = const_value.toString() + "e" + rx_double.cap(3);
                    }
                    i += const_value.toString().length() - 1;
                    const_value = const_value.toDouble(&ok);
                    const_type = TYPE_DOUBLE;
                }
            } else if (rx_int.indexIn(str) > -1) {
                // int (hex/oct/dec) const
                i += rx_int.cap(1).length() - 1;
                const_value = rx_int.cap(1).toInt(&ok, 0);
                const_type = TYPE_INT;
            }

            // check value for min/max range
            if (!ok && (const_type == TYPE_DOUBLE)) {
                tokens_.clear();
                emit lexical_error(i + 1, error_msg(E_INVALID_DOUBLE));
                return false;
            } else if (!ok && (const_type == TYPE_INT)) {
                tokens_.clear();
                emit lexical_error(i + 1, error_msg(E_INVALID_INT));
                return false;
            }
        } else if ((i <= code.length() - 2) && (SeparatorCodes.contains(code.mid(i, 2)))) {
            token_type = T_SEPARATOR;
            token = SeparatorCodes.value(code.mid(i, 2));
            i++;
        } else if (SeparatorCodes.contains(code.at(i))) {
            token_type = T_SEPARATOR;
            token = SeparatorCodes.value(code.at(i));
        } else if (code.at(i) == '\'') {
            // char const
            while ((i < code.length() - 1)
                   && ((code.at(i + 1) != '\'') || ((code.at(i - 1) != '\\') && (code.at(i) == '\\')))) {
                i++;
            }
            if (i == code.length() - 1) {
                tokens_.clear();
                emit lexical_error(i + 1, error_msg(E_CHAR_NOT_CLOSED));
                return false;
            }

            identifier = code.mid(start + 1, i - start);  // +1 because of opening apostrophe
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
                tokens_.clear();
                emit lexical_error(i + 2, error_msg(E_INVALID_CHAR));
                return false;
            }
            i++;  // closing apostrophe
            token_type = T_CONST;
            const_type = TYPE_CHAR;
        } else if (code.at(i) == '"') {
            // string const
            while ((i < code.length() - 1)
                   && ((code.at(i + 1) != '\"') || ((code.at(i - 1) != '\\') && (code.at(i) == '\\')))) {
                i++;
            }
            if (i == code.length() - 1) {
                tokens_.clear();
                emit lexical_error(i + 1, error_msg(E_STRING_NOT_CLOSED));
                return false;
            }

            const_value = code
                            .mid(start + 1, i - start)  // +1 because of opening double quotes
                            .replace("\\'", "'")
                            .replace("\\\"", "\"")
                            .replace("\\\\", "\\")
                            .replace("\\n", "\n")
                            .replace("\\t", "\t");
            i++;  // closing double quotes
            token_type = T_CONST;
            const_type = TYPE_STRING;
        } else {
            // error (unknown token, e.g. after "3_" (or "123?") "_" (or "?") is unknown)
        }

        switch (token_type) {
            case T_ID:
                addToken(start, i - start + 1, token_type, identifier);
                break;

            case T_CONST:
                addToken(start, i - start + 1, token_type, const_type, const_value);
                break;

            case T_KEYWORD:
                addToken(start, i - start + 1, token_type, token);
                break;

            case T_SEPARATOR:
                addToken(start, i - start + 1, token_type, token);
                break;

            default:
                tokens_.clear();
                emit lexical_error(i + 1, error_msg(E_UNKNOWN_TERMINAL_ERROR));
                return false;
                break;
        }
    }

    // trim spaces
    while ((tokens_.first().type == T_SEPARATOR) && table_separators_.at(tokens_.first().index).type == S_SPACE) {
        removeToken(0);
    }
    while ((tokens_.last().type == T_SEPARATOR) && table_separators_.at(tokens_.last().index).type == S_SPACE) {
        removeToken(tokens_.length() - 1);
    }

    TokenPointer new_token = {EOF_TOKEN, -1, code.length(), 1};
    tokens_ << new_token;

    return true;
}

// add id
void SLexicalAnalyzer::addToken(const int start, const int length, const Token type, const QString identifier)
{
    if (type == T_ID) {
        TokenId new_item = {identifier};
        int index = indexOfTokenItem(new_item);
        if (index == -1) {
            table_ids_ << new_item;
            index = table_ids_.length() - 1;
        }

        TokenPointer new_token = {T_ID, index, start, length};
        tokens_ << new_token;
    }
}

// add const
void SLexicalAnalyzer::addToken(const int start,
                                const int length,
                                const Token type,
                                const DataType const_type,
                                QVariant value)
{
    if (type == T_CONST) {
        if (((const_type == TYPE_INT) || (const_type == TYPE_DOUBLE)) && (tokens_.length() >= 1)
            && (tokens_.last().type == T_SEPARATOR)
            && ((table_separators_.at(tokens_.last().index).type == S_PLUS)
                || (table_separators_.at(tokens_.last().index).type == S_MINUS))
            && ((tokens_.length() == 1)
                || ((tokens_.length() >= 2) && (tokens_.at(tokens_.length() - 2).type == T_SEPARATOR)
                    && (table_separators_.at(tokens_.at(tokens_.length() - 2).index).type != S_ROUND_CLOSE)))) {
            // (some separator (excluding ")") -> unary plus/minus -> number) => (separator -> signed number)
            if (table_separators_.at(tokens_.last().index).type == S_MINUS) {
                if (const_type == TYPE_INT) {
                    value = -value.toInt();
                } else {
                    value = -value.toDouble();
                }
            }
            removeToken(tokens_.length() - 1);  // remove that unary +/-
        }

        TokenConst new_item = {const_type, value};
        int index = indexOfTokenItem(new_item);
        if (index == -1) {
            table_consts_ << new_item;
            index = table_consts_.length() - 1;
        }

        TokenPointer new_token = {T_CONST, index, start, length};
        tokens_ << new_token;
    }
}

// add keyword/separator
void SLexicalAnalyzer::addToken(const int start, const int length, const Token type, const Token token)
{
    if (type == T_KEYWORD) {
        TokenKeyword new_item = {token};
        int index = indexOfTokenItem(new_item);
        if (index == -1) {
            table_keywords_ << new_item;
            index = table_keywords_.length() - 1;
        }

        TokenPointer new_token = {T_KEYWORD, index, start, length};
        tokens_ << new_token;
    } else if (type == T_SEPARATOR) {
        // don't add space after separator
        if (token == S_SPACE && !tokens_.isEmpty() && (tokens_.last().type == T_SEPARATOR)) {
            return;
        }

        // remove space before separator
        if (!tokens_.isEmpty() && (tokens_.last().type == T_SEPARATOR)
            && (table_separators_.at(tokens_.last().index).type == S_SPACE)) {
            removeToken(tokens_.length() - 1);
        }

        TokenSeparator new_item = {token};
        int index = indexOfTokenItem(new_item);
        if (index == -1) {
            table_separators_ << new_item;
            index = table_separators_.length() - 1;
        }

        TokenPointer new_token = {T_SEPARATOR, index, start, length};
        tokens_ << new_token;
    }
}

int SLexicalAnalyzer::indexOfTokenItem(const TokenId item)
{
    int i, res = -1;
    for (i = 0; i < table_ids_.length(); i++) {
        if (item.name == table_ids_.at(i).name) {
            res = i;
            break;
        }
    }
    return res;
}

int SLexicalAnalyzer::indexOfTokenItem(const TokenConst item)
{
    int i, res = -1;
    for (i = 0; i < table_consts_.length(); i++) {
        if ((item.type == table_consts_.at(i).type)
            && (item.value.toString() == table_consts_.at(i).value.toString())) {
            res = i;
            break;
        }
    }
    return res;
}

int SLexicalAnalyzer::indexOfTokenItem(const TokenKeyword item)
{
    int i, res = -1;
    for (i = 0; i < table_keywords_.length(); i++) {
        if (item.type == table_keywords_.at(i).type) {
            res = i;
            break;
        }
    }
    return res;
}

int SLexicalAnalyzer::indexOfTokenItem(const TokenSeparator item)
{
    int i, res = -1;
    for (i = 0; i < table_separators_.length(); i++) {
        if (item.type == table_separators_.at(i).type) {
            res = i;
            break;
        }
    }
    return res;
}

void SLexicalAnalyzer::removeToken(const int list_index)
{
    TokenPointer token = tokens_.at(list_index);

    bool got_other_links = false;
    for (int i = 0; i < tokens_.length(); i++) {
        if ((tokens_.at(i).type == token.type) && (tokens_.at(i).index == token.index)) {
            // we can't remove token from table
            got_other_links = true;
            break;
        }
    }
    if (!got_other_links) {
        switch (token.type) {
            case T_ID:
                table_ids_.removeAt(token.index);
                break;

            case T_CONST:
                table_consts_.removeAt(token.index);
                break;

            case T_KEYWORD:
                table_keywords_.removeAt(token.index);
                break;

            case T_SEPARATOR:
                table_separators_.removeAt(token.index);
                break;
            default:
                break;
        }
    }

    tokens_.removeAt(list_index);
}
