#include "slexer.h"
#include <QDebug>

SLexer::SLexer(const QString code) {
    setSource(code);
}

SLexer::~SLexer() {
    qDebug("SLexer destructor");
}

void SLexer::setSource(const QString code) {
    QChar c;
    TokenType type;
    ConstType const_type;
    QVariant const_value;
    QString identifier;
    Keyword keyword;
    Separator separator;
    int i, start;

    for (i = 0; i < code.length(); i++) {
        // main loop
        c = code.at(i);
        if (c == QChar('\n')) {
            tokens << "ok";
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
