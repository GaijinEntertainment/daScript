.. _tutorial_icons:

#######################
Icons
#######################

``imgui_icons`` is an asset-free vector icon set: every glyph is geometry drawn
straight onto the ImGui draw list — no SVG, no texture atlas, no font merge.
Glyphs are authored on a logical 24x24 grid and tinted from the daslang amber
accent, so they scale to any size and follow the theme. Two entry points:

* ``icon_button(IDENT, (glyph = "play", tip = "Play"))`` — a clickable, themed
  icon button. It uses ``ClickState`` like any button (``state.click_count``
  accumulates, snapshot kind ``icon_button``), and a non-empty ``tip`` shows a
  hover tooltip.
* ``icon("waveform", 24.0f)`` — draw a glyph at the cursor with no interaction,
  then advance the cursor. Use it for inline chrome, status rows, or labels.

Glyphs are addressed by name. The whole set is enumerable with ``icon_count()``
/ ``icon_name(i)`` / ``icon_category(i)`` — enough to build an in-app icon
picker. The full visual catalog (every glyph, by category) is the
:doc:`Icon set <../stdlib/generated/imgui_icons>` reference page.

Source: ``examples/tutorial/icons.das``.

************
Walkthrough
************

.. video:: icons.mp4

.. literalinclude:: ../../../examples/tutorial/icons.das
   :language: das
   :linenos:

Requires
========

* ``imgui/imgui_icons`` — the icon set: ``icon``, ``icon_button_draw``, and the
  catalog accessors.
* ``imgui/imgui_widgets_builtin`` — the ``icon_button`` ``[widget]`` (themed,
  snapshot-visible, playwright-drivable wrapper over ``icon_button_draw``).

icon_button — clickable
=======================

``icon_button`` is a registered ``[widget]``: it carries ``ClickState`` and
behaves exactly like ``button``, so it works with the snapshot, the playwright
``click`` verb, and click-count verification.

.. code-block:: das

   icon_button(IC_PLAY, (glyph = "play",  tip = "Play"));  same_line(IC_SL1)
   icon_button(IC_REC,  (glyph = "record", tip = "Record"))
   text("play clicks = {IC_PLAY.click_count}")

``glyph`` is the icon name; ``tip`` (optional) shows a hover tooltip; ``size``
(px, the glyph box side) and ``weight`` (stroke scale) round out the call. Pass
the args in declared order — the indexed call form is positional.

icon() — draw only
==================

When there is nothing to click, ``icon`` draws a glyph inline:

.. code-block:: das

   icon("waveform", 20.0f); same_line()
   icon("waveform", 32.0f); same_line()
   icon("waveform", 48.0f)

Because the glyph is geometry (not a rasterized image), every size is crisp —
there is no atlas resolution to outgrow. ``icon`` advances the cursor by the
box size, so it lays out like any other item.

Enumerating the set
====================

The catalog is data, so it is enumerable at runtime:

.. code-block:: das

   for (i in range(icon_count())) {
       text("{icon_name(i)} ({icon_category(i)})")
   }

This is how the documentation catalog page is generated, and it is the same
loop you would write to populate an icon picker.

Drawing without a button
========================

``icon_button`` is the wrapper most apps want, but two lower-level entry points
exist for custom chrome:

* ``icon(name, size, weight)`` — draw-only, shown above.
* ``icon_button_draw(id, glyph, size, weight)`` — a clickable button that is
  **not** a registered widget (no snapshot path). Use it only for transient
  chrome that no test drives by name; otherwise prefer the ``icon_button``
  widget.

.. seealso::

   Full source: :download:`examples/tutorial/icons.das <../../../examples/tutorial/icons.das>`

   Visual catalog: :doc:`Icon set <../stdlib/generated/imgui_icons>` — every
   glyph, grouped by category, with its name.

   Regenerate the catalog images after adding a glyph:
   ``daslang -project_root . utils/make_icon_doc.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
