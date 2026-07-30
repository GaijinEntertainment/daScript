.. _tutorial_flat_tooltips:

#######################
Flat tooltips
#######################

The container form ``tooltip(IDENT) { ... }`` (already in v1) wraps a full
layout scope — useful when the tooltip needs nested widgets or its own
state. For one-liner annotations that just need *one line of text* on a
hovered item, v2 ships two flat-form rails that fire without opening a
scope:

* ``set_tooltip(IDENT, (text = ...))`` — always shows; rare in practice,
  shown for completeness.
* ``set_item_tooltip(IDENT, (text = ...))`` — shows when the
  most-recently-submitted item is hovered. The canonical replacement for
  the imgui_demo ``HelpMarker`` helper.

.. code-block:: das

   if (button(SAVE_BTN, (text = "Save"))) { /* ... */ }
   set_item_tooltip(SAVE_TIP, (text = "Persists current state to disk."))

Chain them one per row — the tooltip attaches to whatever was just
submitted. No layout scope, no extra widgets, no nested block.

Source: ``examples/tutorial/flat_tooltips.das``.

************
Walkthrough
************

.. video:: flat_tooltips.mp4

.. literalinclude:: ../../../examples/tutorial/flat_tooltips.das
   :language: das
   :linenos:

Requires
========

Baseline boost layer — both rails live in ``imgui/imgui_boost_v2``.

set_item_tooltip vs the container form
======================================

``tooltip(IDENT) { ... }`` is still the right tool when:

* The tooltip body needs nested widgets — separators, colored text,
  inline checkboxes, indented bullets.
* The hover bool is computed at the call site and you want a custom
  block to fire only on certain conditions.

``set_item_tooltip`` is the right tool when:

* You want exactly one line of text on hover.
* The tooltip is purely informational (no nested layout).
* You want to chain many of them — one per button, slider, etc.

The HelpMarker pattern
======================

The imgui_demo's ``HelpMarker(...)`` helper renders a disabled ``(?)``
glyph and shows a tooltip on hover. v2 replaces it with two source lines:

.. code-block:: das

   text("Verbose option (with help marker):")
   same_line(SL_HINT)
   text_disabled(HELP_GLYPH, (text = "(?)"))
   set_item_tooltip(HELP_TIP, (text = "short explanation here"))

The ``text_disabled(HELP_GLYPH, ...)`` renders the dim glyph;
``set_item_tooltip`` attaches the tooltip to that disabled-text item. No
helper function, no extra abstraction — just two boost rails composed.

Standalone vs live
==================

Same convention as the other widget tutorials: ``daslang`` for standalone
or ``daslang-live`` for live-reload + telemetry. ``set_item_tooltip``'s
state struct (``NarrativeState``) carries the text string into the
snapshot for downstream playwright tests.

.. seealso::

   Full source: :download:`examples/tutorial/flat_tooltips.das <../../../examples/tutorial/flat_tooltips.das>`

   Integration test: ``tests/integration/test_tooltip_flat.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
