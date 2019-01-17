#include "ssyntacticanalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject* parent)
{
    this->setParent(parent);
}

void SSyntacticAnalyzer::setGrammar(QList<GrammarRule> grammar, const QString cache_filename)
{
    grammar_ = grammar;

    ultimate_situations_set_.clear();
    action_table_.clear();
    goto_table_.clear();
    first_by_token_.clear();

    QTime timer;
    timer.start();
    bool use_cache = (grammar_.length() > 1);
    if (!use_cache || !readFromCache(cache_filename, false)) {
        if (!use_cache || !readFromCache(cache_filename, true)) {
            if (!generateSetOfSituations()) return;
            if (use_cache) writeToCache(cache_filename);
        }

        if (!generateActionGotoTables()) return;
        if (use_cache) writeToCache(cache_filename);
    }
    qDebug() << QString("grammar updated: %1 sec").arg(QString::number(timer.elapsed() / 1000., 'f', 3));
}

bool SSyntacticAnalyzer::readFromCache(const QString filename, const bool only_situations_set)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDataStream in(&file);
    in >> ultimate_situations_set_ >> action_table_ >> goto_table_;
    file.close();

    if (ultimate_situations_set_.isEmpty()) return false;
    if (!only_situations_set && (action_table_.isEmpty() || goto_table_.isEmpty())) return false;

    qDebug() << "read syntactic tables from" << filename << (only_situations_set ? "(situations only)" : "(all)");
    return true;
}

bool SSyntacticAnalyzer::writeToCache(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QDataStream out(&file);
    out << ultimate_situations_set_ << action_table_ << goto_table_;
    file.close();

    qDebug() << "written syntactic tables to" << filename;
    return true;
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
            if ((dot_pos == -1) || (dot_pos == situation.right_side.length() - 1)) continue;

            Token b = situation.right_side.at(dot_pos + 1);
            QList<GrammarRule> rules = getGrammarRulesByLeftToken(b, grammar_);
            if (rules.isEmpty()) continue;

            // for each derivation rule B -> gamma from G
            foreach (const GrammarRule& rule, rules) {
                QList<Token> beta_a = situation.right_side.mid(dot_pos + 2) << situation.look_ahead_token;
                // for each terminal c from FIRST(beta a)
                foreach (const Token& c, first(beta_a)) {
                    QList<Token> dot_gamma = EmptyTokenList() << DOT_TOKEN << rule.right_side;
                    Situation new_situation = {b, dot_gamma, c};
                    // add [B ->.gamma, c] to I;
                    i << new_situation;
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
        if ((dot_pos == -1) || (dot_pos == situation.right_side.length() - 1)) continue;
        if (situation.right_side.at(dot_pos + 1) != x) continue;

        situation.right_side.swap(dot_pos, dot_pos + 1);  // alpha.Xbeta => alphaX.beta
        j << situation;
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
    if ((symbol.type != SYM_FUNCTION) && (symbol.type != SYM_VARIABLE)) return;

    symbol.access_type = access_type;
    symbol_table_.replace(symbol_index, symbol);
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
        Symbol symbol = symbol_table_.at(declared_symbols_index);
        int index = indexOfSymbolDeclaredInBlock(symbol.name, symbol.type, block_index);
        if (index > -1) {
            emit semantic_error(-1, QString("(%1 @ block #%2) %3")
                                      .arg(symbol.toString())
                                      .arg(block_index)
                                      .arg(error_msg(E_ALREADY_DECLARED_IN_BLOCK)));
            return;
        }
    }

    block.declared_symbols_indexes << declared_symbols_indexes;
    block_table_.replace(block_index, block);
}

int SSyntacticAnalyzer::getParentBlockIndex(int block_index)
{
    return block_table_.at(block_index).parent_block_index;
}

int SSyntacticAnalyzer::indexOfSymbolDeclaredInBlock(QString name, SymbolType type, int block_index)
{
    int res = -1;

    foreach (int declared_symbols_index, block_table_.at(block_index).declared_symbols_indexes) {
        Symbol symbol = symbol_table_.at(declared_symbols_index);
        if ((symbol.name == name)
            && ((symbol.type == type) || ((symbol.type == SYM_ARGUMENT) && (type == SYM_VARIABLE))
                || ((symbol.type == SYM_VARIABLE) && (type == SYM_ARGUMENT)))) {
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

    Situation initial_situation = {N_S, EmptyTokenList() << DOT_TOKEN << N_PROGRAM, EOF_TOKEN};
    QList<QSet<Situation>> c, c_new;
    int q1 = 1;
    QSet<Token> all_tokens = getAllGrammarTokens(grammar_);

    c << closure(QSet<Situation>() << initial_situation);

    int old_count;
    do {
        old_count = c.count();
        foreach (const QSet<Situation>& i, c) {
            foreach (const Token& x, all_tokens) {
                QSet<Situation> go_to = makeStep(i, x);
                if (go_to.isEmpty()) continue;
                if (c.contains(go_to) || c_new.contains(go_to)) continue;

                c_new << go_to;
                q1 += go_to.count();
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
    Situation accept_situation = {N_S, EmptyTokenList() << N_PROGRAM << DOT_TOKEN, EOF_TOKEN};

    int q1 = 0, q2 = 0;
    foreach (const QSet<Situation>& i, ultimate_situations_set_) {
        QHash<Token, Action> action_row;
        QHash<Token, int> goto_row;

        foreach (const Situation& situation, i) {
            // action
            int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
            foreach (const Token& terminal, terminals) {
                if ((dot_pos > -1) && (dot_pos < situation.right_side.length() - 1)
                    && (situation.right_side.at(dot_pos + 1) == terminal)) {
                    // step 1
                    QSet<Situation> go_to = makeStep(i, terminal);
                    int j = ultimate_situations_set_.indexOf(go_to);
                    if (j == -1) continue;

                    Action new_action = {A_SHIFT, j};
                    if (action_row.contains(terminal)) {
                        if (action_row.value(terminal) == new_action) continue;

                        // conflict
                        qDebug() << "(step 1)" << situation << terminal << action_row.value(terminal) << new_action;
                        emit syntax_error(-1, "(step 1) " + error_msg(E_NOT_LR1_GRAMMAR));
                        return false;
                    }

                    action_row.insert(terminal, new_action);
                } else if ((dot_pos == situation.right_side.length() - 1) && (situation.left_token != N_S)
                           && (situation.look_ahead_token == terminal)) {
                    // step 2
                    int j = indexOfGrammarRule(
                      situation.left_token, situation.right_side.mid(0, situation.right_side.length() - 1), grammar_);
                    if (j == -1) {
                        // grammar rule not found
                        emit syntax_error(-1, "(step 2) " + error_msg(E_INTERNAL_GENERATING_TABLES));
                        return false;
                    }

                    Action new_action = {A_REDUCE, j};
                    if (action_row.contains(terminal)) {
                        // conflict
                        qDebug() << "(step 2)" << situation << terminal << action_row.value(terminal) << new_action;
                        emit syntax_error(-1, "(step 2) " + error_msg(E_NOT_LR1_GRAMMAR));
                        return false;
                    }

                    action_row.insert(terminal, new_action);
                }
            }
            if (situation == accept_situation) {
                // step 3
                Action new_action = {A_ACCEPT, 0};
                if (action_row.contains(EOF_TOKEN)) {
                    // error
                    emit syntax_error(-1, "(step 3) " + error_msg(E_INTERNAL_GENERATING_TABLES));
                    return false;
                }

                action_row.insert(EOF_TOKEN, new_action);
            }

            // goto
            foreach (const Token& non_terminal, non_terminals) {
                QSet<Situation> go_to = makeStep(i, non_terminal);
                int j = ultimate_situations_set_.indexOf(go_to);
                if (j == -1) continue;

                if (goto_row.contains(non_terminal)) {
                    if (goto_row.value(non_terminal) == j) continue;

                    // conflict
                    qDebug() << "(goto)" << situation << non_terminal << goto_row.value(non_terminal) << j;
                    emit syntax_error(-1, "(goto) " + error_msg(E_NOT_LR1_GRAMMAR));
                    return false;
                }

                goto_row.insert(non_terminal, j);
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
    int object_symbol_id = -1;
    DataType expr_type;
    bool is_const;
    DataType rvalue_type;
    int temp_class_index;  // for class search
    // symbol params
    QList<int> args_indexes;
    ArgType arg_type;
    QList<DataType> data_types_stack;
    DataType temp_data_type;  // for list of vars
    int decl_vars_count = 0;
    int class_index = -1;  // for data_types_stack
    AccessSpecifier access_type = ACCESS_PUBLIC;
    QList<int> members_indexes;

    states_stack << 0;  // initial state

    symbol_table_.clear();
    block_table_.clear();

    block_index = addEmptyBlock(-1);  // global context

    while (!tokens_accepted && (i < tokens.length())) {
        int state = states_stack.last();
        Token token = tokens.at(i).type;
        if (token == T_KEYWORD) {
            token = table_keywords.at(tokens.at(i).index).type;
        } else if (token == T_SEPARATOR) {
            token = table_separators.at(tokens.at(i).index).type;
        }

        if (!action_table_.at(state).contains(token)) {
            // action not defined
            result.clear();
            emit syntax_error(-1, error_msg(E_INTERNAL_ACTION_UNDEFINED));
            return result;
        }

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

            case A_REDUCE: {
                rule = grammar_.at(action.index);
                if ((states_stack.length() <= rule.right_side.length())
                    || (tokens_stack.length() < rule.right_side.length())) {
                    // stacks have too few items
                    result.clear();
                    emit syntax_error(-1, error_msg(E_INTERNAL_STATES_STACK_EMPTY));
                    return result;
                }

                // full grammar because we logic depends on the indices
                int rule_index_full = indexOfGrammarRule(rule.left_token, rule.right_side, Grammar_full);

                // filling up the symbol table
                switch (rule_index_full) {
                    case 3:
                    case 4:
                        // <program_element>
                        members_indexes.clear();
                        break;

                    case 5:
                    case 6:
                        // <class>
                        if (rule_index_full == 5) Q_ASSERT(members_indexes.isEmpty());

                        declared_symbols_indexes
                          << addSymbolClass(table_ids.at(ids_stack.last().index).name, members_indexes);
                        ids_stack.removeLast();
                        break;

                    case 11:
                        access_type = ACCESS_PUBLIC;
                        break;
                    case 12:
                        access_type = ACCESS_PRIVATE;
                        break;

                    case 14:
                    case 16:
                        // <vars_or_function>, function
                        if (rule_index_full == 16) data_types_stack << TYPE_VOID;
                        symbol_index = addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                         data_types_stack.takeLast(), args_indexes);
                        declared_symbols_indexes << symbol_index;
                        members_indexes << symbol_index;
                        //setSymbolClassMemberAccessType(symbol_index, access_type); :TODO: in/out of class?
                        args_indexes.clear();
                        ids_stack.removeLast();
                        break;

                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                        // <vars>
                        is_const = (rule_index_full >= 26) && (rule_index_full <= 29);
                        temp_data_type = data_types_stack.takeLast();
                        decl_vars_count++;
                        while (decl_vars_count) {
                            symbol_index = addSymbolVariable(table_ids.at(ids_stack.last().index).name, temp_data_type,
                                                             class_index, is_const);
                            declared_symbols_indexes << symbol_index;
                            members_indexes << symbol_index;
                            //setSymbolClassMemberAccessType(symbol_index, access_type); :TODO: in/out of class?
                            ids_stack.removeLast();
                            decl_vars_count--;
                        }
                        break;

                    case 30:
                        data_types_stack << TYPE_INT;
                        break;
                    case 31:
                        data_types_stack << TYPE_DOUBLE;
                        break;
                    case 32:
                        data_types_stack << TYPE_CHAR;
                        break;
                    case 33:
                        data_types_stack << TYPE_BOOL;
                        break;
                    case 34:
                        // <var_type> ::= <id>
                        temp_class_index =
                          indexOfSymbolDeclaredInBlock(table_ids.at(ids_stack.last().index).name, SYM_CLASS, 0);
                        if (temp_class_index > -1) {
                            data_types_stack << TYPE_OBJECT;
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

                    case 35:
                    case 36:
                    case 37:
                    case 38:
                        // <more_vars>
                        decl_vars_count++;
                        break;

                    case 39:
                    case 40:
                        // <function> without arguments
                        Q_ASSERT(args_indexes.isEmpty());
                        break;

                    case 47:
                    case 48:
                    case 51:
                    case 52:
                    case 55:
                    case 56:
                    case 59:
                    case 60:
                        // <argument>
                        is_const = (rule_index_full >= 53) && (rule_index_full <= 60);
                        arg_type = (((rule_index_full >= 45) && (rule_index_full <= 48))
                                    || ((rule_index_full >= 53) && (rule_index_full <= 56)))
                                     ? ARG_BY_VALUE
                                     : ARG_BY_REFERENCE;
                        symbol_index = addSymbolArgument(table_ids.at(ids_stack.last().index).name,
                                                         data_types_stack.takeLast(), class_index, arg_type, is_const);
                        args_indexes << symbol_index;
                        declared_arg_symbols_indexes << symbol_index;
                        ids_stack.removeLast();
                        break;

                    case 108:
                        // <operator_9>, assign const
                        expr_type = table_consts.at(tokens.at(i - 1).index).type;
                        // :TODO: need to use token_stack - not tokens (??? - check)
                        // tokens only work for "a = 12" but not for "a = 12 + d"
                        break;
                    case 109:
                    case 110:
                    case 111:
                        // <operator_9>, assign object or function call
                        expr_type = symbol_table_.at(object_symbol_id).data_type;
                        //qDebug() << dataTypeToString(expr_type, class_index) << "expr_type";
                        // :TODO: use expr_type_stack to check compatibility in rules that combine two operands
                        break;

                    case 113:
                    case 114:
                        // <function_call>
                        if (symbol_table_.at(object_symbol_id).type != SYM_FUNCTION) {
                            emit semantic_error(-1, QString("(Call to %1) %2")
                                                      .arg(symbol_table_.at(object_symbol_id).name)
                                                      .arg(error_msg(E_OBJECT_IS_NOT_FUNCTION)));
                            result.clear();
                            return result;
                        }
                        break;

                    case 115:
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
                    case 116:
                        // <object> ::= <id>.<id>
                        temp_class_index = indexOfSymbolInCurrentBlock(
                          table_ids.at(ids_stack.at(ids_stack.length() - 2).index).name, SYM_CLASS, block_index,
                          declared_arg_symbols_indexes + declared_symbols_indexes);
                        if (temp_class_index > -1) {
                            bool ok = false;
                            foreach (int members_index, symbol_table_.at(temp_class_index).members_indexes) {
                                if (symbol_table_.at(members_index).name == table_ids.at(ids_stack.last().index).name) {
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
                            emit semantic_error(-1,
                                                QString("(%1) %2")
                                                  .arg(table_ids.at(ids_stack.at(ids_stack.length() - 2).index).name)
                                                  .arg(error_msg(E_UNDECLARED_SYMBOL)));
                            result.clear();
                            return result;
                        }
                        ids_stack.removeLast();
                        ids_stack.removeLast();
                        break;
                    case 117:
                        // <object> ::= <id>-">"<id>
                        // :TODO:
                        ids_stack.removeLast();
                        ids_stack.removeLast();
                        break;

                    default:
                        //args_indexes.clear();
                        //addSymbolFunction(table_ids.at(ids_stack.last().index).name, TYPE_VOID, args_indexes);
                        //ids_stack.removeLast();
                        break;
                }

                // perform extra checks
                switch (rule_index_full) {
                    case 24:
                    case 25:
                    case 28:
                    case 29:
                        // <vars> with assignment
                    case 36:
                    case 38:
                        // <more_vars> with assignment
                    case 76:
                    case 77:
                    case 78:
                    case 79:
                    case 80:
                    case 81:
                        // <operator_1> with assignment
                        rvalue_type = expr_type;  // :TODO: data_type? smth else?
                        //qDebug() << dataTypeToString(symbol_table_.at(object_symbol_id).data_type, class_index) << "lvalue_type";
                        if (symbol_table_.at(object_symbol_id).data_type != rvalue_type) {
                            emit semantic_error(
                              -1, QString("(%1 and %2) %3")
                                    .arg(dataTypeToString(symbol_table_.at(object_symbol_id).data_type, class_index))
                                    .arg(dataTypeToString(rvalue_type, class_index))
                                    .arg(error_msg(E_TYPES_MISMATCH)));
                        }
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
                break;
            }
            case A_ACCEPT:
                tokens_accepted = true;

                // save declared symbols & return to parent block
                addBlockSymbols(block_index, declared_symbols_indexes);
                block_index = getParentBlockIndex(block_index);
                Q_ASSERT(block_index == -1);
                declared_symbols_indexes.clear();

                break;
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
