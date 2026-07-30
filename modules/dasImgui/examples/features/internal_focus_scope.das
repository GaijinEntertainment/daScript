options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require strings

// =============================================================================
// FEATURE: internal_focus_scope — the imgui_internal.h focus-scope / activation
//          helpers. Cherry-picked into the v2 binding (bind_imgui.das
//          internal_allow_func): PushFocusScope, PopFocusScope,
//          GetCurrentFocusScope, FocusItem, ActivateItemByID,
//          SetNextItemSelectionUserData. Host-agnostic — they group items into a
//          focus scope (keyboard/gamepad nav + multi-select), focus/activate an
//          item by ID, or tag the next item's selection data, all without v2 host
//          state. This is the "binding is usable" gate: it calls the real bound
//          API at runtime, not just compiles it.
// SHOWS:   with_focus_scope (the new PushFocusScope/PopFocusScope guard) with
//          GetCurrentFocusScope reading 0 outside the scope and the pushed id
//          inside; FocusItem (focus the last-submitted item); ActivateItemByID
//          (queue an item to activate by ID next frame); SetNextItemSelectionUserData
//          (tag the next item with multi-select user-data).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_focus_scope.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_focus_scope.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_focus_scope.das
// =============================================================================

// ImGui IDs / focus-scope ids are conventionally shown as 8-digit hex.
def private hex8(v : uint) : string {
    let digits = "0123456789abcdef"
    return build_string() $(var writer) {
        writer |> write("0x")
        for (i in range(8)) {
            let nib = int((v >> uint((7 - i) * 4)) & 0xfu)
            writer |> write(slice(digits, nib, nib + 1))
        }
    }
}

[export]
def init() {
    harness_init("internal_focus_scope - imgui_internal.h focus scope / activation", 760, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    window(FOCUS_WIN, (text = "internal_focus_scope", closable = false,
                       flags = ImGuiWindowFlags.None)) {
        // with_focus_scope + GetCurrentFocusScope — focus scopes group items for
        // keyboard/gamepad nav and multi-select. GetCurrentFocusScope reads the
        // scope id we are outputting into (0 at the top level).
        text("with_focus_scope(id): group items into a focus scope (nav / multi-select).")
        text("  outside any scope: GetCurrentFocusScope() = {hex8(GetCurrentFocusScope())}")
        let scope_id = GetID("my_scope")
        with_focus_scope(scope_id) {
            text("  inside scope {hex8(scope_id)}: GetCurrentFocusScope() = {hex8(GetCurrentFocusScope())}")
            button(IN_SCOPE_A, (text = "item A (in scope)"))
            same_line()
            button(IN_SCOPE_B, (text = "item B (in scope)"))
        }
        separator()

        // FocusItem — focus the last-submitted item (the nav/keyboard target),
        // without selecting or activating it.
        text("FocusItem(): focuses the last item (the keyboard/gamepad nav target).")
        button(FOCUS_TARGET, (text = "this button is programmatically focused"))
        FocusItem()
        separator()

        // ActivateItemByID — queue an item (by ID) to activate next frame when
        // submitted. We re-queue the button's GetItemID() every frame, so it stays
        // pressed — proof the queued activation actually applies.
        text("ActivateItemByID(id): queue an item to activate by ID (applied next frame).")
        button(ACT_TARGET, (text = "held active by ActivateItemByID"))
        let act_id = GetItemID()
        ActivateItemByID(act_id)
        text("  re-activating the button above by its id {hex8(act_id)} each frame")
        separator()

        // SetNextItemSelectionUserData — tag the NEXT item with a multi-select
        // user-data value (read back via the multi-select API / ImGuiItemFlags
        // HasSelectionUserData). Here it tags a plain selectable.
        text("SetNextItemSelectionUserData(data): tag the next item for multi-select.")
        SetNextItemSelectionUserData(int64(42))
        selectable(SEL_ITEM, (text = "selectable tagged with selection user-data 42"))
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
