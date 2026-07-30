.. _tutorial_live_reload:

#######################
Live reload
#######################

Every earlier tutorial's source banner had two run modes:

* ``daslang.exe <script>`` — standalone; runs ``main()`` which loops
  ``init`` / ``update`` / ``shutdown`` until ``exit_requested()``.
* ``daslang-live <script>`` — same script, hosted inside a wrapper
  process that **watches the file**, **reruns the typer on save**,
  and **swaps the new program in without restarting the GLFW window**.
  ImGui context, registered widget state, dock layout, slider values —
  anything ``@live`` or restored via a hook — carry across the reload.

This tutorial walks through the seams the live-reload framework
exposes:

* ``live_create_window`` / ``live_imgui_init`` — idempotent so they
  no-op on reload (the preserved ImGui context is reused).
* ``live_begin_frame`` / ``live_end_frame`` — per-frame gate / commit.
  ``live_begin_frame`` returns ``false`` while the host is paused or
  in the middle of swapping programs; skip the frame.
* ``@live`` annotation — preserves a global (or struct field) across
  reload via auto-generated ``[before_reload]`` / ``[after_reload]``
  serializers in the ``live/live_vars`` module.
* ``[live_command]`` — registers an HTTP endpoint that the running
  process exposes; called from ``curl`` or any other client. The
  ``imgui_force_set`` / ``imgui_click`` / ``imgui_snapshot`` surface is
  built from ``[live_command]`` declarations.
* ``[before_reload]`` / ``[after_reload]`` — manual save/restore hooks
  for state ``@live`` can't track (raw pointers, GL textures,
  C-owned resources).

Source: ``examples/tutorial/live_reload.das``.

************
Walkthrough
************

.. video:: live_reload.mp4

The recording can't trigger a reload, so it exercises the observable surface and
asserts each piece: it force-sets the ``@live`` ``VOLUME`` slider and verifies the
value (``force_set_verified``), clicks the ``@live`` ``PING_BTN`` twice and verifies
``click_count`` (``hold_through_voice``), then calls the user-defined ``bump_counter``
and ``reset_counter`` ``[live_command]`` endpoints from outside and verifies the
counter climbs to 7 and back to 0 (``record_check_value`` on the snapshot readout).
A command that stopped reaching the running program would abort the recording.

.. literalinclude:: ../../../examples/tutorial/live_reload.das
   :language: das
   :linenos:

The reload boundary
===================

A daslang-live reload runs in this order:

1. File watcher notices a source-tree edit (or an HTTP ``POST /reload``
   request arrives).
2. The HOST collects every ``[before_reload]`` function and runs them
   in the OLD program. The ``live/live_vars`` module auto-generates
   one of these per ``@live`` global; the user can register more.
3. Typer + codegen run against the new source. If they fail, the
   reload aborts and the old program keeps running — ``live_get_error``
   surfaces the diagnostic.
4. The new program is loaded. ``[after_reload]`` hooks run, restoring
   the saved state (``@live`` first, then user hooks).
5. The next ``update()`` call sees ``live_begin_frame() == true`` and
   normal rendering resumes.

The GLFW window and the OS-level ImGui context survive the swap — only
the daslang program is replaced.

``@live`` preservation
======================

The simplest way to keep a value across reload is the ``@live``
annotation on the global (or on individual struct fields). The
``live/live_vars`` module synthesizes the matching save/restore hooks
at compile time:

.. code-block:: das

   var private @live g_custom_counter : int = 0

The serializer uses ``daslib/archive``; primitives, arrays, tables,
strings, and any struct whose fields are themselves ``@live``-friendly
all work out of the box. Each ``@live`` target gets its own storage
key, and the saved data carries a **hash of the initialization
expression** — change the initializer in source, and the stale value
is discarded automatically. (No "I changed the default to 10 and now
my old value of 0 is wrong" foot-gun.)

Boost widget state types (``ClickState``, ``SliderStateFloat``,
``ToggleState``, ``WindowState``, ...) are already structured this
way — their value-carrying fields are ``@live``, their pending-flags
fields are not. That's why a slider's value survives reload but
``pending_value`` doesn't.

The frame gate
==============

``live_begin_frame()`` is the only way the host signals "do not render
this frame":

.. code-block:: das

   def update() {
       if (!live_begin_frame()) return
       // ... NewFrame, your draw calls, Render
       live_end_frame()
   }

States that return ``false``:

* The host is paused (``POST /pause`` from ``daslang-live`` or
  ``mcp__daslang__live_pause``).
* A reload is in progress (between ``[before_reload]`` and
  ``[after_reload]``).
* The most recent typer pass failed and the program is "frozen" on
  the prior version — the next save that compiles will revive it.

Always early-out on ``false`` and always pair with ``live_end_frame()``
on the success branch.

``[live_command]`` — user-defined HTTP endpoints
================================================

The same ``[live_command]`` annotation that registers ``imgui_force_set`` /
``imgui_click`` / ``imgui_snapshot`` works for user functions. The
function takes a ``JsonValue?`` (the request body's ``args`` field)
and returns ``JsonValue?`` (echoed back to the caller):

.. code-block:: das

   struct BumpArgs {
       @optional by : int = 1
   }

   [live_command(description = "Bump the custom counter by N (default 1).")]
   def bump_counter(input : JsonValue?) : JsonValue? {
       let args = from_JV(input, type<BumpArgs>)
       g_custom_counter += args.by
       return JV((ok = true, counter = g_custom_counter))
   }

The endpoint name (``bump_counter``) is the function name; the HTTP
surface routes ``POST /command`` requests with ``{"name":"bump_counter"}``
to this handler. The handler runs on the GLFW main thread between
frames, so it can safely touch daslang globals and ImGui state
without locks.

Manual reload hooks
===================

When ``@live`` doesn't fit — typically because the state is a pointer
to a C-owned resource that the new program won't recognize — declare
a pair of hooks explicitly:

.. code-block:: das

   [before_reload]
   def private on_before_reload() {
       // Stash whatever ``@live`` can't serialize.
       // live_store_bytes / live_store_string store under a string key.
   }

   [after_reload]
   def private on_after_reload() {
       // Re-read the stash and rebuild the in-memory state.
       // live_load_bytes / live_load_string read by the same key.
   }

``imgui_live.das`` itself is the canonical example: it serializes the
``live_imgui_ctx`` pointer as a ``uint64`` in ``[before_reload]`` and
re-binds it with ``SetCurrentContext`` in ``[after_reload]``.

Init / shutdown idempotence
===========================

``init`` runs on both cold-start AND reload. ``shutdown`` runs on
reload AND process exit. The framework helpers
(``live_imgui_init`` / ``live_imgui_shutdown``) are idempotent — they
detect the reload case and no-op accordingly — so the script's
``init`` / ``shutdown`` exports can be written once without
distinguishing cold-start from reload.

User globals initialized at module scope (``var x = 0``) only run their
initializer once at program load — reload starts a NEW program, so
that initializer runs again. Use ``@live`` (or a ``[before_reload]``
hook) for anything you want to preserve. Anything reset INSIDE
``init`` rebuilds each reload — the demo's ``g_session_string`` shows
that pattern.

Standalone vs live
==================

Run standalone with ``daslang.exe`` — every part of this tutorial
still works EXCEPT the ``[live_command]`` HTTP endpoints, which need
the daslang-live host. The reload hooks are silent in standalone
mode (they only fire on the reload boundary, which never happens).

Driving from outside
====================

Standard live-command shape; the user-defined endpoints sit next to
the built-in ones:

.. code-block:: bash

   # built-in: snapshot every registered widget
   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command

   # user-defined: bump the custom counter by 3
   curl -X POST -d '{"name":"bump_counter","args":{"by":3}}' localhost:9090/command

   # user-defined: reset
   curl -X POST -d '{"name":"reset_counter"}' localhost:9090/command

   # framework: trigger a reload (file edit also triggers this)
   curl -X POST localhost:9090/reload

The ``imgui_snapshot`` payload reflects whatever the most recent
``bump_counter`` did — daslang globals and live-command results share
one in-memory model.

Next steps
==========

Now that the live-command surface is explicit, next is the
**driving-from-outside** view: the JSON command set the boost layer
ships (``imgui_force_set`` / ``imgui_click`` / ``imgui_open`` / ...) treated
as its own programming model — a UI that responds to scripted
external events the same way it responds to mouse clicks.

.. seealso::

   Full source: :download:`examples/tutorial/live_reload.das <../../../examples/tutorial/live_reload.das>`

   Framework module: ``live/live_host`` (the host itself),
   ``live/live_commands`` (the ``[live_command]`` annotation), and
   ``live/live_vars`` (the ``@live`` serializer).

   ImGui-specific lifecycle: ``imgui/imgui_live.das`` — the
   ``[before_reload]`` / ``[after_reload]`` pair that preserves the
   ImGui context pointer is the canonical example of a manual hook.

   Previous tutorial: :ref:`tutorial_containers`

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
