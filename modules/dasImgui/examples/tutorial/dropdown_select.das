options gen2
options _comment_hygiene = true

require math
require strings
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

// =============================================================================
// TUTORIAL: dropdown_select — four shapes of pick-one-from-a-list.
//
//   combo(IDENT, (text = "..", items <- ["A", "B", ...]))
//     Items-array sugar — pass the strings, get a single-call dropdown.
//   combo_getter(IDENT, (text = "..", items_count = N,
//                        getter = @(idx; var result : string&) : bool { ... }))
//     Procedural list — the callback supplies row text on demand. Use when
//     items live in an unusual container (table, generator, large dataset).
//   selectable(IDENT, (text = "..", flags = ..., size = ...))
//     One row, self-toggles via ToggleState. Stack N to make a list.
//   selectable_label(IDENT, text, selected, (flags = ..., size = ...))
//     Row whose selected-look is driven by a CALLER-owned bool. Pair with
//     list_box (+ external index) when only one row may be active.
//
// STANDALONE: daslang.exe modules/dasImgui/examples/tutorial/dropdown_select.das
// LIVE:       daslang-live modules/dasImgui/examples/tutorial/dropdown_select.das
//
// DRIVE (when running live):
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DS_WIN/D_FRUIT","value":2}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DS_WIN/D_FONT","value":1}}' \
//        localhost:9090/command
//   curl -X POST -d '{"name":"imgui_force_set","args":{"target":"DS_WIN/D_SHIRT","value":3}}' \
//        localhost:9090/command
// =============================================================================

let FRUITS : array<string> <- ["Apple", "Banana", "Cherry", "Durian", "Elderberry"]
let FONTS  = fixed_array("Serif", "Sans", "Mono", "Cursive")
let TAGS   = fixed_array("urgent", "blocked", "review")
let SHIRTS = fixed_array("XS", "S", "M", "L", "XL", "XXL")

var private D_TAG    : table<int; ToggleState>
var private D_SHIRT_ROW : table<int; ClickState>

[export]
def init() {
    live_create_window("dasImgui dropdown_select tutorial", 760, 640)
    live_imgui_init(live_window)
    let io & = unsafe(GetIO())
    GetStyle().FontScaleMain = 1.4
}

[export]
def update() {
    if (!live_begin_frame()) return
    begin_frame()

    ImGui_ImplGlfw_NewFrame()
    apply_synth_io_override()
    NewFrame()

    SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(720.0f, 600.0f), ImGuiCond.Always)
    window(DS_WIN, (text = "dropdown_select tutorial", closable = false,
                    flags = ImGuiWindowFlags.None)) {

        text("Four shapes of pick-one-from-a-list.")
        text(D_HINT, (text = "combo collapses; selectable rows stay open."))
        separator()

        // ---- Stage 1: combo — items as a string array ----
        combo(D_FRUIT, (text = "Fruit", items <- FRUITS))
        text("D_FRUIT.value = {D_FRUIT.value} ('{FRUITS[clamp(D_FRUIT.value, 0, length(FRUITS) - 1)]}')")
        spacing()

        // ---- Stage 2: combo_getter — callback supplies row text on demand ----
        combo_getter(D_FONT, (text = "Font", items_count = length(FONTS),
            getter = @(idx : int; var result : string&) : bool {
                if (idx >= 0 && idx < length(FONTS)) {
                    result = FONTS[idx]
                    return true
                }
                return false
            }))
        text("D_FONT.value = {D_FONT.value} ('{FONTS[clamp(D_FONT.value, 0, length(FONTS) - 1)]}')")
        spacing()

        // ---- Stage 3: selectable — N rows, each self-toggles independently ----
        text("Tags (multi-select - each row toggles its own ToggleState):")
        for (i in range(length(TAGS))) {
            selectable(D_TAG[i], (text = TAGS[i]))
        }
        text("active tags: {tags_summary()}")
        spacing()

        // ---- Stage 4: selectable_label inside list_box — single-select ----
        text("Shirt size (single-select - caller-owned int picks active row):")
        list_box(D_SHIRT, (text = "size", size = float2(0.0f, 0.0f))) {
            for (i in range(length(SHIRTS))) {
                let is_sel = (i == D_SHIRT.value)
                if (selectable_label(D_SHIRT_ROW[i], SHIRTS[i], is_sel)) {
                    D_SHIRT.value = i
                }
            }
        }
        text("D_SHIRT.value = {D_SHIRT.value} ('{SHIRTS[clamp(D_SHIRT.value, 0, length(SHIRTS) - 1)]}')")
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

def tags_summary() : string {
    var first = true
    return build_string() $(var w : StringBuilderWriter) {
        for (i in range(length(TAGS))) {
            if (D_TAG[i].value) {
                if (!first) {
                    w |> write(", ")
                }
                w |> write(TAGS[i])
                first = false
            }
        }
        if (first) {
            w |> write("(none)")
        }
    }
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
