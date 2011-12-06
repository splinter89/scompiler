#include "slexer.h"
#include <QDebug>

SLexer::SLexer(const QString code) {
    setSource(code);
}

SLexer::~SLexer() {
    qDebug("SLexer destructor");
}

void SLexer::setSource(const QString code) {
    source = QString();

    QChar c;
    for (int i = 0; i < code.length(); i++) {
        c = code.at(i);
        if (c == QChar('\n')) {
            tokens << "ok";
        }
    }
    qDebug() << source;
}

void SLexer::addIdToken(int start, end, QString code) {
    Table_item_id new_item = {start, end, code};
    Table_ids << new_item;
    // new_token = array(table_num, index)
    // tokens << new_token
}

void SLexer::addConstToken(int start, end, QString code, ConstType type, QVariant value) {

}

void SLexer::addKeywordToken(int start, end, QString code) {

}

void SLexer::addSeparatorToken(int start, end, QString code) {

}
