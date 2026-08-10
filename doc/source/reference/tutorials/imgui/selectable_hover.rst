.. _tutorial_selectable_hover:

#######################
Selectable hover
#######################

``selectable`` is a toggleable row — its ``ToggleState`` flips between
selected and unselected on click. Some call sites want the row geometry
only, with the selected flag pinned to false; the hover variant covers
that:

.. code-block:: das

   var private SH_ROW : table<int; EmptyMarkerState>

   for (i in range(length(ROWS))) {
       let hovered = selectable_hover(SH_ROW[i], (text = "Row {i}: {ROWS[i]}"))
       if (hovered) {
           // per-frame side-effect, e.g. SetMouseCursor(...)
       }
   }

``EmptyMarkerState`` backs the widget; the return value is the per-frame
hover bool. The selected flag is fixed at false — clicks render a brief
selection highlight but the row never persists as selected.

One widget per loop iteration means an **indexed** state table, and the
``[widget]`` macro does not auto-emit those — declare
``SH_ROW : table<int; EmptyMarkerState>`` at module scope yourself (string
keys work too: ``table<string; EmptyMarkerState>``).

Source: ``modules/dasImgui/examples/tutorial/selectable_hover.das``.

************
Walkthrough
************

.. video:: selectable_hover.mp4

.. literalinclude:: ../../../../../modules/dasImgui/examples/tutorial/selectable_hover.das
   :language: das
   :linenos:

Requires
========

.. das-doc: given let ROWS = fixed_array("Arrow", "TextInput", "ResizeAll", "Hand", "NotAllowed")

Same baseline as ``selectable`` — ``imgui/imgui_widgets_builtin``, which
every widget tutorial already requires.

When to reach for it
====================

When the selectable row is a *hover surface*, not a *toggle*:

* ``examples/imgui_demo/inputs.das`` — the Mouse Cursors list renders one
  selectable_hover row per ``ImGuiMouseCursor`` value and sets ImGui's
  cursor while a row is hovered. Toggle semantics would be wrong (no
  persistent selection; the cursor follows hover frame-to-frame).
* Per-row preview rails where hovering shows a tooltip / status pane and
  the click semantics don't matter.

Toggle vs hover
===============

* ``selectable(IDENT, (text))`` — returns ``bool changed``, persists
  ``state.value`` (selected/unselected) across frames.
* ``selectable_hover(IDENT, (text))`` — returns ``bool hovered``, no
  persistent selection state.

A site that needs both (click toggles, hover previews) should use the
regular ``selectable`` and read ``IsItemHovered()`` after the call.

Standalone vs live
==================

Same convention as the other widget tutorials.

.. seealso::

   Full source: :download:`modules/dasImgui/examples/tutorial/selectable_hover.das <../../../../../modules/dasImgui/examples/tutorial/selectable_hover.das>`

   Integration test: ``modules/dasImgui/tests/test_selectable_hover.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
