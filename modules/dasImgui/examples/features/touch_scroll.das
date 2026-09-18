options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require strings

// =============================================================================
// FEATURE: touch_scroll — a fixed window taller than its content area, for the
//          two-finger scroll gesture (imgui/imgui_touch).
// DRIVE:   curl -X POST -d '{"name":"imgui_touch_inject","args":{"touches":[{"id":1,"x":150,"y":150,"began":true},{"id":2,"x":170,"y":150,"began":true}]}}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_touch_inject","args":{"touches":[{"id":1,"x":150,"y":90},{"id":2,"x":170,"y":90}]}}' localhost:9090/command
//          curl -X POST -d '{"name":"imgui_touch_inject","args":{"touches":[{"id":1,"x":150,"y":90,"ended":true},{"id":2,"x":170,"y":90,"ended":true}]}}' localhost:9090/command
//          curl -X POST -d '{"name":"touch_scroll_state"}' localhost:9090/command
// HEADLESS: daslang.exe modules/dasImgui/examples/features/touch_scroll.das -- --headless --headless-frames=60
// =============================================================================

var private g_scroll_y : float = 0.0
var private g_scroll_max_y : float = 0.0
var private g_win_pos : float2
var private g_frames : int = 0
var private g_lines : string
var private g_mouse_down : bool = false
var private g_mouse_pos : float2

[live_command(description = "Scroll and window position of the scroll window as of the last frame")]
def touch_scroll_state(_input : JsonValue?) : JsonValue? {
    return JV((scroll_y = g_scroll_y, scroll_max_y = g_scroll_max_y, win_x = g_win_pos.x, win_y = g_win_pos.y, frames = g_frames,
        mouse_down = g_mouse_down, mouse_x = g_mouse_pos.x, mouse_y = g_mouse_pos.y))
}

[export]
def init() {
    harness_init("touch scroll", 640, 480)
    g_lines = build_string() $(var w) {
        for (i in range(80)) {
            w |> write("line {i}\n")
        }
    }
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    let io & = unsafe(GetIO())
    g_mouse_down = io.MouseDown[0]
    g_mouse_pos = float2(io.MousePos.x, io.MousePos.y)
    SetNextWindowPos(ImVec2(40.0, 40.0), ImGuiCond.Once)
    SetNextWindowSize(ImVec2(320.0, 240.0), ImGuiCond.Once)
    window(SCROLL_WIN, (text = "Scroll me", closable = false, flags = ImGuiWindowFlags.None)) {
        text("{g_lines}")
        g_scroll_y = GetScrollY()
        g_scroll_max_y = GetScrollMaxY()
        let p = GetWindowPos()
        g_win_pos = float2(p.x, p.y)
    }
    g_frames++
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
        harness_maybe_collect_gc()
    }
    shutdown()
}
