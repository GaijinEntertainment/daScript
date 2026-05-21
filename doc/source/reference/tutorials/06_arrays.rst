.. _tutorial_arrays:

========================
Arrays
========================

.. index::
    single: Tutorial; Arrays

This tutorial covers fixed-size arrays, dynamic arrays, ``push``/``erase``,
iteration, array comprehensions, and move semantics.

Fixed-size arrays
=================

Fixed arrays are value types that live on the stack.  Use ``fixed_array`` to
create one::

  var scores = fixed_array(10, 20, 30, 40, 50)
  scores[2] = 99      // modify in-place

You can also declare with an explicit type and size::

  var grid : int[3]
  grid[0] = 1

Dynamic arrays
==============

Dynamic arrays are heap-allocated and can grow.  They use move semantics — they
cannot be copied with ``=``::

  var numbers : array<int>
  push(numbers, 10)
  push(numbers, 20)
  numbers |> push(30)     // pipe syntax

Create a dynamic array from a literal with ``<-``::

  var fruits <- ["apple", "banana", "cherry"]

Note the gen2 syntax: square brackets and commas — ``["a", "b", "c"]``.

Operations
==========

::

  erase(fruits, 1)          // remove element at index 1
  print("{length(fruits)}\n")

  resize(buf, 5)            // resize to 5 elements (zero-filled)
  clear(buf)                // remove all elements

Bulk append
===========

To append every element of one array to another, prefer the bulk
``push_from`` / ``push_clone_from`` / ``emplace_from`` overloads over a
hand-rolled element-at-a-time ``for`` loop. The bulk forms reserve the
combined capacity up front, so the allocator is touched once instead of
N times::

  var prefix <- [1, 2, 3]
  var suffix <- [4, 5]
  var combined : array<int>
  combined |> push_from(prefix)
  combined |> push_from(suffix)
  // combined: [1, 2, 3, 4, 5]

The ``PERF022`` lint rule flags the element-at-a-time shape and
recommends the bulk form. ``push_clone_from`` clones each element (deep
copy semantics) and ``emplace_from`` moves each element (consuming the
source).

Iteration
=========

Use ``for`` to iterate, and pair with ``count()`` for an index::

  for (i, color in count(), colors) {
      print("[{i}] {color}\n")
  }

Move semantics
==============

Dynamic arrays cannot be copied — they must be moved with ``<-``.
After a move the source is empty::

  var source <- [1, 2, 3]
  var dest <- source          // dest gets the data
  print("{length(source)}\n") // 0

Array comprehensions
====================

Build a new array from an expression::

  var squares <- [for (x in 0..6); x * x]
  // [0, 1, 4, 9, 16, 25]

Add a filter with ``where``::

  var evens <- [for (x in 0..10); x; where x % 2 == 0]
  // [0, 2, 4, 6, 8]

Running the tutorial
====================

::

  daslang.exe tutorials/language/06_arrays.das

Full source: :download:`tutorials/language/06_arrays.das <../../../../tutorials/language/06_arrays.das>`

See also
========

* **Next:** :ref:`tutorial_strings` — strings
* :ref:`Arrays <arrays>` — full array reference
* :ref:`Comprehensions <comprehensions>` — comprehension syntax
* :ref:`Move, copy, clone <move_copy_clone>` — ownership semantics
