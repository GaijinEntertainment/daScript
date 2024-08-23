Specifies that the function is a type function, and can use type-function syntax::

    [type_function]
    def take ( a : auto(TT); a1,a2 : int )
        return typeinfo(sizeof type<TT>) + a1 + a2

    [export]
    def main
        let size = take<int>(1,2)   // note syntax which expands into take(type<int>,1,2)
