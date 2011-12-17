#ifndef SLEXICALANALYZER_H
#define SLEXICALANALYZER_H

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

    bool processSource(const QString);

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
    void addToken(int start, int length, Token type, QString identifier);
    void addToken(int start, int length, Token type, ConstType const_type, QVariant value);
    void addToken(int start, int length, Token type, Token token);

    int indexOfTokenItem(TokenId item);
    int indexOfTokenItem(TokenConst item);
    int indexOfTokenItem(TokenKeyword item);
    int indexOfTokenItem(TokenSeparator item);

    void removeToken(int index);

signals:
    void lexical_error(int pos, QString msg);
};

#endif // SLEXICAL_ANALYZERH
