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
- How ``commit`` cuts backtracking and shapes the error list
- Reading and interpreting ``ParsingError`` results
- Performance tips

Tracing
=======

Add ``option(tracing)`` inside the ``parse`` block to see which
alternatives are tried and whether they succeed:

.. das-doc: fragment
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

.. das-doc: fragment
.. code-block:: das

   parse(input) {
       var expr : int
       rule(number as n, log("parsed number: {n}"),
            WS, "+", WS,
            number as m, log("parsed second: {m}"), EOF) {
           return n + m
       }
   }

A message prints every time the parser reaches that point, including
alternatives that fail a moment later and backtrack.  A failed parse reads
the input twice --- once to parse, once to collect the errors --- so each
message prints twice on failure.

Commit and Error Reporting
==========================

``commit`` is the cut operator.  Once the parser passes it, the rest of the
rule's alternatives are skipped: the alternative holding the ``commit``
either matches, or the whole rule fails.

.. das-doc: fragment
.. code-block:: das

   // Without commit --- both alternatives are tried
   rule("val", WS, "=", WS, number as n, ";", EOF) {
       return n
   }
   rule("val", WS, "=", WS, "?", EOF) {
       return -1
   }

   // With commit --- after "=", the second alternative is dead
   rule("val", WS, "=", commit, WS, number as n, ";", EOF) {
       return n
   }
   rule("val", WS, "=", WS, "?", EOF) {
       return -1
   }

On ``"val = ;"`` both grammars fail, and the error lists differ.  Without
commit every failing alternative adds its own ``ParsingError``, so you get
two.  With commit you get one, for the alternative the parser committed to.
The cut also costs you matches: ``"val = ?"`` parses without commit, and
fails with it, because the second alternative never runs.

So place ``commit`` after an **unambiguous prefix** --- the point where the
input can only be this alternative.

Errors raised inside a lookahead (``PEEK``, ``!``), a repetition (``*``,
``+``), or an ``MB()`` are suppressed.  A parse that fails only there
reports an empty error array.

Interpreting ParsingError
=========================

Each ``ParsingError`` has two fields:

- ``text : string`` --- human-readable description of what was expected
- ``index : int`` --- byte position in the input where the error occurred

.. das-doc: given def parse_with_commit(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {}
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
