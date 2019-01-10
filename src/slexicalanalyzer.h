#ifndef SLEXICALANALYZER_H
#define SLEXICALANALYZER_H

#include <QObject>
#include <QtCore>
#include "basics.h"
#include "errors.h"

class SLexicalAnalyzer : public QObject
{
    Q_OBJECT

public:
    SLexicalAnalyzer(QObject* parent = 0);

    bool process(const QString);

    QList<TokenPointer> getAllTokens() const { return tokens_; }
    QList<TokenId> getTableIds() const { return table_ids_; }
    QList<TokenConst> getTableConsts() const { return table_consts_; }
    QList<TokenKeyword> getTableKeywords() const { return table_keywords_; }
    QList<TokenSeparator> getTableSeparators() const { return table_separators_; }

private:
    QString source_;
    QList<TokenPointer> tokens_;

    QList<TokenId> table_ids_;
    QList<TokenConst> table_consts_;
    QList<TokenKeyword> table_keywords_;
    QList<TokenSeparator> table_separators_;

    // add id, const, keyword/separator
    void addToken(const int start, const int length, const Token type, const QString identifier);
    void addToken(const int start, const int length, const Token type, const DataType const_type, QVariant value);
    void addToken(const int start, const int length, const Token type, const Token token);

    int indexOfTokenItem(const TokenId item);
    int indexOfTokenItem(const TokenConst item);
    int indexOfTokenItem(const TokenKeyword item);
    int indexOfTokenItem(const TokenSeparator item);

    void removeToken(const int index);

signals:
    void lexical_error(int pos, QString msg);
};

#endif  // SLEXICAL_ANALYZER_H
