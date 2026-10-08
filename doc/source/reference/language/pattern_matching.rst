.. _pattern-matching:

================
Pattern matching
================

Pattern matching allows you to compare a value against a set of structural patterns, extracting specific
fields when a pattern matches.
In Daslang, pattern matching is implemented via macros in the ``daslib/match`` module.

A ``match`` is a list of arms. Each arm is ``pattern => body``; the first arm whose pattern
matches runs, and the rest are skipped:

.. code-block:: das

    require daslib/match

    def describe_number ( n : int ) {
        match ( n ) {
            0 => $ { return "zero" }
            1 | 2 => $ { return "one or two" }
            _ => $ { return "many" }
        }
        return "unreachable"
    }

Arm bodies
----------

A body of several statements is a block literal, ``$ { ... }``. The block is spliced in place,
so ``return`` inside it returns from the enclosing function and ``break`` / ``continue`` act on the
enclosing loop. A body that is a single expression needs no block:

.. code-block:: das

    def print_sign ( n : int ) {
        match ( n ) {
            0 => print("zero\n")
            _ => $ {
                let sign = n > 0 ? "positive" : "negative"
                print("{sign}\n")
            }
        }
    }

An assignment binds looser than ``=>``, so ``pattern => total += n`` does not parse as an arm.
Write an assignment body as a block: ``pattern => $ { total += n }``.

A plain ``{ ... }`` after ``=>`` is a table literal, not a block - always write the ``$``.

match as a value
----------------

In value position - an initializer, a ``return``, an operand - ``match`` produces the value of the
arm that matched. Every body is then an expression, and the arms must cover every value (see
`Coverage`_), so every path yields a value:

.. code-block:: das

    def color_code ( c : int ) : string {
        return match ( c ) {
            0 => "black"
            1 => "red"
            _ => "other"
        }
    }

The value form compiles to a block that the inliner splices back in place, so it costs nothing over
the statement form. When arms produce different types - ``Square?`` and ``Rect?`` for a ``Shape?``
result - the match takes the declared result type of the function it returns from, or the declared
type of the variable it initializes.

Coverage
--------

The compiler checks the arms of a ``pattern => body`` match against each other:

- **An arm that cannot be reached is an error** - a value already matched by an earlier arm
  (``1 => ...`` then ``2 | 1 => ...``), any arm after one that matches everything, and a ``_`` after
  arms that already cover every value of the type.
- **A match must be exhaustive.** For a type with a fixed set of values - an enumeration, ``bool``,
  a variant, a pointer to a struct (``null`` or not) - every value must be matched, in a statement
  as well as in a value; the error names up to three values no arm matches. A match on several
  values, on a tuple, or on a struct whose arms spell struct patterns, is checked field by field and
  combination by combination: ``Light(color = Color.red)``, ``Light(color = Color.green, on = true)``
  misses ``(color = Color.green, on = false)``. A match used as a value
  must also cover the other types (``int``, ``string``, structs...), which in practice means a ``_``
  arm; a statement match over them may end without one, and then no arm runs.

A guarded arm covers nothing on its own, since its guard can fail, but it can still be unreachable.
Arms with the same pattern whose guards compare the same two values - ``int``, enumeration or
``string`` operands that are bound names, variables, constants or fields of variables - cover that pattern
once their comparisons together cover every outcome; so do a ``bool`` value and its negation.
Floats do not combine (with a NaN no comparison holds), and neither do guards that call a function:

.. code-block:: das

    def compare ( a, b : int ) {
        return match ( a, b ) {
            (x, y) && x > y => "greater"
            (x, y) && x <= y => "not greater"
        }
    }

When the arms cover every value, a value match needs no ``_``:

.. code-block:: das

    enum Light {
        Red
        Yellow
        Green
    }

    def can_go ( light : Light ) {
        return match ( light ) {
            Light.Green => true
            Light.Red | Light.Yellow => false
        }
    }

The value being matched is evaluated once, whatever the number of arms. A bound name is a read-only
copy of the part it names; to change the matched value, assign to it directly. A part whose type does
not copy (an array, a table, a variant holding one) is not copied: the name reads that part itself.

Binding names
-------------

A bare name in a pattern binds the matched value to a new variable, visible in the guard and the
body. It never compares against an existing variable of the same name - it shadows it. ``_`` matches
anything and binds nothing. Any other expression that is not a pattern - a field read, a call - is
an error; to compare against the value of an existing variable or a computed value, wrap it in
``match_expr`` (see `Match Expressions`_):

.. code-block:: das

    def at_limit ( x, limit : int ) {
        return match ( x ) {
            match_expr(limit) => "at limit"
            other => "{other} is not {limit}"
        }
    }

Enumeration Matching
--------------------

Enumeration values match by their qualified name:

.. code-block:: das

    enum Color {
        Black
        Red
        Green
        Blue
    }

    def enum_match ( color : Color ) {
        return match ( color ) {
            Color.Black => 0
            Color.Red => 1
            _ => -1
        }
    }

Matching Variants
-----------------

A variant pattern is spelled like the variant constructor; the field value is itself a pattern:

.. code-block:: das

    variant IF {
        i : int
        f : float
    }

    def variant_match ( v : IF ) {
        return match ( v ) {
            IF(i = 0) => "int zero"
            IF(i = n) => "int {n}"
            IF(f = x) => "float {x}"
        }
    }

The ``as`` form tests the alternative by name, with the pattern on its left:

.. code-block:: das

    def variant_as_match ( v : IF ) {
        return match ( v ) {
            as_int as i => as_int
            as_float as f => int(as_float)
        }
    }

Matching Structs
----------------

Structs match by field values, binding the fields given a name:

.. code-block:: das

    struct Foo {
        a : int
    }

    def struct_match ( f : Foo ) {
        return match ( f ) {
            Foo(a = 13) => 0
            Foo(a = anyA) => anyA
        }
    }

The first arm matches only when ``a`` is 13. The second matches any ``Foo`` and binds ``a`` to ``anyA``.
A pointer to a struct matches the same way; a ``null`` pointer fails every struct pattern, and a
``null`` arm matches it explicitly.

Using Guards
------------

A guard is an extra condition after ``&&``; the arm matches only when the pattern matches and the
guard is true. The guard sees the names the pattern binds:

.. code-block:: das

    struct AB {
        a, b : int
    }

    def guards_match ( ab : AB ) {
        return match ( ab ) {
            AB(a = a, b = b) && b > a => "{b} > {a}"
            AB(a = a, b = b) => "{b} <= {a}"
        }
    }

Alternatives with ``|``
-----------------------

``p1 | p2`` matches when either pattern matches. Both sides must bind the same names to the same
fields:

.. code-block:: das

    struct Bar {
        a : int
        b : float
    }

    def or_match ( B : Bar ) {
        return match ( B ) {
            Bar(a = 1 | 2, b = b) => b
            _ => 0.0
        }
    }

Inside a guard and inside ``match_expr``, ``|`` stays the bitwise operator.

Range patterns
--------------

``(lo..hi)`` matches ``lo <= value && value < hi`` - half-open, the same as a ``for`` range, so
``(0..10)`` is 0 to 9. The parentheses are required, except inside a tuple pattern; the bounds are
constants or variables in scope, and the value must have the range's element type (``int`` for
``range``, ``uint`` for ``urange``, and the 64-bit types for ``range64`` and ``urange64``). Coverage
cuts the integers at the ends of constant ranges: a value or a range that earlier constant ranges
already cover is an unreachable arm, and so is the overlapping part of a range. A range with a
variable bound names no values, so for coverage it counts like a guard:

.. code-block:: das

    def size_class ( n : int ) {
        return match ( n ) {
            (0..10) => "digit"
            (10..100) | (1000..2000) => "some"
            _ => "other"
        }
    }

Both patterns with ``&``
------------------------

``p1 & p2`` matches when both patterns match the same value. With a bare name on one side it binds
the whole value while the other side destructures it:

.. code-block:: das

    def on_axis ( p : AB ) {
        return match ( p ) {
            whole & AB(a = 0) => "on the b axis: {whole.b}"
            _ => "off axis"
        }
    }


Matching Several Values
-----------------------

``match ( a, b )`` matches several values at once. Each arm is a tuple-shaped pattern with one
element per value, and ``_`` alone matches every combination:

.. code-block:: das

    def quadrant ( x, y : int ) {
        return match ( x, y ) {
            (0, 0) => "origin"
            (0, _) | (_, 0) => "on an axis"
            (a, b) && a > 0 && b > 0 => "first"
            _ => "elsewhere"
        }
    }

The values need not share a type; no tuple is built, each element pattern matches its value directly.

Tuple Matching
--------------

Tuples are matched element by element. The ``...`` pattern matches any number of elements:

.. code-block:: das

    def tuple_match ( A : tuple<int;float;string> ) {
        return match ( A ) {
            (1, _, "3") => 1
            (13, ...) => 2          // starts with 13
            (..., "13") => 3        // ends with "13"
            (2, ..., "2") => 4      // starts with 2, ends with "2"
            _ => 0
        }
    }

The ``_`` matches any single element; ``...`` matches zero or more elements.

Matching Static Arrays
----------------------

Static arrays use the ``fixed_array`` pattern and support the same wildcard and guard patterns:

.. code-block:: das

    def static_array_match ( A : int[3] ) {
        return match ( A ) {
            fixed_array(a, b, c) && a + b + c == 6 => 1   // total of 3 elements, sum is 6
            fixed_array(0, ...) => 0                      // starts with 0
            fixed_array(..., 13) => 2                     // ends with 13
            fixed_array(12, ..., 12) => 3                 // starts and ends with 12
            _ => -1
        }
    }

Dynamic Array Matching
----------------------

Dynamic arrays use bracket syntax and support the same patterns as tuples and static arrays.
The number of explicit elements in the pattern is checked against the array length:

.. code-block:: das

    def dynamic_array_match ( A : array<int> ) {
        return match ( A ) {
            [a, b, c] && a + b + c == 6 => 1   // total of 3 elements, sum is 6
            [0, 0, 0, ...] => 0                // first 3 are 0
            [..., 1, 2] => 2                   // ends with 1,2
            [0, 1, ..., 2, 3] => 3             // starts with 0,1, ends with 2,3
            _ => -1
        }
    }

Match Expressions
-----------------

The ``match_expr`` pattern matches when the value equals an expression, which may use the names bound
earlier in the same pattern:

.. code-block:: das

    def ascending_array_match ( A : int[3] ) {
        return match ( A ) {
            fixed_array(x, match_expr(x + 1), match_expr(x + 2)) => true
            _ => false
        }
    }

Here the first element is bound to ``x``, and the second and third elements must equal ``x+1`` and ``x+2`` respectively.

[match_as_is] Structure Annotation
-----------------------------------

The ``[match_as_is]`` annotation enables pattern matching for structures of different types,
provided the necessary ``is`` and ``as`` operators have been implemented:

.. das-doc: given struct Cmd { rtti : string }
.. code-block:: das

    [match_as_is]
    struct CmdMove : Cmd {
        override rtti = "CmdMove"
        x : float
        y : float
    }

The required ``is`` and ``as`` operators:

.. code-block:: das

    def operator is CmdMove ( cmd:Cmd ) {
        return cmd.rtti=="CmdMove"
    }

    def operator is CmdMove ( anything ) {
        return false
    }

    def operator as CmdMove ( cmd:Cmd ==const ) : CmdMove const& {
        assert(cmd.rtti=="CmdMove")
        unsafe {
            return reinterpret<CmdMove const&>(cmd)
        }
    }

    def operator as CmdMove ( var cmd:Cmd ==const ) : CmdMove& {
        assert(cmd.rtti=="CmdMove")
        unsafe {
            return reinterpret<CmdMove&>(cmd)
        }
    }

    def operator as CmdMove ( anything ) {
        panic("Cannot cast to CmdMove")
        return default<CmdMove>
    }

With these operators in place, you can match against ``CmdMove``:

.. code-block:: das

    def matching_as_and_is ( cmd : Cmd ) {
        return match ( cmd ) {
            CmdMove(x = x, y = y) => x + y
            _ => 0.
        }
    }

[match_copy] Structure Annotation
----------------------------------

The ``[match_copy]`` annotation provides an alternative to ``[match_as_is]`` by using a ``match_copy`` function
instead of ``is``/``as`` operators. The struct also needs ``safe_when_uninitialized``, because the match macro
declares an uninitialized working instance of the type:

.. code-block:: das

    [match_copy, safe_when_uninitialized]
    struct CmdLocate : Cmd {
        override rtti = "CmdLocate"
        x : float
        y : float
        z : float
    }

The ``match_copy`` function attempts to copy the source into the target type, returning ``true`` on success:

.. code-block:: das

    def match_copy ( var cmdm:CmdLocate; cmd:Cmd ) {
        if ( cmd.rtti != "CmdLocate" ) {
            return false
        }
        unsafe {
            cmdm = reinterpret<CmdLocate const&>(cmd)
        }
        return true
    }

Usage is identical to regular struct matching:

.. code-block:: das

    def matching_copy ( cmd : Cmd ) {
        return match ( cmd ) {
            CmdLocate(x = x, y = y, z = z) => x + y + z
            _ => 0.
        }
    }

Matching AST Nodes
------------------

``match`` decomposes compiler AST nodes (``ExpressionPtr``, ``TypeDeclPtr``, and other
handled-type pointers) directly: the pattern names the node type, fields compare or
bind, and nested node patterns recurse:

.. code-block:: das

    require daslib/ast_boost

    def classify ( e : ExpressionPtr ) {
        return match ( e ) {
            ExprOp2(op = "+", left = ExprOp2(op = "*", left = a, right = b), right = c) => "mad shape"
            ExprOp2(op = "+" | "-") => "additive"
            ExprSwizzle(mask = "xy", value = v) => "xy swizzle"
            _ => "other"
        }
    }

A null pointer fails node patterns cleanly, so no explicit ``null`` arm is
required (one may still be used to handle null specifically). ``das_string`` fields
(operator names, identifiers, swizzle masks) compare directly against string literals.
Node-type checks are exact — ``ExprField`` does not match ``ExprSafeField``.

Static Matching
---------------

``static_match`` works like ``match``, but ignores patterns with type mismatches at compile time instead
of reporting errors. This makes it suitable for generic functions:

.. code-block:: das

    enum Shade {
        light
        dark
    }

    def enum_static_match ( shade, blah ) {
        return static_match ( shade ) {
            Shade.light => 0
            match_expr(blah) => 1
            _ => -1
        }
    }

If ``shade`` is not ``Shade``, the first arm is silently skipped. If ``blah`` is not ``Shade``, the second arm is skipped.
The function always compiles regardless of the argument types.

match_type
----------

The ``match_type(type<T>, pattern)`` subpattern matches when the value has type ``T``, then matches
``pattern`` against it:

.. code-block:: das

    def static_match_by_type ( what ) {
        return static_match ( what ) {
            match_type(type<int>, expr) => expr
            _ => -1
        }
    }

If ``what`` is of type ``int``, it is bound to ``expr`` and returned. Otherwise the catch-all returns ``-1``.


The ``if ( pattern )`` arm form
-------------------------------

Earlier code spells an arm ``if ( pattern ) { body }`` and binds names with ``$v(name)``; in that
form a bare name compares against the variable instead of binding. It still compiles, but a match
block takes one form or the other, never both, and the ``pattern => body`` form is the one to write.

.. seealso::

    :ref:`Variants <variants>` for the variant type used in ``as`` matching,
    :ref:`Enumerations <enumerations>` for enumeration types used in enum matching,
    :ref:`Tuples <tuples>` for tuple types matched by index,
    :ref:`Structs <structs>` for struct types matched by field,
    :ref:`Arrays <arrays>` for dynamic array matching,
    :ref:`Classes <classes>` for the ``[match_as_is]`` annotation,
    :ref:`Expressions <expressions>` for ``is``, ``as``, and ``?as`` operators.
