#ifndef S_LEXICAL_ANALYZER_H
#define S_LEXICAL_ANALYZER_H

#include <QtCore>
#include <QObject>
#include "errors.h"
#include "basics.h"

class SLexicalAnalyzer : public QObject
{
    Q_OBJECT

public:
    SLexicalAnalyzer(QObject * parent = 0);
    ~SLexicalAnalyzer();

    QList<TableItem_id> Table_ids;
    QList<TableItem_const> Table_consts;
    QList<TableItem_keyword> Table_keywords;
    QList<TableItem_separator> Table_separators;

    bool processSource(const QString);

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
    void lex_error(int pos, QString msg);
};

#endif // S_LEXICAL_ANALYZER_H
