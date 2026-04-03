.. _tutorial_blocks:

========================
Blocks
========================

.. index::
    single: Tutorial; Blocks
    single: Tutorial; Callbacks
    single: Tutorial; Higher-Order Functions

This tutorial covers block declarations, passing blocks to functions,
trailing block syntax, simplified ``=>`` syntax, call syntax, and
``invoke()`` for local block variables.

Declaring a block
=================

Blocks are anonymous functions prefixed with ``$()``::

  var blk = $(a, b : int) => a + b
  print("{invoke(blk, 3, 4)}\n")    // 7

Blocks as function arguments
============================

Use ``block`` in the parameter type. Inside the function, call blocks
directly — just like regular functions::

  def do_twice(action : block) {
      action()           // call syntax — preferred
      action()
  }

With typed parameters and return::

  def apply_to(value : int; transform : block<(x:int):int>) : int {
      return transform(value)    // call syntax, not invoke
  }

Trailing block syntax
=====================

Pass a block as the last argument by placing it after the call::

  do_twice() {
      print("hello!\n")
  }

Simplified blocks
=================

When a block has a single expression body, use ``=>``::

  let result = apply_to(5, $(x) => x * x)   // 25

Call syntax vs invoke
=====================

Inside a function that receives a block parameter, use **call syntax**::

  def filter(arr : array<int>; predicate : block<(x:int):bool>) {
      for (v in arr) {
          if (predicate(v)) {      // call syntax: predicate(v)
              print("{v} ")
          }
      }
  }

For **local block variables**, use ``invoke()``::

  var blk = $(a, b : int) => a + b
  print("{invoke(blk, 3, 4)}\n")

Capture rules
=============

Blocks capture outer variables **by reference** and are stack-allocated.
They cannot be stored in containers or returned from functions — use
:ref:`lambdas <tutorial_lambdas>` for that.

.. seealso::

   :ref:`Functions <functions>` in the language reference.

   Full source: :download:`tutorials/language/13_blocks.das <../../../../tutorials/language/13_blocks.das>`

Next tutorial: :ref:`tutorial_lambdas`
