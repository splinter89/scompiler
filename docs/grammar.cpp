<program> ::= {<program>}<program_element>
<program_element> ::= (<class>|<vars_or_function>)
<class> ::= class <id>"{"{[<access_spec>:]<vars_or_function>}"}";
<access_spec> ::= (public|private)
<vars_or_function> ::= (<vars>|[(<var_type>|void) ][<id>::]<id><function>|[<id>::]~<id><function>)
<vars> ::= [const ]<var_type>( |*)<id>[=<operator_2>][<more_vars>];
<var_type> ::= (int|double|char|bool|<id>)
<more_vars> ::= {<more_vars>},[*]<id>[=<operator_2>]
<function> ::= "("[<arguments>]")"(;|<block>)
<arguments> ::= {<arguments>,}[const ]<var_type>[(&| <id>|&<id>)][=<literal>]
<block> ::= "{"{<block_element>}"}"
<block_element> ::= (<vars>|[(<expression>|<return>)];|<loop>|<branching>)
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
<operator_9> ::= ("("<operator_2>")"|<literal>|<object>[(++|--)]|<function_call>)
<function_call> ::= <object>"("[<expression>]")"
<object> ::= [<id>(.|-">")]<id>
<loop> ::= (<while_loop>|<do_while_loop>|<for_loop>)
<while_loop> ::= while"("<expression>")"<block>
<do_while_loop> ::= do<block>while"("<expression>")";
<for_loop> ::= for"("<expression>;<expression>;<expression>")"<block>
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
