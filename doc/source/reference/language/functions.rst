.. _functions:

========
Function
========

.. index::
    single: Functions

Functions pointers are first class values, like integers or strings, and can be stored in table slots,
local variables, arrays, and passed as function parameters.
Functions themselves are declarations (much like in C++).

--------------------
Function declaration
--------------------

.. index::
    single: Function Declaration

Functions are similar to those in most other typed languages::

    def twice(a: int): int
        return a+a

Completely empty functions (without arguments) can be also declared::

    def foo
        print("foo")

    //same as above
    def foo()
        print("foo")

daScript can always infer a function's return type.
Returning different types is a compilation error::

    def foo(a:bool)
        if a
            return 1
        else
            return 2.0  // error, expecting int


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Publicity
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Functions can be `private` or `public` ::

    def private foo(a:bool)

    def public bar(a:float)

If not specified, functions inherit module publicity (i.e. in public modules functions are public,
and in private modules functions are private).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function calls
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can call a function by using its name and passing in all its arguments (with the possible omission of the default arguments)::

    def foo(a, b: int)
        return a + b

    def bar
        foo(1, 2) // a = 1, b = 2

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Named Arguments Function call
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can also call a function by using its name and passing all aits rguments with explicit names (with the possible omission of the default arguments)::

    def foo(a, b: int)
        return a + b

    def bar
        foo([a = 1, b = 2])  // same as foo(1, 2)

Named arguments should be still in the same order::

    def bar
        foo([b = 1, a = 2])  // error, out of order

Named argument calls increase the readability of callee code and ensure correctness in refactorings of the existing functions.
They also allow default values for arguments other than the last ones::

    def foo(a:int=13; b: int)
        return a + b

    def bar
        foo([b = 2])  // same as foo(13, 2)


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function pointer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Pointers to a function use a similar declaration to that of a block or lambda::

    function_type ::= function { optional_function_type }
    optional_function_type ::= < { optional_function_arguments } { : return_type } >
    optional_function_arguments := ( function_argument_list )
    function_argument_list := argument_name : type | function_argument_list ; argument_name : type

    function < (arg1:int;arg2:float&):bool >

Function pointers can be obtained by using the ``@@`` operator::

    def twice(a:int)
        return a + a

    let fn = @@twice

When multiple functions have the same name, a pointer can be obtained by explicitly specifying signature::

    def twice(a:int)
        return a + a

    def twice(a:float)  // when this one is required
        return a + a

    let fn = @@<(a:float):float> twice

Function pointers can be called via ``invoke``::

    let t = invoke(fn, 1)  // t = 2

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Nameless functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Pointers to nameless functions can be created with a syntax
similar to that of lambdas or blocks (see :ref:`Blocks <blocks_declarations>`)::

    let fn <- @@ <| ( a : int )
        return a + a

Nameless local functions do not capture variables at all::

    var count = 1
    let fn <- @@ <| ( a : int )
        return a + count            // compilation error, can't locate variable count

Internally, a regular function will be generated::

    def _localfunction_thismodule_8_8_1`function ( a:int const ) : int
            return a + a

    let fn:function<(a:int const):int> const <- @@_localfunction_thismodule_8_8_1`function

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Generic functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Generic functions are similar to C++ templated functions.
daScript will instantiate them during the infer pass of compilation::

    def twice(a)
        return a + a

    let f = twice(1.0)  // 2.0 float
    let i = twice(1)    // 2 int

Generic functions allow code similar to dynamically-typed languages like Python or Lua,
while still enjoying the performance and robustness of strong, static typing.

Generic function addresses cannot be obtained.

Unspecified types can also be written via ``auto`` notation::

    def twice(a:auto)   // same as 'twice' above
        return a + a

Generic functions can specialize generic type aliases, and use them as part of the declaration::

    def twice(a:auto(TT)) : TT
        return a + a

In the example above, alias ``TT`` is used to enforce the return type contract.

Type aliases can be used before the corresponding ``auto``::

    def summ(base : TT; a:auto(TT)[] )
        var s = base
        for x in a
            s += x
        return s

In the example above, ``TT`` is inferred from the type of the passed array ``a``, and expected as a first argument ``base``.
The return type is inferred from the type of ``s``, which is also ``TT``.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function overloading
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Functions can be specialized if their argument types are different::

    def twice(a: int)
        print("int")
        return a + a
    def twice(a: float)
        print("float")
        return a + a

    let i = twice(1)    // prints "int"
    let f = twice(1.0)  // prints "float"

Declaring functions with the same exact argument list is compilation time error.

Functions can be partially specialized::

    def twice(a:int)        // int
        return a + a
    def twice(a:float)      // float
        return a + a
    def twice(a:auto[])     // any array
        return length(a)*2
    def twice(a)            // any other case
        return a + a

daScript uses the following rules for matching partially specialized functions:

    1. Non-``auto`` is more specialized than ``auto``.
    2. If both are non-``auto``, the one without a cast is more specialized.
    3. Ones with arrays are more specialized than ones without. If both have an array, the one with the actual value is more specialized than the one without.
    4. Ones with a base type of auto\alias are less specialized. If both are auto\alias, it is assumed that they have the same level of specialization.
    5. For pointers and arrays, the subtypes are compared.
    6. For tables, tuples and variants, subtypes are compared, and all must be the same or equally specialized.
    7. For functions, blocks, or lambdas, subtypes and return types are compared, and all must be the same or equally specialized.

When matching functions, daScript picks the ones which are most specialized and sorts by substitute distance.
Substitute distance is increased by 1 for each argument if a cast is required for the LSP (Liskov substitution principle).
At the end, the function with the least distance is picked. If more than one function is left for picking, a compilation error is reported.

Function specialization can be limited by contracts (contract macros)::

    [expect_any_array(blah)]  // array<foo>, [], or dasvector`.... or similar
    def print_arr ( blah )
        for i in range(length(blah))
            print("{blah[i]}\n")

In the example above, only arrays will be matched.

LSP can be explicitly prohibited for a particular function argument via the `explicit` keyword::

    def foo ( a : Foo explicit ) // will accept Foo, but not any subtype of Foo

^^^^^^^^^^^^^^^^^^
Default Parameters
^^^^^^^^^^^^^^^^^^

.. index::
    single: Function Default Parameters

daScript's functions can have default parameters.

A function with default parameters is declared as follows: ::

    def test(a, b: int; c: int = 1; d: int = 1)
        return a + b + c + d

When the function *test* is invoked and the parameters `c` or `d` are not specified,
the compiler will generate a call with default value to the unspecified parameter. A default parameter can be
any valid compile-time const daScript expression. The expression is evaluated at compile-time.

It is valid to declare default values for arguments other than the last one::

    def test(c: int = 1; d: int = 1; a, b: int) // valid!
        return a + b + c + d

Calling such functions with default arguments requires a named arguments call::

    test(2, 3)           // invalid call, a,b parameters are missing
    test([a = 2, b = 3]) // valid call

Default arguments can be combined with overloading::

    def test(c: int = 1; d: int = 1; a, b: int)
        return a + b + c + d
    def test(a, b: int) // now test(2, 3) is valid call
        return test([a = a, b = b])

---------------
OOP-style calls
---------------

There are no methods or function members of structs in daScript.
However, code can be easily written "OOP style" by using the right pipe operator ``|>``::

    struct Foo
        x, y: int = 0

    def setXY(var thisFoo: Foo; x, y: int)
        thisFoo.x = x
        thisFoo.y = y
    ...
    var foo:Foo
    foo |> setXY(10, 11)   // this is syntactic sugar for setXY(foo, 10, 11)
    setXY(foo, 10, 11)     // exactly same as above line


(see :ref:`Structs <structs>`).

---------------------------------------------
Tail Recursion
---------------------------------------------

.. index::
    single: Tail Recursion

Tail recursion is a method for partially transforming recursion in a program into
iteration: it applies when the recursive calls in a function are the last executed
statements in that function (just before the return).

Currently, daScript doesn't support tail recursion.
It is implied that a daScript function always returns.
