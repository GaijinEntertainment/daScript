.. _tutorial_display_widgets:

#######################
Display widgets
#######################

dasImgui's read-only display family wraps ImGui's two main *output* widgets
into the v2 boost surface. ``progress_bar`` wraps ``ImGui::ProgressBar``
with a ``ProgressBarState`` payload carrying ``{fraction, size, overlay}``;
``image`` wraps ``ImGui::Image`` with the full ``uv0`` / ``uv1`` /
``tint_col`` / ``border_col`` quartet plus a ``uint64`` reinterpret of the
texture handle for snapshot readability.

.. code-block:: das

   progress_bar(PB_STATIC, (fraction = 0.33f,
                            size = float2(-1.0f, 0.0f),
                            overlay = "33%"))

   image(IMG_PLAIN, (user_texture_id = font_tex,
                     size = float2(96.0f, 96.0f),
                     uv0 = float2(0.0f, 0.0f),
                     uv1 = float2(1.0f, 1.0f),
                     tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f),
                     border_col = float4(0.0f, 0.0f, 0.0f, 0.0f)))

Display widgets are read-only, so they don't *need* a caller-side ident — call
them anonymously and only the parent ``window(DISPLAY_WIN, ...)`` registers a
routable entry. This example names each one (``PB_STATIC`` / ``PB_DRIVEN`` /
``PB_FIXED`` / ``IMG_PLAIN`` / ``IMG_TINT``) so every widget registers its own
snapshot path — which is what lets the recording assert each one's output (the
static bar on screen, the driven bar's fraction sweeping, the images present).

Source: ``examples/tutorial/display_widgets.das``.

************
Walkthrough
************

.. video:: display_widgets.mp4

The recording narrates the three progress bars and two images while the middle
bar's fraction sweeps under a sine wave. These widgets take no input, so the
self-check is on their output: the static bar and both images are asserted on
screen, and the driven bar's ``fraction`` is asserted to *change* over time -
so a bar that stopped rendering or a sine that stopped sweeping would abort the
recording.

.. literalinclude:: ../../../examples/tutorial/display_widgets.das
   :language: das
   :linenos:

Requires
========

Baseline boost layer (``imgui/imgui_boost_v2`` re-exports the rail family
from ``imgui/imgui_widgets_builtin``). No extra modules.

progress_bar
============

``ProgressBarState`` mirrors the call-site values verbatim. ``fraction``
holds the **raw** call-site value (no clamp at the state level); ImGui
clamps to ``[0, 1]`` for the rendered bar, but the snapshot payload
reflects whatever the caller passed — so out-of-range inputs surface in
the snapshot for assertion. Pass a negative value for indeterminate
animation. ``size`` is the ImGui ``ImVec2``: ``-1.0f`` for the X
component fills available width; explicit pixels for fixed width.
``overlay`` is the centered label drawn on top of the bar — leave empty
for the auto-formatted percentage.

image
=====

``ImageState`` exposes ``user_texture_id`` as a ``uint64`` so the snapshot
can carry the texture handle as a readable number (raw ``void?`` would
serialize as a pointer string). All four ``uv0`` / ``uv1`` /
``tint_col`` / ``border_col`` defaults match the C++ ``ImGui::Image``
defaults so an unset call is the identity render. The tutorial uses the
ImGui font atlas (``GetIO().Fonts.TexID``) as a guaranteed-available
texture; production code passes a ``user_texture_id`` from your renderer
that targets a real GPU resource.

Snapshot shape
==============

Each named widget registers its own snapshot entry, so a rail's state is
readable directly. Probe with:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."DISPLAY_WIN/PB_DRIVEN".payload'

That returns the driven bar's ``{fraction, size, overlay}`` — useful for
snapshot-driven regression tests when you want to assert ``PB_DRIVEN``'s
fraction matches the sine-driven value (the recording does exactly that, via
``record_check_changed``). Left anonymous, a widget folds into a line-keyed
entry under the window instead.

.. seealso::

   Full source: :download:`examples/tutorial/display_widgets.das <../../../examples/tutorial/display_widgets.das>`

   Integration test: ``tests/integration/test_display_progress.das`` and ``tests/integration/test_display_image.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
