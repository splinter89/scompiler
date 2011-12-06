#ifndef SLEXER_H
#define SLEXER_H

#include <QtCore>
#include "basic_structs.h"

class SLexer
{
public:
    SLexer();
    SLexer(const QString);
    virtual ~SLexer();

    QList<Table_item_id *> Table_ids;
    QList<Table_item_const *> Table_consts;
    QList<Table_item_keyword *> Table_keywords;
    QList<Table_item_separator *> Table_separators;

    void setSource(const QString);

    QString getSource() const { return source; }
    QStringList getAllTokens() const { return tokens; }

private:
    QString source;
    QStringList tokens;

    void addIdToken(int start, end, QString code);
    void addConstToken(int start, end, QString code, ConstType type, QVariant value);
    void addKeywordToken(int start, end, QString code);
    void addSeparatorToken(int start, end, QString code);
};

#endif // SLEXER_H
