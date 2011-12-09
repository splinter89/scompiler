#ifndef SLEXER_H
#define SLEXER_H

#include <QtCore>
#include "basic_structs.h"

class SLexer
{
public:
    SLexer(const QString);

    QList<TableItem_id> Table_ids;
    QList<TableItem_const> Table_consts;
    QList<TableItem_keyword> Table_keywords;
    QList<TableItem_separator> Table_separators;

    void setSource(const QString);

    QList<TokenPointer> getAllTokens() const { return tokens; }
    QList<TableItem_id> getTableIds() const { return Table_ids; }
    QList<TableItem_const> getTableConsts() const { return Table_consts; }
    QList<TableItem_keyword> getTableKeywords() const { return Table_keywords; }
    QList<TableItem_separator> getTableSeparators() const { return Table_separators; }

private:
    QString source;
    QList<TokenPointer> tokens;

    void addIdToken(int start, int end, QString identifier);
    void addConstToken(int start, int end, ConstType type, QVariant value);
    void addKeywordToken(int start, int end, Keyword type);
    void addSeparatorToken(int start, int end, Separator type);

    void removeToken(int index);
};

#endif // SLEXER_H
