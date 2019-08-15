.. _expressions:


=================
Expressions
=================

.. index::
    single: Expressions

----------------
Assignment
----------------

.. index::
    single: assignment(=)
    single: move(<-)
    single: clone(:=)

::

    exp := exp '=' exp
    exp := exp '<-' exp
    exp := exp ':=' exp

daScript implements 3 kind of assignment: the normal assignment(=)::

    a = 10

"move" assignment ::

    var b = new Foo
    var a: Foo?
    a <- b

move assignment nullifies source (b)
It's main purpose is to correctly move ownership, and optimize copying if you don't need source for heay types (such as arrays, tables).
Some external handled types can be non assignable, but still moveable;

    var

"clone" assignment ::

    a := b

Clone assignment is syntactic sugar for calling clone(var a: auto&; b: auto&) if it exists or basic assignment for POD types.
It is also implemented for das_string, array and table types, and creates 'deep' copy.

Some external handled types can be non assignable, but still copyable;

----------------
Operators
----------------

.. index::
    single: Operators

^^^^^^^^^^^^^
?: Operator
^^^^^^^^^^^^^

.. index::
    pair: ?: Operator; Operators

::

    exp := exp_cond '?' exp1 ':' exp2

conditionally evaluate an expression depending on the result of an expression.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^
?? Null-coalescing operator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: ?? Operator; Operators

::
    
    exp := exp1 '??' exp2


Conditionally evaluate an expression2 depending on the result of an expression1. 
Given code is equivalent to:

::

    exp := (exp1 '!=' null) '?' deref(exp1) ':' exp2


It evaluates expressions until the first non-null value
(just like || operators for the first 'true' one).

Operator precendence is also follows C# design, so that ?? has
lower priority than ||


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
?. - Null-propagation operator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: ?. Operator; Operators

::

    exp := value '?.' key


If value is not null exists, return deref of field 'key' for struct, else return null.

::

    struct TestObjectFooNative
        fooData : int

    struct TestObjectBarNative
        fooPtr: TestObjectFooNative?
        barData: float

    def test
        var a: TestObjectFooNative?
        var b: TestObjectBarNative?
        var idummy: int
        var fdummy: float
        a?.fooData ?? idummy = 1 // will return reference to idummy, since a is null
        assert(idummy == 1)

        a = new TestObjectFooNative
        a?.fooData ?? idummy = 2 // will return reference to a.fooData, since a is now not null
        assert(a.fooData == 2 & idummy == 1)

        b = new TestObjectBarNative
        b?.fooPtr?.fooData ?? idummy = 3 // will return reference to idummy, since while b is not null, but b.?barData is still null
        assert(idummy == 3)

        b.fooPtr <- a
        b?.fooPtr?.fooData ?? idummy = 4 // will return reference to b.fooPtr.fooData
        assert(b.fooPtr.fooData == 4 & idummy == 3)


^^^^^^^^^^^^^
Arithmetic
^^^^^^^^^^^^^

.. index::
    pair: Arithmetic Operators; Operators

::

    exp:= 'exp' op 'exp'

daScript supports the standard arithmetic operators ``+, -, *, / and %``.
Other than that is also supports compact operators (``+=, -=, *=, /=, %=, ^=, <<=, >>=``) and
increment and decrement operators(++ and --);::

    a += 2
    // is the same as writing
    a = a + 2
    x++
    // is the same as writing
    x = x + 1

All operators work normally with (u)int* and float* and double.

^^^^^^^^^^^^^
Relational
^^^^^^^^^^^^^

.. index::
    pair: Relational Operators; Operators

::

    exp:= 'exp' op 'exp'

Relational operators in daScript are : ``==, <, <=, <, <=, !=``

These operators return true if the expression is false and a value different than true if the
expression is true.

^^^^^^^^^^^^^^
Logical
^^^^^^^^^^^^^^

.. index::
    pair: Logical operators; Operators

::

    exp := exp op exp
    exp := '!' exp

Logical operators in daScript are : ``&, |, !``

The operator ``&`` (logical and) returns false if its first argument is false, otherwise returns
its second argument.
The operator ``|`` (logical or) returns its first argument if is different than false, otherwise
returns the second argument.

It is important to understand, that it won't nesessariy 'evaluates' all arguments.

The '!' operator will return false if the given value to negate was true or false otherwise.

^^^^^^^^^^^^^^^^^^^
Bitwise Operators
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Bitwise Operators; Operators

::

    exp:= 'exp' op 'exp'
    exp := '~' exp

daScript supports the standard C-like bitwise operators ``&, |, ^, ~, <<, >>``.
Those operators only work on (unsigned) integer values.

^^^^^^^^^^^^^^^^^^^
Pipe Operators
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Pipe Operators; Operators

::

    exp:= 'exp' |> 'exp'
    exp:= 'exp' <| 'exp'

daScript supports pipe operators. Pipe operator is similar to 'call' expression with other expresion is first argument.

::

    def addX(a, b)
        assert(b == 2 | b == 3)
        return a + b
    def test
        let t = 12 |> addX(2) |> addX(3)
        assert(t == 17)
        return true

::

    def addOne(a)
        return a + 1

    def test
        let t =  addX |> 2
        assert(t == 3)

^^^^^^^^^^^^^^^^^^^^^
Operators precedence
^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Operators precedence; Operators

+----------------------------------------------------+-----------+
| ``post++  post--  .   ->  ?.``                     | highest   |
+----------------------------------------------------+-----------+
| ``|>  <|``                                         |           |
+----------------------------------------------------+-----------+
| ``-  +  ~  !   ++  --``                            |           |
+----------------------------------------------------+-----------+
| ``??``                                             |           |
+----------------------------------------------------+-----------+
| ``/  *  %``                                        |           |
+----------------------------------------------------+-----------+
| ``+  -``                                           |           |
+----------------------------------------------------+-----------+
| ``<<  >>``                                         |           |
+----------------------------------------------------+-----------+
| ``<  <=  >  >=``                                   |           |
+----------------------------------------------------+-----------+
| ``==  !=``                                         |           |
+----------------------------------------------------+-----------+
| ``&``                                              |           |
+----------------------------------------------------+-----------+
| ``^``                                              |           |
+----------------------------------------------------+-----------+
| ``|``                                              |           |
+----------------------------------------------------+-----------+
| ``?  :``                                           |           |
+----------------------------------------------------+-----------+
| ``+=  =  -=  /=  *=  %=  &=  |=  ^=  <<=  >>=  <-``| ...       |
+----------------------------------------------------+-----------+
| ``',' comma``                                      | lowest    |
+----------------------------------------------------+-----------+

.. _struct_contructor:

------------------
Struct Constructor
------------------

.. index::
    single: Struct Contructor

::

    struct Foo
      x: int = 1
      y: int = 2

    let fExplicit = [[Foo x = 13, y = 11]] // x = 13, y = 11
    let fPartial  = [[Foo x = 13]]         // x = 13, y = 0

(see :ref:`Structs <structs>`).

-----------------
Array contructor
-----------------

.. index::
    single: Array constructor

::

    exp := '[['type[] [explist] ']]'

Creates a new fixed size array.::

    let a = [[int[] 1, 2]]     // creates array of two elements
    let a = [[int[2] 1, 2]]    // creates array of two elements
    let a = [[int[2] 1, 2, 3]] // error, too many initializers

Arrays can be also created with array comprehensions::

    let q <- [[ for x in range(0, 10); x * x ]]
