.. _tutorial_input_numeric:

#######################
Input numeric widgets
#######################

The input_* numeric family is **type-the-number** editing: click to
focus, type, Enter to commit. Optional ``+`` / ``-`` step buttons turn
scalar forms into discrete-step editors. Same call shape spans scalar /
vector / double-precision — nine widgets, one mental model.

.. code-block:: das

   input_float(IDENT, (text = "..", step = 0.0f, step_fast = 0.0f,
                       format = "%.3f", flags = ImGuiInputTextFlags....))
   input_int(IDENT, (text = "..", step = 1, step_fast = 100,
                     flags = ImGuiInputTextFlags....))
   input_double(IDENT, (text = "..", step = 0.0lf, step_fast = 0.0lf,
                        format = "%.6f"))
   input_float2 / input_float3 / input_float4   // vector — no step args
   input_int2   / input_int3   / input_int4

No bounds. ``input_*`` is for **typed entry**; if you need clamped
scrubbing, use :ref:`tutorial_drag` or :ref:`tutorial_slider`.

Source: ``examples/tutorial/input_numeric.das``.

************
Walkthrough
************

.. video:: input_numeric.mp4

.. literalinclude:: ../../../examples/tutorial/input_numeric.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — every ``input_*`` numeric rail.
* ``imgui/imgui_boost_runtime`` — ``InputStateFloat`` / ``InputStateInt`` /
  ``InputStateDouble`` (+ vector variants) state structs.

Step buttons
============

Scalar forms (``input_float``, ``input_int``, ``input_double``) accept
``step`` and ``step_fast``. Non-zero values surface ImGui's ``+`` / ``-``
buttons on the right of the field:

.. code-block:: das

   input_float(MASS, (text = "mass (kg)",
                      step = 0.1f, step_fast = 1.0f))    // +/- buttons visible
   input_int(LEVEL, (text = "level",
                     step = 1, step_fast = 100))         // defaults match this

Plain click = ``step``. Ctrl-click = ``step_fast``. Defaults differ per
type: ``input_int`` defaults ``step = 1`` / ``step_fast = 100`` (buttons
visible by default); ``input_float`` and ``input_double`` default to
``0.0`` (buttons hidden — pure text entry).

Vector forms (``input_float2`` / ``3`` / ``4``, ``input_int2`` / etc.)
**omit step args** — three or four +/- pairs would crowd the row.
Component-wise editing only.

Format
======

``format`` is the printf-style label format. Defaults are sane for most
cases; bump precision when the user needs to see it:

.. code-block:: das

   input_float(MASS, (text = "mass", format = "%.6f"))   // 6 decimal places
   input_int(LEVEL, (text = "level", format = "%03d"))   // 003, 042, etc.
   input_double(EPOCH, (text = "epoch", format = "%.9f")) // sub-ns precision

Vector forms
============

Same ``2`` / ``3`` / ``4`` convention — that many fields on one row.
``state.value`` becomes ``float2`` / ``float3`` / ``float4`` (or
``int2`` / ``int3`` / ``int4``). Tab cycles between components; Enter
commits the whole row.

.. code-block:: das

   input_float3(POSITION, (text = "position", format = "%.2f"))
   // POSITION.value.x, POSITION.value.y, POSITION.value.z

Double precision
================

``input_double`` is the only path to double-precision values in the
input family — drag and slider don't have a double variant. Use it for
timestamps, geographic coordinates, anything that needs more than 7
significant digits:

.. code-block:: das

   input_double(EPOCH_SEC, (text = "epoch",
                            step = 1.0lf, step_fast = 60.0lf,
                            format = "%.6f"))

Default format ``"%.6f"`` (six places) vs ``input_float``'s ``"%.3f"``
reflects the extra precision.

Flags
=====

``flags : ImGuiInputTextFlags`` carries the standard text-input modifiers
— ``CharsDecimal``, ``CharsHexadecimal``, ``CharsScientific``,
``ReadOnly``, ``Password`` (rarely useful on numeric inputs but
allowed), ``EscapeClearsAll``, etc. Composable via ``|``:

.. code-block:: das

   input_int(HEX_ADDR, (text = "addr",
                        flags = ImGuiInputTextFlags.CharsHexadecimal,
                        format = "0x%08X"))

Driving from outside
====================

Every input_* widget exposes the same telemetry channel as drag and
slider — ``imgui_force_set`` writes ``state.pending_value`` which the next
frame consumes:

.. code-block:: bash

   # Scalar:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IN_WIN/I_FLOAT","value":12.5}}' \
        localhost:9090/command
   # Vector — one number per component:
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"IN_WIN/I_VEC3","value":[1.0,2.0,3.0]}}' \
        localhost:9090/command

The dispatcher (``[widget_dispatch]`` on ``InputStateFloat`` and
friends) accepts the right JSON shape per state type.

Input vs drag vs slider
=======================

The three numeric-edit families differ in interaction shape:

* **input** — type the number, optionally with step buttons. Best for
  precise values where the user already knows the number.
* **drag** — click and scrub, no fixed track. Best for "tweak this
  value" with open-ended range.
* **slider** — click and drag along a fixed-width track between
  ``v_min`` / ``v_max``. Best for bounded percentages, settings.

All three families share vector / scalar / format conventions. See
:ref:`tutorial_drag` and :ref:`tutorial_slider`.

Caller-owned variant
====================

For sites where the value lives on an external scalar (not a widget
state struct), use the ``edit_input_*`` rail instead — it takes a
``T?`` pointer via ``safe_addr`` and skips the state-struct allocation:

.. code-block:: das

   var g_mass : float = 1.0f
   edit_input_float(safe_addr(g_mass), (id = "MASS",
                                        text = "mass", step = 0.1f))

See :ref:`tutorial_edit_external_tour`.

.. seealso::

   Full source: :download:`examples/tutorial/input_numeric.das <../../../examples/tutorial/input_numeric.das>`

   Features-side demo: ``examples/features/inputs_numeric.das`` — every
   numeric input in one window, useful for ``imgui_force_set`` smoke testing.

   Sibling tutorials: :ref:`tutorial_drag`, :ref:`tutorial_slider`,
   :ref:`tutorial_input_text`.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
