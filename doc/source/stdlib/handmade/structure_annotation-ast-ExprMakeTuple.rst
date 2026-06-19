Make tuple expression (`[[auto f1,f2,f3]]`)
Location of the expression in source code
Type of the expression
Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)
Expression generation flags
Expression flags
Expression print flags
Type being made
Stack top offset for the data, if applicable
Extra offset for the data, if applicable. If part of the larger initialization, extra offset is that
Flags specific to make-local expressions
Type of the array elements
Array of expressions for the elements
If gen2 syntax is used (i.e. `[...]` instead of `[[...]]`)
Whether the array is built directly on the heap (set by the inferer for gen2 literals feeding to_array_move/to_table_move)
If key-value syntax is used (i.e. `[key=>val; key2=>val2]`)
Field names, when key-value record syntax is used
