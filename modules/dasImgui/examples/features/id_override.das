options gen2

require imgui
require imgui_app
require glfw/glfw_boost
require opengl/opengl_boost
require live/glfw_live
require live/live_api
require live/live_commands
require live/live_vars
require live/opengl_live
require live_host
require imgui/imgui_live
require imgui/imgui_boost_runtime
require imgui/imgui_boost_v2
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_id_builtin

// =============================================================================
// FEATURE: id_override — §4.8 ID-override surface in full.
// SHOWS:   - `with_id("section_a") { button(...) }` segments the widget path so
//            two render sites of the same widget kind (distinct identifiers)
//            register under "<window>/section_a/<IDENT>" vs ".../section_b/...".
//          - Nested `with_id` chains compose ("outer/inner/NESTED_BTN").
//          - The same single-global widget rendered under two distinct with_id
//            scopes does NOT trip the once-per-frame panic — check_unique_render
//            keys off the full path, not the bare identifier.
//          - Per-call `id="x"` named arg: macro wraps the render call in
//            PushID(x)/PopID. Telemetry path stays the bare identifier; only
//            the ImGui hash changes (lets the daslang identifier be renamed
//            without invalidating imgui.ini state or hex_id-based scripts).
//          - Per-call `path="x"` named arg: macro passes "x" as widget_ident
//            to the underlying render fn. Both telemetry path AND ImGui hash
//            adopt "x" (since widget_prelude does PushID(widget_ident)).
//          - Both can be combined per call.
// DRIVE:   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//          # → widgets register under MAIN_WIN/section_a/SAVE_A_BTN etc.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/id_override.das
// LIVE:       daslang-live modules/dasImgui/examples/features/id_override.das
// =============================================================================

[export]
def init() {
    live_create_window("§4.8 — id_override (with_id)", 720, 540)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(MAIN_WIN, (text = "with_id demo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        Text("Distinct widgets, distinct scopes:")
        with_id("section_a") {
            button(SAVE_A_BTN, (text = "Save A"))
        }
        with_id("section_b") {
            button(SAVE_B_BTN, (text = "Save B"))
        }

        Separator()
        Text("Same single-global widget under two scopes:")
        // SHARED_BTN's state global is shared; only the paths and ImGui
        // hashes differ. click_count aggregates across both render sites.
        with_id("xa") {
            button(SHARED_BTN, (text = "Shared (A)"))
        }
        with_id("xb") {
            button(SHARED_BTN, (text = "Shared (B)"))
        }
        Text("shared clicks (sum): {SHARED_BTN.click_count}")

        Separator()
        Text("Nested with_id chains:")
        with_id("outer") {
            with_id("inner") {
                button(NESTED_BTN, (text = "Nested"))
            }
        }

        Separator()
        Text("Per-call id= and path= overrides:")
        // id="hashed_v1" — telemetry path stays "MAIN_WIN/HASHED_BTN"; ImGui's
        // hash for the button is mangled with "hashed_v1" via PushID/PopID
        // wrapping. button is bool-returning, but id= wraps in a void block
        // so we call as a statement (intentional restriction; use with_id for
        // the bool-return form).
        button(HASHED_BTN, (text = "Hashed via id=", id = "hashed_v1"))

        // path="rps_v1" — telemetry path becomes "MAIN_WIN/rps_v1"; the bare
        // identifier RPS_PATH still names the state global (drives daslang
        // visibility, hot-reload), but the registry path uses "rps_v1".
        slider_float(RPS_PATH, (text = "Speed (path=)", path = "rps_v1"))

        // Both: distinct hash AND distinct telemetry path.
        slider_float(RPS_BOTH, (text = "Both (id=+path=)",
                                id = "rps_a", path = "rps_b"))
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    ImGui_ImplOpenGL3_RenderDrawData(GetDrawData())

    live_end_frame()
}

[export]
def shutdown() {
    live_imgui_shutdown()
    live_destroy_window()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
