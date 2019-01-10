#ifndef SSYNTACTICANALYZER_H
#define SSYNTACTICANALYZER_H

#include <QObject>
#include <QtCore>
#include "basics.h"
#include "errors.h"

class SSyntacticAnalyzer : public QObject
{
    Q_OBJECT

public:
    SSyntacticAnalyzer(QObject* parent = 0);

    void setGrammar(QList<GrammarRule> grammar, const QString cache_filename);

    QList<int> process(QList<TokenPointer> tokens,
                       QList<TokenId> table_ids,
                       QList<TokenConst> table_consts,
                       QList<TokenKeyword> table_keywords,
                       QList<TokenSeparator> table_separators);

    QList<QSet<Situation>> getUltimateSetOfSituations() { return ultimate_situations_set_; }
    QList<QHash<Token, Action>> getTableAction() { return action_table_; }
    QList<QHash<Token, int>> getTableGoto() { return goto_table_; }

    QList<Symbol> getSymbolTable() { return symbol_table_; }
    QList<Block> getBlockTable() { return block_table_; }

private:
    QList<GrammarRule> grammar_;

    QList<QSet<Situation>> ultimate_situations_set_;
    QList<QHash<Token, Action>> action_table_;
    QList<QHash<Token, int>> goto_table_;

    QHash<Token, QSet<Token>> first_by_token_;
    QList<Symbol> symbol_table_;
    QList<Block> block_table_;

    bool generateSetOfSituations();
    bool generateActionGotoTables();
    bool readFromCache(const QString filename, const bool only_situations_set);
    bool writeToCache(const QString filename);

    QSet<Token> first(const Token token);
    QSet<Token> first(const QList<Token> tokens);

    QSet<Situation> closure(QSet<Situation> i);
    QSet<Situation> makeStep(const QSet<Situation> i, const Token x);

    int addSymbolFunction(QString name, DataType data_type, QList<int> args_indexes);
    int addSymbolArgument(QString name, DataType data_type, int class_index, ArgType arg_type, bool is_const);
    int addSymbolVariable(QString name, DataType data_type, int class_index, bool is_const);
    int addSymbolClass(QString name, QList<int> members_indexes);
    void setSymbolClassMemberAccessType(int symbol_index, AccessSpecifier access_type);

    int addEmptyBlock(int parent_block_index);
    void addBlockSymbols(int block_index, QList<int> declared_symbols_indexes);
    int getParentBlockIndex(int block_index);
    int indexOfSymbolDeclaredInBlock(QString name, SymbolType type, int block_index);
    int indexOfSymbolInCurrentBlock(QString name,
                                    SymbolType type,
                                    int block_index,
                                    QList<int> declared_but_not_in_block_indexes);

signals:
    void syntax_error(int pos, QString msg);
    void semantic_error(int pos, QString msg);
};

#endif  // SSYNTACTICANALYZER_H
