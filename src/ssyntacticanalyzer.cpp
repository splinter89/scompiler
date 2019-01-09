#include "ssyntacticanalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject* parent)
{
    this->setParent(parent);
}

SSyntacticAnalyzer::~SSyntacticAnalyzer()
{
    // bye
}

void SSyntacticAnalyzer::setGrammar(QList<GrammarRule> grammar)
{
    grammar_ = grammar;

    first_by_token_.clear();
    if (!generateSetOfSituations()) return;
    if (!generateActionGotoTables()) return;
}

QSet<Token> SSyntacticAnalyzer::first(const Token token)
{
    if (first_by_token_.contains(token)) return first_by_token_.value(token);

    QSet<Token> result;
    first_by_token_.insert(token, QSet<Token>());  // to stop infinite recursion

    // step 1
    if (isTokenTerminal(token)) {
        result << token;
    } else if (isTokenNonTerminal(token, grammar_)) {
        // step 2
        if (indexOfGrammarRule(token, EmptyTokenList() << LAMBDA, grammar_) > -1) {
            result << LAMBDA;
        }

        // step 3
        int old_count;
        do {
            old_count = result.count();
            // for each rule X -> Y0Y1...Yk-1
            QList<GrammarRule> rules = getGrammarRulesByLeftToken(token, grammar_);
            for (int rule_i = 0; rule_i < rules.length(); rule_i++) {
                GrammarRule rule = rules.at(rule_i);

                bool all_y_got_lambda = true;
                for (int i = 0; i < rule.right_side.length(); i++) {
                    QSet<Token> first_y = first(rule.right_side.at(i));
                    bool this_y_got_lambda = first_y.remove(LAMBDA);

                    if (all_y_got_lambda) {  // if all previous Y got e
                        // add each terminal a from FIRST(Yi)\{e} to FIRST(X)
                        foreach (const Token a, first_y) {
                            if (isTokenTerminal(a)) {
                                result << a;
                            }
                        }
                    }
                    all_y_got_lambda &= this_y_got_lambda;
                    if (!all_y_got_lambda) break;
                }
                if (all_y_got_lambda) {
                    result << LAMBDA;
                }
            }
        } while (result.count() != old_count);
    }

    first_by_token_.remove(token);
    first_by_token_.insert(token, result);
    return result;
}

QSet<Token> SSyntacticAnalyzer::first(const QList<Token> tokens)
{
    QSet<Token> result;

    // for each X0X1...Xk-1
    bool all_x_got_lambda = true;
    for (int i = 0; i < tokens.length(); i++) {
        QSet<Token> first_x = first(tokens.at(i));
        bool this_x_got_lambda = first_x.remove(LAMBDA);

        if (all_x_got_lambda) {  // if all previous first(X) got e
            result += first_x;   // add FIRST(Xi)\{e}
        }
        all_x_got_lambda &= this_x_got_lambda;
        if (!all_x_got_lambda) break;
    }
    if (all_x_got_lambda) {
        result << LAMBDA;
    }

    return result;
}

QSet<Situation> SSyntacticAnalyzer::closure(QSet<Situation> i)
{
    int old_count;
    do {
        old_count = i.count();
        // for each situation [A -> alpha.Bbeta, a] from I
        foreach (const Situation& situation, i) {
            int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
            if ((dot_pos > -1) && (dot_pos < situation.right_side.length() - 1)) {
                Token b = situation.right_side.at(dot_pos + 1);
                QList<GrammarRule> rules = getGrammarRulesByLeftToken(b, grammar_);
                if (!rules.isEmpty()) {
                    // for each derivation rule B -> gamma from G
                    foreach (const GrammarRule& rule, rules) {
                        QList<Token> test_right_side;
                        if (dot_pos < situation.right_side.length() - 2) {
                            test_right_side = situation.right_side.mid(dot_pos + 2);  // skip .B
                        }
                        test_right_side << situation.look_ahead_token;  // beta a
                        QSet<Token> first_test = first(test_right_side);
                        // for each terminal c from FIRST(beta a), s.t. [B ->.gamma, c] isn't in I
                        foreach (const Token& c, first_test) {
                            QList<Token> new_right_rule = EmptyTokenList() << DOT_TOKEN;
                            new_right_rule += rule.right_side;  // .gamma
                            Situation new_situation = {b, new_right_rule, c};
                            // add [B ->.gamma, c] to I;
                            i << new_situation;
                        }
                    }
                }
            }
        }

    } while (i.count() != old_count);
    return i;
}

QSet<Situation> SSyntacticAnalyzer::makeStep(const QSet<Situation> i, const Token x)
{
    QSet<Situation> j;
    foreach (Situation situation, i) {
        int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
        if ((dot_pos > -1) && (dot_pos < situation.right_side.length() - 1)
            && (situation.right_side.at(dot_pos + 1) == x)) {
            situation.right_side.swap(dot_pos, dot_pos + 1);
            j << situation;
        }
    }
    return closure(j);
}

int SSyntacticAnalyzer::addSymbolFunction(QString name, DataType data_type, QList<int> args_indexes)
{
    Symbol new_symbol;
    new_symbol.name = name;
    new_symbol.type = SYM_FUNCTION;
    new_symbol.data_type = data_type;

    // specific
    new_symbol.args_indexes = args_indexes;

    symbol_table_ << new_symbol;
    return symbol_table_.length() - 1;
}

int SSyntacticAnalyzer::addSymbolArgument(QString name,
                                          DataType data_type,
                                          int class_index,
                                          ArgType arg_type,
                                          bool is_const)
{
    Symbol new_symbol;
    new_symbol.name = name;
    new_symbol.type = SYM_ARGUMENT;
    new_symbol.data_type = data_type;
    if (data_type == TYPE_OBJECT) {
        new_symbol.class_index = class_index;
    }

    // specific
    new_symbol.arg_type = arg_type;
    new_symbol.is_const = is_const;

    symbol_table_ << new_symbol;
    return symbol_table_.length() - 1;
}

int SSyntacticAnalyzer::addSymbolVariable(QString name, DataType data_type, int class_index, bool is_const)
{
    Symbol new_symbol;
    new_symbol.name = name;
    new_symbol.type = SYM_VARIABLE;
    new_symbol.data_type = data_type;
    if (data_type == TYPE_OBJECT) {
        new_symbol.class_index = class_index;
    }

    // specific
    new_symbol.is_const = is_const;

    symbol_table_ << new_symbol;
    return symbol_table_.length() - 1;
}

int SSyntacticAnalyzer::addSymbolClass(QString name, QList<int> members_indexes)
{
    Symbol new_symbol;
    new_symbol.name = name;
    new_symbol.type = SYM_CLASS;

    // specific
    new_symbol.members_indexes = members_indexes;

    symbol_table_ << new_symbol;
    return symbol_table_.length() - 1;
}

void SSyntacticAnalyzer::setSymbolClassMemberAccessType(int symbol_index, AccessSpecifier access_type)
{
    Symbol symbol = symbol_table_.at(symbol_index);
    if ((symbol.type == SYM_FUNCTION) || (symbol.type == SYM_VARIABLE)) {
        symbol.access_type = access_type;
        symbol_table_.replace(symbol_index, symbol);
    }
}

int SSyntacticAnalyzer::addEmptyBlock(int parent_block_index)
{
    Block new_block;
    QList<int> declared_symbols_indexes;
    if ((!block_table_.isEmpty() && (parent_block_index == -1))
        || (block_table_.isEmpty() && (parent_block_index != -1))) {
        // incorrect parent block index
        return -1;
    }
    new_block.parent_block_index = parent_block_index;
    new_block.declared_symbols_indexes = declared_symbols_indexes;

    block_table_ << new_block;
    return block_table_.length() - 1;
}

void SSyntacticAnalyzer::addBlockSymbols(int block_index, QList<int> declared_symbols_indexes)
{
    Block block = block_table_.at(block_index);
    foreach (int declared_symbols_index, declared_symbols_indexes) {
        // check for double declaration
        if (indexOfSymbolDeclaredInBlock(symbol_table_.at(declared_symbols_index).name,
                                         symbol_table_.at(declared_symbols_index).type, block_index)
            > -1) {
            emit semantic_error(-1, QString("(%1 @ block #%2) %3")
                                      .arg(symbol_table_.at(declared_symbols_index).toString())
                                      .arg(block_index)
                                      .arg(error_msg(E_ALREADY_DECLARED_IN_BLOCK)));
            return;
        } else {
            block.declared_symbols_indexes << declared_symbols_index;
            block_table_.replace(block_index, block);
        }
    }
}

int SSyntacticAnalyzer::getParentBlockIndex(int block_index)
{
    int res = block_table_.at(block_index).parent_block_index;
    return res;
}

int SSyntacticAnalyzer::indexOfSymbolDeclaredInBlock(QString name, SymbolType type, int block_index)
{
    int res = -1;

    foreach (int declared_symbols_index, block_table_.at(block_index).declared_symbols_indexes) {
        if ((symbol_table_.at(declared_symbols_index).name == name)
            && ((symbol_table_.at(declared_symbols_index).type == type)
                || ((symbol_table_.at(declared_symbols_index).type == SYM_ARGUMENT) && (type == SYM_VARIABLE)))) {
            res = declared_symbols_index;
        }
    }

    return res;
}
int SSyntacticAnalyzer::indexOfSymbolInCurrentBlock(QString name,
                                                    SymbolType type,
                                                    int block_index,
                                                    QList<int> declared_but_not_in_block_indexes)
{
    int res = -1;

    while ((res == -1) && (block_index != -1)) {
        foreach (int symbol_index, declared_but_not_in_block_indexes) {
            if ((symbol_table_.at(symbol_index).name == name)
                && ((symbol_table_.at(symbol_index).type == type)
                    || ((symbol_table_.at(symbol_index).type == SYM_ARGUMENT) && (type == SYM_VARIABLE)))) {
                res = symbol_index;
            }
        }

        if (res == -1) {
            res = indexOfSymbolDeclaredInBlock(name, type, block_index);
            if (res == -1) {
                block_index = block_table_.at(block_index).parent_block_index;
            }
        }
    }

    return res;
}

bool SSyntacticAnalyzer::generateSetOfSituations()
{
    if (grammar_.isEmpty()) {
        emit syntax_error(-1, error_msg(E_INTERNAL_GENERATING_SITUATIONS));
        return false;
    }

    ultimate_situations_set_.clear();

    // initial situation
    Situation s = {N_S, EmptyTokenList() << DOT_TOKEN << N_PROGRAM, EOF_TOKEN};
    QSet<Situation> i;
    QList<QSet<Situation>> c, c_new;
    int q1 = 1;
    QSet<Token> all_tokens = getAllGrammarTokens(grammar_);

    i << s;
    i = closure(i);
    c << i;

    int old_count;
    do {
        old_count = c.count();
        foreach (const QSet<Situation>& j, c) {
            foreach (const Token& x, all_tokens) {
                QSet<Situation> new_i = makeStep(j, x);
                if (!new_i.isEmpty() && !c.contains(new_i) && !c_new.contains(new_i)) {
                    c_new << new_i;
                    q1 += new_i.count();
                }
            }
        }
        c << c_new;
        c_new.clear();
    } while (c.count() != old_count);
    ultimate_situations_set_ = c;
    qDebug() << "ultimate_situations_set_:" << q1;
    return true;
}

bool SSyntacticAnalyzer::generateActionGotoTables()
{
    if (ultimate_situations_set_.isEmpty()) {
        emit syntax_error(-1, error_msg(E_INTERNAL_GENERATING_TABLES));
        return false;
    }

    QSet<Token> terminals = getAllTerminalTokens();
    QSet<Token> non_terminals = getAllNonTerminalTokens(grammar_);

    action_table_.clear();
    goto_table_.clear();

    int q1 = 0, q2 = 0;
    foreach (const QSet<Situation>& i, ultimate_situations_set_) {
        QHash<Token, Action> action_row;
        QHash<Token, int> goto_row;

        foreach (const Situation& situation, i) {
            // action
            int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
            foreach (const Token& terminal, terminals) {
                // step 1
                if ((dot_pos > -1) && (dot_pos < situation.right_side.length() - 1)
                    && (situation.right_side.at(dot_pos + 1) == terminal)) {
                    QSet<Situation> test_i = makeStep(i, terminal);
                    int j = ultimate_situations_set_.indexOf(test_i);
                    if (j > -1) {
                        Action new_action = {A_SHIFT, j};
                        if (!action_row.contains(terminal)) {
                            action_row.insert(terminal, new_action);
                        } else if ((action_row.value(terminal).type != new_action.type)
                                   || (action_row.value(terminal).index != new_action.index)) {
                            // not LR(1)
                            emit syntax_error(-1, "(step 1)" + error_msg(E_NOT_LR1_GRAMMAR));
                            return false;
                        }
                    }
                }
                // step 2
                if ((dot_pos == situation.right_side.length() - 1) && (situation.left_token != N_S)
                    && (situation.look_ahead_token == terminal)) {
                    int j = indexOfGrammarRule(
                      situation.left_token, situation.right_side.mid(0, situation.right_side.length() - 1), grammar_);
                    if (j > -1) {
                        Action new_action = {A_REDUCE, j};
                        if (!action_row.contains(terminal)) {
                            action_row.insert(terminal, new_action);
                        } else {
                            // not LR(1)
                            emit syntax_error(-1, "(step 2.1)" + error_msg(E_NOT_LR1_GRAMMAR));
                            return false;
                        }
                    } else {
                        // grammar rule not found
                        emit syntax_error(-1, "(step 2.2)" + error_msg(E_INTERNAL_GENERATING_TABLES));
                        return false;
                    }
                }
            }
            // step 3
            if ((situation.left_token == N_S) && (situation.right_side == (EmptyTokenList() << N_PROGRAM << DOT_TOKEN))
                && (situation.look_ahead_token == EOF_TOKEN)) {
                Action new_action = {A_ACCEPT, 0};
                if (!action_row.contains(EOF_TOKEN)) {
                    action_row.insert(EOF_TOKEN, new_action);
                } else {
                    // error
                    emit syntax_error(-1, "(step 3)" + error_msg(E_INTERNAL_GENERATING_TABLES));
                    return false;
                }
            }

            // goto
            foreach (const Token& non_terminal, non_terminals) {
                QSet<Situation> test_i = makeStep(i, non_terminal);
                int j = ultimate_situations_set_.indexOf(test_i);
                if (j > -1) {
                    int new_goto = j;
                    if (!goto_row.contains(non_terminal)) {
                        goto_row.insert(non_terminal, new_goto);
                    } else if (goto_row.value(non_terminal) != new_goto) {
                        // not LR(1)
                        emit syntax_error(-1, "(step goto)" + error_msg(E_NOT_LR1_GRAMMAR));
                        return false;
                    }
                }
            }
        }
        action_table_ << action_row;
        goto_table_ << goto_row;
        q1 += action_row.count();
        q2 += goto_row.count();
    }
    qDebug() << "action_table_:" << q1;
    qDebug() << "goto_table_:" << q2;
    return true;
}

QList<int> SSyntacticAnalyzer::process(QList<TokenPointer> tokens,
                                       QList<TokenId> table_ids,
                                       QList<TokenConst> table_consts,
                                       QList<TokenKeyword> table_keywords,
                                       QList<TokenSeparator> table_separators)
{
    QList<int> result;
    if (grammar_.isEmpty() || action_table_.isEmpty() || goto_table_.isEmpty()) return result;

    int i = 0;
    bool tokens_accepted = false;

    QList<int> states_stack;
    QList<Token> tokens_stack;
    QList<TokenPointer> ids_stack;

    int block_index;
    QList<int> declared_symbols_indexes;
    QList<int> declared_arg_symbols_indexes;
    int symbol_index;
    int object_symbol_id;
    DataType expr_type;
    bool is_const;
    DataType rvalue_type;
    // symbol params
    QList<int> args_indexes;
    ArgType arg_type;
    DataType data_type;
    int decl_vars_count;
    int class_index;
    AccessSpecifier access_type;
    QList<int> members_indexes;

    states_stack.clear();
    tokens_stack.clear();
    ids_stack.clear();
    states_stack << 0;  // initial state

    symbol_table_.clear();
    block_table_.clear();

    block_index = addEmptyBlock(-1);  // global context
    declared_symbols_indexes.clear();
    declared_arg_symbols_indexes.clear();
    object_symbol_id = -1;
    args_indexes.clear();
    decl_vars_count = 0;
    class_index = -1;
    access_type = ACCESS_PRIVATE;
    members_indexes.clear();

    while (!tokens_accepted && (i < tokens.length())) {
        int state = states_stack.last();
        Token token = tokens.at(i).type;
        if (token == T_KEYWORD) {
            token = table_keywords.at(tokens.at(i).index).type;
        } else if (token == T_SEPARATOR) {
            token = table_separators.at(tokens.at(i).index).type;
        }

        if (action_table_.at(state).contains(token)) {
            Action action = action_table_.at(state).value(token);
            GrammarRule rule;

            switch (action.type) {
                case A_SHIFT:
                    states_stack << action.index;
                    if (token == T_ID) {
                        ids_stack << tokens.at(i);
                    } else if (token == S_CURLY_OPEN) {
                        // save declared symbols & enter new block
                        addBlockSymbols(block_index, declared_symbols_indexes);
                        declared_symbols_indexes.clear();

                        block_index = addEmptyBlock(block_index);
                    } else if (token == S_CURLY_CLOSE) {
                        // save declared symbols & return to parent block
                        addBlockSymbols(block_index, declared_arg_symbols_indexes + declared_symbols_indexes);
                        declared_symbols_indexes.clear();
                        declared_arg_symbols_indexes.clear();

                        block_index = getParentBlockIndex(block_index);
                    }
                    tokens_stack << token;
                    i++;
                    break;

                case A_REDUCE:
                    rule = grammar_.at(action.index);
                    if ((states_stack.length() > rule.right_side.length())
                        && (tokens_stack.length() >= rule.right_side.length())) {
                        // full grammar because we logic depends on the indices
                        int rule_index_full = indexOfGrammarRule(rule.left_token, rule.right_side, Grammar_full);

                        int temp_class_index;  // for class search

                        // filling up the symbol table
                        switch (rule_index_full) {
                            case 7:
                            case 8:
                                if (rule_index_full == 7) members_indexes.clear();
                                declared_symbols_indexes
                                  << addSymbolClass(table_ids.at(ids_stack.last().index).name, members_indexes);
                                if (rule_index_full == 8) members_indexes.clear();
                                ids_stack.removeLast();
                                break;

                            case 10:
                            case 12:
                                // <class_body> with <access_spec>
                                // access_type = ...
                                break;
                            case 17:
                                access_type = ACCESS_PUBLIC;
                                break;
                            case 18:
                                access_type = ACCESS_PRIVATE;
                                break;

                            case 21:
                            case 22:
                            case 23:
                            case 24:
                                // <class_method>
                                if ((rule_index_full == 21 || (rule_index_full == 23))) args_indexes.clear();
                                if ((rule_index_full == 23 || (rule_index_full == 24))) data_type = TYPE_VOID;
                                symbol_index =
                                  addSymbolFunction(table_ids.at(ids_stack.last().index).name, data_type, args_indexes);
                                declared_symbols_indexes << symbol_index;
                                members_indexes << symbol_index;
                                setSymbolClassMemberAccessType(symbol_index, access_type);
                                if ((rule_index_full == 22 || (rule_index_full == 24))) args_indexes.clear();
                                ids_stack.removeLast();
                                break;

                            case 35:
                            case 36:
                            case 37:
                            case 38:
                                // <function>
                                if ((rule_index_full == 35 || (rule_index_full == 37))) args_indexes.clear();
                                if ((rule_index_full == 37 || (rule_index_full == 38))) data_type = TYPE_VOID;
                                declared_symbols_indexes << addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                                              data_type, args_indexes);
                                if ((rule_index_full == 36 || (rule_index_full == 38))) args_indexes.clear();
                                ids_stack.removeLast();
                                break;

                            case 41:
                            case 42:
                            case 43:
                            case 44:
                                // <argument>
                                is_const = ((rule_index_full == 42) || (rule_index_full == 44));
                                arg_type = ((rule_index_full == 41) || (rule_index_full == 42)) ? ARG_BY_VALUE
                                                                                                : ARG_BY_REFERENCE;
                                symbol_index = addSymbolArgument(table_ids.at(ids_stack.last().index).name, data_type,
                                                                 class_index, arg_type, is_const);
                                args_indexes << symbol_index;
                                declared_arg_symbols_indexes << symbol_index;
                                ids_stack.removeLast();
                                break;

                            case 60:
                            case 61:
                            case 62:
                            case 63:
                            case 64:
                            case 65:
                                // <operator_1> with assignment
                            case 120:
                            case 121:
                                // <vars_list> with assignment
                                rvalue_type =
                                  ((rule_index_full == 120) || (rule_index_full == 121)) ? expr_type : data_type;
                                //qDebug() << dataTypeToString(symbol_table_.at(object_symbol_id).data_type, class_index) << "lvalue_type";
                                if (symbol_table_.at(object_symbol_id).data_type != rvalue_type) {
                                    emit semantic_error(
                                      -1, QString("(%1 and %2) %3")
                                            .arg(dataTypeToString(symbol_table_.at(object_symbol_id).data_type,
                                                                  class_index))
                                            .arg(dataTypeToString(rvalue_type, class_index))
                                            .arg(error_msg(E_TYPES_MISMATCH)));
                                }
                                break;

                            case 94:
                                // <operator_9> ::= <literal>
                                expr_type = table_consts.at(tokens.at(i - 1).index).type;
                                // :TODO: need to use token_stack - not tokens (??? - check)
                                // tokens only work for "a = 12" but not for "a = 12 + d"
                                break;
                            case 95:
                            case 98:
                                expr_type = symbol_table_.at(object_symbol_id).data_type;
                                //qDebug() << dataTypeToString(expr_type, class_index) << "expr_type";
                                // :TODO: use expr_type_stack to check compatibility in rules that combine two operands
                                break;

                            case 99:
                            case 100:
                                // <function_call>
                                if (symbol_table_.at(object_symbol_id).type != SYM_FUNCTION) {
                                    emit semantic_error(-1, QString("(Call to %1) %2")
                                                              .arg(symbol_table_.at(object_symbol_id).name)
                                                              .arg(error_msg(E_OBJECT_IS_NOT_FUNCTION)));
                                    result.clear();
                                    return result;
                                }
                                break;

                            case 101:
                                // <object> ::= <id>
                                object_symbol_id = indexOfSymbolInCurrentBlock(
                                  table_ids.at(ids_stack.last().index).name, SYM_ARGUMENT, block_index,
                                  declared_arg_symbols_indexes + declared_symbols_indexes);
                                if (object_symbol_id == -1) {
                                    object_symbol_id = indexOfSymbolInCurrentBlock(
                                      table_ids.at(ids_stack.last().index).name, SYM_VARIABLE, block_index,
                                      declared_arg_symbols_indexes + declared_symbols_indexes);
                                    if (object_symbol_id == -1) {
                                        object_symbol_id = indexOfSymbolInCurrentBlock(
                                          table_ids.at(ids_stack.last().index).name, SYM_FUNCTION, block_index,
                                          declared_arg_symbols_indexes + declared_symbols_indexes);
                                        if (object_symbol_id == -1) {
                                            emit semantic_error(-1, QString("(%1) %2")
                                                                      .arg(table_ids.at(ids_stack.last().index).name)
                                                                      .arg(error_msg(E_UNDECLARED_SYMBOL)));
                                            result.clear();
                                            return result;
                                        }
                                    }
                                }
                                ids_stack.removeLast();
                                break;
                            case 102:
                                // <object> ::= <id>.<id>
                                temp_class_index = indexOfSymbolInCurrentBlock(
                                  table_ids.at(ids_stack.at(ids_stack.length() - 2).index).name, SYM_CLASS, block_index,
                                  declared_arg_symbols_indexes + declared_symbols_indexes);
                                if (temp_class_index > -1) {
                                    bool ok = false;
                                    foreach (int members_index, symbol_table_.at(temp_class_index).members_indexes) {
                                        if (symbol_table_.at(members_index).name
                                            == table_ids.at(ids_stack.last().index).name) {
                                            object_symbol_id = members_index;
                                            ok = true;
                                            break;
                                        }
                                    }
                                    if (!ok) {
                                        emit semantic_error(-1, QString("(%1) %2")
                                                                  .arg(table_ids.at(ids_stack.last().index).name)
                                                                  .arg(error_msg(E_UNDECLARED_SYMBOL)));
                                        result.clear();
                                        return result;
                                    }
                                } else {
                                    emit semantic_error(
                                      -1, QString("(%1) %2")
                                            .arg(table_ids.at(ids_stack.at(ids_stack.length() - 2).index).name)
                                            .arg(error_msg(E_UNDECLARED_SYMBOL)));
                                    result.clear();
                                    return result;
                                }
                                ids_stack.removeLast();
                                ids_stack.removeLast();
                                break;
                            case 103:
                                // <object> ::= <id>-">"<id>
                                // :TODO:
                                ids_stack.removeLast();
                                ids_stack.removeLast();
                                break;

                            case 104:
                            case 105:
                                // <class_props_declaration>
                                is_const = (rule_index_full == 105);
                                while (decl_vars_count) {
                                    symbol_index = addSymbolVariable(table_ids.at(ids_stack.last().index).name,
                                                                     data_type, class_index, is_const);
                                    declared_symbols_indexes << symbol_index;
                                    members_indexes << symbol_index;
                                    setSymbolClassMemberAccessType(symbol_index, access_type);
                                    ids_stack.removeLast();
                                    decl_vars_count--;
                                }
                                break;

                            case 106:
                            //case 107:
                            case 108:
                                //case 109:
                                // <vars_declaration>
                                is_const = ((rule_index_full == 108) || (rule_index_full == 109));
                                while (decl_vars_count) {
                                    declared_symbols_indexes << addSymbolVariable(
                                      table_ids.at(ids_stack.last().index).name, data_type, class_index, is_const);
                                    ids_stack.removeLast();
                                    decl_vars_count--;
                                }
                                break;

                            case 110:
                                data_type = TYPE_INT;
                                break;
                            case 111:
                                data_type = TYPE_DOUBLE;
                                break;
                            case 112:
                                data_type = TYPE_CHAR;
                                break;
                            case 113:
                                data_type = TYPE_BOOL;
                                break;
                            case 114:
                                temp_class_index =
                                  indexOfSymbolDeclaredInBlock(table_ids.at(ids_stack.last().index).name, SYM_CLASS, 0);
                                if (temp_class_index > -1) {
                                    data_type = TYPE_OBJECT;
                                    class_index = temp_class_index;
                                } else {
                                    emit semantic_error(-1, QString("(%1) %2")
                                                              .arg(table_ids.at(ids_stack.last().index).name)
                                                              .arg(error_msg(E_INVALID_OBJECT_TYPE)));
                                    result.clear();
                                    return result;
                                }
                                ids_stack.removeLast();
                                break;

                            case 115:
                            case 116:
                                // <class_props_list>
                            case 117:
                            case 118:
                                // vars_list
                                decl_vars_count++;
                                break;

                                //case 120:
                                //case 121:
                                // see 60

                            default:
                                //args_indexes.clear();
                                //addSymbolFunction(table_ids.at(ids_stack.last().index).name, TYPE_VOID, args_indexes);
                                //ids_stack.removeLast();
                                break;
                        }

                        // ...back to our finite-state machine
                        for (int num = 0; num < rule.right_side.length(); num++) {
                            states_stack.removeLast();
                            tokens_stack.removeLast();
                        }
                        if (goto_table_.at(states_stack.last()).contains(rule.left_token)) {
                            int goto_index = goto_table_.at(states_stack.last()).value(rule.left_token);
                            states_stack << goto_index;
                            tokens_stack << rule.left_token;

                            result << action.index;
                        } else {
                            // goto not defined
                            result.clear();
                            emit syntax_error(-1, error_msg(E_INTERNAL_GOTO_UNDEFINED));
                            return result;
                        }
                    } else {
                        // stacks have too few items
                        result.clear();
                        emit syntax_error(-1, error_msg(E_INTERNAL_STATES_STACK_EMPTY));
                        return result;
                    }
                    break;

                case A_ACCEPT:
                    tokens_accepted = true;

                    // save declared symbols & return to parent block
                    addBlockSymbols(block_index, declared_symbols_indexes);
                    block_index = getParentBlockIndex(block_index);
                    Q_ASSERT(block_index == -1);
                    declared_symbols_indexes.clear();

                    break;
            }
        } else {
            // action not defined
            result.clear();
            emit syntax_error(-1, error_msg(E_INTERNAL_ACTION_UNDEFINED));
            return result;
        }
    }
    if (i >= tokens.length()) {
        // tokens not accepted
        result.clear();
        emit syntax_error(-1, error_msg(E_CHAIN_REJECTED));
        return result;
    }

    return result;
}
