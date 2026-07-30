.. _tutorial_custom_widgets:

##################
Custom widgets
##################

ImGui core ships no rotary control; community ports vendor their own. This
tutorial adds a rotary **volume knob** as a user-defined ``[widget]`` kind —
the same annotation every built-in (``button``, ``slider_float``, ``checkbox``,
…) uses. The knob plugs into ``pending_value_finalize`` unchanged: matching
the slider state-struct convention is the only contract.

Source: ``examples/tutorial/custom_widgets.das``.

************
Walkthrough
************

.. video:: custom_widgets.mp4

The recording drives the custom knob with real synthetic input and
self-verifies. It **drags** the Master knob along its 270° arc (asserting
``value`` rose off 0), drags Treble up a little (same assertion), then
drives Bass via ``imgui_force_set`` — the one place ``force_set`` *is* the
subject, so it counts as the real input — and asserts the value took. Finally it
clicks the ordinary ``Reset`` button and asserts Bass snapped from −0.5
back to its default 0, proving the live API and a plain built-in both
operate on the custom widget unchanged. (The arc drag rides a radius-25
circle around the knob disc's centre, which sits in the top third of the
hitbox — ``bbox.x + 36, bbox.y + 36`` — clear of the bottom dead zone.)

.. literalinclude:: ../../../examples/tutorial/custom_widgets.das
   :language: das
   :linenos:

Requires
========

Same boost stack as :ref:`tutorial_widgets_tour`, with two additions:
``imgui/imgui_boost`` for ``IM_COL32`` (the int/uint color helpers from the
legacy boost layer), and ``math`` for ``cos`` / ``sin`` / ``PI`` used by the
indicator angle.

The state struct
================

``VolumeKnobState`` mirrors ``SliderStateFloat`` field-for-field:

.. code-block:: das

   struct VolumeKnobState {
       @live value : float
       @live bounds : tuple<float; float>
       @optional has_pending : bool
       @optional pending_value : float
       @optional changed : bool
   }

This shape is the contract. ``pending_value_finalize`` is generic on the state
type — it reads ``has_pending`` / ``pending_value`` to consume queued
``imgui_force_set`` deliveries, and serializes the whole struct (``value``,
``bounds``, ``changed``) verbatim into the snapshot. Any widget kind that
matches these field names plugs straight into the rails. ``@live`` keeps
``value`` and ``bounds`` preserved across reloads; ``@optional`` lets the
dispatcher-managed fields stay zero-defaulted in older saved states.

The ``[widget]`` annotation
===========================

The annotation does two things to the function it decorates
(``widgets/imgui_boost_v2.das:32``):

- **Injects a ``widget_ident : string`` parameter** at position 1, between
  ``state`` and the user-facing args. Inside the body, ``widget_ident`` is
  the bare identifier string (``"MASTER"`` at the call site
  ``knob(MASTER, ...)``) — pass it to ``pending_value_finalize`` so the
  finalizer can build the registry path.
- **Registers a per-kind ``WidgetCallMacro``** that intercepts
  ``knob(IDENT, ...)`` calls. The macro auto-emits the named global
  (``MASTER``) on first use, parses dotted-suffix flags
  (``.PUBLIC`` / ``.NOTLIVE``), and rewrites the call to thread
  ``widget_ident`` through.

The body also gets ``widget_prelude(widget_ident)`` injected at the top —
that pushes the ImGui ID stack and applies any pending focus from
``imgui_focus``. The user never calls it directly.

The drawlist pattern
====================

Custom widgets follow the **InvisibleButton + DrawList** pattern from the
boost design (``API_REWORK.md §4.9``). Sequence:

1. ``InvisibleButton(text, sz)`` reserves a hitbox of size ``sz``. This is
   the **registered item** — ``widget_finalize`` reads bbox / hex_id /
   hover / active / focus from it, so it must be the last ImGui item before
   the finalizer call. Hitbox includes the label and value-readout area
   below the disc, so adjacent knobs stay aligned regardless of value-text
   width.
2. While ``IsItemActive()`` is true, the body reads ``GetIO().MousePos``
   and computes ``atan2(my - cy, mx - cx)``. The angle is shifted by
   ``-3π/4`` and wrapped into ``[0, 2π)`` so the active arc lands in
   ``[0, 3π/2)`` and the bottom-gap dead zone in ``[3π/2, 2π)``. Dead-zone
   reads are ignored — value holds at its previous reading. ``InvisibleButton``
   handles press / drag / release; the widget never has to track its own
   pressed state.
3. ``GetWindowDrawList()`` returns the per-window draw list. Primitives —
   ``AddCircleFilled`` / ``AddCircle`` / ``AddLine`` / ``AddText`` — render
   inside the bbox but are pure painting; they don't advance the ImGui
   cursor or participate in input.

Indicator angle is the inverse of the input mapping: ``θ = 3π/4 + frac · 3π/2``
sweeps 270° with a 90° gap at the bottom (DAW convention). ImGui's y-axis
points down so positive ``sin θ`` is down on screen; the formula reads
clockwise visually (frac=0 at 7 o'clock, frac=0.5 at 12, frac=1 at 5).
Mouse position drives ``state.value`` which drives the indicator angle —
no delta accumulation, no wraparound bookkeeping.

Reusing ``pending_value_finalize``
==================================

The last line of the knob body —
``pending_value_finalize(widget_ident, "knob", state)`` — is the same line
every value-typed built-in uses (``slider_float``, ``drag_float``,
``input_float``, ``color_edit3``, ``combo``). It builds the two finalize
lambdas:

- **Serializer**: closure over ``widget_ident``, returns
  ``state_jv(path, type<VolumeKnobState>)`` — JSON-ifies the live state
  every time ``imgui_snapshot`` asks.
- **Dispatcher**: closure that handles ``imgui_force_set`` with action
  ``"set"`` — writes ``state.pending_value`` and flips ``has_pending``.
  Next frame the body drains it (step 1).

Then ``widget_finalize`` installs both lambdas keyed on the widget's path,
and ``register_focusable`` makes the widget reachable by ``imgui_focus``.

**For widgets that don't fit this shape** — pure-action buttons, multi-stage
inputs, plots — the escape hatch is to write your own one-screen
``<kind>_finalize`` modeled on ``click_finalize``, ``toggle_finalize``, or
``plot_finalize`` in ``widgets/imgui_widgets_builtin.das``. The shape is
always the same: construct ``ser`` and ``disp`` lambdas via ``state_jv`` /
``with_state``, then call ``widget_finalize``.

Standalone vs live
==================

``main()`` runs the loop when invoked as ``daslang.exe custom_widgets.das``.
Under ``daslang-live`` the host calls ``init`` / ``update`` / ``shutdown``
directly; live-reloading the source preserves ``MASTER.value`` /
``TREBLE.value`` / ``BASS.value`` (via ``@live`` on ``VolumeKnobState``).

Driving from outside
====================

The custom knob takes the same live commands every slider does:

.. code-block:: bash

   # snapshot — knobs appear under "kind":"knob" with bbox + hex_id + payload
   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command

   # set a value programmatically — pending_value_finalize handles the dispatch
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MIXER_WIN/MASTER","value":0.75}}' \
        localhost:9090/command

   # click the built-in reset button next to the knobs
   curl -X POST -d '{"name":"imgui_click","args":{"target":"MIXER_WIN/RESET_BTN"}}' \
        localhost:9090/command

The snapshot payload carries ``value``, ``bounds``, ``changed`` —
whatever fields the state struct declares. No per-kind glue: the
``state_jv`` helper introspects the struct at compile time and serializes
every field.

Next steps
==========

This is the same pattern every built-in widget uses. To wire a wholly new
kind that needs its own dispatcher action (e.g. a 2-D pad with ``set_xy``),
copy a ``*_finalize`` helper from ``widgets/imgui_widgets_builtin.das`` and
rename one action key.

.. seealso::

   Full source: :download:`examples/tutorial/custom_widgets.das <../../../examples/tutorial/custom_widgets.das>`

   Driver script: ``modules/dasImgui/tests/integration/record_custom_widgets.das``
   — same two-shell pattern as :ref:`tutorial_recording`.

   Previous tutorial: :ref:`tutorial_widgets_tour`

   Next tutorial: :ref:`tutorial_layout`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the ``[widget]`` machinery.

   :ref:`Builtin widgets <stdlib_imgui_widgets_section>` — full widget catalog.
