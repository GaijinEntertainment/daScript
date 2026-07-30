options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: drag_drop — [container]s drag_drop_source + drag_drop_target.
//          Both gate their body on ImGui's internal drag-drop state machine.
//          Bodies call raw SetDragDropPayload / AcceptDragDropPayload —
//          payload data is caller-owned and crosses widget boundaries.
// SHOWS:   drag SOURCE_BTN onto TARGET_BTN; TARGET_BTN's body accepts the
//          "MY_INT" payload and writes it into RECEIVED. DROP_COUNT
//          increments on each successful release-over-target.
// STANDALONE: daslang.exe modules/dasImgui/examples/features/drag_drop.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/drag_drop.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/drag_drop.das
// =============================================================================

var PAYLOAD_VALUE : int = 42
var RECEIVED : int = 0
var DROP_COUNT : int = 0

[export]
def init() {
    harness_init("drag_drop — source + target", 720, 480)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.5
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(660.0, 420.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "drag_drop", closable = false,
                      flags = ImGuiWindowFlags.None)) {
        text("Drag the SOURCE button onto the TARGET button.")
        separator()

        // Source: a normal button. Body runs only on active drag.
        button(SOURCE_BTN, (text = "SOURCE"))
        drag_drop_source(SOURCE_DD, (flags = ImGuiDragDropFlags.None)) {
            unsafe {
                SetDragDropPayload("MY_INT", addr(PAYLOAD_VALUE),
                                   uint64(typeinfo sizeof(PAYLOAD_VALUE)),
                                   ImGuiCond.Once)
            }
            text("Dragging int: {PAYLOAD_VALUE}")
        }

        // 40 px gap: same_line's named-tuple destructure is POSITIONAL
        // (offset_from_start_x, spacing). `(spacing = 40.0f)` alone binds 40 to
        // offset and overlaps TARGET onto SOURCE, so the drag never starts.
        same_line((offset_from_start_x = 0.0f, spacing = 40.0f))

        // Target: a normal button. Body runs only when payload-bearing drag hovers.
        button(TARGET_BTN, (text = "TARGET"))
        drag_drop_target(TARGET_DD) {
            let payload = AcceptDragDropPayload("MY_INT", ImGuiDragDropFlags.None)
            if (payload != null) {
                unsafe {
                    RECEIVED = (reinterpret<int? const>(payload.Data))[0]
                }
                DROP_COUNT++
            }
        }

        separator()
        text("Source payload value: {PAYLOAD_VALUE}")
        text("Target received value: {RECEIVED}")
        text("Drops accepted: {DROP_COUNT}")
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
