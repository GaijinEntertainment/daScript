.. _tutorial_toggles:

#########
Toggles
#########

The toggle widgets are **pick-this-not-that** input — three shapes,
one mental model. Click flips state; ``imgui_force_set`` writes it from
outside. The three forms differ in glyph and in whether they share
state across call sites:

.. code-block:: das

   checkbox(IDENT, (text = ".."))                   // single bool, square glyph
   radio_button(IDENT, (text = ".."))               // single bool, circle glyph
   radio_button_int(IDENT, (text = "..",            // N sites share ONE state;
                            v_button = N))          //   click writes state.value = v_button

Source: ``examples/tutorial/toggles.das``.

************
Walkthrough
************

.. video:: toggles.mp4

.. literalinclude:: ../../../examples/tutorial/toggles.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — ``checkbox`` / ``radio_button`` /
  ``radio_button_int`` rails.
* ``imgui/imgui_boost_runtime`` — ``ToggleState`` (checkbox + bool radio)
  and ``RadioIntState`` (grouped radio).

checkbox vs radio_button (bool form)
====================================

``checkbox`` and ``radio_button`` are the same widget under different
glyphs. Both take ``ToggleState``; click flips ``state.value``. The
only difference is the rendering — square check mark vs filled circle.
Pick by visual fit, not by semantics.

.. code-block:: das

   checkbox(ENABLED, (text = "Enabled"))      // [x] Enabled
   radio_button(SUBSCRIBE, (text = "Sub"))    // (•) Sub

Two checkboxes are mutually independent. Two ``radio_button`` calls
with **different** state idents are *also* mutually independent —
they only *look* like a radio group. If you want one-of-N exclusivity,
use ``radio_button_int``.

radio_button_int — grouped, mutually exclusive
==============================================

The grouped form is the canonical "pick one of N." Multiple call sites
share **one** ``RadioIntState``; each call passes its own ``v_button``
integer. Clicking a row writes ``state.value = v_button`` — the other
rows light off automatically because they re-check whether
``state.value == their_v_button`` each frame.

.. code-block:: das

   radio_button_int(MODE, (text = "Off",    v_button = 0)); same_line(SL_1)
   radio_button_int(MODE, (text = "Manual", v_button = 1)); same_line(SL_2)
   radio_button_int(MODE, (text = "Auto",   v_button = 2))
   // MODE.value is 0 / 1 / 2 — exactly one selected.

``same_line`` between rows lays them horizontally. Stack vertically
without it.

Because the rows share one ident (hence one snapshot path), each row
*also* registers a per-site geometry alias ``<path>#<v_button>`` — e.g.
``TG_WIN/T_MODE#0`` / ``#1`` / ``#2``. Use these to click or test one
specific option (``imgui_click`` resolves an alias to that row's bbox);
the shared value still lives at the bare ``TG_WIN/T_MODE``.

Driving from outside
====================

Same telemetry channel as every other widget — ``imgui_force_set`` writes
``state.pending_value``, consumed next frame:

.. code-block:: bash

   # checkbox / bool radio — bool value
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"TG_WIN/T_CHECK","value":true}}' \
        localhost:9090/command
   # grouped radio — int value (the v_button to select)
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"TG_WIN/T_MODE","value":2}}' \
        localhost:9090/command

The dispatcher (``[widget_dispatch]`` on ``ToggleState`` and
``RadioIntState``) parses the right JSON shape per state type.

Caller-owned variants
=====================

For sites where the value already lives on an external bool / int (not
a widget state struct), use the ``edit_*`` rails — they take a ``T?``
pointer via ``safe_addr`` and skip the state-struct allocation:

.. code-block:: das

   var g_enabled : bool = false
   edit_checkbox(safe_addr(g_enabled), (id = "EN", text = "Enabled"))

   var g_mode : int = 0
   edit_radio_button_int(safe_addr(g_mode), (id = "MODE",
                                              text = "Off", v_button = 0))

See :ref:`tutorial_edit_external_tour`.

.. seealso::

   Full source: :download:`examples/tutorial/toggles.das <../../../examples/tutorial/toggles.das>`

   Sibling: :ref:`tutorial_dropdown_select` — for one-of-N picks too
   large for a radio strip, ``combo`` collapses the choices into a
   dropdown.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
