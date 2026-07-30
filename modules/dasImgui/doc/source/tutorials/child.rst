.. _tutorial_child:

#######################
Child windows
#######################

``child`` brackets ``BeginChild``/``EndChild`` into a block-arg container —
a sub-window that can be sized, bordered, scrolled, and addressed in the
registry hierarchy. The wrapper's signature:

.. code-block:: das

   child(IDENT, (text = "...",
                 size = float2(w, h),
                 child_flags = ImGuiChildFlags....,
                 window_flags = ImGuiWindowFlags....)) {
       // body — leaves register under <window>/IDENT
   }

Three knobs do most of the work — ``size``, ``child_flags``, ``window_flags``
— and the boost layer captures ``scroll`` / ``scroll_max`` each frame so a
snapshot reports the current scroll position without polling.

Source: ``examples/tutorial/child.das``.

************
Walkthrough
************

.. video:: child.mp4

The recording drives each of the three children with real synthetic input
and self-verifies. It parks the cursor inside the bordered child **A** and
wheel-scrolls it, asserting ``SCROLL_A``'s ``scroll`` payload actually
moved; toggles the ``VSync`` checkbox inside the AutoResizeY child **B**;
then wheel-scrolls the horizontal child **C** sideways, asserting
``SCROLL_C``'s ``scroll`` moved. The scroll assertions read the boost
layer's per-frame ``scroll`` telemetry directly — a no-op scroll (cursor
not over the child, wheel not attributed) would abort the recording.

.. literalinclude:: ../../../examples/tutorial/child.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — defines ``child``,
  ``child_scroll_reenter``.
* ``imgui/imgui_widgets_builtin`` — ``text``, ``checkbox``, ``slider_int``.

size — three sizing modes
=========================

The ``size : float2(w, h)`` argument has three flavors:

* **Explicit positive** — ``size = float2(360.0f, 140.0f)`` — fixed
  pixel size. The body's content is clipped to that region and scrollbars
  appear if it overflows.
* **Zero** — ``size = float2(0.0f, 0.0f)`` — auto-fit to the available
  column/row. Pair with ``ImGuiChildFlags.AutoResizeX/Y`` to let the body
  grow to the content extent instead.
* **Negative** — ``size = float2(-FLT_MIN, 0.0f)`` — "fill the remaining
  space minus that many pixels". Useful for stretchy panes that should fit
  the parent.

child_flags vs window_flags
===========================

``ImGuiChildFlags`` (the third arg) is child-specific:

* ``Border`` — draw a one-pixel border around the region.
* ``AutoResizeX`` / ``AutoResizeY`` — height/width tracks content extent.
* ``AlwaysAutoResize`` — combine both axes and re-measure every frame.
* ``FrameStyle`` — frame-style chrome (like input groups). Implies a
  background and rounded corners drawn from the active style.
* ``NavFlattened`` — keyboard nav treats the child as part of the parent.

``ImGuiWindowFlags`` (the fourth arg) governs the child's scrollbar
policy and window-level behavior:

* ``HorizontalScrollbar`` — enable the bottom scroll track when content
  overflows horizontally (default = none).
* ``AlwaysVerticalScrollbar`` / ``AlwaysHorizontalScrollbar`` — keep
  the bar visible even when the content fits.
* ``NoMove`` / ``NoResize`` / ``NoBackground`` — standard window knobs.

The two flag sets compose freely; pick from each based on the child's
chrome and scroll needs.

ChildState — what the snapshot reports
======================================

``ChildState`` captures four floats:

.. code-block:: das

   var SCROLL_A : ChildState
   // After the frame renders:
   //   SCROLL_A.size       : float2  // sticky — what was passed in
   //   SCROLL_A.scroll     : float2  // current scroll (GetScrollX/Y)
   //   SCROLL_A.scroll_max : float2  // GetScrollMaxX/Y for this content
   //   SCROLL_A.child_flags / SCROLL_A.window_flags  // sticky

Snapshot reports them under ``<window>/SCROLL_A``. Drivers that want to
verify "the user scrolled to row 12" compare ``scroll.y`` against a
known threshold. ``scroll_max`` is read AFTER the body invoke — it
depends on the content extent, which the body just emitted.

Auto-resize variants
====================

The B child uses ``AutoResizeY | FrameStyle``:

.. code-block:: das

   child(AUTO_B, (text = "auto_b",
                  size = float2(360.0f, 0.0f),
                  child_flags = ImGuiChildFlags.AutoResizeY | ImGuiChildFlags.FrameStyle,
                  window_flags = ImGuiWindowFlags.None)) {
       text("Three rows of input chrome")
       checkbox(B_VSYNC, (text = "VSync"))
       slider_int(B_LIMIT, (text = "frame limit"))
   }

Size's Y is ``0.0f`` — required when ``AutoResizeY`` is active so ImGui
treats the slot as auto. The height grows to fit the body each frame.

``scroll`` and ``scroll_max`` both report (0, 0) for an AutoResize child:
no overflow means no scroll range. ``ChildState.size`` is the
caller-provided input — for AutoResizeY it stays at ``(360.0f, 0.0f)``,
so it does NOT tell you the rendered height. The wrapper does not
currently expose the auto-computed extent; if you need it, call
``GetWindowSize()`` inside the child body, or place a sentinel widget
at the end and read its bbox bottom from the snapshot.

Horizontal scroll
=================

``window_flags.HorizontalScrollbar`` enables the bottom track:

.. code-block:: das

   child(SCROLL_C, (text = "scroll_c",
                   size = float2(720.0f, 90.0f),
                   child_flags = ImGuiChildFlags.Border,
                   window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
       text(LONG_LINE, (text = "very wide content: lorem ipsum..."))
   }

Without the flag, overlong single-line text would be clipped. With it,
the user drags the bottom bar and ``SCROLL_C.scroll.x`` mirrors the
position.

Standalone vs live
==================

Same convention as the other tutorials.

Driving from outside
====================

External drivers can wheel-scroll the active child by posting an
``imgui_mouse_scroll`` command (the live counterpart to
``ImGui_ImplGlfw``'s wheel events):

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_mouse_scroll","args":{"x":0.0,"y":-3.0}}' \
        localhost:9090/command

The cursor must be over the child window for ImGui to attribute the
scroll. The recording driver uses this exact channel to nudge
``SCROLL_A`` between narration stages.

.. seealso::

   Full source: :download:`examples/tutorial/child.das <../../../examples/tutorial/child.das>`

   Features-side demo: ``examples/features/child_scroll_reenter.das`` — the
   ``child_scroll_reenter`` helper for nudging an existing child's scroll
   from outside its block.

   Sibling: :ref:`tutorial_containers` — the umbrella tour of container
   rails.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
