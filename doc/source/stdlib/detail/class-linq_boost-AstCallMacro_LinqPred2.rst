Base call macro for LINQ-style two-argument predicate operators.
This macro converts _where(iterator, expresion) into::
    where_(iterator, $(_) => expression)

