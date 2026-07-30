.. _tutorial_buttons:

#########
Buttons
#########

Six shapes of click trigger. All six use ``ClickState``;
``state.clicked`` is the per-frame bool, ``state.click_count``
accumulates across frames.

.. code-block:: das

   button(IDENT, (text = ".."))                       // the workhorse
   small_button(IDENT, (text = ".."))                 // no frame padding
   arrow_button(IDENT, (text = "..", dir = ...))      // triangle glyph
   invisible_button(IDENT, (text = "..", size = ...)) // hit area, no render
   image_button(IDENT, (text = "..",                  // textured button
                        user_texture_id = tex_id,
                        size = ...))
   tab_item_button(IDENT, (text = "..",               // button styled as tab,
                           flags = ...))              //   inside a tab_bar

Source: ``examples/tutorial/buttons.das``.

************
Walkthrough
************

.. video:: buttons.mp4

.. literalinclude:: ../../../examples/tutorial/buttons.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — every ``*_button`` rail.
* ``imgui/imgui_containers_builtin`` — ``tab_bar`` / ``tab_item``
  for the ``tab_item_button`` host.
* ``imgui/imgui_boost_runtime`` — ``ClickState`` (shared across all
  six widgets).

button — the workhorse
======================

The default click trigger. Returns ``true`` on the frame the user
clicked; ``state.click_count`` tallies across frames:

.. code-block:: das

   if (button(SAVE, (text = "Save"))) {
       // one-shot handler — runs once per click
   }
   // OR drive on accumulated count:
   text("saves: {SAVE.click_count}")

``size = (0, 0)`` lets ImGui auto-size to the label. Pass an explicit
size for fixed-width toolbar rows.

small_button — zero padding
===========================

Same widget shape; ``SmallButton()`` drops the frame padding. Use for
inline tool strips that need to feel like part of the surrounding
text:

.. code-block:: das

   text("Status: ")
   same_line()
   small_button(REFRESH, (text = "refresh"))

No size arg — small_button always auto-sizes.

arrow_button — directional glyph
================================

A triangle pointing in one of four directions. ``dir : ImGuiDir`` picks
the direction:

.. code-block:: das

   arrow_button(BACK, (text = "##back", dir = ImGuiDir.Left)); same_line()
   arrow_button(FWD,  (text = "##fwd",  dir = ImGuiDir.Right))

The ``text`` is the ImGui label (mostly ``"##name"`` to suppress visible
text — the glyph carries the meaning). Each call needs its own IDENT
(separate ``ClickState``).

invisible_button — hit area only
================================

Renders nothing, but captures clicks inside its bbox. The canonical
use: overlay a hit area on top of a drawlist scene, image, or custom
glyph:

.. code-block:: das

   invisible_button(HOTSPOT, (text = "##hotspot",
                              size = float2(120.0f, 24.0f)))
   if (HOTSPOT.clicked) {
       // capture click on the scene below
   }

Requires an explicit ``size`` — there's no glyph to derive bounds from.
ImGui still tracks hover and click via ``IsItemHovered()`` /
``IsItemClicked()``, so it composes with the rest of the imgui
interaction stack.

image_button — textured trigger
===============================

Renders a texture as the button face. ``user_texture_id : void?`` is
ImGui's opaque texture handle — on GL it's the GL texture id cast to
``void?``. The font atlas (``io.Fonts.TexID``) is always available and
makes for a no-setup demo target:

.. code-block:: das

   let io & = unsafe(GetIO())
   let font_tex = io.Fonts.TexID
   if (font_tex != null) {
       image_button(BTN, (text = "##font",
                          user_texture_id = font_tex,
                          size = float2(48.0f, 48.0f)))
   }

Real apps load their own textures via ``stbi`` or
``LoadTextureFromFile`` helpers (see ``examples/imgui_demo/widgets.das``
for the 8-button image-grid pattern). ``uv0`` / ``uv1`` slice into the
texture; ``bg_col`` / ``tint_col`` modulate the rendered face.

tab_item_button — button styled as a tab
========================================

A click trigger that visually matches sibling ``tab_item`` headers.
Must live inside a ``tab_bar`` body. Flag with ``Leading`` to anchor
to the start of the bar, ``Trailing`` to the end — the canonical
``?`` / ``+`` row:

.. code-block:: das

   tab_bar(BAR, (text = "MyTabBar",
                  flags = ImGuiTabBarFlags.FittingPolicyResizeDown)) {
       if (tab_item_button(HELP, (text = "?",
                                   flags = ImGuiTabItemFlags.Leading |
                                           ImGuiTabItemFlags.NoTooltip))) {
           // open help popup
       }
       if (tab_item_button(ADD, (text = "+",
                                  flags = ImGuiTabItemFlags.Trailing |
                                          ImGuiTabItemFlags.NoTooltip))) {
           // push a new entry
       }
       tab_item(TAB_A, (text = "alpha", closable = false,
                        flags = ImGuiTabItemFlags.None)) {
           text(BODY_A, (text = "tab body"))
       }
   }

Unlike ``tab_item`` it has no content pane (no block body) — it's a
trigger only. See :ref:`tutorial_tab_bar` for the regular tab pattern.

Driving from outside
====================

Every ``*_button`` accepts ``imgui_click`` and snapshot probes:

.. code-block:: bash

   # Plain button
   curl -X POST -d '{"name":"imgui_click","args":{"target":"BT_WIN/BT_GO"}}' \
        localhost:9090/command
   # Tab-bar-nested button — path includes the bar segment
   curl -X POST -d '{"name":"imgui_click","args":{"target":"BT_WIN/BT_BAR/BT_HELP"}}' \
        localhost:9090/command

Caller-owned variant
====================

For sites where the click target lives on an external bool, use
``edit_button`` from ``imgui_boost_runtime`` (see
:ref:`tutorial_edit_external_tour`).

.. seealso::

   Full source: :download:`examples/tutorial/buttons.das <../../../examples/tutorial/buttons.das>`

   Features-side demo: ``examples/features/triggers.das`` — every
   ClickState widget in one window, useful for ``imgui_click`` smoke
   testing.

   Sibling tutorials: :ref:`tutorial_color` (color_button is a special
   button variant), :ref:`tutorial_tab_bar` (where tab_item_button
   lives).

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
