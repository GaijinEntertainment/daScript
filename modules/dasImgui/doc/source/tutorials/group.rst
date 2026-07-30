.. _tutorial_group:

#######################
Group
#######################

``group`` brackets ``BeginGroup``/``EndGroup`` — a pure layout container
with no chrome, no flags, and no observable state. What it *does* is
combine its children into a single layout unit so that:

* **same_line** after the closing brace continues from the group's
  right edge, not the last widget's. Two stacked column blocks share a
  row by sandwiching ``same_line()`` between two groups.
* **IsItemHovered** / ``IsItemActive`` / ``GetItemRect*`` after the
  group report against the **combined bbox** of every widget inside.
  Lets a tooltip attach to "anywhere in the icon-plus-label cluster"
  with no manual rectangle math.

``GroupState`` is empty. The path push still happens, so widgets inside
``group(LEFT_GRP)`` register under ``<window>/LEFT_GRP/<leaf>``.

Source: ``examples/tutorial/group.das``.

************
Walkthrough
************

.. video:: group.mp4

The recording walks the three patterns. First it toggles a checkbox in each of
the two side-by-side blocks — both stay live while ``same_line`` keeps them on
one row. Then it hovers the middle of the ``[i] Info`` cluster, *not* the button,
and the tooltip still fires — proof that ``IsItemHovered`` after ``EndGroup``
reads against the whole group's bounding box. Finally it drags the row's slider
and the value updates on the line below. Every step is a real synth gesture the
recording asserts landed.

.. literalinclude:: ../../../examples/tutorial/group.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — ``group`` plus ``tooltip``,
  ``same_line``, ``separator``.
* ``imgui/imgui_widgets_builtin`` — leaves used inside the group.

Two columns via same_line
=========================

The classic use: stack two column blocks side by side without manually
computing column widths.

.. code-block:: das

   group(LEFT_GRP) {
       text("Left block")
       checkbox(L_WIRE, (text = "Wireframe"))
       // Narrow the slider so the left group leaves room for the
       // right group on the same row.
       SetNextItemWidth(160.0f)
       slider_int(L_FRAMES, (text = "frame limit"))
   }
   same_line((spacing = 24.0f))
   group(RIGHT_GRP) {
       text("Right block")
       checkbox(R_VSYNC, (text = "VSync"))
       small_button(R_APPLY, (text = "Apply"))
   }

Without the groups, ``same_line`` would only join the previous *widget*
(the slider) to the next, leaving the rest of the right block to
overlap. The group treats four widgets as one item for ``same_line``'s
"continue from the right edge" semantics.

Hit-testing a cluster
=====================

``IsItemHovered`` after ``EndGroup`` applies to the whole group's bbox.
The tutorial uses it to gate a manual ``tooltip``:

.. code-block:: das

   group(HOVER_GRP) {
       text("[i] Info")
       same_line()
       text("hover me anywhere in this row")
       same_line()
       small_button(HOVER_BTN, (text = "Action"))
   }
   if (IsItemHovered(ImGuiHoveredFlags.None)) {
       tooltip(GRP_TIP) {
           text("BeginTooltip while the whole group is hovered.")
       }
   }

Hovering the icon, the label, the button — any of them — fires the
tooltip. No need for ``IsItemHovered`` on each child or for ``Set...``
rectangle hacks. The group's bbox subsumes everything.

Note that ``HOVER_BTN`` is still independently clickable. Group hit-test
*reads* over the cluster but does NOT *block* child events.

The (label | value | button) row
================================

Compose three primitives into a logical row:

.. code-block:: das

   group(ROW_GRP) {
       text("Speed:")
       same_line()
       slider_float(ROW_SPEED, (text = "##speed"))
       same_line()
       small_button(ROW_RESET, (text = "Reset"))
   }

The group keeps the three on one line if it fits; even if a wider label
later forces a wrap, the group stays cohesive — subsequent ``same_line``
calls outside the group treat it as one unit. ``ROW_SPEED.value`` and
``ROW_RESET.click_count`` register under their respective paths
(``ROW_GRP/ROW_SPEED``, ``ROW_GRP/ROW_RESET``) for driver access.

Why no flags?
=============

ImGui's ``BeginGroup``/``EndGroup`` take no parameters. The wrapper's
only argument is the IDENT. ``GroupState`` carries an
``@optional _placeholder`` field so the ``[container]`` auto-emitter
has *something* to serialize — the field is unused and the JV payload
is ``{}``.

Standalone vs live
==================

Same convention as the other tutorials.

Driving from outside
====================

The group itself isn't open/close-driveable (nothing to open). Its
*children* are reachable at the composed path:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"GRP_WIN/ROW_GRP/ROW_SPEED","value":0.75}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_click","args":{"target":"GRP_WIN/ROW_GRP/ROW_RESET"}}' \
        localhost:9090/command

.. seealso::

   Full source: :download:`examples/tutorial/group.das <../../../examples/tutorial/group.das>`

   Features-side demo: ``examples/features/containers_window.das`` — uses
   ``group`` alongside ``child`` to lay out a window's right pane.

   Sibling: :ref:`tutorial_containers` — umbrella container tour.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
