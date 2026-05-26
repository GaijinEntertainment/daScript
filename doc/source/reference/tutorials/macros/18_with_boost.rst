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

Default-name ``_`` binding works for both struct-element and
workhorse-element arrays (workhorse coverage in Section 3):

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

Any arity works — the macro emits the full lock / invoke / unlock sequence inline, with one lock per container, so a call like ``with_(a[0], b[1], c[2], d[3], e[4]) $(va, vb, vc, vd, ve) { ... }`` scales naturally. Mix arrays and tables freely, subject to the single-table-arg rule (next section).


Section 3 — Workhorse element types (int, float, ...)
======================================================

The block-arg is bound by reference, so workhorse-element containers
work the same as struct-element ones — mutation through ``_ = X`` (or
the named ``x = X``) propagates back to the underlying slot:

.. code-block:: das

   var ints = [1, 2, 3]
   with_(ints[1]) {
       _ = 222
   }
   // ints == [1, 222, 3]

The macro emits each block parameter pinned to the container's element
type with the ref flag set, so daslang resolves the binding as ``int&``
(or whichever workhorse type the element happens to be). No special-case
in the macro for struct vs workhorse — the same pinning path covers both.


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

   var arr = [A(f1 = 1, f2 = 2)]
   with_(arr[0]) $(a) {
       arr |> push(A(f1 = 1000, f2 = 2000))   // panics: "can't push into locked array"
   }

daslang panic is fatal (not a C++/JS-style exception) — the program
prints the diagnostic and exits. ``try/recover`` exists to capture the
message before exit for nicer logging, NOT to recover-and-continue.


Section 6 — Refused container shapes
=====================================

``with_`` is intentionally narrow:

* **Non-``ExprAt`` containers** (plain locals, struct fields on locals,
  function-call results, array literals) are refused. The macro needs
  to ref-bind the container to a local, and only ExprVar-rooted
  lvalue chains (variables, ``obj.field``, ``arr[i]``) have stable
  addressable storage outside the expression. Use built-in ``with`` for
  locals; for literal-or-call containers, hoist to a ``var`` first.

* **More than one table-keyed arg** is refused per the rehash hazard
  noted above.

* **Bodies that ``return`` a value** are refused at typecheck time —
  the synthesized invoke target declares a ``: void`` block return.
  ``with_`` is for in-place mutation; compute values via a local:
  ``var v : T; with_(arr[0]) { v = _.f }``.

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
   section 5: ints = [ 1, 222, 3]
   section 6: tab[k].f1 = 777
   section 7: see comment for the lock-panic shape


.. seealso::

   Full source:
   :download:`18_with_boost.das <../../../../../tutorials/macros/18_with_boost.das>`

   Previous tutorial: :ref:`tutorial_macro_qmacro`

   Standard library: ``daslib/with_boost.das``

   Language reference: :ref:`Macros <macros>` — full macro system documentation
