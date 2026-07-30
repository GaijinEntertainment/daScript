options gen2
options _comment_hygiene = true
options indenting = 4

module app_console

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require imgui/imgui_scope_builtin
require imgui/imgui_style_builtin
require strings
require math

// Mirrors imgui_demo.cpp:7116-7477 — static void ShowExampleAppConsole(bool* p_open).
//
// The C++ original uses a C++ class (`ExampleAppConsole`) that holds Items,
// History, Commands, Filter, AutoScroll/ScrollToBottom flags and an
// InputText callback driving TAB completion + arrow-key history. This port
// keeps the same shape:
//   - Module-scope `var private` globals replace the per-instance fields
//     (they survive live-reload via the in-memory context).
//   - `[widget]`/`[container]` state structs are auto-emitted from the
//     uppercase IDENT args; loop-keyed log lines use an indexed
//     `table<int; NarrativeState>`.
//   - TAB-completion + Up/Down history go through the new
//     `input_text_callback` boost wrapper (callback fires synchronously
//     inside ImGui's InputText, so the lambda's capture is safe).
//   - Comma-separated include/exclude filter goes through the
//     `text_filter` / `passes_filter` boost wrapper over the bound C++
//     ImGuiTextFilter.

// =============================================================================
// Module-scope persistent state (cross-frame, survives live-reload)
// =============================================================================

var private CONSOLE_ITEMS : array<string>           // log lines
var private CONSOLE_HISTORY : array<string>         // command history (push_back order)
var private CONSOLE_HIST_POS = -1                   // -1 = new line; 0..N-1 browsing
var private CONSOLE_AUTO_SCROLL = true              // tail-follow toggle
var private CONSOLE_SCROLL_TO_BOTTOM = false        // one-shot scroll request

let private CONSOLE_COMMANDS <- [
    "HELP", "HISTORY", "CLEAR", "CLASSIFY"          // CLASSIFY = C+TAB ambiguous-completion demo
]

// Indexed widget tables — [widget] auto-emit doesn't synthesize table globals
// for loop-keyed forms; they must be declared at module scope.
var private CONSOLE_ITEM_TEXT : table<int; NarrativeState>

// One-shot init: seeds the welcome line on first frame after module load.
var private CONSOLE_SEEDED = false

// Set by the "Close Console" context-menu entry (right-click title bar); the
// caller polls `take_close_request()` each frame. Private so the consume-and-clear
// API is the only path — direct reads would race with the reset on the next call.
var private CONSOLE_REQUEST_CLOSE = false

def public take_close_request() : bool {
    //! Consume-and-clear: returns `true` once after the user picks "Close Console"
    //! from the title-bar context menu, then resets. Caller (imgui_demo.das
    //! dispatcher) gates `SHOW_APP_CONSOLE` off the return value.
    let r = CONSOLE_REQUEST_CLOSE
    CONSOLE_REQUEST_CLOSE = false
    return r
}

// =============================================================================
// Log / command helpers
// =============================================================================

def private add_log(line : string) {
    CONSOLE_ITEMS |> push_clone(line)
}

def private clear_log() {
    CONSOLE_ITEMS |> clear()
    // Also release per-row text-widget state, else the indexed table retains stale entries after a Clear.
    CONSOLE_ITEM_TEXT |> clear()
}

def private exec_command(cmd : string) {
    add_log("# {cmd}")

    // Dedupe: drop any prior occurrence so the latest entry is at the tail.
    CONSOLE_HIST_POS = -1
    let upper_cmd = to_upper(cmd)
    for (i in range(length(CONSOLE_HISTORY))) {
        if (to_upper(CONSOLE_HISTORY[i]) == upper_cmd) {
            CONSOLE_HISTORY |> erase(i)
            break
        }
    }
    CONSOLE_HISTORY |> push_clone(cmd)

    if (upper_cmd == "CLEAR") {
        clear_log()
    } elif (upper_cmd == "HELP") {
        add_log("Commands:")
        for (c in CONSOLE_COMMANDS) {
            add_log("- {c}")
        }
    } elif (upper_cmd == "HISTORY") {
        let first = max(0, length(CONSOLE_HISTORY) - 10)
        for (i in range(first, length(CONSOLE_HISTORY))) {
            add_log("{i}: {CONSOLE_HISTORY[i]}")
        }
    } else {
        add_log("Unknown command: '{cmd}'")
    }

    CONSOLE_SCROLL_TO_BOTTOM = true     // always tail on user input
}

// =============================================================================
// InputText callback — TAB completion + Up/Down history
// =============================================================================

def private is_word_sep(c : int) : bool {
    return c == ' ' || c == '\t' || c == ',' || c == ';'
}

def private text_edit_callback(var data : ImGuiInputTextCallbackData) : int {
    if (data.EventFlag == ImGuiInputTextFlags.CallbackCompletion) {
        // ----- TAB completion -----
        let buf_str = clone_string(data.Buf)  // Buf is char*, valid for the callback lifetime; snapshot as a daslang string
        let word_end = data.CursorPos
        var word_start = word_end
        peek_data(buf_str) $(arr) {
            while (word_start > 0 && !is_word_sep(int(arr[word_start - 1]))) {
                word_start--
            }
        }
        let prefix = slice(buf_str, word_start, word_end)
        let upper_prefix = to_upper(prefix)
        let plen = word_end - word_start

        var candidates <- [for (c in CONSOLE_COMMANDS);
                               c;
                               where length(c) >= plen && to_upper(slice(c, 0, plen)) == upper_prefix]

        if (empty(candidates)) {
            add_log("No match for \"{prefix}\"!")
        } elif (length(candidates) == 1) {
            // Single match — replace the partial word with the full command (canonical casing) + trailing space.
            data |> DeleteChars(word_start, plen)
            InsertChars(data, data.CursorPos, candidates[0])
            InsertChars(data, data.CursorPos, " ")
        } else {
            // Multiple matches — extend by the longest common prefix (case-insensitive), then list all.
            var match_len = plen
            var growing = true
            while (growing) {
                if (match_len >= length(candidates[0])) {
                    growing = false
                } else {
                    let target = to_upper(slice(candidates[0], match_len, match_len + 1))
                    for (cand in candidates) {
                        if (match_len >= length(cand)) {
                            growing = false
                            break
                        }
                        if (to_upper(slice(cand, match_len, match_len + 1)) != target) {
                            growing = false
                            break
                        }
                    }
                    if (growing) {
                        match_len++
                    }
                }
            }
            if (match_len > plen) {
                data |> DeleteChars(word_start, plen)
                InsertChars(data, data.CursorPos, slice(candidates[0], 0, match_len))
            }
            add_log("Possible matches:")
            for (c in candidates) {
                add_log("- {c}")
            }
        }
        delete candidates
    } elif (data.EventFlag == ImGuiInputTextFlags.CallbackHistory) {
        // ----- Up/Down history -----
        let prev_pos = CONSOLE_HIST_POS
        if (data.EventKey == ImGuiKey.UpArrow) {
            if (CONSOLE_HIST_POS == -1) {
                CONSOLE_HIST_POS = length(CONSOLE_HISTORY) - 1
            } elif (CONSOLE_HIST_POS > 0) {
                CONSOLE_HIST_POS--
            }
        } elif (data.EventKey == ImGuiKey.DownArrow) {
            if (CONSOLE_HIST_POS != -1) {
                CONSOLE_HIST_POS++
                if (CONSOLE_HIST_POS >= length(CONSOLE_HISTORY)) {
                    CONSOLE_HIST_POS = -1
                }
            }
        }
        if (prev_pos != CONSOLE_HIST_POS) {
            let history_str = CONSOLE_HIST_POS >= 0 ? CONSOLE_HISTORY[CONSOLE_HIST_POS] : ""
            data |> DeleteChars(0, data.BufTextLen)
            InsertChars(data, 0, history_str)
        }
    }
    return 0
}

// =============================================================================
// ShowExampleAppConsole — main panel
// =============================================================================

def ShowExampleAppConsole() {
    if (!CONSOLE_SEEDED) {
        add_log("Welcome to Dear ImGui!")
        CONSOLE_SEEDED = true
    }

    // SetNextWindowSize is set inside the window container's default if no
    // size is in flight — for now, ImGui auto-sizes; user can drag-resize.
    window(CONSOLE_WIN, (text = "Example: Console", closable = false,
                          flags = ImGuiWindowFlags.None)) {

        // Right-click the title bar opens a "Close Console" menu; the selection sets
        // CONSOLE_REQUEST_CLOSE. The caller polls `take_close_request()` after this
        // function returns and clears its own visibility toggle.
        popup_context_item(CONSOLE_TITLE_POPUP, (str_id = "", flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(CONSOLE_CLOSE_MENU, (text = "Close Console"))) {
                CONSOLE_REQUEST_CLOSE = true
            }
        }

        text_wrapped((text = "This example implements a console with basic " +
                              "coloring, completion (TAB key) and history " +
                              "(Up/Down keys). A more elaborate implementation " +
                              "may want to store entries along with extra data " +
                              "such as timestamp, emitter, etc."))
        text_wrapped("Enter 'HELP' for help.")

        if (small_button(CONSOLE_BTN_DBG, (text = "Add Debug Text"))) {
            add_log("{length(CONSOLE_ITEMS)} some text")
            add_log("some more text")
            add_log("display very important message here!")
        }
        same_line(CONSOLE_SL_1)
        if (small_button(CONSOLE_BTN_ERR, (text = "Add Debug Error"))) {
            add_log("[error] something went wrong")
        }
        same_line(CONSOLE_SL_2)
        if (small_button(CONSOLE_BTN_CLEAR, (text = "Clear"))) {
            clear_log()
        }
        same_line(CONSOLE_SL_3)
        let copy_to_clipboard = small_button(CONSOLE_BTN_COPY, (text = "Copy"))

        separator(CONSOLE_SEP_1)

        // Options popup body — opened on demand by the "Options" button.
        popup(CONSOLE_OPTS_POPUP, (text = "Options", flags = ImGuiWindowFlags.None)) {
            if (checkbox(CONSOLE_AUTOSCROLL_CHECK, (text = "Auto-scroll",
                                                     init = CONSOLE_AUTO_SCROLL))) {
                CONSOLE_AUTO_SCROLL = CONSOLE_AUTOSCROLL_CHECK.value
            }
        }
        if (button(CONSOLE_BTN_OPTS, (text = "Options"))) {
            open_popup(CONSOLE_OPTS_POPUP)
        }
        same_line(CONSOLE_SL_4)
        text_filter(CONSOLE_FILTER, (text = "Filter (\"incl,-excl\") (\"error\")",
                                      width = 180.0f))

        separator(CONSOLE_SEP_2)

        // Reserve enough left-over height for 1 separator + 1 input text.
        let footer_h = GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing()
        child(CONSOLE_SCROLL_CHILD, (text = "ScrollingRegion",
                                      size = float2(0.0f, -footer_h),
                                      child_flags = ImGuiChildFlags.None,
                                      window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {

            popup_context_window(CONSOLE_SCROLL_CTX_MENU,
                                  (str_id = "##ConsoleScrollMenu", flags = ImGuiPopupFlags.MouseButtonRight)) {
                if (selectable_label(CONSOLE_CTX_CLEAR, "Clear", false)) {
                    clear_log()
                }
            }

            with_style((ImGuiStyleVar.ItemSpacing, float2(4.0f, 1.0f))) {
                if (copy_to_clipboard) log_to_clipboard()
                for (i in range(length(CONSOLE_ITEMS))) {
                    let line = CONSOLE_ITEMS[i]
                    if (!passes_filter(CONSOLE_FILTER, line)) continue
                    // Per-line color: red for [error], gold for # echo prefix.
                    if (starts_with(line, "[error]")) {
                        with_style((ImGuiCol.Text, 0xFF6666FFu)) {
                            text(CONSOLE_ITEM_TEXT[i], (text = line))
                        }
                    } elif (starts_with(line, "# ")) {
                        with_style((ImGuiCol.Text, 0xFF99CCFFu)) {
                            text(CONSOLE_ITEM_TEXT[i], (text = line))
                        }
                    } else {
                        text(CONSOLE_ITEM_TEXT[i], (text = line))
                    }
                }
                if (copy_to_clipboard) log_finish()
            }

            // Auto-tail: scroll to bottom if we were already at the bottom
            // when this frame started, or if a command was just executed.
            if (CONSOLE_SCROLL_TO_BOTTOM ||
                (CONSOLE_AUTO_SCROLL && CONSOLE_SCROLL_CHILD.scroll.y >=
                                         CONSOLE_SCROLL_CHILD.scroll_max.y)) {
                set_scroll_here_y(1.0f)
            }
            CONSOLE_SCROLL_TO_BOTTOM = false
        }

        separator(CONSOLE_SEP_3)

        // Command line.
        var reclaim_focus = false
        let flags = (ImGuiInputTextFlags.EnterReturnsTrue |
                     ImGuiInputTextFlags.EscapeClearsAll |
                     ImGuiInputTextFlags.CallbackCompletion |
                     ImGuiInputTextFlags.CallbackHistory)
        if (input_text_callback(CONSOLE_INPUT, "Input", flags,
                                 @(var data : ImGuiInputTextCallbackData) =>
                                    text_edit_callback(data))) {
            let cmd_in = CONSOLE_INPUT.value
            // Trim trailing whitespace.
            var end = length(cmd_in)
            peek_data(cmd_in) $(arr) {
                while (end > 0 && (arr[end - 1] == 0x20u8 || arr[end - 1] == 0x09u8)) {
                    end--
                }
            }
            let cmd = slice(cmd_in, 0, end)
            if (!empty(cmd)) {
                exec_command(cmd)
            }
            // Clear the input buffer for the next entry.
            CONSOLE_INPUT.pending_value := ""
            CONSOLE_INPUT.has_pending = true
            reclaim_focus = true
        }
        set_item_default_focus()
        if (reclaim_focus) {
            set_keyboard_focus_here(-1)
        }
    }
}
