options gen2
options _comment_hygiene = true

require imgui
require imgui_app
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_visual_aids
require daslib/json public
require daslib/json_boost public

// =============================================================================
// TUTORIAL: live_reload — the daslang-live workflow that every earlier
// tutorial implicitly relied on.
//
// Every tutorial's STANDALONE / LIVE blocks pointed at the same shape:
//
//   STANDALONE: daslang.exe <script>
//   LIVE:       daslang-live <script>
//
// In the live mode, the script is hosted inside daslang-live: a wrapper
// process that runs your `init` / `update` / `shutdown` exports, watches
// the source file for edits, and exposes an HTTP server for `imgui_force_set` /
// `imgui_click` / `imgui_snapshot` and any user-defined `[live_command]`.
// Save the file and daslang-live re-runs the typer/codegen, calls
// `[before_reload]` hooks to stash anything that needs surviving, swaps in
// the new program, then calls `[after_reload]` to restore. The GLFW
// window stays open; the ImGui context, registered widget state, dock
// layout, slider values — anything @live or restored via a hook — all
// carry across the gap.
//
// This tutorial demonstrates each piece:
//
//   1. `live_create_window` / `live_imgui_init` — idempotent, reload-safe
//   2. `live_begin_frame()` — per-frame gate; returns false while paused
//                              or reloading. Skip the frame if it does.
//   3. `@live` on a state struct — value preserved across reload
//   4. `[live_command]` — custom HTTP endpoint that the running process
//                          exposes; driven from curl just like imgui_force_set
//   5. `[before_reload]` / `[after_reload]` — explicit save/restore hooks
//                                              for state the framework
//                                              doesn't track automatically
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/live_reload.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/live_reload.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}'                                                       localhost:9090/command
//   curl -X POST -d '{"name":"bump_counter"}'                                                          localhost:9090/command
//   curl -X POST -d '{"name":"reset_counter"}'                                                         localhost:9090/command
//   curl -X POST                                                                                       localhost:9090/reload
// =============================================================================

// ---- Custom counter that survives reload via @live ----
// Plain module-scope var, marked @live so daslang-live's serializer hauls
// it across the reload boundary. Non-`@live` globals reinitialise on
// reload — see the demo_string field below for that contrast.
var private @live g_custom_counter : int = 0
// NOT @live — gets reset on each reload (intentional for the demo).
var private g_session_string : string = "fresh session"

// ---- Custom live_command — externally callable HTTP endpoint ----
// imgui_force_set / imgui_click / imgui_snapshot are all `[live_command]`
// internally. Users can add their own:
struct BumpArgs {
    @optional by : int = 1
}

[live_command(description = "Bump the custom counter by N (default 1).")]
def bump_counter(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<BumpArgs>)
    g_custom_counter += args.by
    return JV((ok = true, counter = g_custom_counter))
}

[live_command(description = "Reset the custom counter to 0.")]
def reset_counter(_input : JsonValue?) : JsonValue? {
    g_custom_counter = 0
    return JV((ok = true, counter = 0))
}

// ---- Reload hooks — fire on reload boundary ----

// [before_reload] runs in the OLD program before the new one loads — stash
// non-`@live` data for [after_reload] to restore. Usually marking a var `@live`
// suffices; hooks are the escape hatch for raw pointers, file handles, GL textures.
[before_reload]
def private on_before_reload() {
    print("[live_reload tutorial] before_reload fired. g_custom_counter = {g_custom_counter}\n")
}

[after_reload]
def private on_after_reload() {
    print("[live_reload tutorial] after_reload fired. g_custom_counter = {g_custom_counter} (preserved by @live)\n")
}

[export]
def init() {
    // Both calls are idempotent on reload — cold start creates the window + ImGui
    // context; reload reuses the preserved context and skips the duplicate
    // CreateContext (see live_imgui_init in imgui_live.das).
    live_create_window("dasImgui live_reload tutorial", 1040, 720)
    live_imgui_init(live_window)
    DisableIniPersistence()
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5

    // Re-running init resets g_session_string each reload. (Initial-value
    // assignments at module scope run once at program load, but `init` is
    // called both on cold-start AND on reload.)
    g_session_string = "fresh session"
}

[export]
def update() {
    // The frame-gate. Returns false while daslang-live is paused, in the
    // middle of swapping programs, or in any other state where rendering
    // would crash or produce garbage. Always early-out on false.
    if (!live_begin_frame()) return

    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 460.0f), ImGuiCond.FirstUseEver)
    window(LIVE_WIN, (text = "live_reload", closable = false,
                      flags = ImGuiWindowFlags.None)) {

        // ---- A slider whose value survives reload (@live in SliderStateFloat) ----
        text("VOLUME.value is @live - survives reload.")
        slider_float(VOLUME, (text = "Volume"))

        separator(LR_SEP_1)

        // ---- The custom counter, driven from outside via [live_command] ----

        // Mirror the @live counter into text_show so the snapshot can address it;
        // g_custom_counter stays the plain @live var, this is just the readout.
        CTR_TEXT.value := "g_custom_counter = {g_custom_counter}"
        text_show(CTR_TEXT)
        text("  - @live, preserved across reload")
        text("  - mutated externally by `bump_counter` / `reset_counter`")

        separator(LR_SEP_2)

        // ---- Contrast: session string resets each reload ----
        text("g_session_string = \"{g_session_string}\"")
        text("  - NOT @live; init() rewrites it each reload")

        separator(LR_SEP_3)

        // ---- A button whose click count is preserved (ClickState is @live) ----
        if (button(PING_BTN, (text = "Ping (click_count survives reload)"))) {}
        text("PING_BTN.click_count = {PING_BTN.click_count}")
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

    // Always match live_begin_frame with live_end_frame to keep the
    // frame pump alive on the daslang-live side.
    live_end_frame()
}

[export]
def shutdown() {
    // Idempotent on reload - live_imgui_shutdown skips during reload so
    // the ImGui context is preserved; only the cold process exit runs
    // the full teardown.
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    // Standalone entrypoint. daslang-live drives init/update/shutdown
    // directly and ignores main(); plain `daslang.exe <file>` runs this
    // loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
