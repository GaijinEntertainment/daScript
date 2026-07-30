.. _tutorial_with_id:

#######################
With id
#######################

ImGui hashes a widget's identifier into the ID stack to detect clicks,
remember imgui.ini state, and tag draw-list entries. The boost layer
piggybacks on the same hash for its telemetry path. When two render
sites need to share a widget identifier — composed helpers, two panes
showing the same kind of control, a button rendered under two scopes —
the boost ships three knobs for sorting out the collision:

* ``with_id("scope") { ... }`` pushes ``"scope"`` onto **both** ImGui's
  ID stack and the boost path, so two scopes contain the same widget
  identifier without collision.
* ``widget(IDENT, (id = "x"))`` mangles only ImGui's hash — useful when
  the daslang identifier may get renamed but the imgui.ini state and
  any hex_id-driven scripts must stay stable.
* ``widget(IDENT, (path = "x"))`` replaces the registry path leaf — the
  daslang identifier still names the global; ``"x"`` is what external
  drivers target via ``imgui_force_set`` / ``imgui_click``.

Source: ``examples/tutorial/with_id.das``.

************
Walkthrough
************

.. video:: with_id.mp4

.. literalinclude:: ../../../examples/tutorial/with_id.das
   :language: das
   :linenos:

Requires
========

One extra module on top of the baseline boost layer:

* ``imgui/imgui_id_builtin`` — defines ``with_id(s) { ... }`` and wires
  the ``id=`` / ``path=`` named-arg sugar for every widget macro.

The single-render rule
======================

Single-global widgets render **exactly once per frame**. Two violations
collapse into one runtime panic at end-of-frame:

* Single-global widget inside a ``for`` loop (would render N times) —
  the boost macro can usually catch this lexically and ``macro_error`` at
  expansion with a fixit pointing at the indexed form ``IDENT[i]`` /
  ``IDENT[key]`` (see :ref:`tutorial_widgets_tour` for the indexed form).
* Single-global widget called from two distinct sites (e.g. shown in two
  windows at once) — when the macro can't see the second site lexically,
  the runtime registry catches it and panics.

``with_id`` is the explicit ID-stack push for case 2 when the indexed
form can't reach (composed helpers, sub-tree data-driven structure).

with_id scopes
==============

Each ``with_id("scope") { ... }`` block pushes ``"scope"`` onto **two**
stacks: ImGui's ID stack (so child widgets get distinct hashes) and the
boost registry path (so child entries register under distinct paths).
Both pop on block exit:

.. code-block:: das

   with_id("section_a") {
       button(SAVE_BTN, (text = "Save in A"))
   }
   with_id("section_b") {
       button(SAVE_BTN, (text = "Save in B"))
   }

The two renders show up in the snapshot as ``ID_WIN/section_a/SAVE_BTN``
and ``ID_WIN/section_b/SAVE_BTN`` — distinct paths AND distinct ImGui
hashes. ``SAVE_BTN.click_count`` is one global; clicks on either
button increment the same counter. If you need separate counters,
switch to ``SAVE_BTN[key]`` indexed-widget form instead.

Nested chains
=============

``with_id`` chains nest. Each scope contributes one path segment, and
the leaf widget's registry path is the chain joined by ``/``:

.. code-block:: das

   with_id("outer") {
       with_id("inner") {
           button(NESTED_BTN, (text = "Nested deep"))
       }
   }
   // registry path: ID_WIN/outer/inner/NESTED_BTN

``with_id`` doesn't register a container entry of its own — only the
path segment. The leaf widget's entry is what shows up in the snapshot.

Per-call ``id=`` — stable hash across renames
=============================================

The daslang identifier is the **source-of-truth name** for a widget — it
defines the global, drives live-reload visibility, and determines how
the variable is referenced elsewhere in the program. The ImGui hash and
the registry path are **conventions** built on top.

``id="x"`` decouples the ImGui hash from the identifier:

.. code-block:: das

   button(HASHED_BTN, (text = "Hashed via id=", id = "stable_v1"))

Telemetry path is still ``ID_WIN/HASHED_BTN`` (the bare identifier);
ImGui's hash is mangled by ``"stable_v1"`` via ``PushID``/``PopID``
wrapping the render call. Practical wins:

* **imgui.ini stability** — rename ``HASHED_BTN`` to ``SAVE_BUTTON_V2``
  in source, and imgui.ini's open/close state for this button still
  matches the same ImGui hash; users don't lose their layout.
* **Hex_id-driven scripts** — external drivers that target widgets by
  the ``hex_id`` field in the snapshot keep working across daslang
  identifier renames.

Per-call ``path=`` — stable registry path across renames
========================================================

``path="x"`` is the inverse case — keep the daslang identifier internal,
expose a stable string to external drivers:

.. code-block:: das

   slider_float(RPS_PATH, (text = "Speed (path=)", path = "rps_stable"))

The registry path becomes ``ID_WIN/rps_stable`` (the bare identifier is
NOT registered). ``RPS_PATH`` is still the daslang global — rename it
without breaking script drivers. ``path=`` also mangles the ImGui hash
since ``widget_prelude`` does ``PushID(widget_ident)`` and
``widget_ident`` is the overridden value.

Standalone vs live
==================

Same convention as previous tutorials: ``daslang`` for standalone or
``daslang-live`` to keep the live-reload server running.

Driving from outside
====================

The registry paths shown above are exactly what external drivers target:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_click","args":{"target":"ID_WIN/section_a/SAVE_BTN"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_click","args":{"target":"ID_WIN/section_b/SAVE_BTN"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"ID_WIN/rps_stable","value":0.7}}' \
        localhost:9090/command

Note the third targets ``rps_stable`` (the ``path=`` override), not the
``RPS_PATH`` identifier — the override **replaces** the bare-identifier
path entry, so the bare-name target would return "no such widget."

Next steps
==========

State and telemetry come next — the registered widget state structs
that back every boost widget, how dotted flags on the identifier
(``RPS.PUBLIC.NOTLIVE``) tune cross-module visibility and live-reload
behavior, and the auto-emit hook that surfaces app-side values to the
snapshot.

.. seealso::

   Full source: :download:`examples/tutorial/with_id.das <../../../examples/tutorial/with_id.das>`

   Richer reference: ``examples/features/id_override.das`` — the
   features-side demo with the same surface plus a same-state shared-button
   demonstration.

   Integration test: ``tests/integration/test_id_override.das``.

   Previous tutorial: :ref:`tutorial_with_style`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
