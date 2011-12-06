#ifndef SLEXER_H
#define SLEXER_H

#include <QtCore>
#include "basic_structs.h"

class SLexer
{
public:
    SLexer(const QString);

    QList<Table_item_id> Table_ids;
    QList<Table_item_const> Table_consts;
    QList<Table_item_keyword> Table_keywords;
    QList<Table_item_separator> Table_separators;

    void setSource(const QString);

    QStringList getAllTokens() const { return tokens; }

private:
    QString source;
    QStringList tokens;

    void addIdToken(int start, int end, QString identifier);
    void addConstToken(int start, int end, ConstType type, QVariant value);
    void addKeywordToken(int start, int end, Keyword type);
    void addSeparatorToken(int start, int end, Separator type);
};

#endif // SLEXER_H
