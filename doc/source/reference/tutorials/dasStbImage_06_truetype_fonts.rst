.. _tutorial_dasStbImage_truetype_fonts:

==================================
STBIMAGE-06 — TrueType Fonts
==================================

.. index::
    single: Tutorial; TrueType
    single: Tutorial; load_ttf
    single: Tutorial; render_text
    single: Tutorial; dasStbImage

This tutorial covers the ``stbimage_ttf`` module — loading a TrueType
(``.ttf``) font, querying metrics, rasterizing glyphs, and rendering text
onto an image with software alpha blending.

Loading a Font
==============

``load_ttf`` reads a ``.ttf`` file and packs a range of glyphs (by default
ASCII 32..127) into a 1-channel atlas bitmap stored on the returned ``Font``.
The ``DroidSansMono`` font ships with dasStbImage; resolve its path through
``get_das_root()`` so the tutorial runs from any working directory:

.. code-block:: das

   require stbimage/stbimage_boost
   require stbimage/stbimage_ttf

   let font_file = "{get_das_root()}/modules/dasStbImage/fonts/droidsansmono.ttf"
   var font <- load_ttf(font_file, [pixel_height = 32.0])
   assert(is_valid(font))
   // font.bitmap is the packed atlas; font.char_range is (first_char, num_chars)

Use ``load_font`` instead when you only need metrics or glyph shapes and no
atlas.

Font Metrics
============

``font_metrics`` returns ascent, descent, and line height in **pixels** at the
font's loaded ``pixel_height``. ``font_vmetrics`` returns the same values in raw
**font units** (before scaling):

.. code-block:: das

   let m = font_metrics(font)
   // m.ascent, m.descent (negative), m.line_height  — all in pixels

   let vm = font_vmetrics(font)
   // vm.ascent, vm.descent, vm.line_gap  — in font units

Per-Glyph Metrics and Measuring Text
====================================

``codepoint_hmetrics`` gives one glyph's advance width and left side bearing
(in font units). ``measure_text`` sums advances to return a pixel width:

.. code-block:: das

   let hm = codepoint_hmetrics(font, 'A')
   // hm.advance_width, hm.left_side_bearing

   let width = measure_text(font, "Hello", 32.0)   // pixels

Rasterizing a Single Glyph
==========================

``codepoint_bitmap`` renders one glyph to a 1-channel (alpha) ``Image`` at a
scale computed from the desired pixel height, together with a pixel offset from
the origin:

.. code-block:: das

   let scale = scale_for_pixel_height(font, 32.0)
   var bm = codepoint_bitmap(font, 'A', scale, scale)
   // bm.image is 1-channel uint8; bm.offset is the int2 pixel offset
   bm.image |> with_pixels() $(var px : array<uint8>#) {
       // px[i] is the coverage/alpha of pixel i
   }

Rendering Text onto a Canvas
============================

``render_text`` blits the packed atlas glyphs onto a 4-channel RGBA ``Image``
at a baseline origin, in the given ``(r, g, b)`` color. Coordinates are clipped
to the destination bounds automatically:

.. code-block:: das

   var canvas = make_image(256, 64, 4)
   canvas.fill_rect(0, 0, 256, 64, 0xFFFFFFFFu)      // white background
   let m = font_metrics(font)
   render_text(canvas, font, "Hello, daslang!", 8, int(m.ascent) + 8, 0, 0, 0)
   let (ok, err) = canvas.save("text.png")

``render_text`` is built on ``blit_alpha`` (see
:ref:`tutorial_dasStbImage_drawing_and_blending`): the font atlas is a 1-channel
coverage image, and each glyph is alpha-blended onto the canvas.

.. seealso::

   Full source: :download:`tutorials/dasStbImage/06_truetype_fonts.das <../../../../tutorials/dasStbImage/06_truetype_fonts.das>`

   Previous tutorial: :ref:`tutorial_dasStbImage_drawing_and_blending`

   Next tutorial: :ref:`tutorial_dasStbImage_hdr`
