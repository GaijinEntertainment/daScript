options gen2
options _comment_hygiene = true

require imgui/imgui_harness

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
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/id_override.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/id_override.das
// =============================================================================

[export]
def init() {
    harness_init("§4.8 — id_override (with_id)", 720, 540)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.FirstUseEver)
    SetNextWindowSize(ImVec2(640.0f, 480.0f), ImGuiCond.FirstUseEver)
    window(MAIN_WIN, (text = "with_id demo", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Distinct widgets, distinct scopes:")
        with_id("section_a") {
            button(SAVE_A_BTN, (text = "Save A"))
        }
        with_id("section_b") {
            button(SAVE_B_BTN, (text = "Save B"))
        }

        separator()
        text("Same single-global widget under two scopes:")
        // SHARED_BTN's state global is shared; only the paths and ImGui
        // hashes differ. click_count aggregates across both render sites.
        with_id("xa") {
            button(SHARED_BTN, (text = "Shared (A)"))
        }
        with_id("xb") {
            button(SHARED_BTN, (text = "Shared (B)"))
        }
        text("shared clicks (sum): {SHARED_BTN.click_count}")

        separator()
        text("Nested with_id chains:")
        with_id("outer") {
            with_id("inner") {
                button(NESTED_BTN, (text = "Nested"))
            }
        }

        separator()
        text("Per-call id= and path= overrides:")
        // id="hashed_v1" — telemetry path stays "MAIN_WIN/HASHED_BTN" but ImGui's
        // hash is mangled with "hashed_v1" via PushID/PopID. id= wraps the button
        // in a void block, so call it as a statement (use with_id for bool-return).
        button(HASHED_BTN, (text = "Hashed via id=", id = "hashed_v1"))

        // path="rps_v1" — telemetry path becomes "MAIN_WIN/rps_v1"; the bare
        // identifier RPS_PATH still names the state global (drives daslang
        // visibility, hot-reload), but the registry path uses "rps_v1".
        slider_float(RPS_PATH, (text = "Speed (path=)", path = "rps_v1"))

        // Both: distinct hash AND distinct telemetry path.
        slider_float(RPS_BOTH, (text = "Both (id=+path=)",
                                id = "rps_a", path = "rps_b"))
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
