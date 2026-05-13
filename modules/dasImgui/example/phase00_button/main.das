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
require daslib/json
require daslib/json_boost

// =============================================================================
// Phase 00 — proof of concept: one button, hand-written, no macros.
// First concrete milestone of the API_REWORK.md plan (§10 row 0).
//
// Drive it from a second terminal:
//   curl -X POST -d '{"name":"imgui_snapshot"}' localhost:9090/command
//   curl -X POST -d '{"name":"imgui_click","args":{"target":"DEMO_BTN"}}' \
//        localhost:9090/command
// =============================================================================

// ---- per-widget state (hand-rolled; future: macro-emitted via define_widget)

struct ButtonState {
    hex_id        : uint
    bbox_min      : ImVec2
    bbox_max      : ImVec2
    hover         : bool
    active        : bool
    focus         : bool
    clicked       : bool
    pending_click : bool   // queued by imgui_click cmd, consumed next frame
    click_count   : int    // survives reload via @live
}
// NOTE — @live serializes the WHOLE struct: per-frame fields (pending_click,
// clicked, hover/active/focus, bbox_*, hex_id) survive reload too. Harmless
// for the read-back fields (overwritten next frame) but pending_click can
// re-fire across reload. Phase 0a should split persistent / transient state.

var private @live DEMO_BTN : ButtonState

// ---- per-frame registry (hand-rolled; future: RTTI walk over @live globals)

struct WidgetSnapshot {
    kind        : string
    hex_id      : uint
    bbox_x0     : float
    bbox_y0     : float
    bbox_x1     : float
    bbox_y1     : float
    hover       : bool
    active      : bool
    focus       : bool
    clicked     : bool
    click_count : int
}

var private g_registry : table<string; WidgetSnapshot>
var private g_frame    : int = 0

// ---- the one widget (Phase 0a's `define_widget(button, ButtonState)` will
// generate code shaped like this from a single user-side line)

def render_demo_btn() {
    let imgui_clicked = Button("Click me", ImVec2(160.0, 48.0))

    // capture post-call frame state
    DEMO_BTN.bbox_min = GetItemRectMin()
    DEMO_BTN.bbox_max = GetItemRectMax()
    DEMO_BTN.hover    = IsItemHovered(ImGuiHoveredFlags.None)
    DEMO_BTN.active   = IsItemActive()
    DEMO_BTN.focus    = IsItemFocused()
    DEMO_BTN.hex_id   = GetID("Click me")    // Phase 0a derives id from identifier ("DEMO_BTN"), not label

    // resolve clicked: real input OR queued via imgui_click command
    var clicked = imgui_clicked
    if (DEMO_BTN.pending_click) {
        DEMO_BTN.pending_click = false
        clicked = true
    }
    DEMO_BTN.clicked = clicked
    if (clicked) {
        DEMO_BTN.click_count++
    }

    // push into per-frame registry (Phase 1 replaces with RTTI walk)
    g_registry["DEMO_BTN"] = WidgetSnapshot(
        kind        = "button",
        hex_id      = DEMO_BTN.hex_id,
        bbox_x0     = DEMO_BTN.bbox_min.x,
        bbox_y0     = DEMO_BTN.bbox_min.y,
        bbox_x1     = DEMO_BTN.bbox_max.x,
        bbox_y1     = DEMO_BTN.bbox_max.y,
        hover       = DEMO_BTN.hover,
        active      = DEMO_BTN.active,
        focus       = DEMO_BTN.focus,
        clicked     = DEMO_BTN.clicked,
        click_count = DEMO_BTN.click_count
    )
}

// ---- live commands

struct ClickArgs {
    target : string = ""
}

struct ClickResultOk {
    ok     : bool   = true
    queued : string = ""
}

struct ClickResultErr {
    ok    : bool   = false
    error : string = ""
}

[live_command(description="Phase 00: queue an L2 click. Args: \{\"target\":\"DEMO_BTN\"\}")]
def imgui_click(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<ClickArgs>)
    if (args.target != "DEMO_BTN") return JV(ClickResultErr(error = "unknown target: {args.target}"))
    DEMO_BTN.pending_click = true
    return JV(ClickResultOk(queued = args.target))
}

[live_command(description="Phase 00: full snapshot of the per-frame widget registry")]
def imgui_snapshot(input : JsonValue?) : JsonValue? {
    var globals : table<string; JsonValue?>
    for (k, v in keys(g_registry), values(g_registry)) {
        globals |> insert(k, JV(v))
    }
    return JV({ "frame" => JV(g_frame), "globals" => JV(globals) })
}

// ---- lifecycle. ImGui ctx lifecycle delegated to imgui/imgui_live —
// the ctx pointer is roundtripped via [before_reload]/[after_reload] so
// the font atlas, window state, and active-widget state survive reload.
// `live_imgui_init` is idempotent (cold start vs. reload), `live_imgui_shutdown`
// guards `is_reload()` so it only tears down on process exit.

[export]
def init() {
    live_create_window("Phase 00 — DEMO_BTN", 800, 600)
    live_imgui_init(live_window)
    var io & = unsafe(GetIO())
    io.FontGlobalScale = 2.0
    print("phase00: init reload={is_reload()} clicks={DEMO_BTN.click_count}\n")
}

[export]
def update() {
    if (!live_begin_frame()) return
    g_frame++
    g_registry |> clear()    // start of frame: stale entries gone

    ImGui_ImplOpenGL3_NewFrame()
    ImGui_ImplGlfw_NewFrame()
    NewFrame()

    SetNextWindowSize(ImVec2(720.0, 320.0), ImGuiCond.Always)
    if (Begin("Phase 00", null, ImGuiWindowFlags.None)) {
        Text("Hand-rolled button. Click via GUI or curl.")
        render_demo_btn()
        Text("clicks = {DEMO_BTN.click_count}")
    }
    End()

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
    print("phase00: shutdown reload={is_reload()} clicks={DEMO_BTN.click_count}\n")
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
