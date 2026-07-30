options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require imgui/imgui_scope_builtin    // with_button_repeat

// =============================================================================
// FEATURE: internal_item_add_button_behavior — assemble a working button entirely
//          from the imgui_internal.h primitives the public `button` widget is built
//          on. Cherry-picked into the v2 binding via manual C++ wrappers
//          (dasIMGUI.main.cpp): ItemAdd (two overloads — with / without a separate
//          nav rect) and ButtonBehavior (the press/hover/hold state machine; its two
//          state outputs come back through bool& args, written into plain das vars).
//          ItemSize / GetID / GetCursorScreenPos are already-public layout + identity
//          helpers. This is the "binding is usable" gate: it calls the real bound API
//          at runtime, not just compiles it.
// SHOWS:   a hand-rolled `custom_button` (GetID + GetCursorScreenPos -> ItemSize ->
//          ItemAdd -> ButtonBehavior -> drawlist render) used three ways: a plain
//          click button, a Repeat button (ButtonBehavior + ImGuiButtonFlagsPrivate.Repeat
//          combined via PR #173's cross-enum `|` rail — fires while held), and one that
//          uses the 4-arg ItemAdd overload to give the item a nav rect larger than its
//          visual box. Live hovered / held readout proves the state machine runs.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_item_add_button_behavior.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_item_add_button_behavior.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_item_add_button_behavior.das
// =============================================================================

var g_plain = 0
var g_repeat = 0
var g_nav = 0

def private button_fill(hovered, held : bool) : uint {
    let idle = rgba(60u, 90u, 140u, 255u)
    let hover = rgba(90u, 150u, 230u, 255u)
    let down = rgba(40u, 60u, 100u, 255u)
    return held ? down : (hovered ? hover : idle)
}

def private draw_box(bb : float4; lbl : string; fill : uint) {
    let ink = rgba(235u, 240u, 250u, 255u)
    with_window_drawlist() $(var dl) {
        dl |> add_rect_filled(ImVec2(bb.x, bb.y), ImVec2(bb.z, bb.w), fill, 4.0, ImDrawFlags.None)
        dl |> add_text(ImVec2(bb.x + 10.0, bb.y + 8.0), ink, lbl)
    }
}

// The hand-rolled button — every step the public `button` performs, in user code.
// Returns (pressed, hovered, held); the latter two are ButtonBehavior's bool& outputs.
def private custom_button(str_id : string; w, h : float; flags : ImGuiButtonFlags) : tuple<bool; bool; bool> {
    let p = GetCursorScreenPos()
    let bb = float4(p.x, p.y, p.x + w, p.y + h)
    let id = GetID(str_id)
    ItemSize(bb)                        // reserve layout space (advances the cursor)
    // register the item; bail (item is clipped) if ItemAdd returns false
    if (!ItemAdd(bb, id)) return (false, false, false)
    var hovered = false
    var held = false
    let pressed = ButtonBehavior(bb, id, hovered, held, flags)
    draw_box(bb, str_id, button_fill(hovered, held))
    return (pressed, hovered, held)
}

[export]
def init() {
    harness_init("internal_item_add_button_behavior - hand-rolled button from primitives", 760, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    window(CB_WIN, (text = "internal_item_add_button_behavior", closable = false,
                    flags = ImGuiWindowFlags.None)) {
        text("A button assembled from imgui_internal.h primitives:")
        text("  ItemSize -> ItemAdd -> ButtonBehavior -> drawlist render.")
        separator()

        // ----- 1. plain click button -----
        let r1 = custom_button("custom: click me", 220.0, 36.0, ImGuiButtonFlags.None)
        g_plain++ if (r1._0)
        same_line()
        text("clicks: {g_plain}   hovered={r1._1} held={r1._2}")

        // ----- 2. repeat button — fires while held -----
        // with_button_repeat pushes the ButtonRepeat item flag so the inner
        // ButtonBehavior picks it up from the item-flag stack.
        with_button_repeat(true) {
            let r2 = custom_button("custom: hold to repeat", 220.0, 36.0, ImGuiButtonFlags.None)
            g_repeat++ if (r2._0)
            same_line()
            text("repeat fires: {g_repeat}   hovered={r2._1} held={r2._2}")
        }
        separator()

        // ----- 3. ItemAdd 4-arg overload: a nav rect larger than the click box -----
        // The nav rect is what keyboard nav scrolls-to / highlights when the item is
        // focused; here it is inflated 16px around the visual button box.
        text("ItemAdd 4-arg overload — explicit nav rect (inflated around the box):")
        let p3 = GetCursorScreenPos()
        let bb3 = float4(p3.x, p3.y, p3.x + 220.0, p3.y + 36.0)
        let nav3 = float4(bb3.x - 16.0, bb3.y - 16.0, bb3.z + 16.0, bb3.w + 16.0)
        let id3 = GetID("custom: nav rect")
        ItemSize(bb3)
        if (ItemAdd(bb3, id3, nav3)) {
            var h3 = false
            var d3 = false
            if (ButtonBehavior(bb3, id3, h3, d3, ImGuiButtonFlags.None)) {
                g_nav++
            }
            draw_box(bb3, "custom: nav rect", button_fill(h3, d3))
        }
        dummy(SP3, ImVec2(700.0, 10.0))
        text("nav-rect clicks: {g_nav}")
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
