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
require imgui/imgui_id_builtin
require imgui/imgui_visual_aids

// =============================================================================
// TUTORIAL: with_id — scope IDs and registry paths for composed render sites.
//
// Each widget's ImGui hash is derived from its identifier — the same global
// rendered at two sites would hash to the same value and collide. The boost
// already enforces "single-global rendered exactly once per frame" at the
// registry level (loops want the indexed form `IDENT[i]`), but real apps
// still need to compose helpers that surface the same widget kind under
// distinct labels. Three knobs cover the cases:
//
//   with_id("scope") { ... }         — push "scope" onto BOTH the ImGui ID
//                                      stack AND the boost path. Distinct
//                                      scopes → distinct hex_ids AND distinct
//                                      registry paths.
//   widget(IDENT, (id = "x"))        — mangle ImGui's hash with "x" only;
//                                      telemetry path stays the bare
//                                      identifier (script-stable across
//                                      label renames; preserves imgui.ini).
//   widget(IDENT, (path = "x"))      — replace the telemetry path leaf with
//                                      "x"; the bare identifier still names
//                                      the daslang global. Mangles BOTH the
//                                      path AND the ImGui hash.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/with_id.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/with_id.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_snapshot"}'                                                       localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"ID_WIN/section_a/SAVE_BTN"}}'             localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"ID_WIN/section_b/SAVE_BTN"}}'             localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"ID_WIN/rps_stable","value":0.7}}'           localhost:9090/command
// =============================================================================

[export]
def init() {
    live_create_window("dasImgui with_id tutorial", 720, 520)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.5
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(30.0f, 30.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 440.0f), ImGuiCond.FirstUseEver)
    window(ID_WIN, (text = "with_id", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        // ---- with_id scope: same widget, two render sites ----

        // SAVE_BTN is a single-global widget; rendering it twice without with_id
        // panics at end-of-frame (registry N>1). Each scope adds a path segment +
        // PushID, so ID_WIN/section_a/SAVE_BTN and .../section_b/... stay distinct.
        text("Same SAVE_BTN, two scopes:")
        with_id("section_a") {
            button(SAVE_BTN, (text = "Save in A"))
        }
        with_id("section_b") {
            button(SAVE_BTN, (text = "Save in B"))
        }
        text("SAVE_BTN.click_count aggregates: {SAVE_BTN.click_count}")

        separator(WI_SEP_1)

        // ---- Nested chains: outer scope, inner scope, leaf widget ----
        // The path is the chain joined by "/": ID_WIN/outer/inner/NESTED_BTN.
        text("Nested with_id chains:")
        with_id("outer") {
            with_id("inner") {
                button(NESTED_BTN, (text = "Nested deep"))
            }
        }

        separator(WI_SEP_2)

        // ---- id= per-call: stable ImGui hash across label renames ----

        // HASHED_BTN (the daslang identifier) drives visibility + live-reload; the
        // imgui hash is mangled by "stable_v1", so hex_id-targeting scripts and
        // imgui.ini state survive a rename. Telemetry path stays ID_WIN/HASHED_BTN.
        text("Per-call id= - script-stable ImGui hash:")
        button(HASHED_BTN, (text = "Hashed via id=", id = "stable_v1"))

        separator(WI_SEP_3)

        // ---- path= per-call: stable telemetry path across renames ----

        // RPS_PATH is the daslang global (rename breaks the reference). "rps_stable"
        // is the registry path leaf — the imgui_force_set / imgui_click string for
        // external drivers; both the path AND the ImGui hash adopt "rps_stable".
        text("Per-call path= - script-stable telemetry path:")
        slider_float(RPS_PATH, (text = "Speed (path=)",
                                path = "rps_stable"))
    }

    end_of_frame()
    Render()
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f)
    glClear(GL_COLOR_BUFFER_BIT)
    live_imgui_render()

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
