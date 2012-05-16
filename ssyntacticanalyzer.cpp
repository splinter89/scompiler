#include "ssyntacticanalyzer.h"
#include <QDebug>

SSyntacticAnalyzer::SSyntacticAnalyzer(QObject * parent)
{
    this->setParent(parent);
}

SSyntacticAnalyzer::~SSyntacticAnalyzer()
{
    // bye
}

void SSyntacticAnalyzer::setGrammar(QList<GrammarRule> grammar) {
    grammar_ = grammar;
}

QSet<Token> SSyntacticAnalyzer::first(const Token token) {
    QSet<Token> result;

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
            // для каждого правила X -> Y0Y1...Yk-1
            QList<GrammarRule> rules = getGrammarRulesByLeftToken(token, grammar_);
            for (int rule_i = 0; rule_i < rules.length(); rule_i++) {
                GrammarRule rule = rules.at(rule_i);

                bool all_y_got_lambda = true;
                for (int i = 0; i < rule.right_side.length(); i++) {
                    QSet<Token> first_y = first(rule.right_side.at(i));
                    int old_first_y_count = first_y.count();
                    first_y.remove(LAMBDA);

                    // добавляем FIRST(Yi)\{e} к FIRST(X);
                    result += first_y;

                    if (old_first_y_count == first_y.count()) {
                        // в FIRST(Yi) не было e
                        all_y_got_lambda = false;
                        break;
                    }
                }
                if (all_y_got_lambda) {
                    result << LAMBDA;
                }
            }
        } while (result.count() != old_count);
    }

    return result;
}

QSet<Token> SSyntacticAnalyzer::first(const QList<Token> tokens) {
    QSet<Token> result;

    // для каждого X0X1...Xk-1
    bool all_x_got_lambda = true;
    for (int i = 0; i < tokens.length(); i++) {
        QSet<Token> first_x = first(tokens.at(i));
        int old_first_x_count = first_x.count();
        first_x.remove(LAMBDA);

        // добавляем FIRST(Xi)\{e} к FIRST(X);
        result += first_x;

        if (old_first_x_count == first_x.count()) {
            // в FIRST(Xi) не было e
            all_x_got_lambda = false;
            break;
        }
    }
    if (all_x_got_lambda) {
        result << LAMBDA;
    }

    return result;
}


QSet<Situation> SSyntacticAnalyzer::closure(QSet<Situation> i) {
    int old_count;
    do {
        old_count = i.count();
        // для каждой ситуации [A -> alpha.Bbeta, a] из I
        foreach (const Situation &situation, i) {
            int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
            if ((dot_pos > -1) && (dot_pos < situation.right_side.length() - 1)) {
                Token b = situation.right_side.at(dot_pos + 1);
                QList<GrammarRule> rules = getGrammarRulesByLeftToken(b, grammar_);
                if (!rules.isEmpty()) {
                    // для каждого правила вывода B -> gamma из G
                    foreach (const GrammarRule &rule, rules) {
                        QList<Token> test_right_side;
                        if (dot_pos < situation.right_side.length() - 2) {
                            test_right_side = situation.right_side.mid(dot_pos + 2);    // skip .B
                        }
                        test_right_side << situation.look_ahead_token;  // beta a
                        QSet<Token> first_test = first(test_right_side);
                        // для каждого терминала c из FIRST(beta a), такого, что [B ->.gamma, c] нет в I
                        foreach (const Token &c, first_test) {
                            QList<Token> new_right_rule = EmptyTokenList() << DOT_TOKEN;
                            new_right_rule += rule.right_side;     // .gamma
                            Situation new_situation = {b, new_right_rule, c};
                            // добавляем [B ->.gamma, c] к I;
                            i << new_situation;
                        }
                    }
                }
            }
        }

    } while (i.count() != old_count);
    return i;
}

QSet<Situation> SSyntacticAnalyzer::makeStep(const QSet<Situation> i, const Token x) {
    QSet<Situation> j;
    foreach (Situation situation, i) {
        int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
        if ((dot_pos > -1)
            && (dot_pos < situation.right_side.length() - 1)
            && (situation.right_side.at(dot_pos + 1) == x)
        ) {
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

int SSyntacticAnalyzer::addSymbolArgument(QString name, DataType data_type, int class_index, ArgType arg_type, bool is_const)
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
    if ((symbol.type == SYM_FUNCTION)
            || (symbol.type == SYM_VARIABLE)) {
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
                                         symbol_table_.at(declared_symbols_index).type,
                                         block_index) > -1) {
            emit semantic_error(-1, QString("(%1 @ block #%2) %3")
                                .arg(symbol_table_.at(declared_symbols_index).toString())
                                .arg(block_index)
                                .arg(error_msg(E_ALREADY_DECLARED_IN_BLOCK)));
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
                || ((symbol_table_.at(declared_symbols_index).type == SYM_ARGUMENT)
                    && (type == SYM_VARIABLE)))) {
            res = declared_symbols_index;
        }
    }

    return res;
}
int SSyntacticAnalyzer::indexOfSymbolInCurrentBlock(QString name, SymbolType type, int block_index)
{
    int res = -1;

    while ((res == -1) && (block_index != -1)) {
        res = indexOfSymbolDeclaredInBlock(name, type, block_index);
        if (res == -1) {
            block_index = block_table_.at(block_index).parent_block_index;
        }
    }

    return res;
}

bool SSyntacticAnalyzer::generateSetOfSituations() {
    if (grammar_.isEmpty()) return false;

    // initial situation
    Situation s = {
        N_S,
        EmptyTokenList() << DOT_TOKEN << N_PROGRAM,
        EOF_TOKEN
    };
    QSet<Situation> i;
    QList<QSet<Situation> > c;
    QSet<Token> all_tokens = getAllGrammarTokens(grammar_);

    i << s;
    i = closure(i);
    c << i;

    int old_count;
    do {
        old_count = c.count();
        foreach (const QSet<Situation> &j, c) {
            foreach (const Token &x, all_tokens) {
                QSet<Situation> new_i = makeStep(j, x);
                if (!new_i.isEmpty() && !c.contains(new_i)) {
                    c << new_i;
                }
            }
        }
    } while (c.count() != old_count);
    ultimate_situations_set_ = c;
    return true;
}

bool SSyntacticAnalyzer::generateActionGotoTables() {
    if (ultimate_situations_set_.isEmpty()) return false;

    QSet<Token> terminals = getAllTerminalTokens();
    QSet<Token> non_terminals = getAllNonTerminalTokens(grammar_);

    action_table_.clear();
    goto_table_.clear();

    foreach (const QSet<Situation> &i, ultimate_situations_set_) {
        QHash<Token, Action> action_row;
        QHash<Token, int> goto_row;

        foreach (const Situation &situation, i) {
            // action
            foreach (const Token &terminal, terminals) {
                int dot_pos = situation.right_side.indexOf(DOT_TOKEN);
                // step 1
                if ((dot_pos > -1)
                    && (dot_pos < situation.right_side.length() - 1)
                    && (situation.right_side.at(dot_pos + 1) == terminal)
                ) {
                    QSet<Situation> test_i = makeStep(i, terminal);
                    int j = ultimate_situations_set_.indexOf(test_i);
                    if (j > -1) {
                        Action new_action = {A_SHIFT, j};
                        if (!action_row.contains(terminal)) {
                            action_row.insert(terminal, new_action);
                        } else if ((action_row.value(terminal).type != new_action.type)
                                || (action_row.value(terminal).index != new_action.index)
                        ) {
                            // error - not LR(1)
                            emit syntax_error(-1, "(step 1 gen)" + error_msg(E_NOT_LR1_GRAMMAR));
                            return false;
                        }
                    }
                }
                // step 2
                if ((dot_pos == situation.right_side.length() - 1)
                    && (situation.left_token != N_S)
                    && (situation.look_ahead_token == terminal)
                ) {
                    int j = indexOfGrammarRule(
                                situation.left_token,
                                situation.right_side.mid(0, situation.right_side.length() - 1),
                                grammar_
                            );
                    if (j > -1) {
                        Action new_action = {A_REDUCE, j};
                        if (!action_row.contains(terminal)) {
                            action_row.insert(terminal, new_action);
                        } else {
                            // error - not LR(1)
                            emit syntax_error(-1, "(step 2-1 gen)" + error_msg(E_NOT_LR1_GRAMMAR));
                            return false;
                        }
                    } else {
                        // error - grammar rule not found
                        emit syntax_error(-1, "(step 2-2 gen)" + error_msg(E_INTERNAL_GENERATING_TABLES));
                        return false;
                    }
                }
            }
            // step 3
            if ((situation.left_token == N_S)
                && (situation.right_side == (EmptyTokenList() << N_PROGRAM << DOT_TOKEN))
                && (situation.look_ahead_token == EOF_TOKEN)
            ) {
                Action new_action = {A_ACCEPT, 0};
                if (!action_row.contains(EOF_TOKEN)) {
                    action_row.insert(EOF_TOKEN, new_action);
                } else {
                    // error
                    emit syntax_error(-1, "(step 3 gen)" + error_msg(E_INTERNAL_GENERATING_TABLES));
                    return false;
                }
            }

            // goto
            foreach (const Token &non_terminal, non_terminals) {
                QSet<Situation> test_i = makeStep(i, non_terminal);
                int j = ultimate_situations_set_.indexOf(test_i);
                if (j > -1) {
                    int new_goto = j;
                    if (!goto_row.contains(non_terminal)) {
                        goto_row.insert(non_terminal, new_goto);
                    } else if (goto_row.value(non_terminal) != new_goto) {
                        // error - not LR(1)
                        emit syntax_error(-1, "(step goto)" + error_msg(E_NOT_LR1_GRAMMAR));
                        return false;
                    }
                }
            }
        }
        action_table_ << action_row;
        goto_table_ << goto_row;
    }
    return true;
}

QList<int> SSyntacticAnalyzer::process(QList<TokenPointer> tokens,
                                       QList<TokenId> table_ids,
                                       QList<TokenConst> table_consts,
                                       QList<TokenKeyword> table_keywords,
                                       QList<TokenSeparator> table_separators) {
    Q_UNUSED(table_consts)

    QList<int> result;
    if (grammar_.isEmpty() || action_table_.isEmpty() || goto_table_.isEmpty()) return result;

    int i = 0;
    bool tokens_accepted = false;

    QList<int> states_stack;
    QList<Token> tokens_stack;
    QList<TokenPointer> ids_stack;

    int parent_block_index;
    QList<int> declared_symbols_indexes;
    QList<int> declared_arg_symbols_indexes;
    int symbol_index;
    // symbol params
    QList<int> args_indexes;
    DataType data_type;
    int decl_vars_count;
    int class_index;
    AccessSpecifier access_type;
    QList<int> members_indexes;
//    QList<int> declared_members_indexes;

    states_stack.clear();
    tokens_stack.clear();
    ids_stack.clear();
    states_stack << 0; // initial state

    symbol_table_.clear();
    block_table_.clear();

    parent_block_index = addEmptyBlock(-1); // global context
    declared_symbols_indexes.clear();
    declared_arg_symbols_indexes.clear();
    args_indexes.clear();
    decl_vars_count = 0;
    class_index = -1;
    access_type = ACCESS_PRIVATE;
    members_indexes.clear();
//    declared_members_indexes.clear();

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
                    addBlockSymbols(parent_block_index, declared_symbols_indexes);
                    declared_symbols_indexes.clear();

                    parent_block_index = addEmptyBlock(parent_block_index);
                } else if (token == S_CURLY_CLOSE) {
                    // save declared symbols & return to parent block
                    addBlockSymbols(parent_block_index, declared_arg_symbols_indexes + declared_symbols_indexes);
                    declared_symbols_indexes.clear();
                    declared_arg_symbols_indexes.clear();

                    parent_block_index = getParentBlockIndex(parent_block_index);
                }
                tokens_stack << token;
                i++;
                break;

            case A_REDUCE:
                rule = grammar_.at(action.index);
                if ((states_stack.length() > rule.right_side.length())
                     && (tokens_stack.length() >= rule.right_side.length())
                ) {
                    // full grammar because we will depend on the number of rules
                    int rule_index_full = indexOfGrammarRule(rule.left_token, rule.right_side, Grammar_full);

                    // filling up the symbol table
                    switch (rule_index_full) {
                    case 7:
                        members_indexes.clear();
                        declared_symbols_indexes << addSymbolClass(table_ids.at(ids_stack.last().index).name,
                                                                   members_indexes);
                        ids_stack.removeLast();
                        break;

                    case 8:
                        declared_symbols_indexes << addSymbolClass(table_ids.at(ids_stack.last().index).name,
                                                                   members_indexes);
                        members_indexes.clear();
                        ids_stack.removeLast();
                        break;


                    case 9:
                    case 11:
//                        members_indexes += declared_members_indexes;
//                        declared_members_indexes.clear();
                        break;
                    case 10:
                    case 12:
//                        members_indexes += declared_members_indexes;
//                        declared_members_indexes.clear();
//                        access_type = ACCESS_PRIVATE;
                        break;

//                    case 13:
//                    case 15:
//                        break;
//                    case 14:
//                    case 16:
//                        break;


                    case 20:
                        access_type = ACCESS_PUBLIC;
                        break;
                    case 21:
                        access_type = ACCESS_PRIVATE;
                        break;

//                    case 22000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 23000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

                    case 24:
                        args_indexes.clear();
                        symbol_index = addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                         data_type,
                                                         args_indexes);
                        declared_symbols_indexes << symbol_index;
                        members_indexes << symbol_index;
                        setSymbolClassMemberAccessType(symbol_index, access_type);
                        ids_stack.removeLast();
                        break;
                    case 25:
                        symbol_index = addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                         data_type,
                                                         args_indexes);
                        declared_symbols_indexes << symbol_index;
                        members_indexes << symbol_index;
                        setSymbolClassMemberAccessType(symbol_index, access_type);
                        args_indexes.clear();
                        ids_stack.removeLast();
                        break;
                    case 26:
                        args_indexes.clear();
                        symbol_index = addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                         TYPE_VOID,
                                                         args_indexes);
                        declared_symbols_indexes << symbol_index;
                        members_indexes << symbol_index;
                        setSymbolClassMemberAccessType(symbol_index, access_type);
                        ids_stack.removeLast();
                        break;
                    case 27:
                        symbol_index = addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                         TYPE_VOID,
                                                         args_indexes);
                        declared_symbols_indexes << symbol_index;
                        members_indexes << symbol_index;
                        setSymbolClassMemberAccessType(symbol_index, access_type);
                        args_indexes.clear();
                        ids_stack.removeLast();
                        break;

//                    case 28000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 29000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 30000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 31000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 32000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 33000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 34000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 35000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 36000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 37000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

                    case 38:
                        args_indexes.clear();
                        declared_symbols_indexes << addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                                      data_type,
                                                                      args_indexes);
                        ids_stack.removeLast();
                        break;
                    case 39:
                        declared_symbols_indexes << addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                                      data_type,
                                                                      args_indexes);
                        args_indexes.clear();
                        ids_stack.removeLast();
                        break;
                    case 40:
                        args_indexes.clear();
                        declared_symbols_indexes << addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                                      TYPE_VOID,
                                                                      args_indexes);
                        ids_stack.removeLast();
                        break;
                    case 41:
                        declared_symbols_indexes << addSymbolFunction(table_ids.at(ids_stack.last().index).name,
                                                                      TYPE_VOID,
                                                                      args_indexes);
                        args_indexes.clear();
                        ids_stack.removeLast();
                        break;


                    case 44:
                        // :TODO: check whether class exists
                        symbol_index = addSymbolArgument(table_ids.at(ids_stack.last().index).name,
                                                         data_type,
                                                         class_index,
                                                         ARG_BY_VALUE,
                                                         false);
                        args_indexes << symbol_index;
                        declared_arg_symbols_indexes << symbol_index;
                        ids_stack.removeLast();
                        break;
                    case 45:
                        symbol_index = addSymbolArgument(table_ids.at(ids_stack.last().index).name,
                                                         data_type,
                                                         class_index,
                                                         ARG_BY_VALUE,
                                                         true);
                        args_indexes << symbol_index;
                        declared_arg_symbols_indexes << symbol_index;
                        ids_stack.removeLast();
                        break;
                    case 46:
                        symbol_index = addSymbolArgument(table_ids.at(ids_stack.last().index).name,
                                                         data_type,
                                                         class_index,
                                                         ARG_BY_REFERENCE,
                                                         false);
                        args_indexes << symbol_index;
                        declared_arg_symbols_indexes << symbol_index;
                        ids_stack.removeLast();
                        break;
                    case 47:
                        symbol_index = addSymbolArgument(table_ids.at(ids_stack.last().index).name,
                                                         data_type,
                                                         class_index,
                                                         ARG_BY_REFERENCE,
                                                         true);
                        args_indexes << symbol_index;
                        declared_arg_symbols_indexes << symbol_index;
                        ids_stack.removeLast();
                        break;


                    case 104:
                        ids_stack.removeLast();
                        break;
                    case 105:
                        ids_stack.removeLast();
                        ids_stack.removeLast();
                        break;
                    case 106:
                        ids_stack.removeLast();
                        ids_stack.removeLast();
                        break;


                    case 107:
                        while (decl_vars_count) {
                            symbol_index = addSymbolVariable(table_ids.at(ids_stack.last().index).name,
                                                             data_type,
                                                             class_index,
                                                             false);
                            declared_symbols_indexes << symbol_index;
                            members_indexes << symbol_index;
                            setSymbolClassMemberAccessType(symbol_index, access_type);
                            ids_stack.removeLast();
                            decl_vars_count--;
                        }
                        break;
                    case 108:
                        while (decl_vars_count) {
                            symbol_index = addSymbolVariable(table_ids.at(ids_stack.last().index).name,
                                                             data_type,
                                                             class_index,
                                                             true);
                            declared_symbols_indexes << symbol_index;
                            members_indexes << symbol_index;
                            setSymbolClassMemberAccessType(symbol_index, access_type);
                            ids_stack.removeLast();
                            decl_vars_count--;
                        }
                        break;


                    case 109:
                        while (decl_vars_count) {
                            declared_symbols_indexes << addSymbolVariable(table_ids.at(ids_stack.last().index).name,
                                                                          data_type,
                                                                          class_index,
                                                                          false);
                            ids_stack.removeLast();
                            decl_vars_count--;
                        }
                        break;
//                    case 110:
//                        break;
                    case 111:
                        while (decl_vars_count) {
                            declared_symbols_indexes << addSymbolVariable(table_ids.at(ids_stack.last().index).name,
                                                                          data_type,
                                                                          class_index,
                                                                          true);
                            ids_stack.removeLast();
                            decl_vars_count--;
                        }
                        break;
//                    case 112:
//                        break;


                    case 113:
                        data_type = TYPE_INT;
                        break;
                    case 114:
                        data_type = TYPE_DOUBLE;
                        break;
                    case 115:
                        data_type = TYPE_CHAR;
                        break;
                    case 116:
                        data_type = TYPE_BOOL;
                        break;
                    case 117:
//                        if (found class with name "table_ids.at(ids_stack.last().index).name" in top block) {
//                            class_index = ...;
//                        } else {
//                            error - class not defined
//                        }
                        ids_stack.removeLast();
                        break;


                    case 118:
                        decl_vars_count++;
                        break;
                    case 119:
                        decl_vars_count++;
                        break;

                    case 120:
                        decl_vars_count++;
                        break;

                    case 121:
                        decl_vars_count++;
                        break;

//                    case 122000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 123000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 124000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

//                    case 125000:
//                        args_indexes.clear();
//                        addSymbolFunction(table_ids.at(ids_stack.last().index).name,
//                                          TYPE_VOID,
//                                          args_indexes);
//                        ids_stack.removeLast();
//                        break;

                    default:
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
                        // error - goto not defined
                        result.clear();
                        emit syntax_error(-1, error_msg(E_INTERNAL_GOTO_UNDEFINED));
                        return result;
                    }
                } else {
                    // error - stacks have too few items
                    result.clear();
                    emit syntax_error(-1, error_msg(E_INTERNAL_STATES_STACK_EMPTY));
                    return result;
                }
                break;

            case A_ACCEPT:
                tokens_accepted = true;

                // save declared symbols & return to parent block
                addBlockSymbols(parent_block_index, declared_symbols_indexes);
                parent_block_index = getParentBlockIndex(parent_block_index);
                Q_ASSERT(parent_block_index == -1);
                declared_symbols_indexes.clear();

                break;
            }
        } else {
            // error - action not defined
            result.clear();
            emit syntax_error(-1, error_msg(E_INTERNAL_ACTION_UNDEFINED));
            return result;
        }
    }
    if (i >= tokens.length()) {
        // error - tokens not accepted
        result.clear();
        emit syntax_error(-1, error_msg(E_CHAIN_REJECTED));
        return result;
    }

    return result;
}
