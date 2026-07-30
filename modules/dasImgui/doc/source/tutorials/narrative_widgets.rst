.. _tutorial_narrative_widgets:

#######################
Narrative widgets
#######################

The narrative family is dasImgui's set of read-only display widgets — text
emission with optional color, emphasis, bullets, or label/value framing.
All share a ``NarrativeState`` (or ``LabelTextState``) payload that echoes
the call-site string back into the snapshot, so playwright tests can
assert "the label says what I expect" without any layout state.

Every rail accepts an optional leading ident. Pass one — ``text(IDENT,
(text = "..."))`` — and the widget registers at ``NARRATIVE_WIN/IDENT`` so a
snapshot can target it by name; omit it — ``text("...")`` — and it registers
under an auto-generated source-line key instead. This tutorial passes an
ident on every call so each line is assertable.

The nine rails:

* ``text(IDENT, (text = "..."))`` — one line of plain text. Implemented via
  ``TextUnformatted`` (no printf-style format expansion).
* ``text_unformatted(IDENT, (text = "..."))`` — explicit alias for ``text``;
  identical implementation. Exists so call sites can signal intent.
* ``text_wrapped(IDENT, (text = "..."))`` — reflows long strings to the
  window's content edge.
* ``text_colored(IDENT, (color = float4(...), text = ...))`` — colored variant.
* ``text_disabled(IDENT, (text = "..."))`` — greyed for non-actionable hints.
* ``bullet(IDENT)`` — bullet glyph alone.
* ``bullet_text(IDENT, (text = "..."))`` — bullet glyph + text bundled.
* ``label_text(IDENT, (key = ..., value = ...))`` — two-string display, both
  echoed into the payload.
* ``separator_text(IDENT, (text = "..."))`` — horizontal rule with a centered
  label.

Source: ``examples/tutorial/narrative_widgets.das``.

************
Walkthrough
************

.. video:: narrative_widgets.mp4

These widgets take no input, so the recording is a voiced, self-verifying
tour: each beat narrates a group while the cursor points at it and asserts
the widget rendered and echoed its call-site string into the snapshot — the
"the label says what I expect" claim made concrete. A missing or wrong value
aborts the recording at teardown instead of shipping.

.. literalinclude:: ../../../examples/tutorial/narrative_widgets.das
   :language: das
   :linenos:

Requires
========

Baseline boost layer (``imgui/imgui_boost_v2`` re-exports
``imgui/imgui_widgets_builtin``). No extra modules.

text vs text_unformatted vs text_wrapped
========================================

* ``text(s)`` — emits ``s`` through ImGui's ``TextUnformatted(s)``. No
  printf-style format expansion; the string is rendered verbatim.
* ``text_unformatted(s)`` — identical to ``text(s)`` (same
  ``TextUnformatted`` call). Exists as an explicit-intent alias for call
  sites where ``unformatted`` reads better, or where you want to
  underscore the no-printf-expansion contract for readers.
* ``text_wrapped(s)`` — emits through ``TextWrapped(s)`` so the text
  reflows at the window's content edge. The wrap position respects
  ``with_text_wrap_pos`` if you've pushed one.

The payload shape is identical for all three; the alias-pair
(``text`` / ``text_unformatted``) renders identically, while
``text_wrapped`` is the one that actually changes rendering behavior.

Colored vs disabled
===================

``text_colored((color, text))`` takes an inline ``float4`` and renders the
text in that color. Useful for status callouts, error indicators, or
section accents. The color is per-call — no scope push needed.

``text_disabled(s)`` renders in ImGui's ``StyleColor.TextDisabled`` — the
greyed tone used for inert menu items and hints. The most common use is
the ``(?)`` help marker glyph (see :ref:`tutorial_flat_tooltips`).

Bullets
=======

``bullet(IDENT)`` emits the marker glyph alone — useful when you want
custom content (a colored swatch, an icon) immediately after the bullet.

``bullet_text(IDENT, (text = "..."))`` is the bundled form — glyph + text
in one call. Use this 95% of the time; reach for ``bullet(IDENT)`` +
``same_line`` + custom-glyph only when you genuinely need the manual
layout.

label_text
==========

``label_text((key, value))`` maps to ImGui's ``LabelText(key, value)``.
ImGui draws the value text on the left and the key as a label to its
right — so ``(key = "Version", value = "v2.0-detour")`` renders
``v2.0-detour`` on the left with ``Version`` to its right. Both strings
round-trip through the snapshot payload, so snapshot-driven property
tests get both halves for assertion.

separator_text
==============

``separator_text("Section name")`` is a horizontal rule with the label
centered above it. Use it to break a long window into named sections;
much easier to scan than ``separator()`` followed by ``text("Section
name")``.

Snapshot shape
==============

With an ident, a narrative widget registers at ``NARRATIVE_WIN/IDENT`` —
a stable path you can pull a field from. Without one it still surfaces a
path, but keyed by source line (``NARRATIVE_WIN/:61:8``), which shifts when
you edit the file; pass an ident whenever a test or driver needs to target
the line.

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command \
       | jq '.globals."NARRATIVE_WIN/VERSION_LABEL".payload.value'

.. seealso::

   Full source: :download:`examples/tutorial/narrative_widgets.das <../../../examples/tutorial/narrative_widgets.das>`

   Integration tests: ``tests/integration/test_narrative_text.das``,
   ``tests/integration/test_narrative_bullet.das``,
   ``tests/integration/test_narrative_separator.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
