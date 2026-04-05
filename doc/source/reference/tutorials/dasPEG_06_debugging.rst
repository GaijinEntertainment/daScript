.. _tutorial_dasPEG_debugging:

=======================================
PEG-06 --- Debugging and Options
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; Debugging
    single: Tutorial; Tracing
    single: Tutorial; Error Reporting
    single: Tutorial; Commit

This tutorial covers dasPEG's debugging and diagnostic tools.
You will learn:

- ``option(tracing)`` --- trace parser execution
- ``option(color)`` --- colored terminal output
- ``option(print_generated)`` --- inspect generated code
- ``log("message")`` --- inline debug messages during parsing
- How ``commit`` enables meaningful error messages
- Reading and interpreting ``ParsingError`` results
- Performance tips

Tracing
=======

Add ``option(tracing)`` inside the ``parse`` block to see which
alternatives are tried and whether they succeed:

.. code-block:: das

   parse(input) {
       var expr : string
       rule("hello", WS, "{+alpha}" as name, EOF) {
           return name
       }
       rule("bye", EOF) {
           return "goodbye"
       }
       var alpha : void?
       rule(set('a'..'z', 'A'..'Z')) {
           return null
       }
       option(tracing)
   }

The trace output shows each rule entry, match attempt, success or
failure, and backtracking --- invaluable when debugging why a grammar
does not match.

Other Options
=============

============================  =============================================
Option                        Description
============================  =============================================
``option(tracing)``           Print rule-by-rule execution trace
``option(color)``             Use ANSI colors in trace output
``option(print_generated)``   Print the generated parser code at compile time
============================  =============================================

Inline Log Messages
===================

``log("message")`` prints during parsing.  Use string interpolation to
include bound variables:

.. code-block:: das

   parse(input) {
       var expr : int
       rule(number as n, log("parsed number: {n}"),
            WS, "+", WS,
            number as m, log("parsed second: {m}"), EOF) {
           return n + m
       }
   }

Log messages fire during the **first parse pass** --- they may fire for
alternatives that later fail via backtracking.

Commit and Error Reporting
==========================

Without ``commit``, PEG silently backtracks when an alternative fails.
The error array may be empty even on parse failure:

.. code-block:: das

   // Without commit --- may produce no errors
   rule("val", WS, "=", WS, number as n, ";", EOF) {
       return n
   }

   // With commit --- produces meaningful errors
   rule("val", WS, "=", commit, WS, number as n, ";", EOF) {
       return n
   }

Place ``commit`` after an **unambiguous prefix** --- the point where
the parser knows which alternative it is in.  After commit, if the
rest of the alternative fails, a ``ParsingError`` is generated with
the position and description of the failure.

Interpreting ParsingError
=========================

Each ``ParsingError`` has two fields:

- ``text : string`` --- human-readable description of what was expected
- ``index : int`` --- byte position in the input where the error occurred

.. code-block:: das

   parse_with_commit("val = ;") $(val; err) {
       for (e in err) {
           print("position {e.index}: {e.text}\n")
           // position 6: Error: Expected a decimal number at 6
       }
   }

Performance Tips
================

1. **Place common alternatives first** --- PEG tries in order
2. **Use PEEK** to quickly reject impossible alternatives
3. **Use commit** after unambiguous prefixes for better errors and speed
4. **Set** ``options stack = 1000000`` --- PEG parsers need more stack than default
5. **Packrat memoization** --- dasPEG caches results per rule per position automatically

.. seealso::

   Full source: :download:`tutorials/dasPEG/06_debugging.das <../../../../tutorials/dasPEG/06_debugging.das>`

   Next tutorial: :ref:`tutorial_dasPEG_basic_interpreter`
