options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: inputs_choice — combo (items-array, single-call) + list_box
//          (Begin/End + Selectable, unified block-arg form). list_box was
//          unified onto BeginListBox/EndListBox in the boost scope-wrappers
//          pass; body emits per-item Selectables, click writes state.value.
// SHOWS:   combo (items-array sugar) / list_box (block-arg + Selectable)
// DRIVE:   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/FRUIT","value":2}}' \
//               localhost:9090/command
//          curl -X POST -d '{"name":"imgui_force_set","args":{"target":"MAIN_WIN/SHIRT_SIZE","value":1}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/inputs_choice.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/inputs_choice.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/inputs_choice.das
//
// NOT YET COVERED HERE:
//   - BeginCombo/EndCombo (block-arg manual loop) → combo_select container
//   - _builtin_Combo (callback-getter)            → 0b.4 ABI shim
// =============================================================================

let SHIRT_NAMES : array<string> <- ["XS", "S", "M", "L", "XL", "XXL"]
var SHIRT_ROW : table<int; ClickState>

[export]
def init() {
    harness_init("Phase 0b.2 — choice inputs", 720, 500)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(660.0, 460.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "Choice inputs — phase 0b.2", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Combo (dropdown selector)")
        separator()

        combo(FRUIT, (text = "Fruit",
                      items <- ["Apple", "Banana", "Cherry", "Durian", "Elderberry"]))
        text("fruit index = {FRUIT.value}")

        // popup_max_height_in_items caps the dropdown height — useful for long lists.
        combo(WEEKDAY, (text = "Weekday",
                        items <- ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"],
                        popup_max_height_in_items = 4))
        text("weekday index = {WEEKDAY.value}")

        spacing()
        text("ListBox (always-open scrollable list)")
        separator()

        list_box(SHIRT_SIZE, (text = "Shirt size",
                              size = float2(0.0f, 0.0f))) {
            for (i in range(length(SHIRT_NAMES))) {
                let is_sel = (i == SHIRT_SIZE.value)
                if (selectable_label(SHIRT_ROW[i], SHIRT_NAMES[i], is_sel)) {
                    SHIRT_SIZE.value = i
                }
            }
        }
        text("shirt size index = {SHIRT_SIZE.value}")

        spacing()
        text("imgui_force_set with target=MAIN_WIN/FRUIT, value=N jumps to N-th item next frame")
    }

    harness_end_frame()
}

[export]
def shutdown() {
    harness_shutdown()
}

[export]
def main() {
    //! Standalone entrypoint. daslang-live drives init/update/shutdown
    //! directly and ignores main(); plain `daslang.exe <file>` runs this loop.
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
