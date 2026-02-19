.. _tutorial_macro_structure_macro:

.. index::
   single: Tutorial; Macros; Structure Macro
   single: Tutorial; Macros; structure_macro
   single: Tutorial; Macros; AstStructureAnnotation
   single: Tutorial; Macros; apply
   single: Tutorial; Macros; patch
   single: Tutorial; Macros; finish
   single: Tutorial; Macros; serializable

====================================================
 Macro Tutorial 6: Structure Macros
====================================================

Tutorials 3–5 transformed *function calls*.  Structure macros
operate on **struct and class definitions** instead.

``[structure_macro(name="X")]`` registers a class that extends
``AstStructureAnnotation``.  When a struct is annotated with
``[X]``, the compiler calls the macro's methods at three stages of
the compilation pipeline:

+---------------------+----------------------------------------------+
| Method              | When it runs                                 |
+=====================+==============================================+
| ``apply()``         | During parsing, **before** inference.        |
|                     | Can add fields, validate annotation          |
|                     | arguments, and generate new functions.       |
+---------------------+----------------------------------------------+
| ``patch()``         | **After** inference.  Types are resolved,    |
|                     | so type-aware checks are possible.  Can set  |
|                     | ``astChanged`` to restart inference.         |
+---------------------+----------------------------------------------+
| ``finish()``        | After all inference and optimization.        |
|                     | Read-only — useful for diagnostics.          |
+---------------------+----------------------------------------------+

This tutorial builds a ``[serializable]`` annotation that:

1. Adds a ``_version`` field to every annotated struct and generates a
   ``describe_StructName()`` function that prints all fields (``apply``).
2. Rejects structs with unsupported field types like lambdas and
   blocks (``patch``).
3. Prints a compile-time summary of each struct (``finish``).


.. code-block:: das

   [serializable(version=2)]
   struct Player {
       name : string
       health : int
   }

After compilation the struct gains a ``_version : int = 2`` field and a
generated ``describe_Player`` function.


Why three methods?
==================

Each method runs at a different point in the pipeline, which determines
what it can and cannot do:

* **apply()** — The struct definition is parsed but types are not
  resolved.  You can add fields and generate functions, but you cannot
  inspect ``fld._type.baseType`` because types are still
  ``autoinfer``.  Any generated functions or fields **must** be
  created here so they exist when inference runs.

* **patch()** — Runs after inference, so all types are resolved.  This
  is the place for type-level validation.  If you generate new code
  here, set ``astChanged = true`` to restart inference — and use a
  guard (like a ``"patched"`` annotation argument) to avoid infinite
  loops.

* **finish()** — Everything is final: types are resolved, code is
  optimized.  No modifications allowed.  Use it for diagnostics,
  compile-time reporting, or AOT-related output.


The module: ``structure_macro_mod.das``
=======================================

Registration
------------

.. code-block:: das

   [structure_macro(name="serializable")]
   class SerializableMacro : AstStructureAnnotation {
       ...
   }

``[structure_macro(name="serializable")]`` tells the compiler:

   *When a struct or class is annotated with* ``[serializable]``,
   *call this class's methods during compilation.*

Like ``[function_macro]``, the macro class must be compiled before
any module that uses the annotation — hence the two-file setup.


Inside ``apply()``
------------------

The method receives the struct definition, annotation arguments, and
an error string.  It runs during parsing, before inference.


Step 1 — Validate arguments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: das

   var version = 1
   for (arg in args) {
       if (arg.name == "version") {
           let val = get_annotation_argument_value(arg)
           if (val is tInt) {
               version = val as tInt
           } else {
               errors := "[serializable] 'version' argument must be an integer"
               return false
           }
       } else {
           errors := "[serializable] unknown argument — only 'version' is supported"
           return false
       }
   }

We iterate the ``AnnotationArgumentList`` and accept only
``version`` (integer).  Returning ``false`` aborts compilation with
the error message stored in ``errors``.


Step 2 — Add a field
^^^^^^^^^^^^^^^^^^^^^

.. code-block:: das

   st |> add_structure_field("_version",
       clone_type(qmacro_type(type<int>)),
       qmacro($v(version)))

``add_structure_field`` appends a new field to the struct's field
list.  It **moves** both the ``TypeDeclPtr`` and ``ExpressionPtr``
arguments, so they must be either temporaries or clones.

.. warning::

   Never pass a ``var inscope`` variable directly to
   ``add_structure_field`` — it will be moved *and* destroyed at
   scope exit, causing a double-free crash.  Always pass
   ``clone_type(...)`` or an inline temporary.

``qmacro_type(type<int>)`` creates a ``TypeDeclPtr`` for ``int``.
``qmacro($v(version))`` creates an integer constant expression.


Step 3 — Generate the describe function
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: das

   let funcName = "describe_{st.name}"
   var inscope bodyExprs : array<ExpressionPtr>

   bodyExprs |> emplace_new <| qmacro(print($v("{st.name} (version ")))
   bodyExprs |> emplace_new <| qmacro(print("{obj._version}"))
   bodyExprs |> emplace_new <| qmacro(print($v("):\n")))

   for (fld in st.fields) {
       if (fld.name == "_version") {
           continue
       }
       bodyExprs |> emplace_new <| qmacro(print($v("  {fld.name} = ")))
       bodyExprs |> emplace_new <| qmacro(print("{obj.$f(fld.name)}"))
       bodyExprs |> emplace_new <| qmacro(print($v("\n")))
   }

   var inscope fn <- qmacro_function(funcName) $(obj : $t(st)) {
       $b(bodyExprs)
   }
   fn.flags |= FunctionFlags.generated
   fn.body |> force_at(st.at)
   add_function(st._module, fn)

The body is built as an ``array<ExpressionPtr>`` of print
statements.  There are two kinds of content:

* **Compile-time constants** — field names, struct name.  Use
  ``$v("string")`` to splice a constant string into the generated
  code.  String interpolation like ``"{st.name}"`` resolves at
  *macro execution time* and becomes a literal.

* **Runtime values** — ``obj._version``, ``obj.$f(fld.name)``.
  Inside ``qmacro``, ``obj`` refers to the generated function's
  parameter.  ``$f(fld.name)`` splices a string as a field-access
  name.  To convert any value to a string for ``print``, use string
  interpolation: ``print("{obj.$f(fld.name)}")``.

``qmacro_function(funcName) $(obj : $t(st)) { $b(bodyExprs) }``
builds a complete function:

* ``funcName`` — a string for the function name
* ``$t(st)`` — splices the struct type as the parameter type
* ``$b(bodyExprs)`` — splices the statement array into the body

``add_function(st._module, fn)`` registers the function in the
struct's module so callers can find it.


Inside ``patch()``
------------------

.. code-block:: das

   def override patch(var st : StructurePtr; var group : ModuleGroup;
                      args : AnnotationArgumentList; var errors : das_string;
                      var astChanged : bool&) : bool {
       for (fld in st.fields) {
           if (fld._type.baseType == Type.tBlock) {
               errors := "struct has block field — blocks cannot be serialized"
               return false
           }
           // ... also reject tLambda, tFunction
       }
       return true
   }

After inference, ``fld._type.baseType`` is resolved.  We reject
blocks, lambdas, and functions because they cannot be meaningfully
serialized.

.. note::

   ``patch()`` receives a ``var astChanged : bool&`` parameter.
   Setting it to ``true`` restarts inference — essential when
   ``patch`` generates new code.  In this tutorial we only validate,
   so we leave it ``false``.  When using ``astChanged``, always add
   a guard to prevent infinite loops (e.g., check for a
   ``"patched"`` annotation argument before regenerating).


Inside ``finish()``
-------------------

.. code-block:: das

   def override finish(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
       var userFields = 0
       for (fld in st.fields) {
           if (fld.name != "_version") {
               userFields++
           }
       }
       let ver = find_arg(args, "version")
       if (ver is tInt) {
           print("[serializable] {st.name}: {userFields} field(s), version {ver as tInt}\n")
       } else {
           print("[serializable] {st.name}: {userFields} field(s), version 1\n")
       }
       return true
   }

``finish()`` runs after all inference and optimization.  The struct
is in its final form — we count fields and read the ``version``
annotation argument for a compile-time diagnostic.

``find_arg(args, "version")`` returns an ``RttiValue`` variant,
checked with ``is tInt`` / ``as tInt``.


The usage file
==============

.. code-block:: das

   options gen2
   require structure_macro_mod

   [serializable]
   struct Color {
       r : float
       g : float
       b : float
   }

   [serializable(version=2)]
   struct Player {
       name : string
       health : int
       score : float
   }

   [export]
   def main() {
       var c = Color(r = 0.2, g = 0.7, b = 1.0)
       describe_Color(c)

       var p = Player(name = "Alice", health = 100, score = 42.5)
       describe_Player(p)

       print("Color version: {c._version}\n")
       print("Player version: {p._version}\n")
   }

``Color`` uses the default version (1).  ``Player`` specifies
``version=2``.  The generated ``describe_Color`` and
``describe_Player`` functions print each field's name and value.

Compile-time output (from ``finish``)::

   [serializable] Color: 3 field(s), version 1
   [serializable] Player: 3 field(s), version 2

Runtime output::

   --- describe_Color ---
   Color (version 1):
     r = 0.2
     g = 0.7
     b = 1

   --- describe_Player ---
   Player (version 2):
     name = Alice
     health = 100
     score = 42.5

   --- version info ---
   Color version: 1
   Player version: 2


Compilation pipeline summary
=============================

The full sequence for a ``[serializable]`` struct:

.. code-block:: text

   parse struct definition
     ↓
   apply() → add _version field, generate describe_X()
     ↓
   infer types (generated function is also inferred)
     ↓
   patch() → validate field types (reject blocks/lambdas/functions)
     ↓
   optimize
     ↓
   finish() → compile-time diagnostic
     ↓
   simulate → run


Key takeaways
=============

.. list-table::
   :widths: 40 60
   :header-rows: 1

   * - Concept
     - What it does
   * - ``[structure_macro(name="X")]``
     - Registers a class as a structure annotation
   * - ``AstStructureAnnotation``
     - Base class with ``apply``, ``patch``, ``finish`` methods
   * - ``apply()``
     - Pre-inference: add fields, generate functions, validate args
   * - ``patch()``
     - Post-inference: validate types, optionally restart inference
   * - ``finish()``
     - Final: read-only diagnostics and reporting
   * - ``astChanged``
     - Set ``true`` in ``patch`` to restart inference after changes
   * - ``add_structure_field``
     - Appends a field to a struct; moves both type and init expression
   * - ``clone_type``
     - Deep-clones a ``TypeDeclPtr``; required before move operations
   * - ``qmacro_function``
     - Builds a complete function from reification splices
   * - ``$v(value)``
     - Splice a compile-time value as a constant expression
   * - ``$f(name)``
     - Splice a string as a field-access name
   * - ``$t(type)``
     - Splice a ``TypeDeclPtr`` into parameter/return types
   * - ``$b(stmts)``
     - Splice ``array<ExpressionPtr>`` as a statement list
   * - ``find_arg``
     - Look up annotation argument values by name
   * - ``force_at``
     - Stamps source location on all generated AST nodes


.. seealso::

   Full source:
   :download:`structure_macro_mod.das <../../../../../tutorials/macros/structure_macro_mod.das>`,
   :download:`06_structure_macro.das <../../../../../tutorials/macros/06_structure_macro.das>`

   Previous tutorial: :ref:`tutorial_macro_tag_function_macro`

   Standard library examples:
   ``daslib/interfaces.das`` (``apply`` + ``finish``),
   ``daslib/export_constructor.das`` (``apply`` only),
   ``daslib/decs_boost.das`` (``apply`` with field iteration)

   Language reference: :ref:`Macros <macros>` — full macro system documentation
