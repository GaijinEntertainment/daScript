.. _tutorial_drag:

#######################
Drag widgets
#######################

The drag family is **click-and-scrub numeric editing**: press inside the
widget, drag horizontally, release. The value tracks pixel movement
scaled by ``speed``. Same call shape spans scalar / vector / range and
float / int — one mental model, ten widgets.

.. code-block:: das

   drag_float(IDENT, (text = "..", speed = 0.01f, format = "%.3f",
                      flags = ImGuiSliderFlags....))
   drag_int(IDENT, (text = "..", speed = 1.0f, format = "%d"))
   drag_float2 / drag_float3 / drag_float4        // vector forms
   drag_int2   / drag_int3   / drag_int4
   drag_float_range2(IDENT, (text, speed, format)) // paired lo / hi handles
   drag_int_range2(IDENT, (text, speed, format))

Bounds live on the state struct (``IDENT.bounds = (lo, hi)``), set once
per frame from app code. Zero-init = unclamped — scrub goes anywhere.

Source: ``examples/tutorial/drag.das``.

************
Walkthrough
************

.. video:: drag.mp4

.. literalinclude:: ../../../examples/tutorial/drag.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — every ``drag_*`` rail.
* ``imgui/imgui_boost_runtime`` — ``DragStateFloat`` / ``DragStateInt`` /
  ``DragStateFloat3`` / ``DragStateRangeFloat`` state structs.

Speed and format
================

``speed`` is units-per-pixel of horizontal cursor movement:

.. code-block:: das

   drag_float(OPACITY, (text = "opacity", speed = 0.005f))  // 0.005 / px → 200 px = 1.0
   drag_int(SCORE,     (text = "score", speed = 1.0f))      // 1 / px → ImGui clamps to int

``format`` is the printf-style label format. The default ``"%.3f"`` /
``"%d"`` covers most cases; bump to ``"%.6f"`` for fine-grained float
values or ``"%+04d"`` for signed-padded ints.

Bounds
======

Bounds are a property of the **state**, not the call:

.. code-block:: das

   D_FLOAT.bounds = (0.0f, 1.0f)
   drag_float(D_FLOAT, (text = "opacity", speed = 0.005f))

Set them every frame (idempotent assignment, no branching needed). The
wrapper passes ``(min, max)`` to ImGui's ``DragFloat`` which clamps the
scrub. Zero-initialized bounds = ``(0, 0)`` = special-cased to mean
**unclamped**.

Vector forms
============

The ``2`` / ``3`` / ``4`` suffix puts that many components on one row,
each its own drag handle. ``state.value`` becomes ``float2`` / ``float3``
/ ``float4`` (or ``int2`` / ``int3`` / ``int4``):

.. code-block:: das

   D_VEC3.bounds = (-10.0f, 10.0f)   // applies to every component
   drag_float3(D_VEC3, (text = "position", speed = 0.05f))
   // D_VEC3.value.x, D_VEC3.value.y, D_VEC3.value.z

Range forms
===========

``drag_float_range2`` / ``drag_int_range2`` render **two handles sharing
one bar** — useful for "filter between lo and hi" widgets:

.. code-block:: das

   D_RANGE.bounds = (0.0f, 100.0f)
   drag_float_range2(D_RANGE, (text = "band", speed = 0.5f))
   // D_RANGE.value.x = lo, D_RANGE.value.y = hi (ImGui enforces lo <= hi)

Driving from outside
====================

Every drag widget exposes the same telemetry channel — ``imgui_force_set``
writes ``state.pending_value`` which the next frame consumes:

.. code-block:: bash

   # Scalar:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRAG_WIN/D_FLOAT","value":0.75}}' \
        localhost:9090/command
   # Vector — one number per component:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRAG_WIN/D_VEC3","value":[1.0,2.0,3.0]}}' \
        localhost:9090/command
   # Range — (lo, hi) tuple:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRAG_WIN/D_RANGE","value":[10.0,40.0]}}' \
        localhost:9090/command

The dispatcher (``[widget_dispatch]`` on the state struct) accepts the
right JSON shape per state type — scalar number, array of numbers, or
two-element array for ranges.

Drag vs slider vs input
=======================

The three numeric-edit families differ in interaction shape:

* **drag** — click and scrub, no fixed track. Best for "tweak this value"
  where the absolute range is large or open-ended.
* **slider** — click and drag along a fixed-width track between
  ``v_min`` / ``v_max``. Best for bounded percentages, settings sliders.
* **input** — type the number, optionally with ``+`` / ``-`` step buttons.
  Best for precise values where the user knows the number.

All three families share the same vector / scalar / format conventions.
See :ref:`tutorial_slider`.

Caller-owned variant
====================

For sites where the value lives on an external scalar (not a widget
state struct), use the ``edit_drag_*`` rail instead — it takes a
``T?`` pointer via ``safe_addr`` and skips the state-struct allocation:

.. code-block:: das

   var g_opacity : float = 0.5f
   edit_drag_float(safe_addr(g_opacity), (id = "OPACITY",
                                          text = "opacity", speed = 0.005f))

See :ref:`tutorial_edit_external_tour`.

.. seealso::

   Full source: :download:`examples/tutorial/drag.das <../../../examples/tutorial/drag.das>`

   Features-side demo: ``examples/features/inputs_drag.das`` — every drag
   widget in one window, useful for ``imgui_force_set`` smoke testing.

   Sibling tutorial: :ref:`tutorial_slider`.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
