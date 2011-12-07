#include "slexer.h"
#include <QDebug>

SLexer::SLexer(const QString code) {
    setSource(code);
}

void SLexer::setSource(const QString code) {
    QString sequence;

    QList<QString> space_chars;
    space_chars << " " << "\n" << "\t" << "\r";

    TokenType type;
    ConstType const_type;
    QVariant const_value;
    Keyword keyword;
    Separator separator;
    int i, start;

    // :TODO: calculate lines
    for (i = 0; i < code.length(); i++) {
        // main loop
        sequence = code.at(i);

        if ((sequence == " ") || (sequence == "\t")) {
            start = i;
            sequence = code.at(i + 1);
            while ((sequence == " ") || (sequence == "\t")) {
                i++;
                sequence = code.at(i + 1);
            }

            type = T_SEPARATOR;
            separator = S_SPACE;
        } else if (SeparatorCodes.contains(code.mid(i, 2))) {
            start = i;
            i++;
            sequence += code.at(i);

            type = T_SEPARATOR;
            separator = SeparatorCodes.value(sequence);
        } else if (SeparatorCodes.contains(sequence)) {
            start = i;

            type = T_SEPARATOR;
            separator = SeparatorCodes.value(sequence);
        } else if (SeparatorCodes.contains(sequence)) {
            start = i;

            type = T_SEPARATOR;
            separator = SeparatorCodes.value(sequence);
        } else if (sequence == QChar('\n')) {
            tokens << "ok";
        }
    }
    switch (type) {
    case T_ID:
        addIdToken(start, i - start, sequence);
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
    qDebug() << tokens.join(" ");
}

void SLexer::addIdToken(int start, int end, QString identifier) {
    Table_item_id new_item = {start, end, identifier};
    Table_ids << new_item;
    // new_token = array(table_num, index)
    // tokens << new_token

}
void SLexer::addConstToken(int start, int end, ConstType type, QVariant value) {
    Table_item_const new_item = {start, end, type, value};
    Table_consts << new_item;
    // new_token = array(table_num, index)
    // tokens << new_token
}

void SLexer::addKeywordToken(int start, int end, Keyword type) {
    Table_item_keyword new_item = {start, end, type};
    Table_keywords << new_item;
    // new_token = array(table_num, index)
    // tokens << new_token
}

void SLexer::addSeparatorToken(int start, int end, Separator type) {
    Table_item_separator new_item = {start, end, type};
    Table_separators << new_item;
    // new_token = array(table_num, index)
    // tokens << new_token
}
