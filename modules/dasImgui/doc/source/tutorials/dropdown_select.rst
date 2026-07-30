.. _tutorial_dropdown_select:

###################
Dropdown + select
###################

Four shapes of **pick-one-from-a-list**. ``combo`` collapses choices
into a dropdown popup; ``selectable`` stacks them as always-visible
rows. ``combo_getter`` serves a procedural list from a lambda;
``selectable_label`` lets the caller own the selected-flag (so you
can wire it through a list_box for single-select):

.. code-block:: das

   combo(IDENT, (text = "..", items <- ["A", "B", ...]))                       // dropdown, items array
   combo_getter(IDENT, (text = "..", items_count = N,                          // dropdown, procedural list
                        getter = @(idx; var r : string&) : bool { ... }))
   selectable(IDENT, (text = ".."))                                            // row, self-toggles
   selectable_label(IDENT, text, selected, (flags = ...))                      // row, caller owns 'selected'

Source: ``examples/tutorial/dropdown_select.das``.

************
Walkthrough
************

.. video:: dropdown_select.mp4

.. literalinclude:: ../../../examples/tutorial/dropdown_select.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — ``combo`` / ``combo_getter`` /
  ``selectable`` / ``selectable_label`` rails.
* ``imgui/imgui_containers_builtin`` — ``list_box`` block-arg
  container.
* ``imgui/imgui_boost_runtime`` — ``ComboState`` / ``ToggleState`` /
  ``ClickState`` / ``ListBoxState`` state structs.

combo — items array
===================

The simplest form. Pass the labels, get a dropdown; ``state.value`` is
the chosen index.

.. code-block:: das

   combo(FRUIT, (text = "Fruit",
                 items <- ["Apple", "Banana", "Cherry"]))
   // FRUIT.value == 0 / 1 / 2

``items`` is non-copyable (it's a moved ``array<string>``) — pass it
inline with ``<-``, or move from a local. Cap a long popup with
``popup_max_height_in_items = N`` to limit the scroller height.

combo_getter — procedural list
==============================

When labels don't live in a flat ``array<string>`` (table-backed,
generator-driven, computed on the fly), use the getter form. The
callback returns one label per ``idx``:

.. code-block:: das

   combo_getter(FONT, (text = "Font", items_count = length(FONTS),
       getter = @(idx : int; var result : string&) : bool {
           if (idx >= 0 && idx < length(FONTS)) {
               result = FONTS[idx]
               return true
           }
           return false
       }))

Return ``false`` for out-of-range indices — ImGui treats that as an
empty row, useful for sparse lists. ``items_count`` is the upper bound
the dropdown will probe.

selectable — multi-toggle rows
==============================

Each ``selectable`` owns its own ``ToggleState`` — rows toggle
independently, which falls out as multi-select naturally:

.. code-block:: das

   var private TAG : table<int; ToggleState>
   let TAGS = fixed_array("urgent", "blocked", "review")

   for (i in range(length(TAGS))) {
       selectable(TAG[i], (text = TAGS[i]))
   }
   // any subset can be on at once — read TAG[i].value to filter.

The ``table<int; ToggleState>`` pattern is required for any indexed
call (one IDENT, many rows). The state allocates lazily on first
touch of each index.

selectable_label inside list_box — single-select
================================================

For one-of-N, switch to ``selectable_label`` with a **caller-owned**
``selected`` bool, wrapped in ``list_box`` (which adds the bordered
frame + scroller). The list_box's ``state.value`` holds the chosen
index; the loop tags each row's ``selected`` from it:

.. code-block:: das

   var private SHIRT_ROW : table<int; ClickState>
   let SHIRTS = fixed_array("XS", "S", "M", "L", "XL", "XXL")

   list_box(SHIRT, (text = "size", size = float2(0.0f, 0.0f))) {
       for (i in range(length(SHIRTS))) {
           let is_sel = (i == SHIRT.value)
           if (selectable_label(SHIRT_ROW[i], SHIRTS[i], is_sel)) {
               SHIRT.value = i
           }
       }
   }

The ``ClickState`` rows carry only edge-trigger info — no per-row
toggle persistence. The list_box's ``state.value`` is the single
source of truth, so ``imgui_force_set`` on the list_box target works the
same as a combo.

Driving from outside
====================

.. code-block:: bash

   # combo / combo_getter — int value (the chosen index)
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DS_WIN/D_FRUIT","value":2}}' \
        localhost:9090/command
   # list_box single-select — int value (the chosen row)
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DS_WIN/D_SHIRT","value":3}}' \
        localhost:9090/command
   # selectable multi-select — bool per row (use the indexed IDENT)
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DS_WIN/D_TAG[0]","value":true}}' \
        localhost:9090/command

When to use which
=================

* **combo** — known list of choices, screen real estate matters.
  ``items`` is a one-liner.
* **combo_getter** — list is procedural / huge / sparse. Same UX,
  callback-driven content.
* **selectable** — multi-select. Rows always visible (no popup).
  Owns its own state per row.
* **selectable_label + list_box** — single-select that needs to stay
  visible (sidebar, settings panel). The framed list_box is the
  always-visible counterpart to combo's collapsing popup.

If a strip of 2-5 mutually exclusive options would fit inline,
``radio_button_int`` (:ref:`tutorial_toggles`) is lighter than a
combo or list_box.

.. seealso::

   Full source: :download:`examples/tutorial/dropdown_select.das <../../../examples/tutorial/dropdown_select.das>`

   Features-side demo: ``examples/features/inputs_choice.das`` —
   combo + list_box in one window for ``imgui_force_set`` smoke testing.

   Sibling tutorials: :ref:`tutorial_toggles`,
   :ref:`tutorial_selectable_hover` (hover-only variant of
   selectable).

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
