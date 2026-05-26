.. _tutorial_macro_with_boost:

.. index::
   single: Tutorial; Macros; with_
   single: Tutorial; Macros; with_boost
   single: Tutorial; Macros; array lock
   single: Tutorial; Macros; table lock

================================================================
 Macro Tutorial 18: ``with_`` — locked binding of container slots
================================================================

``daslib/with_boost`` adds a ``with_`` call-macro that solves a recurring
ergonomics problem: rebinding a struct field across an array or table
element. The naive form is rejected by daslang's typer:

.. code-block:: das

   var arr = [A(f1 = 1, f2 = 2)]
   var a : A& = arr[0]    // error[31300]: local reference to non-local expression is unsafe
   a.f1 = 99

Between binding ``a`` and writing through it, code could push/resize/erase
``arr``, leaving ``a`` dangling. ``with_`` solves this by:

1. Binding the element inside a block, named ``_`` by default;
2. Wrapping the block in an **automatic lock** on the container, so
   push/erase/resize/clear inside the body panic at runtime instead of
   silently corrupting memory.

The single-arg form is a 1:1 replacement for the rejected pattern above:

.. code-block:: das

   require daslib/with_boost

   var arr = [A(f1 = 1, f2 = 2)]
   with_(arr[0]) {
       _.f1 = 99       // mutation persists in arr[0]
   }


Section 1 — The single-arg form
================================

Default-name ``_`` binding works for any struct-element array (workhorse
types like ``int`` are refused — see Section 6):

.. code-block:: das

   var arr = [A(f1 = 1, f2 = 2), A(f1 = 3, f2 = 4)]
   with_(arr[0]) {
       _.f1 = 99
       _.f2 = 100
   }
   // arr[0] is now A(f1 = 99, f2 = 100)


Named binding via ``$(name)`` is identical in effect — the macro strips
constness so mutations always persist:

.. code-block:: das

   with_(arr[1]) $(elem) {
       elem.f1 = 555
   }


Section 2 — Multi-arg positional form
======================================

Passing multiple containers locks each independently. The block params
are positional (no ``=``-named args; the macro reads them in order):

.. code-block:: das

   var dst = [A(f1 = 0, f2 = 0)]
   var src = [A(f1 = 10, f2 = 20)]
   with_(dst[0], src[0]) $(d, s) {
       d.f1 = s.f1 + 1
       d.f2 = s.f2 + 2
   }

Arities 1, 2, and 3 are supported in this phase. Multi-arg + a
``return`` from the body is refused (use the single-arg form for
value-returning bodies); arity ≥ 4 is also refused.


Section 3 — Return-value form (single-arg only)
================================================

When the block has a ``return X`` statement, the value propagates out
of the ``with_`` expression:

.. code-block:: das

   let f1Val = with_(arr[0]) {
       _.f1 = 42
       return _.f1
   }
   // f1Val == 42, arr[0].f1 == 42

This works only for the single-arg form; multi-arg + return is refused.


Section 4 — Tables
===================

Tables work the same way; ``tab[key]`` upserts (creates a default entry
if the key is missing). Only **one** table-keyed arg per call — any
second insert into a table during the body would rehash and invalidate
the pinned entry, so the macro refuses anything past the first:

.. code-block:: das

   var tab : table<string; A>
   tab |> insert("k", A(f1 = 11, f2 = 22))
   with_(tab["k"]) $(v) {
       v.f1 = 777
   }


Section 5 — Lock is real
=========================

Mutation of the container inside the body panics at runtime — exactly
the failure mode the typer was trying to prevent at compile time:

.. code-block:: das

   try {
       with_(arr[0]) $(a) {
           arr |> push(A(f1 = 1000, f2 = 2000))   // panics: "can't push into locked array"
       }
   } recover {
       // ... clean recovery path
   }

.. note::

   Bodies that ``panic`` leak the lock — the ``finally`` cleanup is
   currently broken on panic (daslang issue #2532). This matches
   existing daslib behaviour (``daslib/array_boost::array_view`` etc.)
   and will be fixed once #2532 lands.


Section 6 — Refused container shapes
=====================================

``with_`` is intentionally narrow:

* **Non-``ExprAt`` containers** (plain locals, struct fields on locals,
  function-call results) are refused with a diagnostic pointing at
  built-in ``with``. ``with_`` is the *array/table specialist*; ``with``
  remains the right tool for locals.

* **Workhorse-element containers** (``array<int>``, ``array<float>``,
  ``table<K; int>``, …) are refused because the block-param binding
  would be by-value and the body's mutation wouldn't propagate. Use
  ``arr[i] = value`` directly — that's a single line and as clear.

* **More than one table-keyed arg** is refused per the rehash hazard
  noted above.

* **Multi-arg + ``return``** is refused (single-arg only for value-returning bodies).

* **Arity > 3** is refused.

All refusals fire at macro-expansion time with the macro-error code
``50503`` and a message describing the failing arg.


Running the tutorial
=====================

::

   daslang.exe tutorials/macros/18_with_boost.das

Expected output::

   section 2: arr[0] = 99, 100
   section 3: arr[1].f1 = 555
   section 4: dst[0] = 11, 22
   section 5: returned = 42, arr[0].f1 = 42
   section 6: tab[k].f1 = 777
   section 7: push during with_ body was caught (lock fired)


.. seealso::

   Full source:
   :download:`18_with_boost.das <../../../../../tutorials/macros/18_with_boost.das>`

   Previous tutorial: :ref:`tutorial_macro_qmacro`

   Standard library: ``daslib/with_boost.das`` — the macro and its
   ``_with_locked_*`` helpers

   Language reference: :ref:`Macros <macros>` — full macro system documentation
