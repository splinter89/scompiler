<program> ::= {<program>}<program_element>
<program_element> ::= (<class>|<class_method_impl>|<vars_declaration>|<function>)
<class> ::= class <id>"{"{[<access_spec>:]{(<class_props_declaration>|<class_method>)}}"}";
<access_spec> ::= (public|private)
<class_method> ::= [((<var_type>|void) |~)]<id>"("[<arguments_list>]")"<block>
<class_method_impl> ::= ([(<var_type>|void) ]<id>::|<id>::~)<id>"("[<arguments_list>]")"<block>
<function> ::= (<var_type>|void) <id>"("[<arguments_list>]")"<block>
<arguments_list> ::= {<arguments_list>,}<argument>
<argument> ::= [const ]<var_type>[&]<id>
<block> ::= "{"{<block_element>}"}"
<block_element> ::= (<vars_declaration>|[(<expression>|<return>)];|<loop>|<branching>)
<return> ::= return[ <expression>]
<expression> ::= [<expression>,]<operator_1>
<operator_1> ::= (<object>(=|+=|-=|*=|/=|%=)<operator_1>|<operator_2>)
<operator_2> ::= [<operator_2>"||"]<operator_3>
<operator_3> ::= [<operator_3>&&]<operator_4>
<operator_4> ::= [<operator_4>(==|!=)]<operator_5>
<operator_5> ::= [<operator_5>("<"|"<"=|">"|">"=)]<operator_6>
<operator_6> ::= [<operator_6>(+|-)]<operator_7>
<operator_7> ::= [<operator_7>(*|/|%)]<operator_8>
<operator_8> ::= ([(+|-|!|*)]<operator_9>|(++|--|&)<object>)
<operator_9> ::= ("("<expression>")"|<literal>|<object>[(++|--)]|<function_call>)
<function_call> ::= <object>"("[<expression>]")"
<object> ::= [<id>(.|-">")]<id>
<class_props_declaration> ::= [const ]<var_type> <class_props_list>;
<vars_declaration> ::= [const ]<var_type> <vars_list>;
<var_type> ::= (int|double|char|bool|<id>)
<class_props_list> ::= {<class_props_list>,}[*]<id>
<vars_list> ::= {<vars_list>,}[*]<id>[=<operator_1>]
<loop> ::= (<while-loop>|<do-while-loop>|<for-loop>)
<while-loop> ::= while"("<expression>")"<block>
<do-while-loop> ::= do<block>while"("<expression>")";
<for-loop> ::= for"("<expression>;<expression>;<expression>")"<block>
<branching> ::= if"("<expression>")"<block>[else<block>]

// lexical stuff
<id> ::= <letter>{(<letter>|<digit>|_)}
<literal> ::= (<boolean_literal>|<integer_literal>|<floating_point_literal>|<char_literal>|<string_literal>)

<boolean_literal> ::= (true|false)
<floating_point_literal> ::= [(+|-)](<digit>{<digit>}.[<digit>{<digit>]|.<digit>{<digit>})[(e|E)<decimal_literal>]
<integer_literal> ::= (<decimal_literal>|<octal_literal>|<hex_literal>)
<decimal_literal> ::= [(+|-)]<digit>{<digit>}
<octal_literal> ::= [(+|-)]0{(0|...|7)}
<hex_literal> ::= [(+|-)]0x(0|...|9|a|...|f|A|...F){(0|...|9|a|...|f|A|...F)}
<digit> ::= (0|...|9)
<letter> ::= (a|...|z|A|...|Z)
<char_literal> ::= '(...|\('|"""|n|t))'
<string_literal> ::= """{(...|\('|"""|n|t))}"""

// lvalue == object
// rvalue == expression
