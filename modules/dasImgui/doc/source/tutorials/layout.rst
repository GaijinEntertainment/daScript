.. _tutorial_layout:

#######################
Layout
#######################

Three boost helpers — ``dock_left``, ``split_h``, ``split_v`` — compose into
an IDE-style layout on a single panel. Each helper takes a state struct, an
``init`` value, optional ``bounds``, and a block per pane; the resulting
splits are draggable at runtime and survive live reload.

Source: ``examples/tutorial/layout.das``.

************
Walkthrough
************

.. video:: layout.mp4

The recording drives the layout with **real synthetic drags** on the splitter
handles — no ``imgui_force_set``. It grabs the sidebar's right edge and pulls it
wider (asserting ``SIDEBAR``'s pixel width rose), drags the vertical handle
between the Files and Editor panes (left-pane fraction rose), then drags the
horizontal handle down so the editor grows and the output shrinks (top fraction
rose). Each handle surfaces its own bbox as a ``<container>/HANDLE`` alias in the
snapshot, so the cursor targets the real 8 px ``InvisibleButton``; a drag that
moved nothing aborts the recording at teardown.

.. literalinclude:: ../../../examples/tutorial/layout.das
   :language: das
   :linenos:

Requires
========

Same backend + boost layer as :ref:`tutorial_widgets_tour`, with two extra
modules pulled in:

* ``imgui/imgui_containers_builtin`` — the ``window`` macro that wraps the
  layout (boost ``window`` is more featureful than raw ``Begin``/``End``;
  it provides ``closable``, ``flags``, registered state, and an implicit
  ``End`` at block exit).
* ``imgui/imgui_layout_builtin`` — the layout helpers themselves
  (``split_h``, ``split_v``, ``dock_left``).

Init and shutdown
=================

Identical to :ref:`tutorial_widgets_tour`. 1024x720 window, font scale 1.5,
standard live-reload pair.

The frame loop
==============

Standard dasImgui v2 shape. ``apply_synth_io_override()`` between
``ImGui_ImplGlfw_NewFrame`` and ``NewFrame`` is required again so the
driver script can synth drags against the splitters without the real GLFW
mouse winning the IO race.

Layout helpers
==============

The panel composes three helpers, nested:

.. code-block:: das

   window(LAYOUT_WIN, (text = "IDE layout", ...)) {
       dock_left(SIDEBAR, (init = 200.0f, bounds = (80.0f, 320.0f))) {
           // sidebar contents
       }
       split_v(SPLIT_VERT, (init = 0.65f, bounds = (0.1f, 0.9f)),
           ${ split_h(SPLIT_MAIN, ...) { ... } },
           ${ /* bottom pane */ })
   }

``dock_left`` carves a fixed-width column off the left edge. Its
``state.value`` is the pane width in **pixels** — clamped to the ``bounds``
tuple at drag time. After ``dock_left``'s block, ImGui's cursor is on the
``SameLine`` to the right of the rail, so subsequent content flows into the
remaining area without explicit positioning.

``split_v`` and ``split_h`` each take **two block arguments** rather than
one. The ``${ ... }`` literal is a block-with-no-args, and the boost macro
takes the helpers' panes by position. Their ``state.value`` is the **first
pane's fraction of the available space**, clamped to ``bounds`` (so
``init = 0.65f`` means the top half occupies 65% of the height).

Standalone vs live
==================

Same as :ref:`tutorial_widgets_tour` — ``main()`` runs the loop standalone;
``daslang-live`` invokes ``init`` / ``update`` / ``shutdown`` directly.

Driving from outside
====================

Every helper's state struct is targetable by name. Drag without a mouse:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"LAYOUT_WIN/SIDEBAR","value":260.0}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"LAYOUT_WIN/SPLIT_VERT/SPLIT_MAIN","value":0.6}}' \
        localhost:9090/command

``value`` is the same type the user would set by dragging — pixels for
``dock_*``, fraction for ``split_*``. Targets are path-qualified by the
enclosing ``window``/``split`` chain (no bare ``SIDEBAR``).

Each helper also exposes its drag handle's geometry as a ``<container>/HANDLE``
alias in the snapshot, so a driver can perform a **real** drag instead of
setting the fraction directly — ``LAYOUT_WIN/SIDEBAR/HANDLE`` and likewise for
each split. The ``drag`` playwright helper targets it by bbox centre; this is
how the recording above drives every split.

Scope wrappers
==============

Three stateless block-arg wrappers in ``imgui/imgui_scope_builtin`` cover
the leftover Push/Pop idioms ImGui uses for ad-hoc layout overrides. Each
brackets the block with the corresponding ImGui Push/Pop pair and takes
no state — they read like inline scopes:

.. code-block:: das

   require imgui/imgui_scope_builtin

   // Indent / Unindent — nest content under a heading.
   with_indent(0.0f) {       // 0.0f defers to style IndentSpacing
       Text("Bullet child")
   }
   with_indent(40.0f) {      // explicit pixel offset
       Text("Hard-indented")
   }

   // PushItemWidth / PopItemWidth — scope a widget-width override.
   with_item_width(120.0f) {
       slider_float(NARROW, (text = "narrow", bounds = (0.0f, 1.0f)))
   }
   with_item_width(-60.0f) { // negative = right-edge minus N
       slider_float(STRETCH, (text = "stretch", bounds = (0.0f, 1.0f)))
   }

   // PushTextWrapPos / PopTextWrapPos — scope where long text wraps.
   with_text_wrap_pos(0.0f) { TextUnformatted(LIPSUM) }   // window right edge
   with_text_wrap_pos(200.0f) { TextUnformatted(LIPSUM) } // wrap at 200 px

Feature demos: ``examples/features/with_indent.das``,
``examples/features/with_item_width.das``,
``examples/features/with_text_wrap_pos.das``.

Next steps
==========

Docking is next — full ImGui dockspaces and the dock helpers that ride on
top of them.

.. seealso::

   Full source: :download:`examples/tutorial/layout.das <../../../examples/tutorial/layout.das>`

   Richer reference: ``examples/features/layout_helpers.das`` — same three
   helpers exercised with every option.

   Previous tutorial: :ref:`tutorial_widgets_tour`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.

   :ref:`Builtin widgets <stdlib_imgui_widgets_section>` — widget reference.
