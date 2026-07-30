.. _tutorial_collapsing_header:

#######################
Collapsing header
#######################

``collapsing_header`` is the section-fold sibling of ``tree_node``: same
expand/collapse chevron, but **no** ``TreePop`` pair — ImGui owns the
close lifecycle. Two distinct gates govern visibility:

* **Expanded** — the chevron click toggles the body. ``state.opened`` is
  the per-frame ``CollapsingHeader`` return value.
* **Closable** — with ``closable=true``, ImGui adds an X-button that
  sets ``state.open=false``. While ``state.open`` is false, **the whole
  header strip is hidden** (not just its body).

The boost wrapper exposes one channel — ``pending_open`` /
``pending_close`` — that drives the **chevron** (expanded gate). The
live commands ``imgui_open`` / ``imgui_close`` ride that channel.
``imgui_open`` additionally re-sets ``state.open=true`` so a previously
X-hidden strip becomes visible again. ``imgui_close`` does NOT touch
``state.open`` — to hide the strip, click the X-button or write
``state.open=false`` from app code.

Source: ``examples/tutorial/collapsing_header.das``.

************
Walkthrough
************

.. video:: collapsing_header.mp4

.. literalinclude:: ../../../examples/tutorial/collapsing_header.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — ``collapsing_header``.
* ``imgui/imgui_widgets_builtin`` — ``text``, ``checkbox``, ``slider_int``.

Two visibility dimensions
=========================

``CollapsingHeaderState`` carries both gates:

.. code-block:: das

   var CLOSABLE_CH : CollapsingHeaderState
   // After the frame renders:
   //   CLOSABLE_CH.open    : bool   // X-button gate (visible at all?)
   //   CLOSABLE_CH.opened  : bool   // chevron gate (body expanded?)
   //   CLOSABLE_CH.flags   : ImGuiTreeNodeFlags   // sticky

``open`` is the X-button channel. ``opened`` is the chevron channel.
Both surface in the snapshot. Tests that want to verify "the user
expanded the section but didn't close it" check ``opened==true &&
open==true``.

The closable form
=================

When ``closable=true``, ``ImGui::CollapsingHeader`` takes a
``bool* p_visible`` and renders an X-button on the right of the header
strip. The boost wrapper feeds ``&state.open`` to that pointer:

.. code-block:: das

   collapsing_header(CLOSABLE_CH, (text = "Closable header",
                                   closable = true,
                                   flags = ImGuiTreeNodeFlags.DefaultOpen)) {
       text("X-button on the right of the header strip closes this section.")
       // body
   }

Click the X — ImGui sets ``state.open=false``. Next frame, the
wrapper sees ``open==false`` and **skips the ImGui call entirely**.
The header row is gone until something flips ``open`` back to true.
That something is either app code (``CLOSABLE_CH.open = true``) or a
live driver (``imgui_open`` against the path).

The pending-flag channel
========================

Two channels feed open/close from outside:

* ``state.pending_open = true`` — set anywhere in app code, then the
  next frame's call to the container clears it and applies
  ``SetNextItemOpen(true, Always)``.
* ``imgui_open`` — the live command. The dispatcher walks the
  ``ImguiPathRegistry`` and sets ``pending_open`` on the matching state.

Same shape for ``pending_close`` / ``imgui_close``. The closable form
also clears ``state.open=false`` from the X-button — three control
surfaces flow through one struct.

DefaultOpen flag
================

``ImGuiTreeNodeFlags.DefaultOpen`` makes the chevron expanded on first
render only. Subsequent frames respect whatever the user clicked.
Useful for "show me the important section by default" without
remembering open-state across runs.

.. code-block:: das

   collapsing_header(STARTUP_CH, (text = "Starts open",
                                  closable = false,
                                  flags = ImGuiTreeNodeFlags.DefaultOpen)) {
       text("Body visible on first render.")
   }

Other ``ImGuiTreeNodeFlags`` compose: ``OpenOnArrow``, ``OpenOnDoubleClick``,
``Bullet``, ``Framed``, ``Leaf`` — same flag enum as ``tree_node`` (the
two rails share semantics intentionally).

Standalone vs live
==================

Same convention as the other tutorials.

Driving from outside
====================

The header strip is a real click target — its bbox is captured into the
snapshot, so a human (or a playwright driver) clicks the chevron to fold
the body and the X-button to hide the whole strip, exactly as the
walkthrough above does (every gesture there is a real click, self-verified
by the body's rendered state). The live commands below drive the same gates
**without** a click, for remote or scripted control.

To collapse the chevron (body stops rendering; the header strip stays
visible):

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_close","args":{"target":"CH_WIN/CLOSABLE_CH"}}' \
        localhost:9090/command

To re-expand the chevron — and, for a closable header that had been
X-hidden, re-show the whole strip:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_open","args":{"target":"CH_WIN/CLOSABLE_CH"}}' \
        localhost:9090/command

``imgui_open`` writes both ``state.pending_open`` AND ``state.open=true``
so a previously X-hidden header re-appears. ``imgui_close``, by
contrast, only writes ``state.pending_close`` — there's no path that
hides the strip via live commands. To hide the strip programmatically,
write ``state.open=false`` from app code (the X-button is the only
user-driven path).

.. seealso::

   Full source: :download:`examples/tutorial/collapsing_header.das <../../../examples/tutorial/collapsing_header.das>`

   Features-side demo: ``examples/features/collapsing_header_closable.das`` —
   minimal closable-X-button repro with integration test.

   Sibling: :ref:`tutorial_containers` — umbrella container tour.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
