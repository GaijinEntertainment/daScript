.. _tutorial_wrap_tab_bar:

#######################
Wrapping tab bar
#######################

ImGui's native ``tab_bar`` keeps its headers on one row: when they overflow the
width it shrinks and ellipsizes the labels, or scrolls them behind arrows. It
never wraps. ``wrap_tab_bar`` / ``wrap_tab_item`` are the container that does —
the header strip flows onto as many rows as the width needs, and only the active
tab's body is drawn.

.. code-block:: das

   wrap_tab_bar(WTB_BAR) {
       wrap_tab_item("physics") { text("...") }
       wrap_tab_item("camera")  { text("...") }
       // ... as many as you like; they wrap across rows
   }

Three things make it tick:

* **The active tab is an INDEX** (like ``combo``), carried in the container's
  ``value`` payload. ``imgui_force_set {"value":N}`` selects tab N
  programmatically; a **real click on a header** selects it interactively (see
  below).
* **Only the active tab's body renders.** Each ``wrap_tab_item`` invokes its
  block only while its index is active — an inactive tab's leaves aren't
  submitted to ImGui, so a snapshot shows them ``rendered:false``.
* **The strip is laid out a frame behind.** The body renders after the whole
  header strip, so the strip uses the labels collected on the *previous* frame.
  A changed label set appears one frame late (never visible for a stable list).
  Keep tab declaration order stable.

Source: ``examples/tutorial/wrap_tab_bar.das``.

************
Walkthrough
************

.. video:: wrap_tab_bar.mp4

.. literalinclude:: ../../../examples/tutorial/wrap_tab_bar.das
   :language: das
   :linenos:

Requires
========

* ``imgui/imgui_harness`` — the tutorial uses the ``harness_*`` lifecycle.
  ``wrap_tab_bar`` / ``wrap_tab_item`` live in
  ``imgui/imgui_containers_builtin`` (pulled in by the harness).

Selecting a tab by name
=======================

Each header is a real button, and the strip registers it as a clickable alias
under the bar's path — ``WTB_WIN/WTB_BAR/<label>`` — so a driver, test, or
recording can select a tab with a **real synthetic click, by name**, exactly
like a user clicking the header:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_click","args":{"target":"WTB_WIN/WTB_BAR/lava"}}' \
        localhost:9090/command

The click drives ImGui's own input path into the header button, which sets the
active index — no different from a mouse click. This is the do-what-it-teaches
form the tutorial recording uses (clicking ``lava``, then ``grid`` on the second
row, then back to ``physics``), and each switch is verified by the selected
tab's body appearing. The index is also settable directly with
``imgui_force_set {"value":N}`` when you want to drive it without a gesture.

Why an index, not ImGui's tab state
====================================

Native ``tab_bar`` lets ImGui own selection internally; the boost wrapper only
observes it. ``wrap_tab_bar`` is drawn from primitives (a wrapped strip of
buttons), so it owns its own active index as plain state — which is why it can
expose ``value`` for snapshot + ``imgui_force_set``, and why each header can be
a named click target. The trade is that it is not ImGui's native tab visual; it
is the right tool when you have more tab categories than fit one row.

Standalone vs live
==================

Same convention as the other tutorials.

.. seealso::

   Full source: :download:`examples/tutorial/wrap_tab_bar.das <../../../examples/tutorial/wrap_tab_bar.das>`

   Sibling: :ref:`tutorial_tab_bar` — ImGui's native single-row tab bar, with
   closable tabs and ``edit_tab_item`` bidirectional binding.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
