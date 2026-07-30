options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_find shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require imgui/imgui_widgets_builtin public
require imgui/imgui_style_builtin
require imgui/imgui_text_source_model public
require daslib/regex
require math
require strings

//! Find model + find bar shared by every text surface: case-insensitive
//! literal or regex search over UTF-8 source bytes. The bar owns the query
//! input, the regex toggle, prev/next and the match counter; the HOST owns
//! what a jump means (selection + scroll) by watching `jump_revision`.

struct public TextFindState {
    open : bool = false
    query : string
    use_regex : bool = false
    matches : array<TextSourceRange>
    current : int = -1
    error : string
    jump_revision : int = 0
    focus_pending : bool = false
    pending_widget_valid : bool = false
    pending_widget_query : string
    matched_revision : int = -1
    matched_query : string
    matched_regex : bool = false
}

def public text_find_open(var state : TextFindState) {
    // Reopening re-arms the jump: whatever cleared the view selection while
    // the bar was closed, the current match gets re-selected and re-scrolled.
    if (!state.open && state.current >= 0 && state.current < length(state.matches)) {
        state.jump_revision++
    }
    state.open = true
    state.focus_pending = true
}

def public text_find_close(var state : TextFindState) {
    state.open = false
}

def public text_find_dispose(var state : TextFindState) {
    delete state.matches
    state = TextFindState()
}

def public text_find_current(state : TextFindState const) : TextSourceRange {
    if (state.current < 0 || state.current >= length(state.matches)) {
        return TextSourceRange(start_byte = -1, end_byte = -1)
    }
    return state.matches[state.current]
}

def public text_find_step(var state : TextFindState; delta : int) {
    let count = length(state.matches)
    return if (count == 0)
    let at = max(state.current, 0)
    state.current = ((at + delta) % count + count) % count
    state.jump_revision++
}

def public text_find_set_query(var state : TextFindState;
                               query : string; use_regex : bool) {
    //! Programmatic query (live rails, host restore). Syncs the input widget
    //! on the next draw WITHOUT taking keyboard focus — an actively-edited
    //! ImGui input reinstates its own buffer, so a human mid-edit wins.
    state.open = true
    state.query = query
    state.use_regex = use_regex
    state.pending_widget_valid = true
    state.pending_widget_query = query
}

def private fold_case(b : int) : int {
    return b >= 'A' && b <= 'Z' ? b + 32 : b
}

def private literal_matches(var state : TextFindState; source : string) {
    var needle : array<int>
    peek_data(state.query) $(bytes) {
        needle |> reserve(length(bytes))
        for (b in bytes) {
            needle |> push(fold_case(int(b)))
        }
    }
    let needle_length = length(needle)
    if (needle_length == 0) {
        delete needle
        return
    }
    peek_data(source) $(hay) {
        let hay_length = length(hay)
        var at = 0
        while (at + needle_length <= hay_length) {
            var hit = true
            for (offset in range(needle_length)) {
                if (fold_case(int(hay[at + offset])) != needle[offset]) {
                    hit = false
                    break
                }
            }
            if (hit) {
                state.matches |> push(TextSourceRange(
                    start_byte = at, end_byte = at + needle_length))
                at += needle_length
            } else {
                at++
            }
        }
    }
    delete needle
}

def private regex_matches(var state : TextFindState; source : string) {
    var re : Regex
    if (regex_compile(re, state.query, true)) {
        var inscope found <- regex_match_all(re, source)
        state.matches |> reserve(length(found))
        for (at in found) {
            continue if (at.y <= at.x)
            state.matches |> push(TextSourceRange(start_byte = at.x, end_byte = at.y))
        }
    } else {
        state.error = "invalid regular expression"
    }
    unsafe {
        delete re
    }
}

def public text_find_rebuild(var state : TextFindState;
                             source : string; revision : int) : bool {
    //! Recompute matches when the query, the regex toggle, or the document
    //! revision changed. Keeps the current match sticky by position. Returns
    //! true when a recompute ran (jump_revision advanced).
    if (revision == state.matched_revision && state.query == state.matched_query
            && state.use_regex == state.matched_regex) {
        return false
    }
    let previous_start = text_find_current(state).start_byte
    state.matched_revision = revision
    state.matched_query = state.query
    state.matched_regex = state.use_regex
    state.error = ""
    delete state.matches
    state.current = -1
    if (!empty(state.query)) {
        if (state.use_regex) {
            regex_matches(state, source)
        } else {
            literal_matches(state, source)
        }
    }
    if (!empty(state.matches)) {
        state.current = 0
        for (index in range(length(state.matches))) {
            if (state.matches[index].start_byte >= previous_start) {
                state.current = index
                break
            }
        }
    }
    state.jump_revision++
    return true
}

// ===== The find bar =====

var private FIND_INPUT : table<int; InputTextState>
var private FIND_REGEX : table<int; ClickState>
var private FIND_PREV : table<int; ClickState>
var private FIND_NEXT : table<int; ClickState>
var private FIND_CLOSE : table<int; ClickState>
var private FIND_COUNT : table<int; NarrativeState>
var private FIND_JOIN : table<int; EmptyMarkerState>

def public text_find_bar(strip : int; var state : TextFindState) {
    //! One-row find bar; call once per frame while `state.open`. `strip`
    //! separates instances (one per document/view). Enter steps forward,
    //! Shift+Enter back, Esc closes; the host binds any global shortcuts.
    return if (!state.open)
    let synced = state.pending_widget_valid
    // Delay a refocus past a pending sync: ImGui re-reads the buffer on
    // activation, so the focus frame must be the frame the value lands.
    if (state.focus_pending && !synced) {
        set_keyboard_focus_here(0)
        state.focus_pending = false
    }
    SetNextItemWidth(240.0f)
    input_text(FIND_INPUT[strip], (text = "##find"))
    let input_active = IsItemActive()
    let input_done = IsItemDeactivated()
    if (!synced) {
        state.query = FIND_INPUT[strip].value
    }
    if (synced) {
        // Push the programmatic query AFTER the input drew once: its working
        // buffer is allocated on the first frame, and the pending mechanism
        // writes into that buffer on the next one.
        unsafe {
            FIND_INPUT[strip].has_pending = true
            FIND_INPUT[strip].pending_value = state.pending_widget_query
        }
        state.pending_widget_valid = false
    }
    same_line(FIND_JOIN[strip * 4])
    let regex_color = GetStyleColorVec4(state.use_regex
        ? ImGuiCol.ButtonActive : ImGuiCol.Button)
    with_style((ImGuiCol.Button, regex_color)) {
        if (icon_button(FIND_REGEX[strip], (glyph = "regex",
                tip = "Regular expression", size = 16.0f))) {
            state.use_regex = !state.use_regex
        }
    }
    let count = length(state.matches)
    same_line(FIND_JOIN[strip * 4 + 1])
    if (icon_button(FIND_PREV[strip], (glyph = "chevron-up",
            tip = "Previous match (Shift+Enter)", size = 16.0f))) {
        text_find_step(state, -1)
    }
    same_line(FIND_JOIN[strip * 4 + 2])
    if (icon_button(FIND_NEXT[strip], (glyph = "chevron-down",
            tip = "Next match (Enter)", size = 16.0f))) {
        text_find_step(state, 1)
    }
    same_line(FIND_JOIN[strip * 4 + 3])
    if (!empty(state.error)) {
        text_colored(FIND_COUNT[strip],
            (color = float4(0.95f, 0.55f, 0.35f, 1.0f), text = state.error))
    } elif (empty(state.query)) {
        text_disabled(FIND_COUNT[strip], (text = "find"))
    } elif (count > 0) {
        text_disabled(FIND_COUNT[strip], (text = "{state.current + 1} of {count}"))
    } else {
        text_colored(FIND_COUNT[strip],
            (color = float4(0.95f, 0.55f, 0.35f, 1.0f), text = "no matches"))
    }
    same_line()
    if (icon_button(FIND_CLOSE[strip], (glyph = "close",
            tip = "Close (Esc)", size = 16.0f))) {
        text_find_close(state)
    }
    if (input_done && IsKeyPressed(ImGuiKey.Escape, false)) {
        text_find_close(state)
    } elif ((input_active || input_done)
            && (IsKeyPressed(ImGuiKey.Enter, false)
                || IsKeyPressed(ImGuiKey.KeypadEnter, false))) {
        let io & = unsafe(GetIO())
        text_find_step(state, io.KeyShift ? -1 : 1)
        state.focus_pending = true
    }
}
