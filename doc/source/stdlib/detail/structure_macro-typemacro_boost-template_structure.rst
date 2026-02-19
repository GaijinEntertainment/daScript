This macro creates typemacro function and associates it with the structure.
It also creates the typemacro_template_function to associate with it.
For example::

    [template_structure(KeyType,ValueType)] struct template TFlatHashTable { ... }
    creates:
    1) [typemacro_function] def TFlatHashTable (macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
        return <- make`template`TFlatHashTable(macroArgument, passArgument, KeyType, ValueType)
     }
    2) [typemacro_template_function(TFlatHashTable)] def make`template`TFlatHashTable (macroArgument, passArgument : TypeDeclPtr; KeyType, ValueType : TypeDeclPtr) : TypeDeclPtr {
        return <- default<TypeDeclPtr>
     }
