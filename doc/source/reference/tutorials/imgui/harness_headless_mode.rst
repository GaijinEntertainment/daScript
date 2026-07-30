.. _tutorial_harness_headless_mode:

#############################
Harness + headless mode
#############################

``imgui/imgui_harness`` is the canonical wrapper for dasImgui examples and
tests. It hides the GLFW/OpenGL backend boilerplate behind six helpers and
adds a ``--headless`` CLI mode that runs the same script with no window and
no GL context — for CI, smoke tests, and Playwright drivers without a
display server.

This tutorial covers what ``--headless`` does to the harness internals,
how to launch a script in either mode, and the limits of headless
(``screenshot`` / ``record_*`` and the live-API HTTP endpoint stay
windowed-only).

************
Why headless
************

A dasImgui test today opens a real GLFW window and a real OpenGL context.
That makes CI runners pick a display server (xvfb on Linux, an interactive
session on Windows), and pins every smoke test to the cost of a windowed
run. Most tests don't need either — they just exercise widget logic, the
boost layer, or live-command dispatch.

The harness keeps the same script source but lets the runtime pick:

* Windowed (default) — open a window, run the GLFW + OpenGL chain, render
  to the back buffer. Identical to the pre-harness path.
* Headless (``-- --headless``) — create an ImGui context with a CPU-only
  font atlas, no window, no GL. ``Render()`` runs and the resulting
  ``ImDrawData`` is discarded.

************************
Running tests headless
************************

The harness parses one CLI flag, lazily on first ``harness_*`` call:

.. code-block:: bash

   # Windowed (default).
   daslang.exe modules/dasImgui/examples/features/foundation.das

   # Headless. Note the `--` separator: daslang's interpreter mode
   # consumes everything after it as user-script argv.
   daslang.exe modules/dasImgui/examples/features/foundation.das -- --headless

   # Headless with a 600-frame auto-exit cap (useful for unattended CI
   # smokes that have no out-of-band exit signal).
   daslang.exe modules/dasImgui/examples/features/foundation.das -- --headless --headless-frames=600

In windowed mode the script's loop terminates when the window closes
(``live_begin_frame`` calls ``request_exit()``). In headless there is no
window, so the script runs until either (a) something inside the script
calls ``request_exit()`` or (b) ``--headless-frames=N`` is set and the
counter hits ``N`` (the harness then calls ``request_exit()`` itself).

************************************
What --headless does internally
************************************

The harness exports six helpers; the frame/backend helpers branch on
``harness_is_headless()`` at call time:

``harness_init(title, width, height)``
   * Windowed: ``live_create_window`` + ``live_imgui_init``.
   * Headless: ``CreateContext`` + ``StyleColorsDark`` + CPU font-atlas
     build + ``DisplaySize``. No window, no GL context.

``harness_begin_frame() : bool``
   * Windowed: poll GLFW events, run boost ``begin_frame``, then
     ``ImGui_ImplGlfw_NewFrame`` (input).
   * Headless: feed ``ImGuiIO::DeltaTime`` from real wall-clock, run
     boost ``begin_frame``, advance the headless frame counter. If
     ``--headless-frames=N`` is set and the counter reaches ``N``, call
     ``request_exit()`` and return ``false`` (the script's loop exits).

``harness_maybe_collect_gc()``
   Standalone-only heap boundary. Call it from ``main`` immediately after
   ``update()`` returns. It is a no-op under ``daslang-live``. The entry script
   must declare ``options persistent_heap`` and ``options gc``.

``harness_apply_synth_io()`` *(optional)*
   Drains the synth IO timeline (``imgui_mouse_play``, ``imgui_key_chord``,
   …). Identical in both modes — windowed needs it to win the GLFW
   backend's per-frame poll race; headless needs it because it's the only
   IO source. Call between ``harness_begin_frame`` and
   ``harness_new_frame``.

``harness_new_frame()``
   ``ImGui::NewFrame()``. Identical in both modes.

``harness_end_frame()``
   * Windowed: boost ``end_of_frame`` + ``Render`` + viewport/clear +
     ``live_imgui_render`` (pure-das GL renderer) + ``live_end_frame``
     (swap buffers).
   * Headless: ``end_of_frame`` + ``Render``. The resulting
     ``ImDrawData`` is discarded — there is no GL backend to consume it.

``harness_shutdown()``
   * Windowed: ``live_imgui_shutdown`` + ``live_destroy_window``.
   * Headless: ``DestroyContext`` on the harness-owned ImGui context.

The C++ side uses two side-by-side shared modules:

* ``imguiApp.shared_module`` — the windowed backend (GLFW + OpenGL, plus
  the ``imgui_synth_*`` event injectors that drive synth IO in either
  mode).
* ``imguiAppHeadless.shared_module`` — three thin helpers
  (``imgui_headless_init_fonts``, ``imgui_headless_set_display_size``,
  ``imgui_headless_advance_dt``). No GLFW, no OpenGL, no gl3w. Loaded by
  the harness alongside ``imguiApp`` so runtime dispatch can pick.

Both DLLs link into every dasImgui binary, so headless still requires
GLFW libs to be findable on the host (the OS DLL loader resolves
``imguiApp.shared_module``'s deps regardless of whether ``--headless`` is
active). Truly minimal-deploy headless (no GLFW libs at all) is a future
build-flag concern.

**********************************
Default-on lint: HARNESS001
**********************************

Bundled with the harness (``require imgui/imgui_harness_lint public`` from
``imgui_harness.das``) is a default-on ``[lint_macro]`` that fires whenever a
file requiring ``imgui/imgui_harness`` reaches into the windowed-backend
modules directly. Forbidden modules:

* ``glfw_boost`` / ``opengl_boost``
* ``glfw_live`` / ``opengl_live``
* ``imgui_live``

The structural private-require gate in ``imgui_harness.das`` already hides
those modules from harness consumers, so a clean file never trips the lint.
The lint is the second line of defense — it catches files that explicitly
re-add a backend ``require`` to bypass the gate. Diagnostic code is
``HARNESS001`` (macro_error 50503).

Per-file escape, for the rare windowed-only test that needs a backend
symbol directly (screenshot pipelines, custom GL clears, etc.):

.. code-block:: das

   options _allow_glfw_calls = true

This is scaffolding only. The target end-state for the migration is
**no opt-outs anywhere in** ``examples/``; the option will be removed from
the registry once every harness-using file is clean. Same pattern as the
``_allow_imgui_legacy`` opt-out for ``imgui_lint``.

***********************
Limits under --headless
***********************

* ``screenshot`` and ``record_start`` / ``record_stop`` from
  ``opengl/opengl_live`` assume a GL framebuffer. They have no headless
  analogue and either no-op or panic depending on the path. Tests that
  capture screenshots or APNGs stay windowed.
* The live-API HTTP endpoint at ``localhost:9090/command`` is installed
  by the ``daslang-live`` host. ``daslang.exe`` (the interpreter) runs
  the script without the HTTP server, so Playwright drivers that send
  JSON commands over HTTP need ``daslang-live`` — but the host runs the
  HTTP stack in either mode (windowed or headless), so the playwright
  flow (``/status`` + ``/command`` + ``/shutdown``) works under
  ``--headless`` too.

``daslang-live`` forwards everything after its own ``--`` separator to
the script's ``get_user_args()`` (daslang PR #2681), so
``daslang-live FILE.das -- --headless`` reaches
``harness_is_headless()`` inside the live-reload session. ``imgui_playwright``
forwards the flag from its own user-args to every spawned subprocess, so a
single ``--headless`` on the dastest command line propagates through:
``daslang dastest.das -- --test integration --headless``.

Tests that need the windowed-only surfaces (screenshot / record_*) opt out
of the harness lint per-file (``options _allow_glfw_calls = true``) and
run windowed regardless of the ``--headless`` flag — ``harness_is_headless()``
is a hint, not a hard switch.
