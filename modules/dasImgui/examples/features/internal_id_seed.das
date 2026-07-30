options gen2
options _comment_hygiene = true

require imgui/imgui_harness
require strings

// =============================================================================
// FEATURE: internal_id_seed — the imgui_internal.h ID-stack seed / override
//          helpers. Cherry-picked into the v2 binding (bind_imgui.das
//          internal_allow_func): GetIDWithSeed, GetFocusID, GetHoveredID,
//          KeepAliveID, MarkItemEdited, PushOverrideID, TreePushOverrideID.
//          Host-agnostic — they hash/seed an ImGuiID, query the hovered/focused
//          item ID, or push a raw ID onto the stack, all without v2 host state. This is
//          the "binding is usable" gate: it calls the real bound API at runtime.
// SHOWS:   GetIDWithSeed(n, seed) (same int, different seed => different ID);
//          with_override_id (the new PushOverrideID scope guard — GetID under a
//          raw seed, NOT hash-combined like with_id); GetHoveredID / GetFocusID
//          (which item is hovered / focused this frame); KeepAliveID +
//          MarkItemEdited (custom-widget ID plumbing); TreePushOverrideID +
//          tree_pop (a tree level under an explicit override ID).
// STANDALONE: daslang.exe modules/dasImgui/examples/features/internal_id_seed.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/internal_id_seed.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/internal_id_seed.das
// =============================================================================

// ImGui IDs are conventionally shown as 8-digit hex (the metrics window does the
// same); daslang's "{u}" interpolation is decimal, so format the nibbles by hand.
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
    harness_init("internal_id_seed - imgui_internal.h ID seed / override helpers", 760, 560)
    var io & = unsafe(GetIO())
    unsafe(GetStyle()).FontScaleMain = 1.25
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(720.0, 520.0), ImGuiCond.Always)
    window(ID_SEED_WIN, (text = "internal_id_seed", closable = false,
                         flags = ImGuiWindowFlags.None)) {
        // GetIDWithSeed(n, seed) — hash an int under an explicit seed, without
        // touching the ID stack. Same int + different seed => different ID, which
        // is how nested scopes keep their child IDs from colliding.
        text("GetIDWithSeed(n, seed): hash an int under an explicit seed (no ID-stack push).")
        text("  GetIDWithSeed(42, 0)    = {hex8(GetIDWithSeed(42, 0u))}")
        text("  GetIDWithSeed(42, 1000) = {hex8(GetIDWithSeed(42, 1000u))}   (same int, new seed)")
        separator()

        // with_override_id — the new scope guard over PushOverrideID. Pushes the
        // raw seed as-is (NOT hash-combined like with_id), so GetID("child") under
        // it equals GetIDWithSeed("child".., 7777). Stable, explicitly-seeded IDs.
        text("with_override_id(seed): GetID(\"child\") under a raw seed - a stable, explicitly-seeded string ID.")
        with_override_id(7777u) {
            let child_id = GetID("child")
            text("  under override 7777: GetID(\"child\") = {hex8(child_id)}")
        }
        separator()

        // GetHoveredID / GetFocusID — introspection. Hover or tab to a button and
        // its ID shows up here (0 when nothing is hovered / focused).
        text("GetHoveredID / GetFocusID: which item is hovered / focused this frame.")
        button(BTN_A, (text = "hover me (A)"))
        same_line()
        button(BTN_B, (text = "or tab to me (B)"))
        text("  hovered = {hex8(GetHoveredID())}   focused = {hex8(GetFocusID())}")
        separator()

        // KeepAliveID / MarkItemEdited — custom-widget ID plumbing. KeepAliveID
        // stops an interacting-but-unsubmitted ID from being cleared this frame;
        // MarkItemEdited drives IsItemDeactivatedAfterEdit for hand-rolled widgets.
        text("KeepAliveID / MarkItemEdited: custom-widget ID plumbing.")
        let custom_id = GetID("my_custom_widget")
        KeepAliveID(custom_id)
        MarkItemEdited(custom_id)
        text("  KeepAliveID + MarkItemEdited called on {hex8(custom_id)}")
        separator()

        // TreePushOverrideID + tree_pop — open a tree depth level keyed by a raw
        // override ID (vs TreeNode's hashed label). Used for stable tree IDs that
        // survive relabeling / reordering.
        text("TreePushOverrideID(id) + tree_pop(): a tree level under a raw override ID.")
        TreePushOverrideID(GetID("explicit_tree"))
        text("  ...content nested inside the explicit-ID tree level")
        tree_pop()
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
