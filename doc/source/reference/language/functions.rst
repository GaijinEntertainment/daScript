.. _functions:

========
Function
========

.. index::
    single: Functions

Functions Pointers are first class values like integer or strings and can be stored in table slots,
local variables, arrays and passed as function parameters.
Functions themselves are declarations (much like in C++ language).

--------------------
Function declaration
--------------------

.. index::
    single: Function Declaration

Functions are similar to those in most other typed languages::

    def twice(a: int): int
        return a+a

Completely empty function (without arguments) can be also declared::

    def foo
        print("foo")

    //same as above
    def foo()
        print("foo")

daScript can always infer function return type.
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

If not specified functions inherit module publicity. I.e. in public modules functions are public,
and in private modules functions are private.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function calls
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can call function by using it's name and passing all arguments (with the possible omission of the default arguments) ::

    def foo(a, b: int)
        return a + b

    def bar
        foo(1, 2) // a = 1, b = 2

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Named Arguments Function call
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can also call function by using it's name and passing all arguments with explicit names (with the possible omission of the default arguments) ::

    def foo(a, b: int)
        return a + b

    def bar
        foo([a = 1, b = 2])  // same as foo(1, 2)

Named arguments should be still in same order::

    def bar
        foo([b = 1, a = 2])  // error, out of order

Named arguments calls increase readability of callee code and ensure correctness of refactoring of the existing functions.
They also allow default values for arguments other than last::

    def foo(a:int=13; b: int)
        return a + b

    def bar
        foo([b = 2])  // same as foo(13, 2)


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function pointer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Pointer to a function uses similar declaration as that of a block or lambda::

    function_type ::= function { optional_function_type }
    optional_function_type ::= < { optional_function_arguments } { : return_type } >
    optional_function_arguments := ( function_argument_list )
    function_argument_list := argument_name : type | function_argument_list ; argument_name : type

    function < (arg1:int;arg2:float&):bool >

Function pointer can be obtained using ``@@`` operator::

    def twice(a:int)
        return a + a

    let fn = @@twice

When multiple functions have the same name, pointer can be obtained by explicitly specifying signature::

    def twice(a:int)
        return a + a

    def twice(a:float)  // when this one is required
        return a + a

    let fn = @@<(a:float):float> twice

Function pointer can be called via ``invoke``::

    let t = invoke(fn, 1)  // t = 2

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Nameless functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Pointer to nameless function can be created with syntax,
similar to that of lambda or block (see :ref:`Blocks <blocks_declarations>`)::

    let fn <- @@ <| ( a : int )
        return a + a

Nameless local functions do not capture variables at all::

    var count = 1
    let fn <- @@ <| ( a : int )
        return a + count            // compilation error, can't locate variable count

Internally regular function will be generated::

    def _localfunction_thismodule_8_8_1`function ( a:int const ) : int
            return a + a

    let fn:function<(a:int const):int> const <- @@_localfunction_thismodule_8_8_1`function

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Generic functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Generic functions are similar to C++ templated functions.
daScript will 'instance' them during the infer pass of the compilation::

    def twice(a)
        return a + a

    let f = twice(1.0)  // 2.0 float
    let i = twice(1)    // 2 int

Generic functions allow code similar to dynamic-type language like Python or Lua,
while still enjoying the performances and robustness of Strong-static typing.

Generic function address can not be obtained.

Unspecified type can also be written via ``auto`` notation::

    def twice(a:auto)   // same as 'twice' above
        return a + a

Generic function can specialize generic type alias, and use it as part of the declaration::

    def twice(a:auto(TT)) : TT
        return a + a

In the example above alias ``TT`` is used to enforce return type contract.

Type aliases can be used before the corresponding auto::

    def summ(base : TT; a:auto(TT)[] )
        var s = base
        for x in a
            s += x
        return s

In the example above TT is inferred from a type of the passed array ``a``, and expected as a first argument ``base``.
Return type will be inferred from the type of ``s``, which would also be TT.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function overloading
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Function can be specialized if function argument types are different ::

    def twice(a: int)
        print("int")
        return a + a
    def twice(a: float)
        print("float")
        return a + a

    let i = twice(1)    // prints "int"
    let f = twice(1.0)  // prints "float"

Declaring function with a same exact argument list is compilation time error.

Function can be partially specialized::

    def twice(a:int)        // int
        return a + a
    def twice(a:float)      // float
        return a + a
    def twice(a:auto[])     // any array
        return length(a)*2
    def twice(a)            // any other case
        return a + a

daScript uses the following rules for matching partially specialized functions

    1. non auto is more specialized than auto
    2. if both are non-auto, the one without cast is more specialized
    3. the one with array is more specialized, than one without. if both have array, one with actual value is more specialized, than the one without
    4. the one with base type of auto\alias is less specialized. if both are auto\alias - its assumed that its the same level of specialization
    5. for pointer or array - subtypes are compared
    6. for table, tuple and variant subtypes are compared, and all must compare same or equally specialized
    7. for functions, blocks, or lambdas subtypes and return types are compared, and all must compare same or equally specialized

When matching function daScript picks the ones which are most specialized, and sorts by substitute distance.
Substitute distance is increased by 1 for each argument, if cast is required for the LSP (Liskov substitution principle).
At the end function with the least distance is picked. If more than one function is left for picking, compilation error is reported.

Function specialization can be limited by contracts (contract macros)::

    [expect_any_array(blah)]  // array<foo>, [], or dasvector`.... or similar
    def print_arr ( blah )
        for i in range(length(blah))
            print("{blah[i]}\n")

In the example above only arrays will be matched.

LSP can be explicitly prohibited for a particular function argument via `explicit` keyword::

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

when the function *test* is invoked and the parameter c or d are not specified,
the compiler will generate call with default value to the unspecified parameter. A default parameter can be
any valid compile-time const daScript expression. The expression is evaluated at compile-time.

it is valid to declare default values for arguments other than last::

    def test(c: int = 1; d: int = 1; a, b: int) // valid!
        return a + b + c + d

Calling such functions with default arguments requires named arguments call::

    test(2, 3)           // invalid call, a,b parameters are missing
    test([a = 2, b = 3]) // valid call

Default arguments can be combined with overloading ::

    def test(c: int = 1; d: int = 1; a, b: int)
        return a + b + c + d
    def test(a, b: int) // now test(2, 3) is valid call
        return test([a = a, b = b])

---------------
OOP-style calls
---------------

There are no methods or function member of structs in daScript.
However, code can be easily written "OOP style" by using right pipe operator ``|>``::

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

Tail recursion is a method for partially transforming a recursion in a program into an
iteration: it applies when the recursive calls in a function are the last executed
statements in that function (just before the return).

Currently daScript doesn't support tail recursion.
It is implied that daScript function always returns.
