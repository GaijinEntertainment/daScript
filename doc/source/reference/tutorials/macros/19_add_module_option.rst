.. _tutorial_macro_add_module_option:

.. index::
   single: Tutorial; Macros; add_module_option
   single: Tutorial; Macros; module options
   single: Tutorial; Macros; this_module

====================================================
 Macro Tutorial 19: Custom module options
====================================================

Most ``options`` are built into the compiler — ``gen2``,
``persistent_heap``, ``rtti``, and so on. A module can also define its
**own** option so that any file requiring it may set it, and a macro in the
module can read it to change behavior. This is the mechanism behind built-in
module options such as ``shader_like`` in ``daslib/validate_code``.

The flow has two halves:

- The module **registers** an option name and type with
  ``add_module_option``. After that, the parser accepts
  ``options <name> = ...`` in any file that requires the module.
- A pass macro in the module **reads** the flag with ``find_arg`` and acts
  on it.


Registering the option
=======================

Full source: :download:`add_module_option_mod.das <../../../../../tutorials/macros/add_module_option_mod.das>`

``add_module_option(module, name, type)`` records an option name and its
type on a module. It must run while the module's macros are being compiled,
so it lives in a ``macro_function`` guarded by
``is_compiling_macros_in_module``:

.. code-block:: das

    [_macro, macro_function]
    def register_options {
        if (is_compiling_macros_in_module("add_module_option_mod")) {
            this_module() |> add_module_option("trace_compile", Type.tBool)
        }
    }

``Type.tBool`` declares the option as boolean; ``Type.tInt`` /
``Type.tString`` register integer and string options. ``this_module()``
returns the module being compiled — the one that owns the option.


Reading the option
==================

A ``[lint_macro]`` runs once per module compiled after this one. It reads
the flag off the program options and, when set, prints a per-module note at
compile time:

.. code-block:: das

    [lint_macro]
    class TraceCompileLint : AstPassMacro {
        def override apply(prog : ProgramPtr; mod : Module?) : bool {
            let on = prog._options |> find_arg("trace_compile") ?as tBool ?? false
            if (!on) return false
            let cm = compiling_module()
            var nfun = 0
            cm |> for_each_function("") $(var func : FunctionPtr) {
                nfun++
            }
            let name = empty(cm.name) ? "<main>" : string(cm.name)
            print("[trace_compile] module '{name}' — {nfun} function(s)\n")
            return false   // lint passes never modify the AST
        }
    }

Key points:

- ``prog._options |> find_arg("trace_compile")`` looks the option up in the
  options of the module being linted. ``?as tBool`` extracts the boolean and
  ``?? false`` supplies the default when the option was never set.
- ``[lint_macro]`` options are **per-module**: the flag is only ``true`` for
  the module that actually set it, so the note prints once for that module.
- ``print(...)`` from a macro outputs at **compile time** — the note appears
  before any runtime output.


The usage file
==============

Full source: :download:`19_add_module_option.das <../../../../../tutorials/macros/19_add_module_option.das>`

.. code-block:: das

    require add_module_option_mod
    options trace_compile = true

    def greet(name : string) {
        print("Hello, {name}!\n")
    }

    def add(a, b : int) : int => a + b

    [export]
    def main() {
        greet("world")
        print("2 + 3 = {add(2, 3)}\n")
    }

The ``options trace_compile = true`` line is accepted only because
``add_module_option_mod`` registered the name — without the ``require``, the
parser would reject it as an unknown option. Flip the flag to ``false`` (or
delete the line) and the note vanishes; the program still compiles, because
the option name stays registered.


Output
======

.. code-block:: text

    [trace_compile] module '<main>' — 3 function(s)
    Hello, world!
    2 + 3 = 5

The first line is emitted at compile time by the lint pass; the root script
module is unnamed, so it reports as ``<main>``, with its three functions
(``greet``, ``add``, ``main``).


.. seealso::

   Full source:
   :download:`19_add_module_option.das <../../../../../tutorials/macros/19_add_module_option.das>`,
   :download:`add_module_option_mod.das <../../../../../tutorials/macros/add_module_option_mod.das>`

   Previous tutorial: :ref:`tutorial_macro_with_boost`

   Standard library: ``daslib/validate_code.das`` registers ``shader_like``
   the same way.

   Language reference: :ref:`Macros <macros>` — full macro system documentation
