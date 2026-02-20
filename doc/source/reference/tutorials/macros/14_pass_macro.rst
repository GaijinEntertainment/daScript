.. _tutorial_macro_pass_macro:

.. index::
   single: Tutorial; Macros; Pass Macro
   single: Tutorial; Macros; pass_macro
   single: Tutorial; Macros; AstPassMacro
   single: Tutorial; Macros; lint_macro
   single: Tutorial; Macros; infer_macro

====================================================
 Macro Tutorial 14: Pass Macro
====================================================

Previous tutorials showed macros attached to specific language
constructs — calls, functions, structures, blocks, loops, enums, and
typeinfo expressions.  **Pass macros** operate at a higher level: they
receive the **entire program** and run during a specific compilation
phase.

``AstPassMacro`` is the base class for all pass macros.  It has a
single method:

``apply(prog : ProgramPtr; mod : Module?) → bool``
   ``prog`` is the full program being compiled.
   ``mod`` is the module that registered the macro.
   The return value depends on the annotation (see below).

Five annotations control **when** the macro runs:

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Annotation
     - Behaviour
   * - ``[infer_macro]``
     - Runs after clean **type inference**.  Returning ``true`` means
       "I changed something" — the compiler re-infers and runs macros
       again.  The loop repeats until every macro returns ``false``.
   * - ``[dirty_infer_macro]``
     - Runs during each **dirty inference** pass (the AST may be
       half-resolved).  All dirty macros fire on every pass.
   * - ``[lint_macro]``
     - Runs after successful compilation in the **lint phase**.
       Read-only — use it for analysis and diagnostics.  Only runs for
       modules that directly require the macro module.
   * - ``[global_lint_macro]``
     - Same as ``[lint_macro]`` but runs for **all** modules, not just
       the one that requires it.
   * - ``[optimization_macro]``
     - Runs during the **optimization** loop, after built-in
       optimisations.  Returning ``true`` continues the loop.

This tutorial demonstrates the two most common types:

- **Section 1** — ``[lint_macro]``: compile-time analysis
  (``CodeStatsLint``).
- **Section 2** — ``[infer_macro]``: AST transformation
  (``TraceCallsPass``).


The module file
===============

Full source: :download:`pass_macro_mod.das <../../../../../tutorials/macros/pass_macro_mod.das>`

Both macros live in a single ``shared`` module that the user requires.


Section 1 — lint_macro (compile-time analysis)
----------------------------------------------

.. code-block:: das

    [lint_macro]
    class CodeStatsLint : AstPassMacro {
        def override apply(prog : ProgramPtr; mod : Module?) : bool {
            var nFunctions = 0
            for_each_function(mod, "") <| $(func) {
                nFunctions++
            }
            print("[lint] pass_macro_mod defines {nFunctions} function(s)\n")
            return true
        }
    }

Key points:

- ``[lint_macro]`` means this class runs **after inference succeeds**,
  during the read-only lint phase.
- ``mod`` is the module that owns the macro — here, ``pass_macro_mod``.
  ``for_each_function(mod, "")`` iterates the module's functions.
- ``print(...)`` outputs at **compile time** — the message appears
  before any runtime output.
- The return value of a lint macro is ignored; lint macros never trigger
  re-inference.
- Real-world lint macros typically create an ``AstVisitor``, walk the
  AST with ``visit(prog, adapter)`` or ``visit(func, adapter)``, and
  report errors with ``compiling_program() |> macro_error(at, text)``.
  See ``daslib/lint.das`` for a full example.


Section 2 — infer_macro (AST transformation)
---------------------------------------------

The infer macro instruments every function in the program by inserting a
``_trace_enter("function_name")`` call at the start of each function
body.  It follows the same visitor pattern as ``daslib/heartbeat.das``.

First, a helper function that the injected code will call:

.. code-block:: das

    def public _trace_enter(name : string) {
        print(">>> {name}\n")
    }

The visitor walks the AST and modifies function bodies:

.. code-block:: das

    class TraceCallsVisitor : AstVisitor {
        astChanged : bool = false
        @do_not_delete func : Function?
        def override preVisitFunction(var fun : FunctionPtr) {
            func = get_ptr(fun)
        }
        def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
            // Skip our own helper to avoid infinite recursion at runtime.
            if (string(fun.name) == "_trace_enter") {
                func = null
                return <- fun
            }
            if (fun.body == null || !(fun.body is ExprBlock)) {
                func = null
                return <- fun
            }
            var body = fun.body as ExprBlock
            if (length(body.list) == 0) {
                func = null
                return <- fun
            }
            // Idempotency: skip if already instrumented.
            if ((body.list[0] is ExprCall) &&
                (body.list[0] as ExprCall).name == "_trace_enter") {
                func = null
                return <- fun
            }
            // Insert _trace_enter("function_name") at the beginning.
            let fname = string(fun.name)
            var inscope expr <- qmacro(_trace_enter($v(fname)))
            body.list |> emplace(expr, 0)
            astChanged = true
            func.not_inferred()
            func = null
            return <- fun
        }
    }

Key visitor techniques:

- **``preVisitFunction``** captures a raw pointer (``@do_not_delete
  func : Function?``) to the function being visited.
- **``visitFunction``** fires *after* the function body has been visited.
  It returns a ``FunctionPtr`` — returning ``<- fun`` keeps the
  original unchanged.
- **Self-exclusion** — ``_trace_enter`` must not instrument itself, or
  it would cause infinite recursion at runtime.
- **Idempotency** — checking whether the first statement is already a
  ``_trace_enter`` call prevents the macro from modifying the
  same function again on re-inference.
- **``qmacro(_trace_enter($v(fname)))``** generates a call expression.
  ``$v(fname)`` splices the string value of ``fname`` as a constant.
- **``body.list |> emplace(expr, 0)``** inserts the expression at
  position 0 (the start of the function body).
- **``func.not_inferred()``** marks the function as needing
  re-inference, so the compiler processes the injected call.
- **``astChanged = true``** signals that the pass made modifications.

The pass macro creates the visitor and walks the full program:

.. code-block:: das

    [infer_macro]
    class TraceCallsPass : AstPassMacro {
        def override apply(prog : ProgramPtr; mod : Module?) : bool {
            var astVisitor = new TraceCallsVisitor()
            var inscope astVisitorAdapter <- make_visitor(*astVisitor)
            visit(prog, astVisitorAdapter)
            var result = astVisitor.astChanged
            unsafe {
                delete astVisitor
            }
            return result
        }
    }

Key points:

- **``new TraceCallsVisitor()``** allocates the visitor on the heap
  (macro code cannot use stack-allocated visitors).
- **``make_visitor(*astVisitor)``** wraps it in a ``smart_ptr``
  adapter for the ``visit`` function.
- **``visit(prog, astVisitorAdapter)``** walks the entire program — all
  modules, all functions.  Use ``visit(func, adapter)`` to walk a
  single function instead.
- **Return value** — ``true`` tells the compiler to re-infer.  Since
  the idempotency check prevents double-instrumentation, the second
  pass returns ``false`` and inference stabilises.
- **``unsafe { delete astVisitor }``** cleans up the raw pointer.  The
  smart-ptr adapter is destroyed by ``var inscope``.


The usage file
==============

Full source: :download:`14_pass_macro.das <../../../../../tutorials/macros/14_pass_macro.das>`

.. code-block:: das

    require pass_macro_mod

    def greet(name : string) {
        print("Hello, {name}!\n")
    }

    def sum_to(n : int) : int {
        var total = 0
        for (i in range(n)) {
            total += i
        }
        return total
    }

    def countdown(n : int) {
        var i = n
        while (i > 0) {
            i--
        }
        print("counted down from {n}\n")
    }

    [export]
    def main() {
        greet("world")
        let s = sum_to(5)
        print("sum = {s}\n")
        countdown(3)
    }

The user code contains no trace calls — the ``[infer_macro]`` injects
them automatically.  The ``[lint_macro]`` prints its compile-time
message before any runtime output.


Output
======

.. code-block:: text

    [lint] pass_macro_mod defines 4 function(s)
    >>> main
    >>> greet
    Hello, world!
    >>> sum_to
    sum = 10
    >>> countdown
    counted down from 3

The first line is the lint macro's compile-time message.  The ``>>>``
lines come from the infer macro's injected ``_trace_enter`` calls,
proving that every user function was instrumented at compile time.


How it works — compilation pipeline
====================================

When the compiler processes the user's program:

1. **Parsing** — the source is parsed into an AST.
2. **Dirty inference** — ``[dirty_infer_macro]`` macros run on each
   pass (not used in this tutorial).
3. **Clean inference** — type inference runs.  After it succeeds,
   ``[infer_macro]`` macros execute.  ``TraceCallsPass`` instruments
   functions and returns ``true``.  The compiler re-infers.  On the
   second pass, no new changes are made, so it returns ``false``.
4. **Optimisation** — ``[optimization_macro]`` macros run in the
   optimisation loop (not used here).
5. **Lint** — ``[lint_macro]`` and ``[global_lint_macro]`` macros run.
   ``CodeStatsLint`` prints its summary.
6. **Execution** — the instrumented program runs.


Avoiding infinite loops
=======================

An infer macro that always returns ``true`` will cause the compiler to
hit its maximum pass limit and report an error.  Two techniques prevent
this:

- **Idempotency check** — before modifying a function, verify that the
  modification hasn't already been applied (e.g. check whether the
  first statement is already the injected call).
- **Targeted modification** — only modify functions that match specific
  criteria (e.g. skip ``_trace_enter`` to avoid self-instrumentation).


Real-world examples
===================

The standard library uses pass macros for several purposes:

- **``daslib/heartbeat.das``** — ``[infer_macro]`` that inserts a
  ``heartbeat()`` call at every function and loop entry, enabling
  cooperative multitasking for long-running scripts.
- **``daslib/coverage.das``** — ``[infer_macro]`` that instruments every
  block with coverage-tracking calls.
- **``daslib/lint.das``** — ``[lint_macro]`` that enables paranoid
  compilation checks (unreachable code, unused variables, const
  upgrades, etc.).
- **``daslib/lint_everything.das``** — ``[global_lint_macro]`` that
  applies paranoid checks to **all** modules, not just the one that
  requires it.


.. seealso::

   Full source:
   :download:`14_pass_macro.das <../../../../../tutorials/macros/14_pass_macro.das>`,
   :download:`pass_macro_mod.das <../../../../../tutorials/macros/pass_macro_mod.das>`

   Previous tutorial: :ref:`tutorial_macro_enumeration_macro`

   Standard library: ``daslib/heartbeat.das``, ``daslib/coverage.das``,
   ``daslib/lint.das``

   Language reference: :ref:`Macros <macros>` — full macro system documentation
