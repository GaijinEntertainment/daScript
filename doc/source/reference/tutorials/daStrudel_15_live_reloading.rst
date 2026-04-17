.. _tutorial_dastrudel_live_reloading:

====================================
STRUDEL-15 — Live-Reloading Patterns
====================================

.. index::
    single: Tutorial; Strudel; Live-reloading
    single: Tutorial; Hot reload
    single: Tutorial; Live commands

This is the killer feature.  ``daslang-live.exe`` compiles your script,
runs it, then watches the file on disk — every save triggers a recompile
and a hot-reload of the new code **without stopping the audio stream**.
Edit a pattern, save, hear the change.  No process restart, no decoder
cost, no audio glitch.

Run this tutorial under the live host:

::

   bin/Release/daslang-live.exe tutorials/daStrudel/daStrudel_15_live_reloading.das

It also runs under plain ``daslang.exe`` — the file works in both hosts.
In standalone mode there is no hot-reload and the lifecycle collapses to
``main()``.

Part A: Lifecycle Functions
===========================

Scripts that export ``init`` / ``update`` / ``shutdown`` run in
**lifecycle mode** under daslang-live:

.. code-block:: das

    [export] def init() { ... }      // called on start and after every reload
    [export] def update() { ... }    // called every frame
    [export] def shutdown() { ... }  // called on exit and before every reload

The audio thread and any OS resources (windows, sample banks) live
outside the reload scope — only the daScript side recompiles.

Part B: Preserving Player State
===============================

``require strudel/strudel_live`` is all it takes to keep the strudel
player state alive across reloads.  The module registers
``[before_reload]`` and ``[after_reload]`` hooks that serialise the
player (tracks, SID, BPM, timing) and the loaded SF2 data into the
persistent byte store:

.. code-block:: das

    require strudel/strudel public
    require strudel/strudel_player
    require strudel/strudel_live   // auto-persists player state
    require live_host

    def strudel_main() {
        strudel_add_track(s("bd [hh hh] sd [hh cp]"))
        strudel_set_bpm(120.0lf)
        strudel_play()
    }

    [export] def init() {
        load_samples()
        strudel_init(@@strudel_main)
    }

Edit the pattern inside ``strudel_main`` and save — the audio system
keeps rolling, ``strudel_init`` runs again, and the new pattern takes
over on the next cycle boundary.  The SID (sound identifier) stays the
same, so external audio routing is unaffected.

Part C: Live Commands (REST API)
================================

``[live_command]`` annotations expose daScript functions over HTTP.
daslang-live starts a REST server on port 9090; any external tool can
drive the running script via ``POST /command``:

.. code-block:: das

    require live/live_commands
    require daslib/json_boost

    struct CmdBpmArgs {
        bpm : double = 120.0lf
    }

    [live_command(description="Set tempo in BPM")]
    def cmd_set_bpm(input : JsonValue?) : JsonValue? {
        let args = from_JV(input, type<CmdBpmArgs>)
        strudel_set_bpm(args.bpm)
        return JV("bpm set to {args.bpm}")
    }

Call it from the shell:

.. code-block:: bash

    curl -X POST http://localhost:9090/command \
      -d '{"name":"cmd_set_bpm","args":{"bpm":140}}'

The MCP server exposes these commands through the ``live_command`` tool
so AI assistants can steer a running strudel patch directly.  See
:ref:`tutorial_annotations` for annotation syntax.

Part D: Custom Persistent State
===============================

strudel_live handles the player.  Your own state needs its own hooks —
use ``[before_reload]`` / ``[after_reload]`` and the persistent byte
store to keep globals alive:

.. code-block:: das

    var g_reload_count : int = 0

    [before_reload]
    def save_my_state() {
        var data : array<uint8>
        data |> resize(4)
        unsafe { *reinterpret<int?>(addr(data[0])) = g_reload_count; }
        live_store_bytes("tutorial15_reload_count", data)
    }

    [after_reload]
    def restore_my_state() {
        var data : array<uint8>
        if (live_load_bytes("tutorial15_reload_count", data) && length(data) >= 4) {
            unsafe { g_reload_count = *reinterpret<int?>(addr(data[0])); }
            g_reload_count ++
        }
    }

The store takes arbitrary byte arrays keyed by string.  For structured
data, serialise with ``daslib/archive`` (which is how strudel_live
preserves the player itself).

Workflow Tips
=============

- Save the file to reload — the file-watcher does the rest.
- Hit ``POST /reload`` for a manual reload.
- ``POST /reload/full`` does a deeper recompilation if module-level
  structure changed.
- ``is_live_mode()`` returns ``true`` under daslang-live, ``false`` under
  daslang.exe — branch on it for dual-mode scripts.
- A compile error freezes the running version — fix, save again, and the
  new code takes over.  The audio never stops.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_15_live_reloading.das <../../../../tutorials/daStrudel/daStrudel_15_live_reloading.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_midi_files`

   Comparison with strudel.cc: :ref:`strudel_vs_strudel_cc`

   Full daStrudel reference: :ref:`stdlib_strudel_section`
