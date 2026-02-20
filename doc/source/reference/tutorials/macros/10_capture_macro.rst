.. _tutorial_macro_capture_macro:

.. index::
   single: Tutorial; Macros; Capture Macro
   single: Tutorial; Macros; capture_macro
   single: Tutorial; Macros; AstCaptureMacro
   single: Tutorial; Macros; lambda capture auditing

====================================================
 Macro Tutorial 10: Capture Macros
====================================================

Previous tutorials transformed calls, functions, structures, blocks,
variants, and for-loops.  Capture macros intercept **lambda capture** —
they fire when a lambda (or generator) captures outer variables,
letting you wrap capture expressions and inject per-invocation cleanup.

``[capture_macro(name="X")]`` registers a class that extends
``AstCaptureMacro``.  The compiler calls two methods during lambda
code generation:

``captureExpression(prog, mod, expr, etype)``
   Called **per captured variable** when the lambda struct is being
   built.  ``expr`` is the expression being assigned to the capture
   field (typically an ``ExprVar``).  ``etype`` is the variable's type.
   Return a replacement expression to wrap the capture, or
   ``default<ExpressionPtr>`` to leave it unchanged.

``captureFunction(prog, mod, lcs, fun)``
   Called **once** after the lambda function is generated.  ``lcs`` is
   the hidden lambda struct (with fields for each capture).  ``fun``
   is the lambda function.  Use this to inspect captured fields and
   append code to ``(fun.body as ExprBlock).finalList`` — which runs
   **after each invocation** (per-call finally), not on destruction.

.. note::

   Code added to ``(fun.body as ExprBlock).finalList`` by
   ``captureFunction`` runs after **every** lambda invocation.  This
   is different from a user-written ``finally {}`` on the lambda
   literal, which goes into the **finalizer** and runs once on
   destruction.

   Generators are a special case — their function body's ``finalList``
   runs on every yield iteration, which is why the standard library's
   ``ChannelAndStatusCapture`` skips generators entirely.


Motivation
==========

When lambdas capture complex resources (file handles, GPU objects,
reference-counted channels), it is useful to **audit** captures
automatically — log when a resource is captured and verify it after
each call — without modifying every lambda by hand.

This tutorial builds a capture macro driven by a **tag annotation**:
only structs marked ``[audited]`` are monitored.  Non-annotated types
are silently ignored.  This pattern (annotation + macro) is the same
used by ``daslib/jobque_boost.das`` for ``Channel`` and ``JobStatus``
reference counting.

.. note::

   Macros cannot be used in the module that defines them.  This
   tutorial has **two** source files: a *module* file containing the
   macro definition and a *usage* file that requires the module.


The module file
===============

``capture_macro_mod.das`` defines three pieces:

1. ``[audited]`` — a no-op structure annotation used as a tag
2. Runtime helpers — ``audit_on_capture`` and ``audit_after_invoke``
3. ``CaptureAuditMacro`` — the capture macro class

The tag annotation
~~~~~~~~~~~~~~~~~~

.. code-block:: das

   [structure_macro(name=audited)]
   class AuditedAnnotation : AstStructureAnnotation {
       def override apply(var st : StructurePtr; var group : ModuleGroup;
               args : AnnotationArgumentList; var errors : das_string) : bool {
           return true  // no-op tag
       }
   }

This registers ``[audited]`` as a valid struct annotation.  It does
nothing at compile time — the capture macro checks for it at capture
time.

Type checking helper
~~~~~~~~~~~~~~~~~~~~

A ``[macro_function]`` inspects whether a ``TypeDeclPtr`` refers to a
struct with the ``[audited]`` annotation:

.. code-block:: das

   [macro_function]
   def private is_audited(typ : TypeDeclPtr) : bool {
       if (!typ.isStructure || typ.structType == null) {
           return false
       }
       for (ann in typ.structType.annotations) {
           if (ann.annotation.name == "audited") {
               return true
           }
       }
       return false
   }

This iterates ``typ.structType.annotations`` — the same pattern used
by ``daslib/match.das`` to check for ``[match_as_is]``.

captureExpression
~~~~~~~~~~~~~~~~~

When an ``[audited]`` variable is captured, the macro wraps the
capture expression in a call to ``audit_on_capture(value, "name")``:

.. code-block:: das

   def override captureExpression(prog : Program?; mod : Module?;
           expr : ExpressionPtr; etype : TypeDeclPtr) : ExpressionPtr {
       if (!is_audited(etype)) {
           return <- default<ExpressionPtr>
       }
       var field_name = "unknown"
       if (expr is ExprVar) {
           field_name = string((expr as ExprVar).name)
       }
       var inscope pCall <- new ExprCall(at = expr.at,
           name := "capture_macro_mod::audit_on_capture")
       pCall.arguments |> emplace_new <| clone_expression(expr)
       pCall.arguments |> emplace_new <| new ExprConstString(
           at = expr.at, value := field_name)
       return <- pCall
   }

``audit_on_capture`` prints ``[audit] captured 'name'`` and returns
the value unchanged, so the capture proceeds normally.

captureFunction
~~~~~~~~~~~~~~~

For each ``[audited]`` field in the lambda struct, the macro appends
a print call to the function body's ``finalList``:

.. code-block:: das

   def override captureFunction(prog : Program?; mod : Module?;
           var lcs : Structure?; var fun : FunctionPtr) : void {
       if (fun.flags._generator) {
           return    // generators run finally on every yield — skip
       }
       for (fld in lcs.fields) {
           if (!is_audited(fld._type)) {
               continue
           }
           if (true) {   // scope needed for var inscope inside a loop
               var inscope pCall <- new ExprCall(at = fld.at,
                   name := "capture_macro_mod::audit_after_invoke")
               pCall.arguments |> emplace_new <| new ExprConstString(
                   at = fld.at, value := string(fld.name))
               (fun.body as ExprBlock).finalList |> emplace(pCall)
           }
       }
   }

The ``if (true)`` wrapper is required because ``var inscope`` is not
allowed directly inside a ``for`` loop — the extra scope satisfies
the compiler.


The usage file
==============

``10_capture_macro.das`` defines an ``[audited]`` struct and a plain
struct, then creates lambdas that capture them:

.. code-block:: das

   require capture_macro_mod

   [audited]
   struct Resource {
       name : string
       id : int
   }

   struct Plain {
       x : int
   }

**Section 1** — one ``[audited]`` and one plain capture:

.. code-block:: das

   var res = Resource(name = "texture.png", id = 1)
   var pl = Plain(x = 42)
   var fn <- @() {
       print("  body: res.name={res.name}, pl.x={pl.x}\n")
   }
   fn()
   unsafe { delete fn; }

Output::

   [audit] captured 'res'
     body: res.name=texture.png, pl.x=42
   [audit] after-call: 'res' still captured
     about to delete fn...

- ``[audit] captured 'res'`` — from ``captureExpression`` at lambda creation
- ``[audit] after-call`` — from ``captureFunction``'s ``finalList`` after the call
- No messages for ``pl`` (``Plain`` has no ``[audited]`` annotation)

**Section 2** — two ``[audited]`` captures, two calls:

.. code-block:: das

   var a = Resource(name = "mesh.obj", id = 2)
   var b = Resource(name = "shader.hlsl", id = 3)
   var fn <- @() {
       print("  body: a.id={a.id}, b.id={b.id}\n")
   }
   fn()
   fn()

Each call produces after-call messages for both ``a`` and ``b``.

**Section 3** — only non-annotated types (``int``): completely silent.


Real-world usage
================

The standard library's ``ChannelAndStatusCapture`` in
``daslib/jobque_boost.das`` uses the same two-hook pattern:

- ``captureExpression``: calls ``add_ref`` on captured ``Channel`` or
  ``JobStatus`` pointers (increases reference count)
- ``captureFunction``: appends a ``panic`` call that fires if the
  object was not properly released after each lambda invocation

This ensures that thread-communication objects are never leaked,
without requiring any changes to user lambda code.


.. seealso::

   Full source:
   :download:`10_capture_macro.das <../../../../../tutorials/macros/10_capture_macro.das>`,
   :download:`capture_macro_mod.das <../../../../../tutorials/macros/capture_macro_mod.das>`

   Previous tutorial: :ref:`tutorial_macro_for_loop_macro`

   Standard library: ``daslib/jobque_boost.das`` (``ChannelAndStatusCapture``)

   Language reference: :ref:`Macros <macros>` — full macro system documentation
