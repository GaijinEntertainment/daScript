.. _style_lint:

===========================
Style Lint (``style_lint``)
===========================

.. index::
    single: style_lint
    single: Style Lint

The ``style_lint`` module detects non-idiomatic patterns in daslang code
at compile time. When required, a lint pass runs after compilation and reports
warnings as ``CompilationError::style_lint`` (error code ``40218``).

-----------
Quick start
-----------

Add ``require daslib/style_lint`` to any file. The lint runs automatically at compile
time and reports warnings inline::

    options gen2
    require daslib/style_lint
    require strings

    def process() {
        let s = build_string() <| $(var w) {    // warning: STYLE001
            write(w, "hello")
        }
    }

-------------------
Standalone utility
-------------------

A standalone utility is available for batch-checking files from the command line::

    bin/Release/daslang.exe utils/style_lint/main.das -- file1.das file2.das [--quiet] [--postfix-conditionals]

The utility compiles each file (without simulation or execution), runs the style lint
visitor, and prints any warnings. Use ``--quiet`` to suppress progress messages.
Use ``--postfix-conditionals`` to enable the optional STYLE005 rule.

-----
Rules
-----

STYLE001 — unnecessary ``<|`` pipe before block argument
==========================================================

The ``<|`` pipe syntax is gen1 style and unnecessary in gen2. Use direct
trailing block syntax instead.

.. code-block:: das

    // Bad — gen1 pipe style
    build_string() <| $(var w) {                // STYLE001
        write(w, "hello")
    }

    // Good — gen2 trailing block
    build_string() $(var w) {
        write(w, "hello")
    }

STYLE002 — ``<|`` pipe before parameterless block
===================================================

When the block takes no parameters, both the ``<|`` pipe and ``$()`` are
unnecessary. Use a direct trailing block.

.. code-block:: das

    // Bad — pipe and $() both unnecessary
    takes_block() <| $() {                      // STYLE002
        print("done\n")
    }

    // Good — direct block
    takes_block() {
        print("done\n")
    }

STYLE003 — redundant ``$()`` on parameterless block
=====================================================

When a block takes no parameters, the ``$()`` prefix is unnecessary even
without a pipe. Use a bare trailing block.

.. code-block:: das

    // Bad — redundant $()
    takes_block() $() {                         // STYLE003
        print("done\n")
    }

    // Good — bare block
    takes_block() {
        print("done\n")
    }

STYLE005 — single-statement ``if`` can use postfix form (configurable)
========================================================================

.. note::

    This rule is **off by default**. Enable with ``--postfix-conditionals``
    in the standalone utility.

A single-statement ``if`` without an ``else`` branch that contains only
``return``, ``break``, or ``continue`` can be written more concisely as a
postfix conditional.

.. code-block:: das

    // Suggestion — can use postfix form
    if (x > 0) {
        return x                                // STYLE005
    }
    return -x

    // Postfix form
    return x if (x > 0)
    return -x

STYLE006 — ``string(__rtti)`` comparison should use ``is``
============================================================

Comparing ``string(expr.__rtti) == "ExprFoo"`` is verbose and fragile.
Use the ``is`` operator instead, which is type-safe and cleaner.

.. code-block:: das

    // Bad — manual RTTI string comparison
    if (string(expr.__rtti) == "ExprReturn") { ... }    // STYLE006

    // Good — is operator
    if (expr is ExprReturn) { ... }

STYLE010 — ``if (true)`` should be a bare block
=================================================

``if (true)`` is always taken and adds unnecessary noise. Use a bare
block (lexical scope) instead.

.. code-block:: das

    // Bad — always true
    if (true) {                                 // STYLE010
        print("always\n")
    }

    // Good — bare block
    {
        print("always\n")
    }

------------------------------
Suppressing specific warnings
------------------------------

To suppress a warning on a specific line, add a ``// nolint:STYLExxx`` comment
on the same line as the flagged expression::

    build_string() <| $(var w) {  // nolint:STYLE001 intentional pipe for readability
        write(w, "hello")
    }

The suppression is exact: ``// nolint:STYLE001`` only suppresses STYLE001, not other
rules. The comment must appear after ``//`` on the same line that triggers the warning.

-----
Tests
-----

Tests are in ``utils/style_lint/tests/``::

    bin/Release/daslang.exe utils/style_lint/main.das -- utils/style_lint/tests/

.. seealso::

    ``daslib/style_lint.das`` (source),
    ``utils/style_lint/main.das`` (standalone utility),
    :ref:`perf_lint` (performance lint rules)
