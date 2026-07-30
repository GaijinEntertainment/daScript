options gen2
options _comment_hygiene = true
options indenting = 4

module app_log

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_layout_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_style_builtin
require daslib/safe_addr

// Mirrors imgui_demo.cpp:7478-7637 — ExampleAppLog class + ShowExampleAppLog().
//
// The C++ original is a class holding a contiguous text buffer
// (`ImGuiTextBuffer`), per-line offset table (`ImVector<int>`), a
// `ImGuiTextFilter`, and an `AutoScroll` bool. The class exposes
// `AddLog(fmt, ...)` and `Draw(title)` methods.
//
// Daslang port shape:
//   - Module-scope `var private` globals replace the per-instance fields.
//     `LOG_ITEMS : array<string>` holds one line per entry (skipping the
//     contiguous-buffer + line-offset bookkeeping the C++ version needs
//     for clipper random access — random access by index is O(1) on the
//     daslang array form too).
//   - `text_filter(LOG_FILTER, ...)` rides on the bound `ImGuiTextFilter`
//     wrapper; `passes_filter(LOG_FILTER, line)` matches the C++ pattern.
//   - Per-line text widgets use an indexed `table<int; NarrativeState>`
//     so each rendered row owns its own state global. `clear_log()`
//     releases the table along with the lines.
//   - Visible-region culling on the unfiltered path uses the boost
//     `list_clipper` wrapper (added alongside this port). When the
//     filter is active we drop to a plain `for` since the result is
//     not random-access by physical row.
//   - The C++ debug button calls `AddLog` directly; we keep the same
//     entry-points (`add_log`, `clear_log`) module-public so future
//     callers can post diagnostic lines.

// =============================================================================
// Module-scope persistent state
// =============================================================================

var private LOG_ITEMS : array<string>
var private LOG_AUTO_SCROLL = true
var private LOG_DEBUG_COUNTER = 0

// Indexed widget state for the rendered log lines.
var private LOG_ITEM_TEXT : table<int; NarrativeState>

// =============================================================================
// Public helpers — append / clear
// =============================================================================

def public add_log(line : string) {
    //! Append a single line to the log buffer. Caller pre-formats the
    //! string (the C++ `AddLog(fmt, ...)` varargs form is collapsed).
    LOG_ITEMS |> push_clone(line)
}

def public clear_log() {
    //! Drop every log line and release the per-row text-widget state.
    LOG_ITEMS |> clear()
    LOG_ITEM_TEXT |> clear()
}

// =============================================================================
// ShowExampleAppLog — main panel
// =============================================================================

def ShowExampleAppLog() {
    // Debug button row above the log proper. C++ uses two Begin()/End() pairs on
    // the same window title to append; daslang's [container] window can't model
    // that, so the debug controls live inline at the top.
    window(LOG_WIN, (text = "Example: Log", closable = false,
                      flags = ImGuiWindowFlags.None)) {

        if (small_button(LOG_BTN_ADD5, (text = "[Debug] Add 5 entries"))) {
            let categories = fixed_array("info", "warn", "error")
            let words = fixed_array("Bumfuzzled", "Cattywampus", "Snickersnee",
                                    "Abibliophobia", "Absquatulate",
                                    "Nincompoop", "Pauciloquent")
            for (_n in range(5)) {
                let category = categories[LOG_DEBUG_COUNTER % length(categories)]
                let word = words[LOG_DEBUG_COUNTER % length(words)]
                add_log("[{LOG_DEBUG_COUNTER}] [{category}] " +
                        "Hello, current time is {GetTime()}, " +
                        "here's a word: '{word}'")
                LOG_DEBUG_COUNTER++
            }
        }

        separator(LOG_SEP_HEADER)

        // Options popup body — opened on demand by the Options button.
        popup(LOG_OPTS_POPUP, (text = "Options", flags = ImGuiWindowFlags.None)) {
            if (checkbox(LOG_AUTOSCROLL_CHECK, (text = "Auto-scroll",
                                                  init = LOG_AUTO_SCROLL))) {
                LOG_AUTO_SCROLL = LOG_AUTOSCROLL_CHECK.value
            }
        }
        if (button(LOG_BTN_OPTS, (text = "Options"))) {
            open_popup(LOG_OPTS_POPUP)
        }
        same_line(LOG_SL_1)
        let clear = button(LOG_BTN_CLEAR, (text = "Clear"))
        same_line(LOG_SL_2)
        let copy = button(LOG_BTN_COPY, (text = "Copy"))
        same_line(LOG_SL_3)
        text_filter(LOG_FILTER, (text = "Filter (\"incl,-excl\") (\"error\")",
                                  width = -100.0f))

        separator(LOG_SEP_FILTER)

        // Scrolling region with horizontal scroll for long lines.
        child(LOG_SCROLL_CHILD, (text = "scrolling",
                                  size = float2(0.0f, 0.0f),
                                  child_flags = ImGuiChildFlags.None,
                                  window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {

            if (clear) {
                clear_log()
            }
            if (copy) {
                log_to_clipboard()
            }

            with_style((ImGuiStyleVar.ItemSpacing, float2(0.0f, 0.0f))) {
                if (!is_active(LOG_FILTER)) {
                    // Filter inactive — clipper-cull the line range. Don't gate on
                    // `passes_filter("")`: an exclude-only filter like `-error` has no
                    // include terms, so PassFilter("") returns true and renders excluded lines.
                    list_clipper(length(LOG_ITEMS)) $(start, end_) {
                        for (i in range(start, end_)) {
                            text(LOG_ITEM_TEXT[i], (text = LOG_ITEMS[i]))
                        }
                    }
                } else {
                    // Filter active — random access by physical row no
                    // longer maps to visible rows, so render the matches
                    // sequentially.
                    for (i in range(length(LOG_ITEMS))) {
                        let line = LOG_ITEMS[i]
                        if (passes_filter(LOG_FILTER, line)) {
                            text(LOG_ITEM_TEXT[i], (text = line))
                        }
                    }
                }
            }

            if (copy) {
                log_finish()
            }

            // Tail-follow: scroll to bottom only if we were already at the
            // bottom when the frame started (scroll != scroll_max means
            // the user dragged away — don't yank them back).
            if (LOG_AUTO_SCROLL &&
                LOG_SCROLL_CHILD.scroll.y >= LOG_SCROLL_CHILD.scroll_max.y) {
                set_scroll_here_y(1.0f)
            }
        }
    }
}
