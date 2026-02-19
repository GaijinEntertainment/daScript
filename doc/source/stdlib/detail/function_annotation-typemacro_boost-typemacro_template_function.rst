This one converts function into a type macro that uses template arguments.
For example [typemacro_template(TFlatHashTable)] def makeFlatHashTable ( macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr; hashFunctionName : string) : TypeDeclPtr { ... }
We generate the body that handles template argument inference and instantiation.
