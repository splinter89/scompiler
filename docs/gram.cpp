<программа> ::= [<декларативный_раздел>]int main(<список_аргументов>)"{"{<элемент_блока>}"}"[<декларативный_раздел>]
<декларативный_раздел> ::= {(<класс>|<реализация_метода_класса>|<декларация_переменных>|<функция>)}
<класс> ::= class <идентификатор> "{"{[<спецификатор_доступа>:]{(<декларация_свойств_класса>|<метод_класса>)}}"}"
<спецификатор_доступа> ::= (public|private)
<метод_класса> ::= <тип_функции> [~]<идентификатор>"("<список_аргументов>")"<блок>
<реализация_метода_класса> ::= <тип_функции> <идентификатор>::[~]<идентификатор>"("<список_аргументов>")"<блок>
<функция> ::= <тип_функции> <идентификатор>"("<список_аргументов>")"<блок>
<тип_функции> ::= (<тип_переменной>|void)
<список_аргументов> ::= [<аргумент>{, <аргумент>}]
<аргумент> ::= <тип_переменной>[&] <идентификатор>
<блок> ::= (<элемент_блока>|"{"{<элемент_блока>}"}")
<элемент_блока> ::= (;|<декларация_переменных>|<выражение>;|<цикл>|<ветвление>|<возврат>)
<выражение> ::= (<идентификатор>|<литерал>|<оператор>)
<возврат> ::= return <выражение>;
<оператор> ::= (<оператор_1>|<оператор>{,<оператор_1>})
<оператор_1> ::= (<оператор_2>|<объект>(=|+=|-=|*=|/=|%=)<оператор_2>)
<оператор_2> ::= (<оператор_3>|<оператор_2>"||"<оператор_3>)
<оператор_3> ::= (<оператор_4>|<оператор_3>&&<оператор_4>)
<оператор_4> ::= (<оператор_5>|<оператор_4>(==|!=)<оператор_5>)
<оператор_5> ::= (<оператор_6>|<оператор_5>(<|<=|>|>=)<оператор_6>)
<оператор_6> ::= (<оператор_7>|<оператор_6>(+|-)<оператор_7>)
<оператор_7> ::= (<оператор_8>|<оператор_7>(*|/|%)<оператор_8>)
<оператор_8> ::= (<оператор_9>|(++|--)<объект>|(+|-|!|*)<оператор_9>|&<объект>)
<оператор_9> ::= ("("<выражение>")"|<объект>(++|--)|<вызов_функции>|<идентификатор>(.|->)<идентификатор>)
<вызов_функции> ::= [<идентификатор>(.|->)]<идентификатор>"("<выражение>")"
<объект> ::= <идентификатор>[(.|->)<идентификатор>]
<декларация_свойств_класса> ::= [const ]<тип_переменной> <список_свойств_класса>;
<декларация_переменных> ::= [const ]<тип_переменной> <список_переменных>;
<тип_переменной> ::= (int|double|char|bool)[*]
<список_свойств_класса> ::= <идентификатор>{, <идентификатор>}
<список_переменных> ::= <идентификатор>[=<выражение>]{, <идентификатор>[=<выражение>]}
<идентификатор> ::= <буква>{(<буква>|<цифра>|_)}
<цикл> ::= (<while-цикл>|<do-while-цикл>|<for-цикл>)
<while-цикл> ::= while"("<выражение>")" <блок>
<do-while-цикл> ::= do <блок> while "("<выражение>")";
<for-цикл> ::= for"("<выражение>; <выражение>; <выражение>")" <блок>
<ветвление> ::= if"("<выражение>")" <блок>[ else <блок>]
<литерал> ::= (<логический_литерал>|<целая_константа>|<константа_с_плавающей_точкой>|<символьная_константа>|<строковый_литерал>)
<логический_литерал> ::= (true|false)
<константа_с_плавающей_точкой> ::= [<десятичная_целая_константа>].[цифра{<цифра>}][(e|E)<десятичная_целая_константа>]
<целая_константа> ::= (<десятичная_целая_константа>|<восьмеричная_целая_константа>|<шестнадцатеричная_целая_константа>)
<десятичная_целая_константа> ::= [(+|-)]цифра{<цифра>}
<восьмеричная_целая_константа> ::= 0{(0|…|7)}
<шестнадцатеричная_целая_константа> ::= 0x(0|…|9|a|…|f|A|…F){(0|…|9|a|…|f|A|…F)}
<цифра> ::= (0|…|9)
<символьная_константа> ::= '?'
<буква> ::= (a|…|z|A|…|Z|)
<строковый_литерал> ::= """{?}"""

// lvalue == объект
// rvalue == выражение