.. _tutorial_layout_primitives:

#######################
Layout primitives
#######################

ImGui's layout cursor advances after every widget — each ``button``,
``text``, or other rail bumps the cursor to the next line by default.
Four boost rails move the cursor *without* rendering anything visible:

* ``same_line(IDENT)`` — keep the next widget on the current row (wraps
  ``ImGui::SameLine``).
* ``spacing(IDENT)`` — insert one line of vertical gap.
* ``new_line(IDENT)`` — advance one full text-line of vertical space.
* ``dummy((size = float2(x, y)))`` — reserve arbitrary cursor space
  without rendering.

All four accept the ``[widget]``-style optional IDENT; the tutorial
shows IDENTs on ``same_line`` / ``spacing`` / ``new_line`` (useful when
playwright tests assert their snapshot entries) and the anonymous
``dummy`` form (cursor reservation that no test targets).

All four share ``EmptyMarkerState`` — the payload is ``{}``, but the
snapshot still records that the marker *fired*, so playwright tests can
assert ``"spacing#3 was rendered between button A and button B"``.

.. code-block:: das

   if (button(BTN_A, (text = "A"))) { /* ... */ }
   same_line(SL_AB)
   if (button(BTN_B, (text = "B"))) { /* ... */ }
   same_line(SL_BC)
   if (button(BTN_C, (text = "C"))) { /* ... */ }

   spacing(SP_TOP)
   new_line(NL_INSERT)
   dummy((size = float2(0.0f, 40.0f)))

Source: ``examples/tutorial/layout_primitives.das``.

************
Walkthrough
************

.. video:: layout_primitives.mp4

The markers render nothing, so the recording's self-check is that each *named*
marker fired: it narrates the ``same_line`` row while asserting the three
buttons and both ``same_line`` markers are on screen, then the ``spacing`` and
``new_line`` markers in turn (``record_check_rendered`` on each). A marker that
silently dropped out of the layout would abort the recording.

.. literalinclude:: ../../../examples/tutorial/layout_primitives.das
   :language: das
   :linenos:

Requires
========

Baseline boost layer (``imgui/imgui_boost_v2`` re-exports
``imgui/imgui_layout_builtin``). No extra modules.

When to reach for each
======================

``same_line`` is the workhorse — every multi-column row, every label-then-input
pattern uses it. Pass an explicit ``offset`` if the next widget needs a
column-aligned position; default packs against the previous item.

``spacing`` is a minimal 1-line gap — cheaper to read than ``dummy`` when
you just want breathing room between sections. Stack three of them if you
want a slightly larger gap without committing to a fixed pixel height.

``new_line`` is conceptually ``\n`` at the layout level — one full text
line. Useful when the current row had a tall widget and you want the next
row to start fresh from the left margin without accumulating Y from the
tall content.

``dummy(size)`` reserves an arbitrary rectangle. Pass ``size = float2(0, 40)``
for a 40-pixel-tall invisible spacer. The X component can pre-allocate a
horizontal slot too — useful for grid-like alignment when widgets vary in
width.

Snapshot shape
==============

Each layout marker registers an entry under its ident with kind
``"empty_marker"``:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."LAYOUT_WIN/SL_AB"'

Tests that want to verify "this layout was produced" can walk the
snapshot and confirm the markers fired in the expected order — see
``tests/integration/test_layout_primitives.das`` for the assertion shape.

.. seealso::

   Full source: :download:`examples/tutorial/layout_primitives.das <../../../examples/tutorial/layout_primitives.das>`

   Companion tutorial: :ref:`tutorial_layout` — the higher-level layout
   helpers (``with_indent``, ``with_item_width``, ``with_text_wrap_pos``).

   Integration test: ``tests/integration/test_layout_primitives.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
