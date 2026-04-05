.. _tutorial_dasPEG_calculator:

=======================================
PEG-02 --- Arithmetic Calculator
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; Calculator
    single: Tutorial; Operator Precedence
    single: Tutorial; Recursion
    single: Tutorial; Commit

This tutorial builds an arithmetic calculator that handles operator
precedence, parentheses, and unary negation.  You will learn:

- Encoding operator precedence via grammar layering
- The ``number`` built-in terminal
- ``PEEK`` for lookahead
- ``commit`` (cut) for error reporting
- Recursive rules

Precedence via Grammar Layering
===============================

The classic PEG technique encodes precedence by nesting rules ---
lower-precedence operators at the top, higher at the bottom:

::

   expr  ->  add       (lowest: + -)
   add   ->  mul       (* /)
   mul   ->  unary     (unary -)
   unary ->  prim      (highest: numbers, parens)

Each level tries its operator, then falls through to the next level.

The Grammar
===========

.. code-block:: das

   def calc(input : string;
            blk : block<(val : int; err : array<ParsingError>) : void>) {
       parse(input) {
           var expr : int
           rule(add as a, WS, EOF) {
               return a
           }

           var add : int
           rule(add as a, WS, "+", commit, WS, mul as m) {
               return a + m
           }
           rule(add as a, WS, "-", commit, WS, mul as m) {
               return a - m
           }
           rule(mul as m) {
               return m
           }

           var mul : int
           rule(mul as m, WS, "*", commit, WS, unary as u) {
               return m * u
           }
           rule(mul as m, WS, "/", commit, WS, unary as u) {
               return m / u
           }
           rule(unary as u) {
               return u
           }

           var unary : int
           rule("-", commit, WS, unary as u) {
               return -u
           }
           rule(prim as p) {
               return p
           }

           var prim : int
           rule("(", commit, WS, add as a, WS, ")") {
               return a
           }
           rule(PEEK(set('0'..'9')), commit, number as n) {
               return n
           }
       }
   }

Left Recursion
==============

PEG does not natively support left recursion, but dasPEG handles it
via packrat memoization.  ``add as a`` in the first ``add`` alternative
refers to itself --- the parser memoizes intermediate results to avoid
infinite loops.

Commit (Cut)
============

``commit`` tells the parser "we are committed to this alternative --- do
not backtrack."  This serves two purposes:

1. **Better errors** --- without commit, a failed match silently
   backtracks and the error array may be empty.  After commit, a
   failure produces a meaningful ``ParsingError``.

2. **Performance** --- the parser skips remaining alternatives once
   committed.

Place ``commit`` after an unambiguous prefix.  For example, after
matching ``"+"`` in an addition rule, the parser is committed ---
if the right-hand operand fails, that is a real error.

PEEK (Lookahead)
================

``PEEK(rule)`` checks whether ``rule`` would match **without consuming
input**.  In the calculator, ``PEEK(set('0'..'9'))`` verifies the next
character is a digit before committing to the ``number`` terminal:

.. code-block:: das

   rule(PEEK(set('0'..'9')), commit, number as n) {
       return n
   }

This prevents the parser from committing to the number alternative when
the input does not start with a digit.

Examples
========

.. code-block:: das

   calc("2 + 3 * 4") $(val; err) {
       print("{val}\n")   // 14 (not 20 --- multiplication binds tighter)
   }

   calc("(2 + 3) * 4") $(val; err) {
       print("{val}\n")   // 20
   }

   calc("-5 + 3") $(val; err) {
       print("{val}\n")   // -2
   }

.. seealso::

   Full source: :download:`tutorials/dasPEG/02_calculator.das <../../../../tutorials/dasPEG/02_calculator.das>`

   Next tutorial: :ref:`tutorial_dasPEG_csv_parser`

   Related: :ref:`tutorial_dasPEG_debugging` (tracing, error reporting)
