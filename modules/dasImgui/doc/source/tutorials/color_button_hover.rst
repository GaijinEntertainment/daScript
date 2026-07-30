.. _tutorial_color_button_hover:

#######################
Color button hover
#######################

``color_button`` is a click trigger — its ``ClickState`` records click
counts and the ``clicked`` per-frame flag. Some call sites want the
swatch's geometry only, with no click bookkeeping; the hover variant
covers that:

.. code-block:: das

   let hovered = color_button_hover(SWATCH,
       (desc_id = "##red", col = float4(0.85f, 0.20f, 0.20f, 1.0f),
        size = float2(96.0f, 64.0f), flags = ImGuiColorEditFlags.NoTooltip))
   if (hovered) {
       // per-frame side-effect, e.g. SetNextFrameWantCaptureMouse(...)
   }

``EmptyMarkerState`` backs the widget; the return value is the per-frame
hover bool (``ImGui::IsItemHovered()`` of the just-drawn swatch).

Source: ``examples/tutorial/color_button_hover.das``.

************
Walkthrough
************

.. video:: color_button_hover.mp4

.. literalinclude:: ../../../examples/tutorial/color_button_hover.das
   :language: das
   :linenos:

Requires
========

Same baseline as ``color_button`` — already in
``imgui/imgui_widgets_builtin`` (re-exported by ``imgui/imgui_boost_v2``).

When to reach for it
====================

When the swatch is a *surface*, not a *trigger*:

* ``examples/imgui_demo/inputs.das`` — the WantCapture-override panel uses
  a hover-target swatch to override ``io.WantCaptureMouse`` /
  ``io.WantCaptureKeyboard`` while the swatch is hovered. The
  ``ClickState`` of the regular ``color_button`` would force the caller
  to distinguish click vs hover.
* Style previews where moving the mouse over the swatch shows a tooltip
  or status line — the hover bool is the natural drive signal.

Click vs hover
==============

The two variants share the same call shape but pick the right state shape
for the call site:

* ``color_button(IDENT, (col, size, flags))`` — returns ``bool clicked``,
  records ``state.click_count`` and ``state.clicked``.
* ``color_button_hover(IDENT, (col, size, flags))`` — returns
  ``bool hovered``, no click bookkeeping.

A site that needs both (click triggers an action, hover updates a
preview) should use the regular ``color_button`` and read
``IsItemHovered()`` after the call — the post-item hover query is
allowed by the lint as a pure read.

Standalone vs live
==================

Same convention as the other widget tutorials.

.. seealso::

   Full source: :download:`examples/tutorial/color_button_hover.das <../../../examples/tutorial/color_button_hover.das>`

   Integration test: ``tests/integration/test_color_button_hover.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
