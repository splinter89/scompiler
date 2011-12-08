#include "slexer.h"
#include <QDebug>

SLexer::SLexer(const QString code) {
    setSource(code);
}

void SLexer::setSource(const QString code) {
    QList<QString> space_chars;
    space_chars << " " << "\n" << "\t" << "\r";

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
        // :TODO: calculate lines
        if (code.at(i) == '\n') {
            // newline
            continue;
        } else if (code.mid(i, 2) == "//") {
            // single-line comment
            i++;
            while ((code.at(i + 1) != '\n')) {
                i++;
            }
            continue;
        } else if (code.mid(i, 2) == "/*") {
            // multi-line comment
            i++;
            while ((code.mid(i + 1, 2) != "*/")) {
                i++;
            }
            continue;
        } else if ((code.at(i) == ' ') || (code.at(i) == '\t')) {
            // space (have special behavior)
            while ((code.at(i + 1) == ' ') || (code.at(i + 1) == '\t')) {
                i++;
            }

            type = T_SEPARATOR;
            separator = S_SPACE;
        } else if (SeparatorCodes.contains(code.mid(i, 2))) {
            i++;

            type = T_SEPARATOR;
            separator = SeparatorCodes.value(code.mid(i, 2));
        } else if (SeparatorCodes.contains(code.at(i))) {

            type = T_SEPARATOR;
            separator = SeparatorCodes.value(code.at(i));
        } else if (code.at(i).isLetter()) {

            while (code.at(i + 1).isLetterOrNumber() || (code.at(i + 1) == '_')) {
                i++;
            }
            identifier = code.mid(start, i - start);
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
            QString str = code.mid(i);

            QRegExp rx_double("^(\\d+)?\\.(\\d+)?(?:(?:[eE]([+-]?(\\d+))))?");
            QRegExp rx_hex("^0x([\\dabcdef]+)");
            rx_hex.setCaseSensitivity(Qt::CaseInsensitive);
            QRegExp rx_oct("^(0[01234567]+)");
            QRegExp rx_dec("^(\\d+)");

            if (rx_double.indexIn(str) > -1) {
                // double const
                const_value = rx_double.cap(1) + "." + rx_double.cap(2);
                if (!rx_double.cap(3).isEmpty()) {
                    // got an exponent
                    const_value = const_value.toString() + "e" + rx_double.cap(3);
                }
                if (const_value == ".") {
                    // :TODO: error ("." is invalid double)
                }
                const_value = const_value.toDouble();
                const_type = CONST_DOUBLE;
            } else if (rx_hex.indexIn(str) > -1) {
                // hex const
                const_value = rx_double.cap(1).toInt(0, 16);
                const_type = CONST_INT;
            } else if (rx_oct.indexIn(str) > -1) {
                // oct const
                const_value = rx_double.cap(1).toInt(0, 8);
                const_type = CONST_INT;
            } else if (rx_dec.indexIn(str) > -1) {
                // dec const
                const_value = rx_double.cap(1).toInt();
                const_type = CONST_INT;
            } else {
                // :TODO: error (e.g. ".-" is wrong double)
                // hex/oct/dec are always correct by this point
            }
            type = T_CONST;
        } else if (code.at(i) == '\'') {
            // char const
            while ((code.at(i + 1) != '\'') || (code.at(i) == '\\')) {
                i++;
            }
            identifier = code.mid(start, i - start);
            if (identifier == "\\'") {
                const_value = '\'';
            } else if (identifier == "\\\"") {
                const_value = '\"';
            } else if (identifier == "\\n") {
                const_value = '\n';
            } else if (identifier == "\\t") {
                const_value = '\t';
            } else if (identifier.length() == 1) {
                const_value = identifier.at(1);
            } else {
                // :TODO: error
            }
            type = T_CONST;
            const_type = CONST_CHAR;
        } else if (code.at(i) == '"') {
            // string const
            while ((code.at(i + 1) != '\"') || (code.at(i) == '\\')) {
                i++;
            }
            const_value = code.mid(start, i - start)
                    .replace("\\'", "'")
                    .replace("\\\"", "\"")
                    .replace("\\n", "\n")
                    .replace("\\t", "\t");
            type = T_CONST;
            const_type = CONST_STRING;
        } else {
            // :TODO: error (unknown token, e.g. after "3_" "_" is unknown)
        }
    }
    switch (type) {
    case T_ID:
        addIdToken(start, i - start, identifier);
        break;

    case T_CONST:
        addConstToken(start, i - start, const_type, const_value);
        break;

    case T_KEYWORD:
        addKeywordToken(start, i - start, keyword);
        break;

    case T_SEPARATOR:
        addSeparatorToken(start, i - start, separator);
        break;

    }
    qDebug() << tokens.count();
}

// TODO: add search for existing token in table!!!
void SLexer::addIdToken(int start, int end, QString identifier) {
    TableItem_id new_item = {start, end, identifier};
    Table_ids << new_item;

    TokenPointer new_token = {T_ID, Table_ids.count() - 1};
    tokens << new_token;
}
void SLexer::addConstToken(int start, int end, ConstType type, QVariant value) {
    if (((type == CONST_INT) || (type == CONST_DOUBLE))
        && (tokens.length() >= 2) && (tokens.last().type == T_SEPARATOR)
        && ((Table_separators.at(tokens.last().index).type == S_PLUS)
            || (Table_separators.at(tokens.last().index).type == S_MINUS))
        && (tokens.at(tokens.length() - 1).type == T_SEPARATOR)
        && (Table_separators.at(tokens.at(tokens.length() - 1).index).type != S_ROUND_CLOSE)
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

    TableItem_const new_item = {start, end, type, value};
    Table_consts << new_item;

    TokenPointer new_token = {T_CONST, Table_consts.count() - 1};
    tokens << new_token;
}

void SLexer::addKeywordToken(int start, int end, Keyword type) {
    TableItem_keyword new_item = {start, end, type};
    Table_keywords << new_item;

    TokenPointer new_token = {T_KEYWORD, Table_keywords.count() - 1};
    tokens << new_token;
}

void SLexer::addSeparatorToken(int start, int end, Separator type) {
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

    TableItem_separator new_item = {start, end, type};
    Table_separators << new_item;

    TokenPointer new_token = {T_SEPARATOR, Table_separators.count() - 1};
    tokens << new_token;
}

void SLexer::removeToken(int list_index) {
    TokenPointer token = tokens.at(list_index);

    bool got_other_links = false;
    for (int i = 0; i < tokens.length(); i++) {
        if ((tokens.at(i).type == token.type) && (tokens.at(i).index == token.index)) {
            got_other_links = true;
            break;
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

            }
        }
    }

    tokens.removeAt(list_index);
}
