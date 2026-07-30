.. _tutorial_narrative_layout_tour:

###############################
Narrative + layout primitives
###############################

A one-window tour of the read-only widget families: narrative text,
layout primitives, display widgets, and the flat tooltip form. Each
widget renders into a single panel sized for an APNG recording; the
driver moves a cursor across them with a spoken line per group.

Source: ``examples/tutorial/narrative_layout_tour.das``.

.. video:: narrative_layout_tour.mp4

The recording is voiced and self-verifying: most of these widgets take no
input, so each beat narrates a group while the cursor points and asserts the
widget rendered and echoed its string. The final beat rests the cursor on the
button and asserts the flat tooltip actually fired. A missing or wrong value
aborts the recording at teardown instead of shipping.

What the tour shows
===================

The widgets, in order of appearance:

* ``text(IDENT, (text = "..."))`` — one-line, no wrap. Telemetry
  carries the source string.
* ``text_wrapped(IDENT, (text = "..."))`` — reflows to the window's
  right edge; the snapshot still carries the full unwrapped string.
* ``text_colored(IDENT, (color = float4(...), text = "..."))`` —
  arbitrary RGBA tint.
* ``text_disabled(IDENT, (text = "..."))`` — the ImGui disabled-text
  color (greyed-out hint style).
* ``bullet_text(IDENT, (text = "..."))`` — bullet marker + text in a
  single call.
* ``label_text(IDENT, (key = "Version", value = "v2.0-detour"))`` —
  ImGui's two-string form: value at the cursor, label to its right.
* ``separator_text(IDENT, (text = "..."))`` — inline section header.
  Paired with ``dummy((size = float2(0, 8)))`` for inter-section gap.
* ``progress_bar(IDENT, (fraction, size, overlay))`` — driven by a
  per-frame counter in ``update()``.
* ``button(HOVER_TIP, ...)`` + ``set_item_tooltip(IDENT, (text = "..."))``
  — the flat tooltip form. Fires on hover over the previous item; no
  ``[container]`` body needed.

Driving the tour
================

Standalone:

.. code-block:: bash

   daslang modules/dasImgui/examples/tutorial/narrative_layout_tour.das

Live (with reload):

.. code-block:: bash

   daslang-live modules/dasImgui/examples/tutorial/narrative_layout_tour.das

The standalone form opens a window with the static layout. The driver
under ``tests/integration/record_narrative_layout_tour.das`` spawns it,
speaks a line per group while pointing the cursor, and verifies each
widget — see :doc:`recording`.

Idents matter for recordings
============================

Each widget takes an explicit ``IDENT`` (``INTRO``, ``WRAP``, etc.).
Without one, the ``[widget]`` macro falls back to a path keyed by
source-file line/column (``TOUR_WIN/:66:8``) — those resolve fine for
in-process telemetry, but break the moment the file is edited and a
recording driver tries to ``widget_center(snap, "TOUR_WIN/WRAP")``.
Stable idents keep the driver-script paths file-edit-resistant.

.. seealso::

   Full source: :download:`examples/tutorial/narrative_layout_tour.das <../../../examples/tutorial/narrative_layout_tour.das>`

   Recording driver: ``tests/integration/record_narrative_layout_tour.das``.

   :ref:`Containers <tutorial_containers>` — the ``[container]`` family
   counterpart (these are flat ``[widget]`` calls; no body).

   :ref:`Recording <tutorial_recording>` — the two-shell driver setup
   that produces APNGs like the one above.
