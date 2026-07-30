.. _tutorial_with_tab_stop:

#######################
With tab stop
#######################

ImGui's ``PushTabStop`` / ``PopTabStop`` pair lets you control which widgets
participate in TAB / Shift+TAB focus cycling. The boost layer wraps it as a
stateless scope wrapper:

.. code-block:: das

   with_tab_stop(false) {
       input_text(TS_FIELD_SKIP, (text = "Skipped by TAB", init = "..."))
   }

The wrapper itself emits no telemetry — it only pushes the tab-stop flag on
entry and pops it on exit, matching the rest of the ``with_*`` family
(``with_indent``, ``with_disabled``, ``with_button_repeat``, ``with_font``,
``with_clip_rect``).

Source: ``examples/tutorial/with_tab_stop.das``.

************
Walkthrough
************

.. video:: with_tab_stop.mp4

.. literalinclude:: ../../../examples/tutorial/with_tab_stop.das
   :language: das
   :linenos:

Requires
========

Same baseline as the other ``with_*`` wrappers — already in
``imgui/imgui_scope_builtin`` (re-exported by ``imgui/imgui_boost_v2``).

Behaviour
=========

While the wrapped block is active, every focusable widget inside has its
ImGui tab-stop flag suppressed:

* TAB and Shift+TAB cycling skip past those widgets.
* Mouse click still focuses the widget (tab-stop is keyboard-only).
* Programmatic focus (``SetKeyboardFocusHere``) still works.

The flag scope follows the block exactly: as soon as the wrapped block
exits, the previous tab-stop policy is restored, so wrapping a help-marker
or decorative input inside ``with_tab_stop(false)`` doesn't leak the
suppression onto sibling widgets.

When to reach for it
====================

The two recurring shapes:

* **Help markers / tooltips** — a ``(?)`` glyph that opens an info popup on
  hover but shouldn't intercept TAB navigation through the editable form.
* **Decorative inputs** — a read-only field that displays computed state
  next to editable siblings; keyboard navigation should skip past it
  entirely.

The same pattern is used in ``examples/imgui_demo/inputs.das`` — both the
Tabbing and Focus-from-code sub-sections wrap their "(tab skip)" field +
help marker in ``with_tab_stop(false) { ... }``.

Standalone vs live
==================

Same convention as the other scope-wrapper tutorials. ``daslang.exe`` runs
the demo headlessly to frame N; ``daslang-live`` keeps the window open
and reloads on source edits.

.. seealso::

   Full source: :download:`examples/tutorial/with_tab_stop.das <../../../examples/tutorial/with_tab_stop.das>`

   Integration test: ``tests/integration/test_with_tab_stop.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
