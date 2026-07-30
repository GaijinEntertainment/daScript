options gen2
options _comment_hygiene = true

require imgui/imgui_harness

// =============================================================================
// FEATURE: tab_item_button — button styled as a tab strip element.
// SHOWS:   Click-trigger sibling of `tab_item`. No content pane attached —
//          use for Leading-/Trailing-anchored controls (add-tab "+", help
//          popup "?"). Click semantics match `button`: returns true on click;
//          state.click_count accumulates. Must be called INSIDE a `tab_bar`.
//          The "+" Trailing button drives a dynamic `active_tabs` list; the
//          Leading "?" just tallies clicks (no popup, to keep the feature
//          isolated).
// DRIVE:   curl -X POST -d '{"name":"imgui_click","args":{"target":"MAIN_WIN/BAR/ADD_TAB"}}' \
//               localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/tab_item_button.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/tab_item_button.das -- --headless --headless-frames=60
// LIVE:       daslang-live modules/dasImgui/examples/features/tab_item_button.das
// =============================================================================

var private ACTIVE_TABS : array<int>
var private NEXT_ID = 0
var private DYN_TAB : table<int; TabItemState>

[export]
def init() {
    harness_init("tab_item_button — leading/trailing tab buttons", 720, 360)
    // Seed with 3 tabs so the bar is non-empty on frame 1.
    ACTIVE_TABS |> reserve(3)
    for (_ in range(3)) {
        ACTIVE_TABS |> push(NEXT_ID)
        NEXT_ID++
    }
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    SetNextWindowSize(ImVec2(680.0, 300.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "tab_item_button",
                       closable = false, flags = ImGuiWindowFlags.None)) {
        text(LEAD,
            (text = "Leading '?' tallies clicks. Trailing '+' adds a tab. X on any tab removes it."))

        tab_bar(BAR, (text = "MyTabBar",
                       flags = ImGuiTabBarFlags.AutoSelectNewTabs |
                               ImGuiTabBarFlags.Reorderable |
                               ImGuiTabBarFlags.FittingPolicyShrink)) {
            if (tab_item_button(HELP, (text = "?",
                                        flags = ImGuiTabItemFlags.Leading |
                                                ImGuiTabItemFlags.NoTooltip))) {
                // Click captured in HELP.click_count for telemetry.
            }
            if (tab_item_button(ADD_TAB, (text = "+",
                                           flags = ImGuiTabItemFlags.Trailing |
                                                   ImGuiTabItemFlags.NoTooltip))) {
                ACTIVE_TABS |> push(NEXT_ID)
                NEXT_ID++
            }

            // Submit tab list; drop X-closed entries. `table[k]` auto-insert is
            // zero-init, so TabItemState's `open = true` default never fires —
            // construct each row explicitly, else frame 1 erases every seed tab.
            var n = 0
            while (n < length(ACTIVE_TABS)) {
                let tab_id = ACTIVE_TABS[n]
                if (!key_exists(DYN_TAB, tab_id)) {
                    DYN_TAB[tab_id] = TabItemState(open = true)
                }
                tab_item(DYN_TAB[tab_id],
                    (text = "{tab_id:04}", closable = true,
                     flags = ImGuiTabItemFlags.None)) {
                    text(TAB_BODY, (text = "This is tab {tab_id}"))
                }
                if (!DYN_TAB[tab_id].open) {
                    ACTIVE_TABS |> erase(n)
                    // DYN_TAB entry left in place; cheap and the table key
                    // never collides since NEXT_ID monotonically increments.
                } else {
                    n++
                }
            }
        }

        text(STATS,
            (text = "active_tabs.length = {length(ACTIVE_TABS)}, next_id = {NEXT_ID}"))
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
