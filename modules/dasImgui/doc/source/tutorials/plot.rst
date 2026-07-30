.. _tutorial_plot:

######
Plot
######

Four read-only sample plots. Two **array-form** widgets take a
per-frame ``array<float>`` and copy it synchronously; two
**lambda-form** widgets call back once per sample, skipping the
backing array entirely.

.. code-block:: das

   plot_lines(IDENT, "title", values, scale_min, scale_max, size)
   plot_histogram(IDENT, "title", values, scale_min, scale_max, size)
   plot_lines_getter(IDENT, "title", count,
                     @(idx : int) : float => ...,
                     overlay, scale_min, scale_max, size)
   plot_histogram_getter(IDENT, "title", count,
                         @(idx : int) : float => ...,
                         overlay, scale_min, scale_max, size)

All four share ``PlotState`` — title + samples round-trip into the
snapshot so visual regression tests can diff the data.

Source: ``examples/tutorial/plot.das``.

************
Walkthrough
************

.. video:: plot.mp4

.. literalinclude:: ../../../examples/tutorial/plot.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_widgets_builtin`` — both array and lambda rails.
* ``imgui/imgui_boost_runtime`` — ``PlotState``.

Array form vs lambda form
=========================

* **Array** (``plot_lines`` / ``plot_histogram``) — pass a
  ``values : array<float>``. ImGui copies element-by-element inside
  the C call. Use when you already have a backing buffer (telemetry
  ring, captured samples, table column).
* **Lambda** (``*_getter``) — pass ``values_count`` and a
  ``getter : lambda<(idx : int) : float>``. The callback fires once
  per sample, synchronously inside the C call. Use for synthesized
  signals (sine, noise, generated frequency response) or virtualized
  buffers where building an array would waste work.

Both forms are O(n) per frame — the choice is about whether you want
to allocate the array or generate on the fly.

Auto-fit vs explicit bounds
===========================

``scale_min`` and ``scale_max`` default to ``FLT_MAX`` (3.4e38), which
ImGui treats as "auto-fit to the data this frame." That's tempting,
but the bounds **change every frame** — the plot stretches/compresses
as the data range shifts, which reads as jitter:

.. code-block:: das

   // Auto-fit — bounds drift per frame:
   plot_lines(LATENCY, "ms", samples)               // FLT_MAX defaults

   // Explicit bounds — calm plot, comparable across frames:
   plot_lines(LATENCY, "ms", samples,
              0.0f, 200.0f, float2(0.0f, 80.0f))    // 0..200 ms y-axis

Use auto-fit only when you don't know the data range and don't care
about per-frame stability. For anything you'll watch over time,
explicit bounds.

Size
====

The last arg is the plot bbox in pixels — ``float2(width, height)``.
``width = 0.0f`` lets ImGui auto-size to the available content width;
``height = 0.0f`` falls back to a small default. Pass ``80.0f`` or
``120.0f`` for the height to keep two plots visible side-by-side.

The overlay arg
===============

Lambda-form widgets take an extra ``overlay_text : string`` arg before
``scale_min``. Pass ``""`` for no overlay, or a status string that
prints centered on top of the plot:

.. code-block:: das

   plot_lines_getter(FRAMETIME, "ms/frame", 60,
                     @(idx : int) : float => sample_at(idx),
                     "avg = {avg_ms:.2f}",
                     0.0f, 33.0f, float2(0.0f, 80.0f))

Useful for the "rolling average" or "max" annotation under the line.

Driving from outside
====================

PlotState exposes ``title`` + ``samples`` in the snapshot payload —
no ``imgui_force_set`` channel (the samples are caller-pushed). Snapshot
probes are the right tool:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."PL_WIN/PL_LINES".payload'

The samples array gives visual-regression tests something to diff
without screenshotting.

.. seealso::

   Full source: :download:`examples/tutorial/plot.das <../../../examples/tutorial/plot.das>`

   Features-side demos: ``examples/features/plot_widgets.das``
   (array form), ``examples/features/plot_getter.das`` (lambda form).

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
