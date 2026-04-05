.. _tutorial_dasPEG_basic_interpreter:

=======================================
PEG-07 --- C64 BASIC Interpreter
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; BASIC
    single: Tutorial; Interpreter
    single: Tutorial; FOR/NEXT
    single: Tutorial; GOTO
    single: Tutorial; GOSUB

This tutorial builds a Commodore 64-style BASIC interpreter ---
a complete parser-to-interpreter pipeline with 15+ grammar rules.
You will learn:

- Building a parser -> interpreter pipeline
- A grammar with many statement types
- Runtime expression evaluation using PEG
- Keyword disambiguation with negative lookahead
- Line-number-based program storage with ``GOTO`` / ``GOSUB``
- ``FOR`` / ``NEXT`` loops, ``IF`` / ``THEN`` branching

Prerequisites: Tutorials PEG-01 through PEG-06.

Architecture
============

The interpreter has three layers:

1. **Line parser** --- PEG grammar that parses BASIC statements into a
   ``Stmt`` variant.  Expression text is captured as strings, not
   evaluated.

2. **Expression evaluator** --- a second PEG grammar that parses and
   evaluates arithmetic expressions at runtime, with access to the
   current variable table.

3. **Interpreter loop** --- walks stored program lines, dispatching each
   statement type.

Statement Types
===============

Statements are represented as a variant:

.. code-block:: das

   variant Stmt {
       print_expr : string         // PRINT <expr>
       print_str : string          // PRINT "literal"
       print_semi : string         // PRINT <expr>;  (no newline)
       print_nl : bool             // bare PRINT
       let_stmt : tuple<string; string>    // LET var = <expr>
       if_goto : tuple<string; int>        // IF <cond> THEN linenum
       goto_stmt : int
       gosub_stmt : int
       return_stmt : bool
       for_stmt : tuple<string; string; string; string>  // var, from, to, step
       next_stmt : string
       rem_stmt : string
       end_stmt : bool
       nop : bool
   }

Expression text is stored as strings and re-parsed each time the
statement executes --- just like classic BASIC.

Keyword Disambiguation
======================

Variable names must not match keywords.  Negative lookahead (``!``)
prevents ``TO``, ``STEP``, or ``THEN`` from being parsed as identifiers:

.. code-block:: das

   var primary : double
   rule(!"TO", !"STEP", !"THEN", "{+ident_ch}" as name) {
       return eval_vars?[name] ?? 0.0lf
   }

Text Capture for Deferred Evaluation
=====================================

The line parser captures expression text **without evaluating it**.
Helper rules like ``until_then`` match everything up to a keyword:

.. code-block:: das

   var until_then : void?
   rule(!"THEN", any) {
       return null
   }

Then in the statement rule:

.. code-block:: das

   rule("IF", WS, "{+until_then}" as cond, WS, "THEN", WS, number as target) {
       return Stmt(if_goto = (cond, target))
   }

At runtime, the captured text is passed to the expression evaluator.

Demo Programs
=============

The tutorial runs four BASIC programs:

**Hello World + FOR loop:**

::

   10 PRINT "HELLO WORLD"
   20 FOR I = 1 TO 5
   30 PRINT I;
   35 PRINT " ";
   40 NEXT I
   50 PRINT
   60 END

**Countdown with IF/GOTO:**

::

   10 I = 5
   20 PRINT I
   30 I = I - 1
   40 IF I >= 0 THEN 20
   50 PRINT "LIFTOFF!"

**GOSUB/RETURN:**

::

   10 X = 5
   20 GOSUB 100
   30 X = 10
   40 GOSUB 100
   50 END
   100 REM Print X * X
   110 PRINT X * X
   120 RETURN

**Fibonacci sequence:**

::

   10 A = 0
   20 B = 1
   30 FOR I = 1 TO 10
   40 PRINT A;
   50 PRINT " ";
   60 C = A + B
   70 A = B
   80 B = C
   90 NEXT I
   100 PRINT

Output:

::

   0 1 1 2 3 5 8 13 21 34

.. seealso::

   Full source: :download:`tutorials/dasPEG/07_basic.das <../../../../tutorials/dasPEG/07_basic.das>`
