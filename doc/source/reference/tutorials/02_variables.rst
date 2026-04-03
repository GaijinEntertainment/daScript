.. _tutorial_variables:

========================
Variables and Types
========================

.. index::
    single: Tutorial; Variables
    single: Tutorial; Types

This tutorial covers mutable and immutable variables, daslang's basic types,
type inference, explicit annotations, and the strict no-implicit-conversion
rule.

var vs let
==========

Use ``var`` for mutable variables and ``let`` for immutable ones::

  var score = 0
  score = 100        // OK — score is mutable

  let maxScore = 999
  // maxScore = 0    // ERROR — cannot modify a constant

Type inference
==============

The compiler infers the type from the right-hand side::

  let i = 42          // int
  let f = 3.14        // float
  let b = true        // bool
  let s = "hello"     // string

You can also state the type explicitly with ``: Type``::

  let x : int = 10
  let y : float = 2.5
  let z : double = 1.0lf     // 'lf' suffix for double literals
  let flag : bool = false

No implicit conversions
=======================

daslang is strict — you cannot mix ``int`` and ``float`` in arithmetic.
Both sides must match::

  let i = 42
  let f = 3.14
  // let bad = i + f       // ERROR: int + float
  let result = float(i) + f    // OK — explicit cast

Likewise, ``bool`` is not interchangeable with ``int``::

  // let wrong = bool(1)   // ERROR
  let right = 1 != 0       // use comparison instead

Hex literals and unsigned types
===============================

Hex literals like ``0xFF`` are ``uint`` by default.  Use ``int(0xFF)`` to get
an ``int``.  64-bit literals use the ``l`` and ``ul`` suffixes::

  let h = 0xFF                  // uint
  let hi = int(0xFF)            // int
  let bigInt : int64 = 9_000_000_000l
  let bigUint : uint64 = 18_000_000_000ul

Zero initialization
===================

Variables declared with a type but no initializer are zero-initialized::

  let zeroInt : int        // 0
  let zeroFloat : float    // 0.0
  let zeroBool : bool      // false
  let zeroString : string  // ""

Running the tutorial
====================

::

  daslang.exe tutorials/language/02_variables.das

Full source: :download:`tutorials/language/02_variables.das <../../../../tutorials/language/02_variables.das>`

See also
========

* **Next:** :ref:`tutorial_operators` — operators and expressions
* :ref:`Datatypes <datatypes_and_values>` — complete type reference
* :ref:`Constants and enumerations <enumerations>` — constant values
