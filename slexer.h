#ifndef SLEXER_H
#define SLEXER_H

#include <QtCore>
#include <QObject>
#include "basics_errors.h"
#include "basics_lexer.h"

class SLexer : public QObject
{
    Q_OBJECT

public:
    SLexer(const QString);
    ~SLexer();

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

    void addIdToken(int start, int length, QString identifier);
    void addConstToken(int start, int length, ConstType type, QVariant value);
    void addKeywordToken(int start, int length, Keyword type);
    void addSeparatorToken(int start, int length, Separator type);

    int indexOfIdToken(TableItem_id item);
    int indexOfConstToken(TableItem_const item);
    int indexOfKeywordToken(TableItem_keyword item);
    int indexOfSeparatorToken(TableItem_separator item);

    void removeToken(int index);

signals:
    void lex_error(int pos, ErrorType type, QString param);
};

#endif // SLEXER_H
