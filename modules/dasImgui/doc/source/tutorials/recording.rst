.. _tutorial_recording:

###########################
Recording tutorial videos
###########################

Every tutorial recording is produced the same way: a **driver** script
uses :code:`with_recording_app` to spawn a tiny **subject** app as
:code:`daslang-live`, toggles visual aids, calls ``record_start``,
narrates each interaction, performs the action, and lets the helper
shut down. The recorder writes :code:`.apng`; a single ffmpeg pass
converts each to :code:`.mp4` (H.264) before the file ships in-tree.
This is the meta tutorial — the driver script is the artifact, and the
embedded video demonstrates itself.

The helper passes :code:`--imgui-content-scale=1.0` +
:code:`--no-hdpi-framebuffer` to the spawned :code:`daslang-live` so
APNGs stay at logical 1x — small files, fast encode, even on retina.
Tutorials run by users directly (no driver) keep their native HDPI
look.

Source files:

* ``examples/tutorial/recording.das`` — the subject: a one-slider
  one-button window. Tiny on purpose so every frame in the APNG
  corresponds to one specific driver call.
* ``tests/integration/record_recording.das`` — the driver. Walked
  through below.

************
The subject
************

.. literalinclude:: ../../../examples/tutorial/recording.das
   :language: das
   :linenos:

The subject is a normal dasImgui live-reload app — same shape as every
other tutorial's subject. The recorder doesn't touch the subject at
all; it operates entirely through the live-command HTTP surface.

************
The driver
************

.. video:: recording.mp4

The video below is produced by the driver listed under it — the meta loop closes
on itself. It drags ``Volume`` (asserting the value moved) and clicks ``Save``
(asserting the click landed), narrating the recipe as it goes; a silently-broken
beat would abort the recording instead of shipping.

.. literalinclude:: ../../../tests/integration/record_recording.das
   :language: das
   :linenos:

Anatomy of a driver
===================

The driver's :code:`main` is a single :code:`with_recording_app(...)
$(app) { ... }` call. The helper owns the boilerplate; the body owns
the timeline.

1. **Helper-owned: spawn + ready-wait.** :code:`with_recording_app`
   spawns :code:`daslang-live <feature_path>` with
   :code:`--imgui-content-scale=1.0` + :code:`--no-hdpi-framebuffer`
   (and forwards :code:`-project_root` from the driver's own argv),
   waits for the HTTP server to answer :code:`/status` 200, and yields
   an :code:`ImguiApp` to the body block.

2. **Body-owned: resolve widget centers.** Inside the body,
   :code:`wait_for_render` polls :code:`imgui_snapshot` until the named
   widget shows up in the registry (covers the first-frame gap where
   the subject's window exists but hasn't rendered yet). The helper
   :code:`widget_bbox` / :code:`widget_center` extract the geometry
   from the snapshot JSON.

3. **Helper-owned: enable visual aids + ``record_start``.** The
   helper posts :code:`imgui_mouse_trail` + :code:`imgui_cursor_sprite`
   :code:`(enabled=true)` and then :code:`record_start` with the
   :code:`(file, fps, max_seconds)` you passed in. The writer pulls
   from a PBO ring on the GL side (4 buffers by default) —
   :code:`glReadPixels` returns immediately, the actual readback
   happens 3 frames later, the encoder runs on a worker thread.
   Frames drop only if the worker can't keep up with the GL output
   rate, and even then dropping is graceful (the APNG just gets
   slightly choppier — never breaks).

   The absolute APNG path is :code:`<dasimgui>/doc/source/_static/tutorials/<basename>`,
   resolved via :code:`get_this_module_dir()` so caller cwd is
   irrelevant.

4. **Body-owned: narrate, act, verify — repeat.** Each beat pairs an
   ear-first caption with a real gesture and a self-check:

   * :code:`say_begin(app, caption, target, [voice = "..."])` posts the
     caption box and returns the voiceover's length, so the beat is
     *paced by the spoken line* — no hand-tuned frame counts. The terse
     on-screen :code:`caption` is decoupled from the natural spoken
     :code:`voice` (which keys the TTS manifest). Both must be ASCII —
     the bundled ImGui font has no em-dash / arrow / smart-quote glyph,
     so non-ASCII renders as tofu.
   * The gesture runs UNDER the voice and is VERIFIED:
     :code:`drag_through_voice` scrubs a slider and asserts the value
     changed; :code:`hold_through_voice` clicks and asserts the effect
     registered; :code:`force_set_verified` drives a value from outside
     and asserts it took. A no-op accumulates a miss and the recording
     aborts at teardown (``g_record_failures`` panic) — a silently
     broken demo never ships. The driver below is exactly this shape.

5. **Helper-owned: ``record_stop`` + shutdown.** When the body
   returns, the helper posts :code:`record_stop` (flushes the writer,
   joins the encoder thread, prints :code:`(saved, frames, duration_s,
   dropped, ok)`), disables the visual aids, posts :code:`/shutdown`,
   drains stdout. The driver process exits when daslang-live does.

The visual-aids stack
=====================

Four ``[live_command]`` toggles in ``imgui_visual_aids.das``:

* ``imgui_mouse_trail`` — fading line behind ``io.MousePos``. Args:
  ``enabled``, ``fade`` (seconds), ``color`` (RGBA uint).
* ``imgui_cursor_sprite`` — visible pointer drawn at ``io.MousePos``.
  Without it the cursor only exists in the OS layer, which screen
  recordings don't capture.
* ``imgui_narrate`` — a sticky-note callout with optional connector
  line to a target widget. Auto-fits to avoid sibling overlap;
  ``frames`` controls visibility duration.
* ``imgui_highlight`` — a colored rectangle around a widget's bbox for
  N frames. Useful for "look here" without text. ``imgui_auto_highlight``
  flips a global flag that fires highlight on every accepted live
  command — a one-shot debug aid.

Two more for keyboard work:

* ``imgui_key_hud`` — bottom-center keycap overlay + modifier strip.
  Pops a keycap for every synth key event; lights modifier pills
  while held. Useful for input-heavy demos.
* ``imgui_focus_rect`` — a rectangle around ``io.active_widget`` so
  the viewer can tell which widget keystrokes will land in.

The driver workflow in shell
============================

One shell, one command:

.. code-block:: bash

   bin/Release/daslang.exe -project_root <dasimgui> \
       <dasimgui>/tests/integration/record_recording.das

The helper spawns daslang-live, runs the body, posts :code:`/shutdown`,
drains stdout. Wall time = :code:`max_seconds` + ~3s headroom. The APNG
lands at :code:`<dasimgui>/doc/source/_static/tutorials/recording.apng`,
which is gitignored — it's the raw artifact, not the deliverable.

After the recorder finishes, one ffmpeg pass converts to the shipped
:code:`.mp4`:

.. code-block:: bash

   ffmpeg -y -i recording.apng -c:v libx264 -crf 23 -pix_fmt yuv420p \
          -movflags +faststart recording.mp4

Typical UI recording: 50-300 KB MP4 (vs 50-100 MB APNG, ~300x smaller).
The :code:`.mp4` is what RSTs reference and what ships in source.

If you want to iterate without re-recording the host's state, drive a
manually-launched host via :code:`mcp__daslang__live_command` instead
(see "Verifying a recording" in :code:`skills/recording.md`). The
driver script is for the canonical artifact pass.

Stop conditions
===============

``record_stop`` is the clean exit. Three other ways the writer
finalizes:

* ``max_seconds`` expires — same APNG, frame count + duration match
  the cap.
* ``stbi_apng_frame`` returns an error (rare; usually disk full or
  permission denied) — writer auto-stops, returns the partial APNG.
* daslang-live shuts down — the ``[on_app_exit]`` hook on the
  recorder finalizes any in-flight ring before tearing down GL state.

In practice ``record_stop`` is the only exit you'll see; the others
are safety nets.

Replay stability
================

The same driver run against the same subject produces APNGs that look
the same to a viewer — but they're not byte-identical. ImGui's frame
times jitter, vsync alignment shifts, the PBO ring may stall once or
twice on encoder backpressure. ``dropped`` in the response is the
useful metric: anything under ``capture_pbo_count + a few`` is fine.
Higher means the encoder couldn't keep up — try lowering the subject's
render rate, simplifying the subject, or raising ``capture_pbo_count``.

Standalone vs live
==================

The whole recording surface — visual aids, ``record_start`` /
``record_stop``, the playwright helpers — runs only under
``daslang-live``. Standalone ``daslang.exe`` runs the subject, but
the HTTP-bound live commands aren't registered. The driver script
itself is a normal ``daslang.exe`` script — it just talks HTTP to a
host running on another process.

Driving from outside
====================

The same JSON commands the driver issues are reachable from curl:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_mouse_trail","args":{"enabled":true}}'      localhost:9090/command
   curl -X POST -d '{"name":"imgui_cursor_sprite","args":{"enabled":true}}'    localhost:9090/command
   curl -X POST -d '{"name":"record_start","args":{"file":"manual.apng","fps":30,"max_seconds":15}}' \
        localhost:9090/command

   # do whatever interactions interactively in the live window ...

   curl -X POST -d '{"name":"record_stop"}' localhost:9090/command

Use this for ad-hoc captures when the scripted driver would be
overkill — bug repros, "show me what this looks like" pings.

The end of the curriculum
=========================

The 12 tutorials in this set walk the dasImgui surface end-to-end:
basics, widgets, layout, docking, style, identity, state, containers,
live reload, the JSON-driven view, the visual aids tour, and this
recording recipe. Beyond the curriculum, every ``examples/features/``
demo is a richer reference for one specific surface — those are the
go-to once the tutorials are familiar.

.. seealso::

   Subject source: :download:`examples/tutorial/recording.das <../../../examples/tutorial/recording.das>`

   Driver source: :download:`tests/integration/record_recording.das <../../../tests/integration/record_recording.das>`

   Recorder implementation: ``modules/dasOpenGL/opengl/opengl_live.das``
   (``record_start`` / ``record_stop`` plus the PBO ring).

   Visual aids: ``modules/dasImgui/widgets/imgui_visual_aids.das``.

   Playwright helpers: ``modules/dasImgui/widgets/imgui_playwright.das``.

   Previous tutorial: :ref:`tutorial_visual_aids_tour`

   Curriculum top: :ref:`tutorials_dasimgui`
