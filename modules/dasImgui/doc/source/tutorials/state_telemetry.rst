.. _tutorial_state_telemetry:

#######################
State & telemetry
#######################

Widget state lives in daslang, not in ImGui. Every boost widget macro
emits a module-scope global named by the first argument — typed as
``ClickState`` / ``SliderStateFloat`` / ``ToggleState`` / etc. The
global holds the widget's value plus any pending overrides queued by
external drivers. The same global is what the registry serializes for
``imgui_snapshot``, so the daslang side, the test side, and the
external-driver side all see the same value.

Three immediate wins fall out of that design:

* **Auto-emit** — no ``var SAVE_BTN : ClickState`` declaration to keep
  in sync with the call site. The macro declares it on first compile.
* **Read anywhere** — ``SAVE_BTN.click_count``, ``SPEED.value``, etc.
  are plain daslang globals, readable from any module that requires
  this one.
* **Dotted flags** — ``IDENT.PUBLIC`` / ``IDENT.PRIVATE`` /
  ``IDENT.NOTLIVE`` tune visibility and live-reload behavior on the
  emitted global without claiming new syntactic positions in the call.

Source: ``examples/tutorial/state_telemetry.das``.

************
Walkthrough
************

.. video:: state_telemetry.mp4

The recording drives every channel and asserts the snapshot followed: it clicks
``Save`` twice and verifies ``SAVE_BTN.click_count`` (``hold_through_voice``),
force-sets ``SPEED`` and ``VOLUME`` from outside and verifies each value
(``force_set_verified``), force-sets ``STATUS_TEXT`` and verifies the mirror took
the string, then clicks ``bump`` and verifies the app rewrote ``STATUS_TEXT.value``
from inside (``record_check_changed``). Any channel that stopped reaching the
snapshot would abort the recording.

.. literalinclude:: ../../../examples/tutorial/state_telemetry.das
   :language: das
   :linenos:

Auto-emit
=========

The first time ``button(SAVE_BTN, ...)`` is compiled, the macro emits
the matching global at module scope:

.. code-block:: das

   // emitted automatically — no manual declaration
   @live variable private SAVE_BTN : ClickState = ClickState()

That's why there's no ``var SAVE_BTN`` at the top of the file. The
state struct is owned by daslang — visible to ``grep``, walkable via
RTTI, persistable through the standard serializer, preserved across
``daslang-live`` reloads thanks to ``@live``.

Reading state
=============

Once emitted, the global behaves like any other daslang global —
``SAVE_BTN.click_count`` is a plain field access:

.. code-block:: das

   if (button(SAVE_BTN, (text = "Save"))) { ... }
   text("SAVE_BTN.click_count = {SAVE_BTN.click_count}")

Two distinct value channels are available:

* ``button(...)`` returns ``bool`` — ``true`` on the frame the click
  fired. Inline-friendly for the "do thing now" case.
* ``SAVE_BTN.clicked`` is the same flag, surfaced as a field. Useful
  when the click handler is far from the call site, or in another
  module that requires this one.

Cumulative counters (``click_count`` for buttons, ``changed`` for
sliders, etc.) live alongside on the state struct. Walk
``imgui_boost_runtime.das`` for the full field list per state struct.

Dotted flags
============

A dot suffix on the identifier flips flags on the emitted global. The
telemetry path uses only the bare identifier (``STATE_WIN/SPEED``,
never ``STATE_WIN/SPEED.PUBLIC``) — flags never leak into the path or
the ImGui hash.

* ``SPEED.PUBLIC`` — emit as ``variable public`` instead of the default
  ``variable private``. Sibling modules requiring this one can then
  read ``SPEED.value`` directly.
* ``VOLUME.NOTLIVE`` — skip the ``@live`` annotation on the emitted
  global. Useful when you change the slider bounds and want the
  source-side initial value to take effect immediately on reload
  rather than be preserved.
* ``IDENT.PRIVATE`` — explicit default (same as no suffix). Lists
  cleanly when you grep for visibility intent.

Multiple flags compose: ``RPS.PUBLIC.NOTLIVE`` emits a public,
non-``@live`` global. New flags can land on demand without affecting
the call syntax.

text_show — the app-driven mirror
=================================

``text_show`` is the read-only counterpart to ``text_input`` —
``state.value`` is what the widget renders, and the value can be
written by the app (``STATUS_TEXT.value := "..."``) **or** by an
external driver (``imgui_force_set`` with a string value). Either way the
snapshot exposes the current value under the standard
``payload.value`` field, so integration tests can assert on computed
status strings the same way they assert slider values:

.. code-block:: das

   text_show(STATUS_TEXT)
   if (button(BUMP_STATUS, (text = "bump status"))) {
       STATUS_TEXT.value := "saved at frame {get_uptime()}"
   }

The ``:=`` clones the new string into the current context's heap —
required because ``daslang-live``'s HTTP handler runs in a different
context than the GLFW main loop. Plain ``=`` would assign a pointer
that becomes invalid the moment the request returns.

Standalone vs live
==================

Same convention as previous tutorials.

Driving from outside
====================

The snapshot exposes the state structs as JSON:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command

   # Excerpt of the response:
   #   "globals": {
   #     "STATE_WIN/SAVE_BTN":    { "kind": "button", "payload": {"click_count": 2}, ... },
   #     "STATE_WIN/SPEED":       { "kind": "slider_int", "payload": {"value": 7, ...}, ... },
   #     "STATE_WIN/STATUS_TEXT": { "kind": "text_show", "payload": {"value": "saved at frame 12.3"}, ... }
   #   }

Drivers go through the same registry — ``imgui_force_set`` looks up the
target, queues the pending value on the matching state struct, and
the renderer consumes it next frame:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"STATE_WIN/SPEED","value":7}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"STATE_WIN/STATUS_TEXT","value":"hello"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_click","args":{"target":"STATE_WIN/SAVE_BTN"}}' \
        localhost:9090/command

Next steps
==========

So far every tutorial has used a single ``window(...)`` container.
Containers come next — modal dialogs, popups, tab bars, child windows,
and menus, all sharing the same block-arg pattern.

.. seealso::

   Full source: :download:`examples/tutorial/state_telemetry.das <../../../examples/tutorial/state_telemetry.das>`

   Richer reference: ``examples/features/foundation.das`` — the
   features-side demo that established the auto-emit + dotted-flag
   surface plus the unified L2/L3 dispatch.

   Snapshot contract: see ``imgui_boost_runtime.das`` for the per-kind
   ``state_struct`` definitions (``ClickState``, ``SliderStateInt``,
   ``SliderStateFloat``, ``ToggleState``, ``TextShowState``, ...).

   Previous tutorial: :ref:`tutorial_with_id`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
