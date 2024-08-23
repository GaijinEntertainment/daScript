Marks function as nodiscard. Result of the function should be used.
Here is an example from buildin.das::

    [nodiscard] // can't call each(foo) without using the result, need to assign or pass to other function
    def each ( lam : lambda<(var arg:auto(argT)):bool> ) : iterator<argT -&>
        concept_assert(typeinfo(is_ref type<argT>),"lamda-to-iterator argument has to be reference; expecting value& or reference type")
        ...


