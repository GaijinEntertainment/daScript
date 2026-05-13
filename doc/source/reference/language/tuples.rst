.. _tuples:

=====
Tuple
=====

Tuples are a concise syntax to create anonymous data structures.
A tuple type is declared with the ``tuple`` keyword followed by a list of element types
(optionally named) in angle brackets:

.. code-block:: das

    tuple<int; float>               // unnamed elements
    tuple<i:int; f:float>           // named elements

Tuple field names are part of the type. Two tuple declarations are the same only if
they have the same number of elements, the same element types, **and the same
field names** (in the same positions). An unnamed tuple is **not** assignable to a
named tuple, and a named tuple is **not** assignable to a tuple with different
names — even when the element types match:

.. code-block:: das

    var a : tuple<int; float>
    var b : tuple<i:int; f:float>
    var c : tuple<x:int; y:float>
    // a = b   // error: tuple<int;float> is not the same type as tuple<i:int;f:float>
    // b = c   // error: tuple<i:int;f:float> is not the same type as tuple<x:int;y:float>
    var d : tuple<i:int; f:float>
    b = d      // ok — same names, same types

The same rule applies to construction: a bare positional literal ``(1, 2.0)``
produces an unnamed ``tuple<int;float>`` and is not accepted where a named
tuple type is expected. Use the named-field literal form to construct a named
tuple directly:

.. code-block:: das

    var b : tuple<i:int; f:float> = (i = 1, f = 2.0)   // ok
    // var b : tuple<i:int; f:float> = (1, 2.0)         // error: not the same type

Mixing named and positional fields in the same literal is **not** supported —
either every field is named or none are.

Shorthand promotion
-------------------

When every element of a positional tuple literal is a bare variable reference,
the compiler may promote it to a named tuple by taking the field names from the
variables. This only fires when the target type is unambiguously a named tuple
and the variable names match the field names in order:

.. code-block:: das

    let eid = 7
    let distSq = 2.5
    var t : tuple<eid:int; distSq:float> = (eid, distSq)   // ok, promoted
    var arr : array<tuple<eid:int; distSq:float>>
    arr |> push((eid, distSq))                              // ok, promoted

Promotion is a fallback: if a matching overload already exists for the unnamed
tuple type, that overload wins and no promotion happens. If you want the named
overload, use the explicit named-field literal:

.. code-block:: das

    def overload_pick(x : tuple<int; float>) { return 1 }
    def overload_pick(x : tuple<x:int; y:float>) { return 2 }
    let x = 1
    let y = 2.0
    overload_pick((x, y))          // returns 1: unnamed overload wins
    overload_pick((x=x, y=y))      // returns 2: explicit named literal

A name mismatch fails compilation rather than silently constructing the
unnamed tuple:

.. code-block:: das

    let foo = 1
    let bar = 1.1
    var arr : array<tuple<eid:int; distSq:float>>
    // arr |> push((foo, bar))   // error: function_not_found

Promotion does not fire when any element is not a bare variable reference, e.g.
``(a, a+1)`` stays unnamed.

Tuple elements can be accessed via nameless fields, i.e. _ followed by the 0 base field index:

.. code-block:: das

    a._0 = 1
    a._1 = 2.0

Named tuple elements can be accessed by name as well as via nameless field:

.. code-block:: das

    b.i = 1         // same as _0
    b.f = 2.0       // same as _1
    b._1 = 2.0      // _1 is also available

Tuples follow the same alignment rules as structures (see :ref:`Structures <structs_alignment>`).

Tuple alias types can be constructed the same way as structures. For example:

.. code-block:: das

    tuple Foo {
        a : int
        b : float
    }

It's the same as:

.. code-block:: das

    typedef Foo = tuple<a:int,b:float>

Tuples can be constructed using the tuple constructor, for example:

.. code-block:: das

    var a = (1,2.0,"3")
    var b = tuple(1, 2.0, "3")

The ``=>`` operator creates a 2-element tuple from its left and right operands:

.. code-block:: das

    var c = "one" => 1   // same as tuple<string,int>("one", 1)

This works in any expression context, not just table literals.
Table literals like ``{ "one"=>1, "two"=>2 }`` use ``=>`` to form key-value tuples
that are then inserted into the table (see :ref:`Tables <tables>`).

Tuple elements can be assigned names via tuple constructor:

.. code-block:: das

    var a = tuple<a:int,b:float,c:string>(a=1, b=2.0, c="3")

Both ``auto`` and a full type specification can be used to construct a tuple.
Array of tuples can be constructed using similar syntax, with a comma as a separator:

.. code-block:: das

    let H : array<Tup> <- array tuple<Tup>((a = 1, b = 2., c = "3"), (a = 4, b = 5., c = "6"))

Tuples can be expanded upon the variable declaration, for example:

.. code-block:: das

    var (a, b, c) = (1, 2.0, "3")

In this case only one variable is created, as well as for 'assume' expressions. I.e:

.. code-block:: das

    var a`b`c = (1, 2.0, "3")
    assume a  = a`b`c._0
    assume b  = a`b`c._1
    assume c  = a`b`c._2

Iterators and containers can be expanded in the for-loop in a similar way:

.. code-block:: das

    var H <- [(1, 2.0, "3"), (4, 5.0, "6")]
    for ( (a, b, c) in H ) {
        assert(a == 1)
        assert(b == 2.0)
        assert(c == "3")
    }

Passing tuples as arguments — const widening
============================================

When a tuple value is passed as a function argument, each pointer field
inside the tuple participates in the same const-widening rule used for
top-level pointer parameters (see :ref:`Pointers <pointer_const_argument>`).
A non-const pointer ``T?`` inside the argument tuple is accepted where the
parameter tuple has ``T const?``:

.. code-block:: das

    struct Node { at : Loc }
    var ats : array<tuple<string; Loc const?>>

    // Exact-match overload.
    def takeng(a : array<tuple<string; Loc const?>>;
               b : tuple<string; Loc const?>) { ... }

    // Generic overload — TT is inferred from the array element type,
    // so b must match tuple<string; Loc const?> as well.
    def take(a : array<auto(TT)>; b : TT) { ... }

    def feed(var a : Node?&) {
        take(ats,   ("test", unsafe(addr(a.at))))   // tuple<string; Loc?>
        takeng(ats, ("test", unsafe(addr(a.at))))   // accepted for
                                                    // tuple<string; Loc const?>
    }

The widening is one-directional (``T?`` widens to ``T const?``, not the
reverse) and applies to tuples that appear **directly** as an argument type.
Tuples nested inside containers or inside other structures are not relaxed —
their element types must match exactly.  Variants and options do **not**
participate in this relaxation.

The implementation lives in ``TypeDecl::isSameType`` in
``src/ast/ast_typedecl.cpp``: the ``isPassType`` flag is propagated into the
tuple's ``argTypes`` comparison so the pointer field inherits the same
relaxation that the top-level parameter pointer gets.

.. seealso::

    :ref:`Datatypes <datatypes_and_values>` for a list of built-in types,
    :ref:`Pattern matching <pattern-matching>` for matching and destructuring tuples,
    :ref:`Finalizers <finalizers>` for tuple finalization,
    :ref:`Move, copy, and clone <clone_to_move>` for tuple copy and move rules,
    :ref:`Aliases <aliases>` for the ``typedef`` shorthand tuple syntax,
    :ref:`Pointers <pointer_const_argument>` for the matching rule on top-level pointer arguments.

