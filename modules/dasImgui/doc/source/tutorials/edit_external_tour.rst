.. _tutorial_edit_external_tour:

#################################
External-pointer editing rail
#################################

State-managed boost widgets (``slider_float``, ``checkbox``, ``combo``,
…) auto-emit a per-call global state struct that owns ``.value``.
That's fine for new code, but doesn't help when the data already lives
in someone else's struct, a daslang global, or a C-side configuration
block.

The ``edit_*`` family takes a caller-owned ``T?`` pointer instead. The
widget reads + writes through that pointer; a hidden internal mirror
keeps the snapshot stable across frames.

Source: ``examples/tutorial/editing_external.das``.

.. video:: edit_external_tour.mp4

What the tour shows
===================

Seven external-pointer widgets, each editing a ``var private`` global.
The tour drives every one the way a person would — a real drag, type,
click, or popup pick — so you can watch the bound global change in the
readout, which is the whole point of the rail:

* ``edit_slider_float(safe_addr(g_volume), (id = "VOL_SLIDER", text =
  "Volume", v_min, v_max))`` — drag the handle.
* ``edit_input_int(safe_addr(g_count), (id = "COUNT_INPUT", text =
  "Count", step = 1))`` — click to focus, type a number, Enter.
* ``edit_checkbox(safe_addr(g_enabled), (id = "ENABLED_CHECK", text =
  "Enabled"))`` — click.
* ``edit_drag_float3(safe_addr(g_pos), (id = "POS_DRAG", ...))`` — drag
  the first field; a Drag *scrubs* on click-drag (only a Ctrl+click would
  enter text), so the gesture is a drag, not a type.
* ``edit_color_edit3(safe_addr(g_accent), (id = "ACCENT_COLOR", ...))``
  — drag the red channel; the swatch shifts toward red.
* ``edit_combo(safe_addr(g_quality_idx), (id = "QUALITY_COMBO",
  items <- g_qualities))`` — open the popup, click an item.
* ``edit_slider_angle(safe_addr(g_angle), (id = "ANGLE_SLIDER", ...))``
  — drag the handle (the face shows degrees, the bound variable is radians).

Each widget's read-only sibling under the ``// Readout`` separator
shows the underlying ``var private`` value updating live with each
edit.

safe_addr vs unsafe(addr)
=========================

``safe_addr(local_or_global)`` is the right form for plain locals and
globals. ImGui needs a stable pointer to ``T``; ``safe_addr`` enforces
that.

It rejects struct-field-via-reference lvalues at compile time -- there
the field's lifetime is the reference's, which the compiler can't
guarantee outlives the widget call. For those, fall back to
``unsafe(addr(struct.field))`` and accept the responsibility for
lifetime.

The ``id = "..."`` literal is REQUIRED on every ``edit_*`` call. It's
the path leaf in widget telemetry; missing it is a compile-time error
from ``EditExternalCallMacro``. No implicit ``:line:col`` fallback,
because the edit_* family doesn't auto-emit a state struct that could
carry the path.

Driving the tour
================

Standalone (windowed):

.. code-block:: bash

   daslang modules/dasImgui/examples/tutorial/editing_external.das

Live (with reload):

.. code-block:: bash

   daslang-live modules/dasImgui/examples/tutorial/editing_external.das

The recorded tour above is captured by
``tests/integration/record_editing_external.das``, which drives every
widget with real synthetic input -- a drag, a type, a click, a popup pick
-- and asserts each bound ``var private`` actually moved (the recording is
also a test). Nothing is ``force_set``: binding to your data is the
subject, and the proof is that a real gesture writes through the pointer.

That said, an ``edit_*`` widget *can* also be driven externally without
the gesture -- ``imgui_force_set`` writes the bound pointer directly,
``imgui_focus`` + ``imgui_key_type`` script the keyboard. That path is the
subject of :ref:`Driving outside <tutorial_driving_outside>`.

.. seealso::

   Full source: :download:`examples/tutorial/editing_external.das <../../../examples/tutorial/editing_external.das>`

   Recording driver: ``tests/integration/record_editing_external.das``.

   :ref:`State telemetry <tutorial_state_telemetry>` — the snapshot
   path that ``edit_*`` widgets register under.

   :ref:`Driving outside <tutorial_driving_outside>` — the
   ``imgui_force_set`` / ``imgui_mouse_play`` / ``imgui_key_type`` verbs
   for driving widgets programmatically.

   :ref:`Recording <tutorial_recording>` — the two-shell driver setup.
