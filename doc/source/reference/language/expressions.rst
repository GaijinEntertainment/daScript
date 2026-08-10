.. _expressions:


===========
Expressions
===========

.. index::
    single: Expressions

----------------
Assignment
----------------

.. index::
    single: assignment(=)
    single: move(<-)
    single: clone(:=)

Daslang provides three kinds of assignment:

**Copy assignment** (``=``) performs a bitwise copy of the value:

.. das-doc: given var a : int
.. code-block:: das

    a = 10

Copy assignment is only available for POD types and types that support copying.
Arrays, tables, and other container types cannot be copied — use move or clone instead.

**Move assignment** (``<-``) transfers ownership of a value, zeroing the source:

.. code-block:: das

    var src = new Foo()
    var dst : Foo?
    dst <- src          // dst now points to the Foo instance, src is null

Move is the primary mechanism for transferring ownership of heavy types such as arrays and tables.
Some handled types may be movable but not copyable.

**Clone assignment** (``:=``) creates a deep copy of the value:

.. code-block:: das

    var src : array<int>
    var dst : array<int>
    dst := src          // dst is now a deep copy of src

Clone is syntactic sugar for calling the ``clone`` function.
For POD types, clone falls back to a regular copy.

(see :ref:`Move, Copy, and Clone <move_copy_clone>` for a complete guide, and
:ref:`Clone <clone>` for detailed cloning rules).

----------------
Operators
----------------

.. index::
    single: Operators

^^^^^^^^^^^^^
Arithmetic
^^^^^^^^^^^^^

.. index::
    pair: Arithmetic Operators; Operators

Daslang supports the standard arithmetic operators ``+``, ``-``, ``*``, ``/``, and ``%``
(modulo). Compound assignment operators ``+=``, ``-=``, ``*=``, ``/=``, ``%=`` and
increment/decrement operators ``++`` and ``--`` are also available:

.. das-doc: alt
.. das-doc: given var x : int
.. das-doc: given var y : int
.. code-block:: das

    a += 2          // equivalent to a = a + 2
    x++             // equivalent to x = x + 1
    --y             // prefix decrement

All arithmetic operators are defined for numeric and vector types (``int``, ``uint``,
``int2``–``int4``, ``uint2``–``uint4``, ``float``–``float4``, ``double``).

^^^^^^^^^^^^^
Relational
^^^^^^^^^^^^^

.. index::
    pair: Relational Operators; Operators

Relational operators compare two values and return a ``bool`` result:
``==``, ``!=``, ``<``, ``<=``, ``>``, ``>=``:

.. das-doc: given var b : int
.. code-block:: das

    if ( a == b ) { print("equal\n") }
    if ( x < 0 ) { print("negative\n") }

^^^^^^^^^^^^^^
Logical
^^^^^^^^^^^^^^

.. index::
    pair: Logical operators; Operators

Logical operators work with ``bool`` values:

* ``&&`` — logical AND. Returns ``false`` if the left operand is ``false``; otherwise evaluates and returns the right operand.
* ``||`` — logical OR. Returns ``true`` if the left operand is ``true``; otherwise evaluates and returns the right operand.
* ``^^`` — logical XOR. Returns ``true`` if the operands differ.
* ``!`` — logical NOT. Returns ``false`` if the value is ``true``, and vice versa.

Compound assignment forms are available: ``&&=``, ``||=``, ``^^=``.

.. important::
    ``&&`` and ``||`` use short-circuit evaluation — the right operand is not evaluated
    if the result can be determined from the left operand alone. Unlike their C++ counterparts,
    ``&&=`` and ``||=`` also short-circuit the right side.

^^^^^^^^^^^^^^^^^^^
Bitwise Operators
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Bitwise Operators; Operators

Daslang supports C-like bitwise operators for integer types:

* ``&`` — bitwise AND
* ``|`` — bitwise OR
* ``^`` — bitwise XOR
* ``~`` — bitwise NOT (complement)
* ``<<`` — shift left
* ``>>`` — shift right
* ``<<<`` — rotate left
* ``>>>`` — rotate right

Compound assignment forms: ``&=``, ``|=``, ``^=``, ``<<=``, ``>>=``, ``<<<=``, ``>>>=``:

.. das-doc: given var value : int
.. code-block:: das

    let flags = 0xFF & 0x0F     // 0x0F
    let rotated = value <<< 3   // rotate left by 3 bits

^^^^^^^^^^^^^^^^^^^
Pipe Operators
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Pipe Operators; Operators

Pipe operators pass a value as the first (right pipe) or last (left pipe) argument to a function call:

* ``|>`` — right pipe. ``x |> f(y)`` is equivalent to ``f(x, y)``
* ``<|`` — left pipe. ``f(y) <| x`` is equivalent to ``f(y, x)``

.. code-block:: das

    def addX(a, b) {
        return a + b
    }

    let t = 12 |> addX(2) |> addX(3)   // addX(addX(12, 2), 3) = 17

Left pipe is commonly used to pass blocks and lambdas to functions:

.. code-block:: das

    def doSomething(blk : block) {
        invoke(blk)
    }

    doSomething() <| $() {
        print("hello\n")
    }

In gen2 syntax a block or lambda that immediately follows a function call is
automatically piped as the last argument, so the explicit ``<|`` can be omitted.
Parameterless blocks also do not need the ``$`` prefix:

.. das-doc: given require strings
.. das-doc: given var arr : array<int>
.. das-doc: given def apply_twice(n : int; l : lambda<(a : int) : int>) : int { return invoke(l, invoke(l, n)) }
.. code-block:: das

    doSomething() {                      // same as doSomething() <| $() { ... }
        print("hello\n")
    }

    build_string() $(var writer) {       // block with parameters — $ is still required
        write(writer, "hello")
    }

    sort(arr) $(a, b) => a < b           // single-expression block — arrow shorthand

    apply_twice(3) @(a : int) => a * 2   // lambda — @ is still required

This shorthand — called **assumed pipe** — works with all three call forms:

* **Named function calls** — ``func()``
* **Dot-method calls** — ``obj.method()``
* **Arrow-method calls** — ``obj->fn()``

.. das-doc: given struct Callable { fn : lambda<(blk : block<() : int>) : int> }
.. code-block:: das

    // dot-method call with assumed pipe
    class Receiver {
        def call_method(blk : block<() : int>) : int {
            return invoke(blk)
        }
    }
    var r = new Receiver()
    let res = r.call_method() $ : int {    // same as r.call_method() <| $ : int { ... }
        return 42
    }

    // arrow-method call with assumed pipe
    var c : Callable                       // struct with a lambda field
    c->fn() $ : int {                      // same as c->fn() <| $ : int { ... }
        return 77
    }

It also works with lambdas (``@``) and no-capture lambdas (``@@``), and with the
``=>`` single-expression shorthand — ``arr |> sort() $(a, b) => a < b`` needs no ``<|``.
The explicit ``<|`` is still needed when a non-block expression is piped as the last
argument (e.g. ``take() <| new Node(v = 5)``).

The ``lpipe`` macro from ``daslib/lpipe`` allows piping to the expression on the previous line:

.. code-block:: das

    require daslib/lpipe

    def main {
        print()
        lpipe() <| "this is a string"
    }

^^^^^^^^^^^^^^^^^^^
Interval Operator
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Interval Operator; Operators

The ``..`` operator creates a range from two values:

.. code-block:: das

    let r = 1 .. 10     // equivalent to interval(1, 10)

By default, ``interval(a, b : int)`` returns ``range(a, b)`` and
``interval(a, b : uint)`` returns ``urange(a, b)``.
Custom ``interval`` functions or generics can be defined for other types.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Null-Coalescing Operator (??)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: ?? Operator; Operators

The ``??`` operator returns the dereferenced value of the left operand if it is not ``null``,
otherwise returns the right operand:

.. code-block:: das

    var p : int?
    let value = p ?? 42     // value is 42 because p is null

This is equivalent to:

.. code-block:: das

    let value = (p != null) ? *p : 42

``??`` evaluates expressions left to right until the first non-null value is found
(similar to how ``||`` works for booleans).

The ``??`` operator has higher precedence than the bitwise ``|`` operator (see the precedence table below).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Ternary Operator (? :)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: ?: Operator; Operators

The ternary operator conditionally evaluates one of two expressions:

.. code-block:: das

    let result = (a > b) ? a : b    // returns the larger value

Only the selected branch is evaluated.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Null-Safe Navigation (?. and ?[)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: ?. Operator; Operators
    pair: ?[ Operator; Operators

The ``?.`` operator accesses a field of a pointer only if the pointer is not null.
If the pointer is null, the result is null:

.. code-block:: das

    struct Foo {
        x : int
    }

    struct Bar {
        fooPtr : Foo?
    }

    def getX(bar : Bar?) : int {
        return bar?.fooPtr?.x ?? -1     // returns -1 if bar or fooPtr is null
    }

The ``?[`` operator provides null-safe indexing into tables:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    let i = tab?["three"] ?? 3      // returns 3 because "three" is not in the table

Both operators can be used on the left side of an assignment with ``??``:

.. das-doc: given struct Foo { x : int }
.. das-doc: given struct Bar { fooPtr : Foo? }
.. das-doc: given var bar : Bar?
.. code-block:: das

    var dummy = 0
    bar?.fooPtr?.x ?? dummy = 42    // the assignment targets the ?? dummy fallback lvalue, so writes land in dummy when navigation yields null

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Type Operators (is, as, ?as)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: is Operator; Operators
    pair: as Operator; Operators
    pair: ?as Operator; Operators

The ``is`` operator checks the active variant case:

.. code-block:: das

    variant Value {
        i : int
        f : float
    }
    var v = Value(i = 42)
    if ( v is i ) { print("it's an int\n") }

The ``as`` operator accesses the value of a variant case. It panics if the wrong case is active:

.. code-block:: das

    let x = v as i      // returns 42

The ``?as`` operator is a safe version of ``as`` that returns null if the case does not match:

.. code-block:: das

    let x = v ?as f ?? 0.0     // returns 0.0 because v is not f

These operators can also be used with classes and the ``is``/``as`` operator overloading mechanism
(see :ref:`Pattern Matching <pattern-matching>`).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^
is type<T>
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: is type; Operators

The ``is type<T>`` expression performs a compile-time type check. It returns ``true`` if the
expression's type matches the specified type, and ``false`` otherwise:

.. code-block:: das

    let a = 42
    let b = 3.14
    print("{a is type<int>}\n")     // true
    print("{b is type<float>}\n")   // true
    print("{b is type<int>}\n")     // false

This is useful in generic functions to branch on the actual type of a parameter:

.. code-block:: das

    def describe(x) {
        static_if (x is type<int>) {
            print("an integer\n")
        } static_elif (x is type<float>) {
            print("a float\n")
        } else {
            print("something else\n")
        }
    }

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Cast, Upcast, and Reinterpret
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Cast Operators; Operators

**cast** performs a safe upcast from a derived structure type to a parent (base) type:

.. das-doc: given struct Base { z : int }
.. das-doc: given struct Derived : Base { w : int }
.. code-block:: das

    var derived : Derived = Derived()
    var base : Base = cast<Base>(derived)

**upcast** performs an unsafe upcast from a base type to a derived type.
This requires ``unsafe`` because the actual runtime type may not match:

.. das-doc: given var base_ref : Base
.. code-block:: das

    unsafe {
        var d = upcast<Derived>(base_ref)
    }

**reinterpret** reinterprets the raw bits of a value as a different type.
This is unsafe and should be used with extreme caution:

.. code-block:: das

    unsafe {
        let p = reinterpret<void?>(13)
    }

^^^^^^^^^^^^^^^^^^^
Dereference
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Dereference; Operators

The ``*`` prefix operator dereferences a pointer, converting it to a reference.
Dereferencing a null pointer causes a panic:

.. code-block:: das

    var p = new Foo()
    var ref = *p        // ref is Foo&

The ``deref`` keyword can be used as an alternative:

.. code-block:: das

    var ref = deref(p)

^^^^^^^^^^^^^^^^^^^
Address-of
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: addr; Operators

The ``addr`` function takes the address of a value, creating a pointer.
This is an unsafe operation:

.. code-block:: das

    unsafe {
        var x = 42
        var p = addr(x)     // p is int?
    }

``addr<T?>(x)`` takes the address and reinterprets it as ``T?`` in one step —
pure syntax sugar for ``reinterpret<T?>(addr(x))``. A single ``unsafe()``
covers both halves, where the spelled-out form needs two:

.. code-block:: das

    var v = float3(1., 2., 3.)
    let p = unsafe(addr<float?>(v))                         // one unsafe gate
    // equivalent long form, two gates:
    // let q = unsafe(reinterpret<float?>(unsafe(addr(v))))

The target type must be a pointer — ``addr<int>(x)`` is a compile error
(it is always a typo for ``addr<int?>(x)``).

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Original Operator Access (!)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: !. Operator; Operators
    pair: . . Operator; Operators

Smart pointers (``smart_ptr<T>``) are accessed the same way as regular pointers —
using ``.`` for field access and ``?.`` for null-safe field access.

Placing ``!`` in front of an overloadable access or test operator gives the
**original** operator, never the overload.  ``a!.x`` bypasses any ``.`` operator
overloading (a property, see :ref:`Classes <classes>`) and accesses the
underlying field directly.  This is useful when a type defines a custom ``.``
operator but you need to reach the actual field:

.. das-doc: given struct Boxed { payload : int }
.. das-doc: given var sp : Boxed?
.. code-block:: das

    sp!.payload = 42    // accesses field payload directly, skipping any . overload

The whole family follows the same rule.  Each spelling is the overloadable
operator with ``!`` in front, and yields the built-in behavior even when a
matching overload (or a variant macro) exists:

+--------------+---------------------------------------------------+
| Raw spelling | Original operation                                |
+==============+===================================================+
| ``a!.x``     | field access (skips ``operator .``)               |
+--------------+---------------------------------------------------+
| ``a!?.x``    | null-safe field access (skips ``operator ?.``)    |
+--------------+---------------------------------------------------+
| ``a![i]``    | indexing (skips ``operator []``)                  |
+--------------+---------------------------------------------------+
| ``a!?[i]``   | null-safe indexing (skips ``operator ?[]``)       |
+--------------+---------------------------------------------------+
| ``a !?? b``  | pointer null-coalescing (skips ``operator ??``)   |
+--------------+---------------------------------------------------+
| ``a !is x``  | variant / type check (skips ``operator is``)      |
+--------------+---------------------------------------------------+
| ``a !as x``  | variant access (skips ``operator as``)            |
+--------------+---------------------------------------------------+
| ``a !?as x`` | safe variant access (skips ``operator ?as``)      |
+--------------+---------------------------------------------------+

Generated and generic code uses these forms to guarantee the language's own
semantics no matter what overloads a user module brings into scope.

The older dot-prefixed spellings remain valid and equivalent: ``a. .x``
(the two dots must be separated by a space — ``..`` with no space is the
:ref:`interval <expressions>` operator token, so ``sp..payload`` parses as
``interval(sp, payload)``), ``a.?.x``, ``a.[i]``, and ``a.?[i]``.  The variant
and coalescing operators have no dot-prefixed form — ``!`` is the only bypass
spelling there.

^^^^^^^^^^^^^^^^^^^
Safe Index (?[)
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: ?[ Operator; Operators

The ``?[`` operator provides null-safe indexing.  If the pointer or table key
is null or missing, the result is ``null`` instead of a panic:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    let i = tab?["three"] ?? 3      // returns 3 because "three" is not in the table

^^^^^^^^^^^^^^^^^^^
Unsafe Expression
^^^^^^^^^^^^^^^^^^^

.. index::
    pair: unsafe; Expression

Individual expressions can be marked as unsafe without wrapping an entire block:

.. das-doc: alt
.. code-block:: das

    let p = unsafe(addr(x))

This is equivalent to wrapping the expression in an ``unsafe { }`` block.

^^^^^^^^^^^^^^^^^^^^^
Operators Precedence
^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: Operators Precedence; Operators

+--------------------------------------------------------------------------+-----------+
| ``post++  post--  .   ->  ?. ?[ *(deref)``                               | highest   |
+--------------------------------------------------------------------------+-----------+
| ``|>  <|``                                                               |           |
+--------------------------------------------------------------------------+-----------+
| ``is  as``                                                               |           |
+--------------------------------------------------------------------------+-----------+
| ``-  +  ~  !   ++  --``                                                  |           |
+--------------------------------------------------------------------------+-----------+
| ``??``                                                                   |           |
+--------------------------------------------------------------------------+-----------+
| ``/  *  %``                                                              |           |
+--------------------------------------------------------------------------+-----------+
| ``+  -``                                                                 |           |
+--------------------------------------------------------------------------+-----------+
| ``<<  >> <<< >>>``                                                       |           |
+--------------------------------------------------------------------------+-----------+
| ``<  <=  >  >=``                                                         |           |
+--------------------------------------------------------------------------+-----------+
| ``==  !=``                                                               |           |
+--------------------------------------------------------------------------+-----------+
| ``&``                                                                    |           |
+--------------------------------------------------------------------------+-----------+
| ``^``                                                                    |           |
+--------------------------------------------------------------------------+-----------+
| ``|``                                                                    |           |
+--------------------------------------------------------------------------+-----------+
| ``&&``                                                                   |           |
+--------------------------------------------------------------------------+-----------+
| ``^^``                                                                   |           |
+--------------------------------------------------------------------------+-----------+
| ``||``                                                                   |           |
+--------------------------------------------------------------------------+-----------+
| ``?  :``                                                                 |           |
+--------------------------------------------------------------------------+-----------+
| ``+=  =  -=  /=  *=  %=  &=  |=  ^=  <<=  >>=  <- <<<= >>>= &&= ||=      |           |
| ^^=  :=``                                                                |           |
+--------------------------------------------------------------------------+-----------+
| ``..  =>``                                                               |           |
+--------------------------------------------------------------------------+-----------+
| ``,``                                                                    | lowest    |
+--------------------------------------------------------------------------+-----------+

.. _array_contructor:

-----------------
Array Initializer
-----------------

.. index::
    single: Fixed Array Initializer

Fixed-size arrays can be created with the ``fixed_array`` keyword:

.. code-block:: das

    let a = fixed_array<int>(1, 2)      // int[2]
    let b = fixed_array(1, 2, 3)        // inferred as int[3]

.. index::
    single: Dynamic Array Initializer

Dynamic arrays can be created with several syntaxes:

.. code-block:: das

    let a <- [1, 2, 3]                  // array<int>
    let b <- array(1, 2, 3)             // array<int>
    let c <- array<int>(1, 2, 3)        // explicitly typed
    let d <- [for (x in range(0, 10)); x * x]  // comprehension

(see :ref:`Arrays <arrays>`, :ref:`Comprehensions <comprehensions>`).

.. _struct_contructor:

-------------------------------------------
Struct, Class, and Handled Type Initializer
-------------------------------------------

.. index::
    single: Struct, Class, and Handled type Initializer

Structures can be initialized by specifying field values:

.. code-block:: das

    struct Point {
        x : int = 1
        y : int = 2
    }

    let a = Point(x = 13, y = 11)                   // x = 13, y = 11
    let b = Point(x = 13)                           // x = 13, y = 2 (default)
    let c = unsafe(Point(uninitialized x = 13))     // x = 13, y = 0 (uninitialized construction requires unsafe)

Arrays of structures can be constructed inline:

.. code-block:: das

    var arr <- array struct<Point>((x=11, y=22), (x=33), (y=44))

Classes and handled (external) types can also be initialized using this syntax.
Classes and handled types cannot use ``uninitialized``.

(see :ref:`Structs <structs>`, :ref:`Classes <classes>`).

.. _tuple_contructor:

-----------------
Tuple Initializer
-----------------

.. index::
    single: Tuple Initializer

Tuples can be created with several syntaxes:

.. code-block:: das

    let a = (1, 2.0, "3")                                  // inferred tuple type
    let b = tuple(1, 2.0, "3")                              // same as above
    let c = tuple<a:int; b:float; c:string>(a=1, b=2.0, c="3")  // named fields

(see :ref:`Tuples <tuples>`).

-------------------
Variant Initializer
-------------------

.. index::
    single: Variant Initializer

Variants are created by specifying exactly one field:

.. code-block:: das

    variant Number {
        i : int
        f : float
    }

    let ni = Number(i = 3)
    let nf = Number(f = 4.0)

Variants can also be declared as type aliases:

.. code-block:: das

    typedef Number = variant<i:int; f:float>

(see :ref:`Variants <variants>`).

-----------------
Table Initializer
-----------------

.. index::
    single: Table Initializer

Tables are created by specifying key-value pairs separated by ``=>``:

.. code-block:: das

    var a <- { 1=>"one", 2=>"two" }
    var b <- table("one"=>1, "two"=>2)      // alternative syntax
    var c <- table<string; int>("one"=>1)    // explicitly typed

All values in a table literal must be of the same type. Similarly, all keys must be of the same type.

(see :ref:`Tables <tables>`).

------------------------------
default and new
------------------------------

.. index::
    single: default
    single: new

The ``default`` expression creates a default-initialized value of a given type:

.. code-block:: das

    var a = default<Point>          // all fields zeroed, then default initializer called
    var b = unsafe(default<Point> uninitialized)  // all fields zeroed, no initializer (uninitialized requires unsafe)

The ``new`` operator allocates a value on the heap and returns a pointer:

.. code-block:: das

    var p = new Point()             // Point? pointer, default initialized
    var q = new Point(x = 13)       // with field initialization

``new`` can also be combined with array and table literals to allocate them on the heap:

.. code-block:: das

    var p <- new [1, 2, 3]          // heap-allocated array<int>

------------------------------
typeinfo
------------------------------

.. index::
    single: typeinfo

The ``typeinfo`` expression provides compile-time type information. It is primarily
used in generic functions to inspect argument types:

.. das-doc: given var myStruct : Foo
.. code-block:: das

    typeinfo typename(type<int>)        // returns "int" at compile time
    typeinfo sizeof(type<float3>)       // returns 12
    typeinfo is_pod(type<int>)          // returns true
    typeinfo has_field<x>(myStruct)     // returns true if myStruct has field x

(see :ref:`Generic Programming <generic_programming>` for a complete list of typeinfo traits).

------------------------------
String Interpolation
------------------------------

.. index::
    single: string interpolation

Expressions inside curly brackets within a string are evaluated and converted to text:

.. code-block:: das

    let name = "world"
    print("Hello, {name}!")             // Hello, world!
    print("1 + 2 = {1 + 2}")           // 1 + 2 = 3

Format specifiers can be added after a colon:

.. code-block:: das

    let pi = 3.14159
    print("pi = {pi:5.2f}")            // formatted output

To include literal curly brackets, escape them with backslashes:

.. code-block:: das
    :force:

    print("Use \{curly\} brackets")     // Use {curly} brackets

(see :ref:`String Builder <string_builder>`).

.. seealso::

    :ref:`Statements <statements>` for control flow and variable declarations,
    :ref:`Pattern matching <pattern-matching>` for ``is``/``as``/``?as`` operator details,
    :ref:`Datatypes <datatypes_and_values>` for a list of types used in expressions.

