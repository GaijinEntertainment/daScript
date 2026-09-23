Properties of the `ExprMove` object.
Its 'foo <- [MakeLocal]' and temp stack value is allocated by move expression.
Move is allowed for constant lvalue, for example x <- 5
Move is a POD delete.
The raw form `a !<- b`: the builtin move, skipping any `operator <-` overload
