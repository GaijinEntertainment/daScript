.. _tutorial_apply:

==========================================
Compile-Time Field Iteration with apply
==========================================

.. index::
    single: Tutorial; apply
    single: Tutorial; Compile-Time Iteration
    single: Tutorial; Struct Fields
    single: Tutorial; Field Annotations

This tutorial covers ``daslib/apply`` — a call macro that iterates
struct, tuple, and variant fields at compile time.  Unlike runtime
RTTI walkers, ``apply`` generates specialized code per field with
zero reflection overhead.

Prerequisites: basic daslang knowledge (structs, tuples, variants).

.. code-block:: das

    options gen2
    options rtti

    require daslib/apply
    require daslib/strings_boost


Basic struct iteration
=======================

``apply(value) $(name, field) { ... }`` visits every field of a struct.
``name`` is a compile-time string constant with the field name, and
``field`` is the field value with its concrete type:

.. code-block:: das

    struct Hero {
        name   : string
        health : int
        speed  : float
    }

    let hero = Hero(name = "Archer", health = 100, speed = 3.5)

    apply(hero) $(name, field) {
        print("  {name} = {field}\n")
    }

Output::

    name = Archer
    health = 100
    speed = 3.5


Compile-time dispatch with static_if
======================================

Because ``name`` is known at compile time, ``static_if`` can branch
on it.  Only the matching branch compiles for each field — the others
are discarded entirely:

.. code-block:: das

    struct Config {
        width      : int
        height     : int
        title      : string
        fullscreen : bool
    }

    let cfg = Config(width = 1920, height = 1080,
                     title = "My Game", fullscreen = true)

    apply(cfg) $(name, field) {
        static_if (name == "title") {
            print("  Title (special handling): \"{field}\"\n")
        } else {
            print("  {name} = {field}\n")
        }
    }

You can also dispatch on the **type** with ``typeinfo stripped_typename(field)``.


Mutating fields
================

Pass a ``var`` (mutable) variable and ``apply`` gives mutable
references to each field:

.. code-block:: das

    struct Stats {
        attack  : int
        defense : int
        magic   : int
    }

    var stats = Stats(attack = 10, defense = 5, magic = 8)

    apply(stats) $(name, field) {
        field *= 2
    }
    // stats is now Stats(attack=20, defense=10, magic=16)


Tuples
=======

``apply`` works on tuples too.  Unnamed tuple fields are named
``_0``, ``_1``, etc.  Named tuples use their declared names:

.. code-block:: das

    let pair : tuple<int; string> = (42, "hello")
    apply(pair) $(name, field) {
        print("  {name} = {field}\n")
    }
    // _0 = 42
    // _1 = hello

    let point : tuple<x : float; y : float> = (1.0, 2.0)
    apply(point) $(name, field) {
        print("  {name} = {field}\n")
    }
    // x = 1
    // y = 2


Variants
=========

For variants, ``apply`` visits **only the currently active
alternative**.  The block fires for the one alternative that is set:

.. code-block:: das

    variant Shape {
        circle   : float
        rect     : float2
        triangle : float3
    }

    let s = Shape(circle = 5.0)
    apply(s) $(name, field) {
        print("  Shape is {name}: {field}\n")
    }
    // Shape is circle: 5


Generic describe function
==========================

``apply`` is ideal for building generic utilities that work on any
struct without knowing its fields in advance:

.. code-block:: das

    def describe(value) {
        var first = true
        print("\{")
        apply(value) $(name, field) {
            if (!first) {
                print(", ")
            }
            first = false
            static_if (typeinfo stripped_typename(field) == "string") {
                print("{name}=\"{field}\"")
            } else {
                print("{name}={field}")
            }
        }
        print("\}")
    }

This prints any struct in ``{field=value, ...}`` format without
writing type-specific code.


Field annotations (3-argument form)
=====================================

Struct fields can carry metadata via ``@`` annotations:

.. code-block:: das

    struct DbRecord {
        @column="user_name"  name  : string
        @column="user_email" email : string
        @skip                id    : int
        @column="age"        age   : int
    }

Annotation syntax:

- ``@name`` — boolean (defaults to ``true``)
- ``@name=value`` — integer, float, or bare identifier (string)
- ``@name="text"`` — quoted string

The 3-argument form ``apply(value) $(name, field, annotations)``
receives annotations as ``array<tuple<name:string; data:RttiValue>>``
for each field.  ``RttiValue`` is a variant with alternatives
``tBool``, ``tInt``, ``tFloat``, ``tString``, etc.

.. code-block:: das

    apply(record) $(name : string; field; annotations) {
        var column_name = name
        var skip = false
        for (ann in annotations) {
            if (ann.name == "skip") {
                skip = true
            } elif (ann.name == "column") {
                column_name = ann.data as tString
            }
        }
        if (!skip) {
            // use column_name and field ...
        }
    }

This pattern powers ``daslib/json_boost``'s ``@rename``,
``@optional``, ``@enum_as_int``, ``@unescape``, and ``@embed``
field annotations.


Skipping a field with ``return``
=================================

A block may use ``return`` to skip the rest of the current field and move on to the next — handy in a
serializer that drops some fields:

.. code-block:: das

    apply(record) $(name : string; field; annotations) {
        for (ann in annotations) {
            if (ann.name == "skip") {
                return            // skip this field entirely
            }
        }
        // ... serialize field ...
    }

Internally this is the one case where ``apply`` does **not** inline: a block with an escaping
``return`` falls back to a generated per-field helper that the macro invokes (so ``return`` stays a
block-local "skip this field", not a function exit).  The fallback is transparent — nothing you write
changes — but it is why ``apply_imm`` (below) cannot accept such a block.


``apply`` vs ``apply_imm``
==========================

``apply`` inlines the block once per field — no helper function and no per-field block invoke — so it is
already cheap enough for hot paths like serialization.  (The sole exception is a block that uses
``return`` to skip a field, which transparently falls back to the per-field invoke codegen, as above.)
For a struct-only hot field walk there is a slightly faster sibling, ``apply_imm``: it aliases the block
parameters instead of binding reference locals, which is about 25% faster under the interpreter
(identical under JIT).  Use ``apply_imm`` for **structs** in a hot loop, and ``apply`` for tuples,
variants, a side-effecting source value, or a block that uses ``return`` — because ``apply_imm`` always
inlines, it has no invoke fallback and rejects a ``return`` block at compile time.  The block shape is
otherwise the same:

.. code-block:: das

    apply_imm(record) $(name : string; var field) {
        // same $(name, field[, annotations]) block as apply
    }


Full source
============

The complete tutorial source is in
``tutorials/language/48_apply.das``.

Run it with::

    daslang.exe tutorials/language/48_apply.das


.. seealso::

   Full source: :download:`tutorials/language/48_apply.das <../../../../tutorials/language/48_apply.das>`

   :ref:`stdlib_apply` — ``apply`` call macro reference.

   :ref:`stdlib_rtti` — ``RttiValue`` variant type used by the
   3-argument annotation form.

   :ref:`tutorial_data_walker` — runtime data walking with
   ``DapiDataWalker`` (Tutorial 47).

   Previous tutorial: :ref:`tutorial_data_walker`

   Next tutorial: :ref:`tutorial_async`
