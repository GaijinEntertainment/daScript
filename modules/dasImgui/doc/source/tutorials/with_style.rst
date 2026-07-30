.. _tutorial_with_style:

#######################
With style
#######################

ImGui's style stack lets you change a color or metric for a scoped region
of the UI — push it once, pop it when you're done. Hand-balancing the
push/pop pairs is bug-prone (miss a pop and ``EndFrame`` asserts), and
heterogeneous tuples can't go through daslang's uniform-element-type
varargs, so the boost layer ships ``with_style((key, value), ...) { ... }``
as a ``[call_macro]``. Each ``(key, value)`` tuple is dispatched at
compile-time — ``ImGuiCol`` keys route to ``PushStyleColor``, ``ImGuiStyleVar``
keys route to ``PushStyleVar`` — and a single bulk ``pop_style_n`` undoes
them all at block exit.

Source: ``examples/tutorial/with_style.das``.

************
Walkthrough
************

.. video:: with_style.mp4

.. literalinclude:: ../../../examples/tutorial/with_style.das
   :language: das
   :linenos:

Requires
========

One extra module on top of the baseline boost layer:

* ``imgui/imgui_style_builtin`` — the ``with_style`` ``[call_macro]`` plus
  the ``push_style_one`` / ``pop_style_n`` primitives it lowers to.

Single block, mixed types
=========================

Each tuple in the ``with_style(...)`` argument list is independent — colors
and metric overrides mix freely:

.. code-block:: das

   with_style((ImGuiCol.Button, ImVec4(0.85f, 0.20f, 0.20f, 1.0f)),
              (ImGuiCol.ButtonHovered, ImVec4(0.95f, 0.30f, 0.30f, 1.0f)),
              (ImGuiCol.ButtonActive, ImVec4(0.70f, 0.12f, 0.12f, 1.0f)),
              (ImGuiStyleVar.FrameRounding, 8.0f)) {
       button(STYLED_BTN, (text = "Red rounded"))
   }

A full button restyle pushes all three interaction states — ``Button``,
``ButtonHovered``, ``ButtonActive`` — so the color holds while the pointer
is over it (push only ``Button`` and the default hover/active theme colors
show through on interaction). Here they mix with a rounding metric in one
block.

The macro emits one ``push_style_one(key, val)`` per tuple in source order,
then ``invoke(blk)``, then a single ``pop_style_n(N)`` that pops the
matching count in two bulk ImGui calls (one ``PopStyleColor`` for the color
pushes, one ``PopStyleVar`` for the metric pushes).

Nesting
=======

Nested ``with_style`` blocks stack: the inner block adds to the outer's
overrides without disturbing them. When the inner block exits, the outer
scope is restored:

.. code-block:: das

   with_style((ImGuiCol.Text, ImVec4(1.0f, 0.95f, 0.30f, 1.0f))) {
       Text("Yellow text inherited from outer scope.")
       with_style((ImGuiCol.Button, ImVec4(0.20f, 0.45f, 0.85f, 1.0f)),
                  (ImGuiStyleVar.FrameRounding, 4.0f)) {
           // Yellow text + blue button + 4-radius rounding all active.
           button(NESTED_STYLED_BTN, (text = "Blue button + yellow text"))
       }
       // Back to: yellow text, default button color/rounding.
   }

Pop balance
===========

After every ``with_style`` block exits, the baseline style is restored —
the underlying ``g_style_pop_stack`` tracks per-push kind tags so the
bulk pop fires the right number of ``PopStyleColor`` / ``PopStyleVar``
calls. Miss a pop and ImGui asserts at ``EndFrame``; the macro is the
only API surface that pushes, so user code can't accidentally leak
pushes past the block boundary.

Standalone vs live
==================

Same convention as previous tutorials.

Driving from outside
====================

``with_style`` is purely structural — there's no per-block state to set.
Drive the buttons inside instead:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_click","args":{"target":"STYLE_WIN/STYLED_BTN"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_click","args":{"target":"STYLE_WIN/NESTED_STYLED_BTN"}}' \
        localhost:9090/command

Next steps
==========

Widget identity comes next — ``with_id`` to disambiguate widgets that
share the same name within an ImGui ID scope, plus the ``id=``/``path=``
sugar that lets the boost layer steer the registry path without
restructuring the call hierarchy.

.. seealso::

   Full source: :download:`examples/tutorial/with_style.das <../../../examples/tutorial/with_style.das>`

   Richer reference: ``examples/features/style_override.das`` — the
   features-side demo with the same surface plus a baseline-restore check.

   Integration test: ``tests/integration/test_style_with_style.das``.

   Previous tutorial: :ref:`tutorial_docking`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
