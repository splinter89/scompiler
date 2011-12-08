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

void SLexer::addIdToken(int start, int end, QString identifier) {
    TableItem_id new_item = {start, end, identifier};
    Table_ids << new_item;

    TokenPointer new_token = {T_ID, Table_ids.count() - 1};
    tokens << new_token;
}
void SLexer::addConstToken(int start, int end, ConstType type, QVariant value) {
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
        tokens.removeLast();
    }

    TableItem_separator new_item = {start, end, type};
    Table_separators << new_item;

    TokenPointer new_token = {T_SEPARATOR, Table_separators.count() - 1};
    tokens << new_token;
}
