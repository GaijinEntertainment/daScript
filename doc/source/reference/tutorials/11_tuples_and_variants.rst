.. _tutorial_tuples_and_variants:

========================
Tuples and Variants
========================

.. index::
    single: Tutorial; Tuples
    single: Tutorial; Variants
    single: Tutorial; Union Types

This tutorial covers tuples (anonymous and named), tuple destructuring,
variants (tagged unions), and type-safe variant access with ``is``, ``as``,
and ``?as``.

Tuples
======

A tuple groups values of different types into a single value.
Access fields by index::

  var pair = (42, "hello")
  print("{pair._0}, {pair._1}\n")    // 42, hello

Use ``tuple()`` for explicit construction::

  var t = tuple(1, 2.0, "three")

Named tuples
============

Give a tuple named fields for readability::

  tuple Point2D {
      x : float
      y : float
  }

  var p = Point2D(x=3.0, y=4.0)
  print("{p.x}, {p.y}\n")

Destructuring
=============

Unpack a tuple into individual variables::

  var (a, b, c) = tuple(10, 20.0, "thirty")

This also works in ``for`` loops::

  var pairs <- [tuple(1, "one"), tuple(2, "two"), tuple(3, "three")]
  for ((num, name) in pairs) {
      print("{num}: {name}\n")
  }

Variants
========

A variant holds exactly one of several typed alternatives — a tagged union::

  variant Value {
      i : int
      f : float
      s : string
  }

  var v : Value = Value(i = 42)

Checking the active case
=========================

Use ``is`` to test which alternative is active::

  if (v is i) {
      print("it's an int\n")
  }

Use ``as`` to extract the value (panics if wrong)::

  let n = v as i       // 42

Use ``?as`` for safe access with a fallback::

  let maybe_f = v ?as f ?? 0.0

The ``variant_index()`` function returns the zero-based index of the active case.

.. seealso::

   :ref:`Tuples <tuples>`, :ref:`Variants <variants>` in the language reference.

   Full source: ``tutorials/language/11_tuples_and_variants.das``

Next tutorial: :ref:`tutorial_function_pointers`
