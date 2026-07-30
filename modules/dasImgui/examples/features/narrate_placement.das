options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_visual_aids

// =============================================================================
// FEATURE: narrate_placement - interactive harness for the narrate() banner
//          placement cascade. 4x5 grid of buttons spanning the viewport, with
//          edge buttons pushed close to the corners so right/left/below/above
//          placements each fail in different regions. Click any button to make
//          it the narration target; the banner moves to wherever the placement
//          cascade decides.
// SHOWS:   narrate() + highlight() retargeting frame-to-frame, plus a toggle
//          to hide narration. Selected button is outlined by highlight(); the
//          banner points at it via a connector line.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/narrate_placement.das
// LIVE:       daslang-live modules/dasImgui/examples/features/narrate_placement.das
// =============================================================================

let GRID_ROWS = 4
let GRID_COLS = 5
let VIEW_W = 1280.0f
let VIEW_H = 720.0f
let BTN_W  = 96.0f
let BTN_H  = 32.0f

// One ClickState slot per button. Indexed-form path: MAIN_WIN/GRID_BTN[<idx>].
var GRID_BTN : table<int; ClickState>

var g_selected_path : string = ""
var g_selected_text : string = ""

[export]
def init() {
    harness_init("narrate placement test", int(VIEW_W), int(VIEW_H))
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.2
}

def private grid_pos(r : int; c : int) : float2 {
    // Edge buttons hug the viewport corners so placement candidates fail there — exercises tier-2/tier-3 fallback.
    let mx = 16.0f
    let my_top = 96.0f
    let my_bot = 16.0f
    let stride_x = (VIEW_W - 2.0f * mx - BTN_W) / float(GRID_COLS - 1)
    let stride_y = (VIEW_H - my_top - my_bot - BTN_H) / float(GRID_ROWS - 1)
    return float2(mx + float(c) * stride_x, my_top + float(r) * stride_y)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(VIEW_W, VIEW_H), ImGuiCond.Always)
    window(MAIN_WIN, (text = "narrate placement", closable = false,
                      flags = ImGuiWindowFlags.NoMove |
                              ImGuiWindowFlags.NoResize |
                              ImGuiWindowFlags.NoCollapse)) {
        checkbox(SHOW_NARRATION, (text = "Show narration", init = true))
        let caption = empty(g_selected_text) ? "(none - click a button)" : g_selected_text
        text("Selected: {caption}")

        for (r in range(GRID_ROWS)) {
            for (c in range(GRID_COLS)) {
                let idx = r * GRID_COLS + c
                let p = grid_pos(r, c)
                SetCursorPos(ImVec2(p.x, p.y))
                if (button(GRID_BTN[idx], (text = "r{r}c{c}",
                                           size = float2(BTN_W, BTN_H)))) {
                    g_selected_path = "MAIN_WIN/GRID_BTN[{idx}]"
                    g_selected_text = "Button r{r}c{c} at ({int(p.x)}, {int(p.y)})"
                }
            }
        }
    }

    // Re-push narrate + highlight every frame while a selection is active.
    // frames=1: paint draws this frame then auto-erases, so swapping selection
    // immediately drops the previous target's banner/outline.
    if (SHOW_NARRATION.value && !empty(g_selected_path)) {
        narrate(g_selected_text, g_selected_path, 1)
        highlight(g_selected_path, 1)
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
