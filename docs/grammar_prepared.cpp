<program> ::= <program_element>
<program> ::= <program><program_element>
<program_element> ::= <class>
<program_element> ::= <vars_or_function>
<class> ::= class <id>"{""}";
<class> ::= class <id>"{"<class_body>"}";
<class_body> ::= <class_element>
<class_body> ::= <class_body><class_element>
<class_element> ::= <vars_or_function>
<class_element> ::= <access_spec>:<vars_or_function>
<access_spec> ::= public
<access_spec> ::= private
<vars_or_function> ::= <vars>
<vars_or_function> ::= <var_type> <id><function>
<vars_or_function> ::= <var_type> <id>::<id><function>
<vars_or_function> ::= void <id><function>
<vars_or_function> ::= void <id>::<id><function>
<vars_or_function> ::= <id><function>
<vars_or_function> ::= <id>::<id><function>
<vars_or_function> ::= ~<id><function>
<vars_or_function> ::= <id>::~<id><function>
<vars> ::= <var_type> <id>;
<vars> ::= <var_type> <id><more_vars>;
<vars> ::= <var_type> <id>=<operator_2>;
<vars> ::= <var_type> <id>=<operator_2><more_vars>;
<vars> ::= <var_type>*<id>;
<vars> ::= <var_type>*<id><more_vars>;
<vars> ::= <var_type>*<id>=<operator_2>;
<vars> ::= <var_type>*<id>=<operator_2><more_vars>;
<vars> ::= const <var_type> <id>;
<vars> ::= const <var_type> <id><more_vars>;
<vars> ::= const <var_type> <id>=<operator_2>;
<vars> ::= const <var_type> <id>=<operator_2><more_vars>;
<vars> ::= const <var_type>*<id>;
<vars> ::= const <var_type>*<id><more_vars>;
<vars> ::= const <var_type>*<id>=<operator_2>;
<vars> ::= const <var_type>*<id>=<operator_2><more_vars>;
<var_type> ::= int
<var_type> ::= double
<var_type> ::= char
<var_type> ::= bool
<var_type> ::= <id>
<more_vars> ::= ,<id>
<more_vars> ::= ,<id>=<operator_2>
<more_vars> ::= ,*<id>
<more_vars> ::= ,*<id>=<operator_2>
<more_vars> ::= <more_vars>,<id>
<more_vars> ::= <more_vars>,<id>=<operator_2>
<more_vars> ::= <more_vars>,*<id>
<more_vars> ::= <more_vars>,*<id>=<operator_2>
<function> ::= "("")";
<function> ::= "("")"<block>
<function> ::= "("<arguments>")";
<function> ::= "("<arguments>")"<block>
<arguments> ::= <argument>
<arguments> ::= <arguments>,<argument>
<argument> ::= <var_type>
<argument> ::= <var_type>=<literal>
<argument> ::= <var_type> <id>
<argument> ::= <var_type> <id>=<literal>
<argument> ::= <var_type>&
<argument> ::= <var_type>&=<literal>
<argument> ::= <var_type>&<id>
<argument> ::= <var_type>&<id>=<literal>
<argument> ::= const <var_type>
<argument> ::= const <var_type>=<literal>
<argument> ::= const <var_type> <id>
<argument> ::= const <var_type> <id>=<literal>
<argument> ::= const <var_type>&
<argument> ::= const <var_type>&=<literal>
<argument> ::= const <var_type>&<id>
<argument> ::= const <var_type>&<id>=<literal>
<block> ::= "{""}"
<block> ::= "{"<block_body>"}"
<block_body> ::= <block_element>
<block_body> ::= <block_body><block_element>
<block_element> ::= <vars>
<block_element> ::= ;
<block_element> ::= <expression>;
<block_element> ::= <return>;
<block_element> ::= <loop>
<block_element> ::= <branching>
<return> ::= return
<return> ::= return <expression>
<expression> ::= <operator_1>
<expression> ::= <expression>,<operator_1>
<operator_1> ::= <operator_2>
<operator_1> ::= <object>=<operator_1>
<operator_1> ::= <object>+=<operator_1>
<operator_1> ::= <object>-=<operator_1>
<operator_1> ::= <object>*=<operator_1>
<operator_1> ::= <object>/=<operator_1>
<operator_1> ::= <object>%=<operator_1>
<operator_2> ::= <operator_3>
<operator_2> ::= <operator_2>"||"<operator_3>
<operator_3> ::= <operator_4>
<operator_3> ::= <operator_3>&&<operator_4>
<operator_4> ::= <operator_5>
<operator_4> ::= <operator_4>==<operator_5>
<operator_4> ::= <operator_4>!=<operator_5>
<operator_5> ::= <operator_6>
<operator_5> ::= <operator_5>"<"<operator_6>
<operator_5> ::= <operator_5>"<"=<operator_6>
<operator_5> ::= <operator_5>">"<operator_6>
<operator_5> ::= <operator_5>">"=<operator_6>
<operator_6> ::= <operator_7>
<operator_6> ::= <operator_6>+<operator_7>
<operator_6> ::= <operator_6>-<operator_7>
<operator_7> ::= <operator_8>
<operator_7> ::= <operator_7>*<operator_8>
<operator_7> ::= <operator_7>/<operator_8>
<operator_7> ::= <operator_7>%<operator_8>
<operator_8> ::= <operator_9>
<operator_8> ::= +<operator_9>
<operator_8> ::= -<operator_9>
<operator_8> ::= !<operator_9>
<operator_8> ::= *<operator_9>
<operator_8> ::= ++<object>
<operator_8> ::= --<object>
<operator_8> ::= &<object>
<operator_9> ::= "("<expression>")"
<operator_9> ::= <literal>
<operator_9> ::= <object>
<operator_9> ::= <object>++
<operator_9> ::= <object>--
<operator_9> ::= <function_call>
<function_call> ::= <object>"()"
<function_call> ::= <object>"("<expression>")"
<object> ::= <id>
<object> ::= <id>.<id>
<object> ::= <id>-">"<id>
<loop> ::= <while_loop>
<loop> ::= <do_while_loop>
<loop> ::= <for_loop>
<while_loop> ::= while"("<expression>")"<block>
<do_while_loop> ::= do<block>while"("<expression>")";
<for_loop> ::= for"("<expression>;<expression>;<expression>")"<block>
<branching> ::= if"("<expression>")"<block>
<branching> ::= if"("<expression>")"<block>else<block>

// lexical stuff
<id> ::= ... // T_ID
<literal> ::= ... // T_CONST

<boolean_literal> ::= (true|false)
<floating_point_literal> ::= [(+|-)](<digit>{<digit>}.[<digit>{<digit>]|.<digit>{<digit>})[(e|E)<decimal_literal>]
<integer_literal> ::= (<decimal_literal>|<octal_literal>|<hex_literal>)
<decimal_literal> ::= [(+|-)]<digit>{<digit>}
<octal_literal> ::= [(+|-)]0{(0|...|7)}
<hex_literal> ::= [(+|-)]0x(0|...|9|a|...|f|A|...F){(0|...|9|a|...|f|A|...F)}
<digit> ::= (0|...|9)
<letter> ::= (a|...|z|A|...|Z)
<char_literal> ::= '(?|\('|"""|n|t))'
<string_literal> ::= """{(?|\('|"""|n|t))}"""

// lvalue == object
// rvalue == expression
