<программа> ::= <элемент_программы>
<программа> ::= <программа><элемент_программы>
<элемент_программы> ::= <класс>
<элемент_программы> ::= <реализация_метода_класса>
<элемент_программы> ::= <декларация_переменных>
<элемент_программы> ::= <функция>
<класс> ::= class <идентификатор>"{}";
<класс> ::= class <идентификатор>"{"<тело_класса>"}";
<тело_класса> ::= <элемент_класса>
<тело_класса> ::= <спецификатор_доступа>:<элемент_класса>
<тело_класса> ::= <тело_класса><элемент_класса>
<тело_класса> ::= <тело_класса><спецификатор_доступа>:<элемент_класса>
<элемент_класса> ::= <декларация_свойств_класса>
<элемент_класса> ::= <метод_класса>
<элемент_класса> ::= <элемент_класса><декларация_свойств_класса>
<элемент_класса> ::= <элемент_класса><метод_класса>
<элемент_класса> ::= ::
<элемент_класса> ::= :::
<элемент_класса> ::= ::::
<спецификатор_доступа> ::= public
<спецификатор_доступа> ::= private
<метод_класса> ::= <идентификатор>"()"<блок>
<метод_класса> ::= <идентификатор>"("<список_аргументов>")"<блок>
<метод_класса> ::= <тип_переменной> <идентификатор>"()"<блок>
<метод_класса> ::= <тип_переменной> <идентификатор>"("<список_аргументов>")"<блок>
<метод_класса> ::= void <идентификатор>"()"<блок>
<метод_класса> ::= void <идентификатор>"("<список_аргументов>")"<блок>
<метод_класса> ::= ~<идентификатор>"()"<блок>
<метод_класса> ::= ~<идентификатор>"("<список_аргументов>")"<блок>
<реализация_метода_класса> ::= <идентификатор>::<идентификатор>"()"<блок>
<реализация_метода_класса> ::= <идентификатор>::<идентификатор>"("<список_аргументов>")"<блок>
<реализация_метода_класса> ::= <тип_переменной> <идентификатор>::<идентификатор>"()"<блок>
<реализация_метода_класса> ::= <тип_переменной> <идентификатор>::<идентификатор>"("<список_аргументов>")"<блок>
<реализация_метода_класса> ::= void <идентификатор>::<идентификатор>"()"<блок>
<реализация_метода_класса> ::= void <идентификатор>::<идентификатор>"("<список_аргументов>")"<блок>
<реализация_метода_класса> ::= <идентификатор>::~<идентификатор>"()"<блок>
<реализация_метода_класса> ::= <идентификатор>::~<идентификатор>"("<список_аргументов>")"<блок>
<функция> ::= <тип_переменной> <идентификатор>"()"<блок>
<функция> ::= <тип_переменной> <идентификатор>"("<список_аргументов>")"<блок>
<функция> ::= void <идентификатор>"()"<блок>
<функция> ::= void <идентификатор>"("<список_аргументов>")"<блок>
<список_аргументов> ::= <аргумент>
<список_аргументов> ::= <список_аргументов>,<аргумент>
<аргумент> ::= <тип_переменной> <идентификатор>
<аргумент> ::= const <тип_переменной> <идентификатор>
<аргумент> ::= <тип_переменной>&<идентификатор>
<аргумент> ::= const <тип_переменной>&<идентификатор>
<блок> ::= "{}"
<блок> ::= "{"<тело_блока>"}"
<тело_блока> ::= <элемент_блока>
<тело_блока> ::= <тело_блока><элемент_блока>
<элемент_блока> ::= <декларация_переменных>
<элемент_блока> ::= ;
<элемент_блока> ::= <выражение>;
<элемент_блока> ::= <возврат>;
<элемент_блока> ::= <цикл>
<элемент_блока> ::= <ветвление>
<возврат> ::= return
<возврат> ::= return <выражение>
<выражение> ::= <оператор_1>
<выражение> ::= <выражение>,<оператор_1>
<оператор_1> ::= <оператор_2>
<оператор_1> ::= <объект>=<оператор_1>
<оператор_1> ::= <объект>+=<оператор_1>
<оператор_1> ::= <объект>-=<оператор_1>
<оператор_1> ::= <объект>*=<оператор_1>
<оператор_1> ::= <объект>/=<оператор_1>
<оператор_1> ::= <объект>%=<оператор_1>
<оператор_2> ::= <оператор_3>
<оператор_2> ::= <оператор_2>"||"<оператор_3>
<оператор_3> ::= <оператор_4>
<оператор_3> ::= <оператор_3>&&<оператор_4>
<оператор_4> ::= <оператор_5>
<оператор_4> ::= <оператор_4>==<оператор_5>
<оператор_4> ::= <оператор_4>!=<оператор_5>
<оператор_5> ::= <оператор_6>
<оператор_5> ::= <оператор_5>"<"<оператор_6>
<оператор_5> ::= <оператор_5>"<"=<оператор_6>
<оператор_5> ::= <оператор_5>">"<оператор_6>
<оператор_5> ::= <оператор_5>">"=<оператор_6>
<оператор_6> ::= <оператор_7>
<оператор_6> ::= <оператор_6>+<оператор_7>
<оператор_6> ::= <оператор_6>-<оператор_7>
<оператор_7> ::= <оператор_8>
<оператор_7> ::= <оператор_7>*<оператор_8>
<оператор_7> ::= <оператор_7>/<оператор_8>
<оператор_7> ::= <оператор_7>%<оператор_8>
<оператор_8> ::= <оператор_9>
<оператор_8> ::= +<оператор_9>
<оператор_8> ::= -<оператор_9>
<оператор_8> ::= !<оператор_9>
<оператор_8> ::= *<оператор_9>
<оператор_8> ::= ++<объект>
<оператор_8> ::= --<объект>
<оператор_8> ::= &<объект>
<оператор_9> ::= "("<выражение>")"
<оператор_9> ::= <литерал>
<оператор_9> ::= <объект>
<оператор_9> ::= <объект>++
<оператор_9> ::= <объект>--
<оператор_9> ::= <вызов_функции>
<вызов_функции> ::= <объект>"()"
<вызов_функции> ::= <объект>"("<выражение>")"
<объект> ::= <идентификатор>
<объект> ::= <идентификатор>.<идентификатор>
<объект> ::= <идентификатор>-">"<идентификатор>
<декларация_свойств_класса> ::= <тип_переменной> <список_свойств_класса>;
<декларация_свойств_класса> ::= const <тип_переменной> <список_свойств_класса>;
<декларация_переменных> ::= <тип_переменной> <список_переменных>;
<декларация_переменных> ::= <тип_переменной>*<список_переменных>;
<декларация_переменных> ::= const <тип_переменной> <список_переменных>;
<декларация_переменных> ::= const <тип_переменной>*<список_переменных>;
<тип_переменной> ::= int
<тип_переменной> ::= double
<тип_переменной> ::= char
<тип_переменной> ::= bool
<тип_переменной> ::= <идентификатор>
<список_свойств_класса> ::= <идентификатор>
<список_свойств_класса> ::= <список_свойств_класса>,<идентификатор>
<список_переменных> ::= <идентификатор>
<список_переменных> ::= <список_переменных>,<идентификатор>
<список_переменных> ::= <список_переменных>,*<идентификатор>
<список_переменных> ::= <идентификатор>=<оператор_1>
<список_переменных> ::= <список_переменных>,<идентификатор>=<оператор_1>
<список_переменных> ::= <список_переменных>,*<идентификатор>=<оператор_1>
<цикл> ::= <while-цикл>
<цикл> ::= <do-while-цикл>
<цикл> ::= <for-цикл>
<while-цикл> ::= while"("<выражение>")"<блок>
<do-while-цикл> ::= do<блок>while"("<выражение>")";
<for-цикл> ::= for"("<выражение>;<выражение>;<выражение>")"<блок>
<ветвление> ::= if"("<выражение>")"<блок>
<ветвление> ::= if"("<выражение>")"<блок>else<блок>

<T_ID> ::= ...
<T_CONST> ::= ...

<логический_литерал> ::= (true|false)
<константа_с_плавающей_точкой> ::= [(+|-)](<цифра>{<цифра>}.[<цифра>{<цифра>]|.<цифра>{<цифра>})[(e|E)<десятичная_целая_константа>]
<целая_константа> ::= (<десятичная_целая_константа>|<восьмеричная_целая_константа>|<шестнадцатеричная_целая_константа>)
<десятичная_целая_константа> ::= [(+|-)]<цифра>{<цифра>}
<восьмеричная_целая_константа> ::= [(+|-)]0{(0|…|7)}
<шестнадцатеричная_целая_константа> ::= [(+|-)]0x(0|…|9|a|…|f|A|…F){(0|…|9|a|…|f|A|…F)}
<цифра> ::= (0|…|9)
<буква> ::= (a|…|z|A|…|Z)
<символьная_константа> ::= '(?|\('|"""|n|t))'
<строковый_литерал> ::= """{(?|\('|"""|n|t))}"""
// lvalue == объект
// rvalue == выражение