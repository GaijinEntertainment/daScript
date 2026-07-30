.. _tutorial_dynamic_fonts:

#######################
Dynamic fonts
#######################

Dear ImGui 1.92 reworked font handling to **re-rasterize on demand**: changing
a font's scale no longer bilinear-stretches a fixed-size bitmap, it draws crisp
glyphs at the requested pixel size. dasImgui exposes that through two surfaces.

The whole-frame global scale lives on the style:

.. code-block:: das

   GetStyle().FontScaleMain = 1.5f   // every glyph this frame is 1.5x

``FontScaleMain`` is the 1.92 replacement for the old ``io.FontGlobalScale`` and
the **removed** ``SetWindowFontScale`` — set it before any text is submitted and
the entire frame scales.

The per-push pixel size lives on the ``with_font`` wrapper, which now forwards a
``size`` argument to ``PushFont``:

.. code-block:: das

   let f = GetFont()
   with_font(f, 16.0f) { text(SIZE_16, (text = "small")) }
   with_font(f, 32.0f) { text(SIZE_32, (text = "large")) }

So the *same* font renders at several sizes in one frame. ``size = 0.0f`` (the
default) keeps the current size, so ``with_font(f) { ... }`` stays a pure font
swap — existing call sites are unchanged.

Source: ``examples/tutorial/dynamic_fonts.das``.

************
Walkthrough
************

.. video:: dynamic_fonts.mp4

.. literalinclude:: ../../../examples/tutorial/dynamic_fonts.das
   :language: das
   :linenos:

Requires
========

Same baseline as the other ``with_*`` wrappers — ``with_font`` is already in
``imgui/imgui_scope_builtin`` (re-exported by ``imgui/imgui_boost_v2``). The
slider that feeds ``FontScaleMain`` uses the ``edit_*`` external-binding rail
plus ``daslib/safe_addr``.

Behaviour
=========

* **FontScaleMain** applies to the *whole frame*. Assign it once, before
  submitting widgets; it is not a stack like the ``with_*`` wrappers.
* **with_font(font, size)** scopes to its block exactly: ``PushFont(font, size)``
  on entry, ``PopFont()`` on exit. Pass ``size = 0.0f`` to re-use the current
  size and only swap the font face.
* Because 1.92 rasterizes per requested size, a large ``FontScaleMain`` or a
  large ``with_font`` size produces sharp glyphs rather than a magnified bitmap.

Migration note
==============

Pre-1.92 code that called ``io.FontGlobalScale = s`` or
``SetWindowFontScale(s)`` should move to ``GetStyle().FontScaleMain = s``.
``SetWindowFontScale`` no longer exists; the per-window scale is gone in favour
of the global style scale plus per-push ``with_font`` sizes.

Standalone vs live
==================

Same convention as the other tutorials. ``daslang.exe`` runs the demo
headlessly to frame N; ``daslang-live`` keeps the window open and reloads on
source edits — drag the slider to watch the whole frame rescale live.

.. seealso::

   Full source: :download:`examples/tutorial/dynamic_fonts.das <../../../examples/tutorial/dynamic_fonts.das>`

   Integration test: ``tests/integration/test_dynamic_font_scale.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
