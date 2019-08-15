.. _functions:


=================
Functions
=================

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

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function calls
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
You can call function by using it's name and passing all arguments (with a possible ommision of default arguments) ::

    def foo(a, b: int)
        return a + b

    def bar
        foo(1, 2) // a = 1, b = 2

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Named Arguments Function call
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
You can also call function by using it's name and passing all arguments with explicit names (with a possible ommision of default arguments) ::

    def foo(a, b: int)
        return a + b

    def bar
        foo([a = 1, b = 2])  // same as foo(1, 2)

Named arguments should be still in same order, i.e., this is error ::

    def bar
        foo([b = 1, a = 2])  // error

Named arguments call is to increase readibility of callee code and ensure correctness of refactor of existing functions. It also allows defaulting not just last arguments.


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function pointer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Function pointer to function can be obtained using @ and called using invoke builtin::

    let fn = @twice
    let t = invoke(fn, 1)  // t = 2

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Generic functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

However, there are generic (templated) functions, which will be 'instantiated' during compilation of call to them::

    def twice(a)
        return a + a

    let f = twice(1.0)  // 2.0 float
    let i = twice(1)    // 2 int

Generic functions allows writing in daScript like it is dynamic-type language, similar to Python or Lua, but still enjoy the performances and robustness of Strong-static typing.

Generic function address can not be obtained.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Function overloading
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Function names can be overloaded, if function argument types are different ::

    def twice(a: int)
        print("int")
        return a + a
    def twice(a: float)
        print("float")
        return a + a

    let i = twice(1)    // prints "int"
    let f = twice(1.0)  // prints "float"

Declaring function with a same list of arguments is compile time error.

Function arguments of same type can be delcared as follows ::

    def foo(a, b: int)
        return a + b

    //same as above
    def foo(a: int; b: int)
        return a + b

Completely empty function (without arguments) can be also declared as ::

    def foo
        print("foo")

    //same as above
    def foo()
        print("foo")

^^^^^^^^^^^^^^^^^^
Default Paramaters
^^^^^^^^^^^^^^^^^^

.. index::
    single: Function Default Paramaters

daScript's functions can have default parameters.

A function with default parameters is declared as follows: ::

    def test(a, b: int; c: int = 1; d: int = 1)
        return a + b + c + d

when the function *test* is invoked and the parameter c or d are not specified,
the compiler will generate call with default value to the unspecified parameter. A default parameter can be
any valid compile-time const daScript expression. The expression is evaluated at compile-time.

it is valid to declare function with not only last arguments with default values ::

    def test(c: int = 1; d: int = 1; a, b: int) // valid!
        return a + b + c + d

Such syntax can only be used with named arguments call ::

    test(2, 3)           // invalid call, a,b parameters are missing
    test([a = 2, b = 3]) // valid call

And can still be combined with overloading :: 

    def test(c: int = 1; d: int = 1; a, b: int)
        return a + b + c + d
    def test(a, b: int) // now test(2, 3) is valid call
        return test([a = a, b = b])

---------------
OOP-style calls
---------------

There are no methods or function member of structs in daScript.
However, code can be easily written "OOP style" by using rpipe operator '|>'::

    struct Foo
        x, y: int = 0

    def setXY(var this: Foo; x, y: int)
        this.x = x
        this.y = y
    ...
    var foo:Foo
    foo |> setXY(10, 11)   // this is syntactic sugar for setXY(foo, 10, 11)
    setXY(foo, 10, 11)     // exactly same as above line


(see :ref:`Structs <structs>`).

---------------------------------------------
Lambda Functions
---------------------------------------------

...to be written...

---------------------------------------------
Tail Recursion
---------------------------------------------

.. index::
    single: Tail Recursion

Tail recursion is a method for partially transforming a recursion in a program into an
iteration: it applies when the recursive calls in a function are the last executed
statements in that function (just before the return).

Currently daScript doesn't support tail recursion.