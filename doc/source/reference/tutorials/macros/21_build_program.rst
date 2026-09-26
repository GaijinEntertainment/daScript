.. _tutorial_macro_build_program:

.. index::
   single: Tutorial; Macros; make_program
   single: Tutorial; Macros; building a program from AST nodes

=================================================
 Macro Tutorial 21: Building a program from nodes
=================================================

``compile`` and ``compile_file`` (:ref:`tutorial_compile_and_run`) turn source
text into a program. ``make_program`` skips the text. The host script builds
each module out of AST nodes, and the compiler does the rest - inference,
optimization, simulation - the same way it does for a parsed file.

Three functions from the ``ast`` module do the work:

- ``make_program(cop) $ { ... }`` opens one compile. It has its own module
  group and loads ``builtin.das`` into it, the way ``compile_file`` does.
- ``make_module(name) $(mod) { ... }`` builds a named module in the block and
  compiles it as soon as the block ends, like a ``require``\ d file. It returns
  the module, so the next modules can require it.
- ``make_main_module() $(mod) { ... }`` builds the program's own module and
  returns the finished program, ready for ``simulate``.

Nothing here is a compile-time macro. The builders are ordinary functions that
run while the host script runs.


The builders
============

The module file holds three functions that make ``Function`` nodes.
``make_greet_function`` builds ``greet`` one node at a time: a string builder,
a call to ``print``, a block, and the function with its argument.
``make_main_function`` and ``make_broken_function`` quote their bodies with
``qmacro_function``, which is shorter and gives the same kind of nodes.

A function that the host calls by name must be exported, so ``main`` sets
``flags.exports``. Without it, the program drops ``main`` as unused.

Full source: :download:`build_program_mod.das <../../../../../tutorials/macros/build_program_mod.das>`

.. das-doc: file build_program_mod.das
.. code-block:: das

    options gen2

    module build_program_mod public

    require daslib/ast_boost public
    require daslib/templates_boost public

    def public make_greet_function() : FunctionPtr {
        var greeting = new ExprStringBuilder(at = LineInfo())
        greeting.elements |> emplace_new(new ExprConstString(at = LineInfo(), value := "hello, "))
        greeting.elements |> emplace_new(new ExprVar(at = LineInfo(), name := "name"))
        greeting.elements |> emplace_new(new ExprConstString(at = LineInfo(), value := "!\n"))
        var call = new ExprCall(at = LineInfo(), name := "print")
        call.arguments |> emplace(greeting)
        var body = new ExprBlock(at = LineInfo())
        body.list |> emplace(call)
        var fn = new Function(at = LineInfo(), atDecl = LineInfo(), name := "greet",
            result = new TypeDecl(at = LineInfo(), baseType = Type.tVoid))
        fn.arguments |> emplace_new(new Variable(at = LineInfo(), name := "name",
            _type = new TypeDecl(at = LineInfo(), baseType = Type.tString)))
        fn.body = body
        return fn
    }

    def public make_main_function() : FunctionPtr {
        var fn = qmacro_function("main") $() {
            greet("world")
            print("sqrt(2) = {sqrt(2.0)}\n")
        }
        fn.flags.exports = true
        return fn
    }

    def public make_broken_function() : FunctionPtr {
        return qmacro_function("broken") $() {
            greet(42)
        }
    }


A program that runs
===================

The ``greeter`` module is compiled when its block ends, so the main module can
require it. ``math`` is a C++ module: ``get_module`` finds it, and the same
``add_module_require`` puts it in reach of the main module. ``add_function``
takes the function by reference, so each one goes into a variable first.

Full source: :download:`21_build_program.das <../../../../../tutorials/macros/21_build_program.das>`

.. code-block:: das

    options multiple_contexts

    require daslib/debugger
    require build_program_mod

    def print_errors(program : ProgramPtr) {
        for (err in program.errors) {
            print("  error: {err.what}\n")
        }
    }

    def build_and_run() {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            make_program(cop) $ {
                let greeter = make_module("greeter") $(var mod) {
                    var greet = make_greet_function()
                    mod |> add_function(greet)
                }
                var inscope program <- make_main_module() $(var mmain) {
                    mmain |> add_module_require(greeter, false)
                    mmain |> add_module_require(get_module("math"), false)
                    var main_fn = make_main_function()
                    mmain |> add_function(main_fn)
                }
                if (program.failed) {
                    print_errors(program)
                    return
                }
                simulate(program) $(sok; context; serrors) {
                    if (!sok) {
                        print("simulate error: {serrors}\n")
                        return
                    }
                    unsafe(invoke_in_context(context, "main"))
                }
            }
        }
    }

The program is simulated and run inside ``make_program``. Its modules live in
the scope's module group, and they are freed when the scope ends. A program
that is still referenced when the block returns makes ``make_program`` panic.
``threadlock_context`` is set because ``invoke_in_context`` locks the
program's context.


A program that fails to compile
===============================

``broken`` passes an ``int`` to ``greet(name : string)``. Inference of the main
module rejects it. The program comes back with ``failed`` set and its errors in
``program.errors`` - the same errors a parsed file would report.

.. code-block:: das

    def build_broken() {
        using() $(var cop : CodeOfPolicies) {
            make_program(cop) $ {
                let greeter = make_module("greeter") $(var mod) {
                    var greet = make_greet_function()
                    mod |> add_function(greet)
                }
                var inscope program <- make_main_module() $(var mmain) {
                    mmain |> add_module_require(greeter, false)
                    var broken = make_broken_function()
                    mmain |> add_function(broken)
                }
                if (program.failed) {
                    print("compile failed, as expected\n")
                    print_errors(program)
                }
            }
        }
    }

When a module built with ``make_module`` fails, ``make_module`` returns null.
The next ``make_main_module`` returns the failed program without running its
block, so all the errors are checked in one place.


What happens underneath
=======================

A built program goes through the same steps as ``compile_file``. Only the
front end differs: a block fills each module where the parser would.

- ``make_program`` loads the default modules into its module group.
- ``make_module`` makes a program for one module, runs the block, then runs
  the same inference and finalization a required file gets. Then it moves the
  finished module into the group.
- ``make_main_module`` builds the program's own module in the same way, then
  gives it the steps an executable gets: unused symbols are removed and the
  stack is allocated.

Each module owns the nodes its block creates. While the block runs, the
module's own gc root is the active one, so every node the block allocates
lands there. When the module ends, the nodes the module reaches are kept and
the rest are freed. So build every node inside the block of the module that
uses it: a function built anywhere else fails the build.


Output
======

.. code-block:: text

    === greeter + main ===
    hello, world!
    sqrt(2) = 1.4142135

    === a type error ===
    compile failed, as expected
      error: no matching functions or generics: greet(int const)


.. seealso::

   Full source:
   :download:`21_build_program.das <../../../../../tutorials/macros/21_build_program.das>`,
   :download:`build_program_mod.das <../../../../../tutorials/macros/build_program_mod.das>`

   Previous tutorial: :ref:`tutorial_macro_template_struct_instance`

   Related: :ref:`tutorial_compile_and_run` - compiling a program from source
   text; :ref:`Macro Tutorial 17 <tutorial_macro_qmacro>` - ``qmacro_function``
   and the other quoting forms.

   Language reference: :ref:`Macros <macros>` - full macro system documentation
