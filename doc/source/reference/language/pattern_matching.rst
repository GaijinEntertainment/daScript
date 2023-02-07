.. _match:

================
Pattern matching
================

Pattern matching is a powerful feature in programming that allows for more concise and readable code by allowing a programmer
to easily test a value against a set of patterns, and bind values in the process. It is a versatile mechanism that can be used
for many different tasks, such as data validation, data extraction, and flow control. With pattern matching, you can write more
expressive code by breaking down complex structures into simple, recognizable patterns. Whether you're working with simple data types
like integers and strings, or more complex data structures like arrays and variants, pattern matching is a valuable tool for writing clean and efficient code.

^^^^^^^^^^^^^^^^^^^^
Enumeration Matching
^^^^^^^^^^^^^^^^^^^^

daScript supports pattern matching on enumerations, which allows you to match the value of an enumeration with specific patterns.
You can use this feature to simplify your code by eliminating the need for multiple if-else statements or switch statements.
To match enumerations in daScript, you use the match keyword followed by the enumeration value, and a series of if statements,
each representing a pattern to match. If a match is found, the corresponding code block is executed.

Example::

    enum Color
        Black
        Red
        Green
        Blue

    def enum_match (color:Color)
        match color
            if Color Black
                return 0
            if Color Red
                return 1
            if _
                return -1

In the example, the enum_match function takes a Color enumeration value as an argument and returns a value based on the matched pattern.
The if Color Black statement matches the Black enumeration value, the if Color Red statement matches the Red enumeration value,
and the if _ statement is a catch-all that matches any other enumeration value that hasn't been explicitly matched.

^^^^^^^^^^^^^^^^^^^^
Matching Variants
^^^^^^^^^^^^^^^^^^^^

Variants in daScript can be matched using the match statement.
A variant is a discriminated union type that holds one of several possible values, each of a different type.

In the example, the IF variant has two possible values: i of type int, and f of type float.
The variant_as_match function takes a value of type IF as an argument, and matches it to determine its type.

The if _ as i statement matches any value and assigns it to the declared variable i.
Similarly, the if _ as f statement matches any value and assigns it to the declared variable f.
The final if _ statement matches any remaining values, and returns "anything".

Example::

    variant IF
        i : int
        f : float

    def variant_as_match (v:IF)
        match v
            if _ as i
                return "int"
            if _ as f
                return "float"
            if _
                return "anything"

Variants can be matched in daScript using the same syntax used to create new variants.

Here's an example::

    def variant_match (v : IF)
        match v
            if [[IF i=$v(i)]]
                return 1
            if [[IF f=$v(f)]]
                return 2
            if _
                return 0

In the example above, the function variant_match takes a variant v of type IF. The first case matches v if it contains an i and binds the value of i to a variable i.
In this case, the function returns 1. The second case matches v if it contains an f and binds the value of f to a variable f. In this case, the function returns 2. T
he last case matches anything that doesn't match the first two cases and returns 0.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Declaring Variables in Pattern Matching
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In daScript, you can declare variables in pattern matching statements, including variant matching.
To declare a variable, use the syntax $v(decl) where decl is the name of the variable being declared.
The declared variable is then assigned the value of the matched pattern.

This feature is not restricted to variant matching, and can be used in any pattern matching statement in daScript.
In the example, the if $v(as_int) statement matches the variant value when it holds an integer and declares a variable as_int
to store the value. Similarly, the if $v(as_float) statement matches the variant value when it holds a floating-point value and declares a variable as_float to store the value.

Example::

    variant IF
        i : int
        f : float

    def variant_as_match (v:IF)
        match v
            if $v(as_int)
                return as_int
            if $v(as_float)
                return as_float
            if _
                return None

^^^^^^^^^^^^^^^^^^^^
Matching Structs
^^^^^^^^^^^^^^^^^^^^

daScript supports matching structs using the match statement.
A struct is a composite data type that groups variables of different data types under a single name.

In the example, the Foo struct has one member a of type int.
The struct_match function takes an argument of type Foo, and matches it against various patterns.

The first match if [[Foo a=13]] matches a Foo struct where a is equal to 13, and returns 0 if this match succeeds.
The second match if [[Foo a=$v(anyA)]] matches any Foo struct and binds its a member to the declared variable anyA.
This match returns the value of anyA if it succeeds.

Example::

    struct Foo
        a : int

    def struct_match (f:Foo)
        match f
            if [[Foo a=13]]
                return 0
            if [[Foo a=$v(anyA)]]
                return anyA

^^^^^^^^^^^^^^^^^^^^
Using Guards
^^^^^^^^^^^^^^^^^^^^

daScript supports the use of guards in its pattern matching mechanism.
Guards are conditions that must be satisfied in addition to a successful pattern match.

In the example, the AB struct has two members a and b of type int.
The guards_match function takes an argument of type AB, and matches it against various patterns.

The first match if [[AB a=$v(a), b=$v(b)]] && (b > a) matches an AB struct and binds its a and b members to the declared variables a and b, respectively.
The guard condition b > a must also be satisfied for this match to be successful. If this match succeeds, the function returns a string indicating that b is greater than a.

The second match if [[AB a=$v(a), b=$v(b)]] matches any AB struct and binds its a and b members to the declared variables a and b, respectively.
No additional restrictions are placed on the match by means of a guard. If this match succeeds, the function returns a string indicating that b is less than or equal to a.

Example::

    struct AB
        a, b : int

    def guards_match (ab:AB)
        match ab
            if [[AB a=$v(a), b=$v(b)]] && (b > a)
                return "{b} > {a}"
            if [[AB a=$v(a), b=$v(b)]]
                return "{b} <= {a}"

^^^^^^^^^^^^^^^^^^^^
Tuple Matching
^^^^^^^^^^^^^^^^^^^^

Matching tuples in daScript is done with double square brackets and uses the same syntax as creating a new tuple.
The type of the tuple must be specified or auto can be used to indicate automatic type inference.

Here is an example that demonstrates tuple matching in daScript::

    def tuple_match ( A : tuple<int;float;string> )
        match A
            if [[auto 1,_,"3"]]
                return 1
            if [[auto 13,...]]      // starts with 13
                return 2
            if [[auto ...,"13"]]    // ends with "13"
                return 3
            if [[auto 2,...,"2"]]   // starts with 2, ends with "2"
                return 4
            if _
                return 0

In this example, a tuple A of type tuple<int;float;string> is passed as an argument to the function tuple_match.
The function uses a match statement to match different patterns in the tuple A.
The if clauses inside the match statement use double square brackets to specify the pattern to be matched.

The first pattern to be matched is [[auto 1,_,"3"]].
The pattern matches a tuple that starts with the value 1, followed by any value, and ends with the string "3".
The _ symbol in the pattern indicates that any value can be matched at that position in the tuple.

The second pattern to be matched is [[auto 13,...]], which matches a tuple that starts with the value 13.
The ... symbol in the pattern indicates that any number of values can be matched after the value 13.

The third pattern to be matched is [[auto ...,"13"]], which matches a tuple that ends with the string "13".
The ... symbol in the pattern indicates that any number of values can be matched before the string "13".

The fourth pattern to be matched is [[auto 2,...,"2"]], which matches a tuple that starts with the value 2 and ends with the string "2".

If none of the patterns match, the _ clause is executed and the function returns 0.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Matching Static Arrays
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Static arrays in daScript can be matched using the double square bracket syntax, similarly to tuples.
Additionally, static arrays must have their type specified, or the type can be automatically inferred using the auto keyword.

Here is an example of matching a static array of type int[3]::

    def static_array_match ( A : int[3] )
        match A
            if [[auto $v(a);$v(b);$v(c)]] && (a+b+c)==6 // total of 3 elements, sum is 6
                return 1
            if [[int 0;...]]    // starts with 0
                return 0
            if [[int ...;13]]   // ends with 13
                return 2
            if [[int 12;...;12]]    // starts and ends with 12
                return 3
            if _
                return -1

In this example, the function static_array_match takes an argument of type int[3], which is a static array of three integers.
The match statement uses the double square bracket syntax to match against different patterns of the input array A.

The first case, [[auto $v(a);$v(b);$v(c)]] && (a+b+c)==6, matches an array where the sum of its three elements is equal to 6.
The matched elements are assigned to variables a, b, and c using the $v syntax.

The next three cases match arrays that start with 0, end with 13, and start and end with 12, respectively.
The ... syntax is used to match any elements in between.

Finally, the _ case matches any array that does not match any of the other cases, and returns -1 in this case.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Dynamic Array Matching
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Dynamic arrays are used to store a collection of values that can be changed during runtime.
In daScript, dynamic arrays can be matched with patterns using similar syntax as for tuples, but with the added check for the number of elements in the array.

Here is an example of matching on a dynamic array of integers::

    def dynamic_array_match ( A : array<int> )
        match A
            if [{auto $v(a);$v(b);$v(c)}] && (a+b+c)==6 // total of 3 elements, sum is 6
                return 1
            if [{int 0;0;0;...}]    // first 3 are 0
                return 0
            if [{int ...;1;2}]      // ends with 1,2
                return 2
            if [{int 0;1;...;2;3}]    // starts with 0,1, ends with 2,3
                return 3
            if _
                return -1

In the code above, the dynamic_array_match function takes a dynamic array of integers as an argument.
The match statement then tries to match the elements in the array against a series of patterns.

The first pattern if [{auto $v(a);$v(b);$v(c)}] && (a+b+c)==6 matches arrays that contain three elements and the sum of those elements is 6.
The $v syntax is used to match and capture the values of the elements in the array. The captured values can then be used in the condition (a+b+c)==6.

The second pattern if [{int 0;0;0;...}] matches arrays that start with three zeros. The ... syntax is used to match any remaining elements in the array.

The third pattern if [{int ...;1;2}] matches arrays that end with the elements 1 and 2.

The fourth pattern if [{int 0;1;...;2;3}] matches arrays that start with the elements 0 and 1 and end with the elements 2 and 3.

The final pattern if _ matches any array that didn't match any of the previous patterns.

It is important to note that the number of elements in the dynamic array must match the number of elements in the pattern for the match to be successful.

^^^^^^^^^^^^^^^^^^^^
Match Expressions
^^^^^^^^^^^^^^^^^^^^

In daScript, match expressions allow you to reuse variables declared earlier in the pattern to match expressions later in the pattern.

Here's an example that demonstrates how to use match expressions to check if an array of integers is in ascending order::

    def ascending_array_match ( A : int[3] )
        match A
            if [[int $v(x);match_expr(x+1);match_expr(x+2)]]
                return true
            if _
                return false

In this example, the first element of the array is matched to x. Then, the next two elements are matched using match_expr and the expression x+1 and x+2 respectively.
If all three elements match, the function returns true. If there is no match, the function returns false.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Matching with || Expression
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In daScript, you can use the || expression to match either of the provided options in the order they appear. This is useful when you want to match a variant based on multiple criteria.

Here is an example of matching with || expression::

    struct Bar
        a : int
        b : float

    def or_match ( B:Bar )
        match B
            if [[Bar a=1, b=$v(b)]] || [[Bar a=2, b=$v(b)]]
                return b
            if _
                return 0.0

In this example, the function or_match takes a variant B of type Bar and matches it using the || expression.
The first option matches when the value of a is 1 and b is captured as a variable.
The second option matches when the value of a is 2 and b is captured as a variable.
If either of these options match, the value of b is returned. If neither of the options match, 0.0 is returned.

It's important to note that for the || expression to work, both sides of the statement must declare the same variables.

^^^^^^^^^^^^^^^^^^^^
Static Matching
^^^^^^^^^^^^^^^^^^^^

Static matching is a way to match on generic expressions daScript. It works similarly to regular matching, but with one key difference:
when there is a type mismatch between the match expression and the pattern, the match will be ignored at compile-time, as opposed to a compilation error.
This makes static matching robust for generic functions.

The syntax for static matching is as follows::

    static_match match_expression
        if pattern_1
            return result_1
        if pattern_2
            return result_2
        ...
        if _
            return result_default

Here, match_expression is the expression to be matched against the patterns. Each pattern is a value or expression that the match_expression will be compared against.
If the match_expression matches one of the patterns, the corresponding result will be returned. If none of the patterns match, the result_default will be returned.
If pattern can't be matched, it will be ignored.

Here is an example::

    enum Color
        red
        green
        blue

    def enum_static_match ( color, blah )
        static_match color
            if Color red
                return 0
            if match_expr(blah)
                return 1
            if _
                return -1

In this example, color is matched against the enumeration values red, green, and blue. If the match expression color is equal to the enumeration value red, 0 will be returned.
If the match expression color is equal to the value of blah, 1 will be returned. If none of the patterns match, -1 will be returned.

Note that match_expr is used to match blah against the match expression color, rather than directly matching blah against the enumeration value.

If color is not Color first match will fail. If blah is not Color, second match will fail. But the function will always compile.
