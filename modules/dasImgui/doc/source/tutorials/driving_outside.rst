.. _tutorial_driving_outside:

#######################
Driving from outside
#######################

Every boost widget that previous tutorials wrote registers a path in
the telemetry tree (``DRIVE_WIN/USER``, ``DRIVE_WIN/SPEED``,
``DRIVE_WIN/RUN_BTN`` ...). That path is **also** an HTTP endpoint:
the boost layer ships ``[live_command]`` handlers (``imgui_snapshot``,
``imgui_force_set``, ``imgui_click``, ``imgui_open``, ``imgui_close``,
``imgui_focus``) that look up the target in the registry. ``imgui_click``
fires a **real synthetic mouse click** at the widget's center;
``imgui_force_set`` / ``imgui_open`` / ``imgui_close`` write the matching
field on the widget's state struct directly. Either way ImGui sees the
effect as if a real input device — or an external editor — drove it.

This tutorial flips the point of view: instead of writing the daslang
side, write the **driver** — a curl / Python / Bash script that issues
JSON commands at a running daslang-live app. Every interaction the
user could perform via mouse/keyboard has a curl equivalent, and the
two surfaces use one in-memory model.

Source: ``examples/tutorial/driving_outside.das`` — a small target app
exposing five widget kinds. The recording is **driven entirely by JSON
commands**: value writes and container toggles mutate state directly
(no mouse), while ``imgui_click`` fires a real synthetic click.

************
Walkthrough
************

.. video:: driving_outside.mp4

.. literalinclude:: ../../../examples/tutorial/driving_outside.das
   :language: das
   :linenos:

The command surface
===================

Two kinds of command — **faithful input** (does what a user does) and
**bypass** (does what a user can't):

* **Raw synth IO** (faithful): ``imgui_mouse_pos``,
  ``imgui_mouse_button``, ``imgui_mouse_play``, ``imgui_key_press``,
  ``imgui_key_type``. The driver pretends to be a mouse or keyboard,
  feeding events into the ImGui input queue. Used by ``imgui_playwright``
  for cursor-visible recordings.
* **Click a widget by name** (faithful): ``imgui_click``,
  ``imgui_focus``. ``imgui_click`` resolves the widget by path (or
  hex_id), warps to its center, and presses/releases through ImGui's
  own input path — a real click, so the widget behaves exactly as a
  user click would (it errors if the target isn't rendered this frame).
  ``imgui_focus`` forces keyboard focus. No trajectory to script, but
  the widget must be on screen.
* **Write a value directly** (bypass): ``imgui_force_set``. The
  framework looks up the widget and queues ``state.has_pending = true``
  + ``state.pending_value = ...``; the render function submits it next
  frame. Does what a click can't — an exact value, an off-screen or
  inactive widget.

Plus the read side and the container channel:

* ``imgui_snapshot`` — full registry as JSON, the first call in any
  driver.
* ``imgui_open`` / ``imgui_close`` — set ``state.pending_open`` /
  ``state.pending_close`` on container widgets (popups, closable
  windows, tabs, tree nodes).

Prefer ``imgui_click`` for clicks and ``imgui_force_set`` for values —
the first is a faithful click, the second a deterministic value write.
Drop to raw synth IO only when there's no higher-level counterpart
(drag along a custom trajectory, paste a long string into a focused
input, sustain a chord, ...).

imgui_snapshot — read the world
===============================

The first call every driver makes:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command

Response shape:

.. code-block:: text

   {
     "frame": 412,
     "globals": {
       "DRIVE_WIN": {
         "kind": "window",
         "bbox": [30, 30, 670, 490],
         "hex_id": "0x2c1a8f4b",
         "payload": { "open": true, "size": [640, 460], ... }
       },
       "DRIVE_WIN/SPEED": {
         "kind": "slider_int",
         "bbox": [...],
         "hex_id": "0x...",
         "payload": { "value": 5, "bounds": [0, 10], ... }
       },
       "DRIVE_WIN/RUN_BTN": {
         "kind": "button",
         "bbox": [...],
         "payload": { "click_count": 0 }
       },
       ...
     },
     "io": {
       "mouse_pos": [320, 180],
       "active_widget": "..."
     }
   }

Use it to:

* discover what's on screen and what kind each widget is;
* read ``bbox`` for L1 mouse synthesis (when needed);
* check ``payload`` for current state (test assertions);
* read ``hex_id`` for fallback dispatch when the path isn't stable.

imgui_force_set — value writes
==============================

``imgui_force_set`` is the universal value-write endpoint — slider, checkbox,
combo, color, text input, dock-window position. Type-dispatched on the
value's JSON shape:

.. code-block:: bash

   # string into a text input
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/USER","value":"Alice"}}' \
        localhost:9090/command

   # int into a slider
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/SPEED","value":7}}' \
        localhost:9090/command

   # int into a combo (selected index)
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/PRESET","value":2}}' \
        localhost:9090/command

   # array-of-floats into a color picker
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/TINT","value":[0.2,0.7,0.4]}}' \
        localhost:9090/command

Under the hood: the registered dispatcher for the widget's state struct
unpacks the JSON, type-checks it against the state's value field,
flips ``has_pending = true``, stores ``pending_value``. The render
function picks it up next frame; ImGui submits the new value through
its own ``UpdateValue`` path.

imgui_click — fire a click
==========================

``imgui_click`` is a **real synthetic mouse click**: it resolves the
target to its on-screen bbox, warps the cursor to the center, and
presses then releases the button across one frame — through ImGui's own
input path, so the widget can't tell it apart from a hardware click:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_click","args":{"target":"DRIVE_WIN/RUN_BTN"}}' \
        localhost:9090/command

The button's render function returns ``true``, ``click_count``
increments, and the daslang side sees both the inline ``if
(button(...))`` and ``RUN_BTN.clicked / RUN_BTN.click_count`` as
expected. Pass ``"button": 1`` for a right-click (context menus), ``2``
for middle. Because it's a real click, the target must be rendered this
frame — clicking an unrendered widget returns an error (use
``imgui_force_set`` to drive a widget that isn't on screen).

imgui_open / imgui_close — containers
=====================================

Containers expose an open-state channel through ``state.pending_open``
and ``state.pending_close``. ``imgui_open`` flips ``pending_open``;
``imgui_close`` flips ``pending_close``. The next frame's render
function applies the change:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_open","args":{"target":"DRIVE_WIN/STATUS_POPUP"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_close","args":{"target":"DRIVE_WIN/STATUS_POPUP"}}' \
        localhost:9090/command

The same channel handles popups, closable windows, tabs (closable-tab
visibility specifically — see ``tutorial_containers`` for the tab-item
caveat), tree nodes, and collapsing headers.

The flow on a single command
============================

Every command runs the same path:

1. ``daslang-live`` HTTP server receives ``POST /command``.
2. Routes by ``name`` to the registered ``[live_command]`` handler.
3. Handler looks up ``target`` in the registry's path map (or hex_id
   reverse map).
4. Either spawns a synthetic-input coroutine (``imgui_click`` — warp +
   press/release over a frame) or mutates the matching state struct's
   pending field (``imgui_force_set`` / ``imgui_open`` / ``imgui_close``);
   returns ``{"ok": true, ...}`` on the HTTP response.
5. **Over the next frame(s)** the script's ``update()`` runs; ImGui
   processes the synthetic input, or the render function applies the
   pending field, and the updated state is observable from the next
   ``imgui_snapshot``.

So **commands settle over the next frame or two by design** — there's no
ambiguity about which frame's state corresponds to a given response.
For test harnesses that need to read the result, the canonical pattern
is: command, then ``await_quiescent`` (waits a frame), then
``imgui_snapshot``.

Standalone vs live
==================

The HTTP server only exists under ``daslang-live``. Standalone
``daslang.exe`` runs the same script but the live-command endpoints
aren't bound — drive-from-outside scenarios require the live host.

Driving from outside (recap)
============================

A complete drive sequence for this tutorial's app:

.. code-block:: bash

   # Read the world
   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command

   # Write each widget kind
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/USER","value":"Alice"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/SPEED","value":7}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DRIVE_WIN/PRESET","value":2}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_click","args":{"target":"DRIVE_WIN/RUN_BTN"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_open","args":{"target":"DRIVE_WIN/STATUS_POPUP"}}' \
        localhost:9090/command

The recording at the top of this page runs this exact sequence — just
JSON commands. The value writes and container toggles flow through the
state-struct pending channel with no mouse motion; the ``imgui_click``
is a real synthetic click at the button's center.

Next steps
==========

Now that the JSON-driven view is explicit, the **visual aids tour**
walks through every overlay the recordings used: highlight, mouse
trail, cursor sprite, narrate, key HUD, focus rect — all
``[live_command]``-wrapped so the same curl pattern reaches them.

.. seealso::

   Full source: :download:`examples/tutorial/driving_outside.das <../../../examples/tutorial/driving_outside.das>`

   Richer reference: ``examples/features/io_synth_text.das`` —
   ``imgui_key_type`` streams ``text`` as synthetic key + char events through the
   key timeline; the synthetic keyboard layer in action.

   Snapshot contract: ``imgui_boost_runtime.das``'s
   ``g_serializers`` per-kind payload definitions.

   Previous tutorial: :ref:`tutorial_live_reload`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
