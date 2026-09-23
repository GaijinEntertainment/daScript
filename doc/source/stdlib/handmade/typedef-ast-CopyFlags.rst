properties of the `ExprCopy` object.
This copy is allowed to copy a temporary value.
Its 'foo = [MakeLocal]' and temp stack value is allocated by copy expression.
Copy is allowed into a constant lvalue, for example a let the compiler relocates into an assignment
The raw form `a !== b`: the builtin copy, skipping any `operator =` overload
