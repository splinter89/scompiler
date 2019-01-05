<program> ::= <program_element>
<program> ::= <program><program_element>
<program_element> ::= <class>
<program_element> ::= <class_method_impl>
<program_element> ::= <vars_declaration>
<program_element> ::= <function>
<class> ::= class <id>"{}";
<class> ::= class <id>"{"<class_body>"}";
<class_body> ::= <class_element>
<class_body> ::= <access_spec>:<class_element>
<class_body> ::= <class_body><class_element>
<class_body> ::= <class_body><access_spec>:<class_element>
<class_element> ::= <class_props_declaration>
<class_element> ::= <class_method>
<class_element> ::= <class_element><class_props_declaration>
<class_element> ::= <class_element><class_method>
<class_element> ::= ::
<class_element> ::= :::
<class_element> ::= ::::
<access_spec> ::= public
<access_spec> ::= private
<class_method> ::= <id>"()"<block>
<class_method> ::= <id>"("<arguments_list>")"<block>
<class_method> ::= <var_type> <id>"()"<block>
<class_method> ::= <var_type> <id>"("<arguments_list>")"<block>
<class_method> ::= void <id>"()"<block>
<class_method> ::= void <id>"("<arguments_list>")"<block>
<class_method> ::= ~<id>"()"<block>
<class_method> ::= ~<id>"("<arguments_list>")"<block>
<class_method_impl> ::= <id>::<id>"()"<block>
<class_method_impl> ::= <id>::<id>"("<arguments_list>")"<block>
<class_method_impl> ::= <var_type> <id>::<id>"()"<block>
<class_method_impl> ::= <var_type> <id>::<id>"("<arguments_list>")"<block>
<class_method_impl> ::= void <id>::<id>"()"<block>
<class_method_impl> ::= void <id>::<id>"("<arguments_list>")"<block>
<class_method_impl> ::= <id>::~<id>"()"<block>
<class_method_impl> ::= <id>::~<id>"("<arguments_list>")"<block>
<function> ::= <var_type> <id>"()"<block>
<function> ::= <var_type> <id>"("<arguments_list>")"<block>
<function> ::= void <id>"()"<block>
<function> ::= void <id>"("<arguments_list>")"<block>
<arguments_list> ::= <argument>
<arguments_list> ::= <arguments_list>,<argument>
<argument> ::= <var_type> <id>
<argument> ::= const <var_type> <id>
<argument> ::= <var_type>&<id>
<argument> ::= const <var_type>&<id>
<block> ::= "{}"
<block> ::= "{"<block_body>"}"
<block_body> ::= <block_element>
<block_body> ::= <block_body><block_element>
<block_element> ::= <vars_declaration>
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
<class_props_declaration> ::= <var_type> <class_props_list>;
<class_props_declaration> ::= const <var_type> <class_props_list>;
<vars_declaration> ::= <var_type> <vars_list>;
<vars_declaration> ::= <var_type>*<vars_list>;
<vars_declaration> ::= const <var_type> <vars_list>;
<vars_declaration> ::= const <var_type>*<vars_list>;
<var_type> ::= int
<var_type> ::= double
<var_type> ::= char
<var_type> ::= bool
<var_type> ::= <id>
<class_props_list> ::= <id>
<class_props_list> ::= <class_props_list>,<id>
<vars_list> ::= <id>
<vars_list> ::= <vars_list>,<id>
<vars_list> ::= <vars_list>,*<id>
<vars_list> ::= <id>=<operator_1>
<vars_list> ::= <vars_list>,<id>=<operator_1>
<vars_list> ::= <vars_list>,*<id>=<operator_1>
<loop> ::= <while-loop>
<loop> ::= <do-while-loop>
<loop> ::= <for-loop>
<while-loop> ::= while"("<expression>")"<block>
<do-while-loop> ::= do<block>while"("<expression>")";
<for-loop> ::= for"("<expression>;<expression>;<expression>")"<block>
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
