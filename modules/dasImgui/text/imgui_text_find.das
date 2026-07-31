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
    replace_open : bool = false
    replace_text : string
    replace_focus_pending : bool = false
    pending_replace_valid : bool = false
    pending_replace_text : string
    //! Replace request counters: the bar (or a live rail) bumps the producer,
    //! the host applies the edit and advances the `applied_` twin.
    replace_revision : int = 0
    replace_all_revision : int = 0
    applied_replace_revision : int = 0
    applied_replace_all_revision : int = 0
    //! Consumer-side jump tracker (`text_edit_find_sync` uses it; hosts with
    //! their own glue may keep their own counter instead).
    applied_jump_revision : int = 0
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

def public text_find_open_replace(var state : TextFindState) {
    //! Open the bar with the replace row visible; focus lands in the replace
    //! input (Ctrl+H semantics).
    text_find_open(state)
    state.focus_pending = false
    state.replace_open = true
    state.replace_focus_pending = true
}

def public text_find_set_replace(var state : TextFindState; text : string) {
    //! Programmatic replace text; same widget-sync rules as the query.
    state.replace_open = true
    state.replace_text = text
    state.pending_replace_valid = true
    state.pending_replace_text = text
}

def public text_find_request_replace(var state : TextFindState) {
    state.replace_revision++
}

def public text_find_request_replace_all(var state : TextFindState) {
    state.replace_all_revision++
}

def public text_find_needs_rebuild(state : TextFindState const;
                                   revision : int) : bool {
    //! True when `text_find_rebuild` would recompute — hosts check this to
    //! skip joining a large source when nothing changed.
    return (revision != state.matched_revision || state.query != state.matched_query
        || state.use_regex != state.matched_regex)
}

def private fold_case(b : int) : int {
    return b >= 'A' && b <= 'Z' ? b + 32 : b
}

def private literal_matches(query : string; source : string;
                            var matches : array<TextSourceRange>) {
    var needle : array<int>
    peek_data(query) $(bytes) {
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
                matches |> push(TextSourceRange(
                    start_byte = at, end_byte = at + needle_length))
                at += needle_length
            } else {
                at++
            }
        }
    }
    delete needle
}

def private regex_matches(query : string; source : string;
                          var matches : array<TextSourceRange>) : string {
    var error = ""
    var re : Regex
    if (regex_compile(re, query, true)) {
        var inscope found <- regex_match_all(re, source)
        matches |> reserve(length(matches) + length(found))
        for (at in found) {
            continue if (at.y <= at.x)
            matches |> push(TextSourceRange(start_byte = at.x, end_byte = at.y))
        }
    } else {
        error = "invalid regular expression"
    }
    unsafe {
        delete re
    }
    return error
}

def public text_find_collect(query : string; use_regex : bool; source : string;
                             var matches : array<TextSourceRange>) : string {
    //! Pure match collection - the searching half of `text_find_rebuild`,
    //! appended to `matches`; returns the error text ("" when fine). Hosts
    //! with several search domains (a diff's two panes) call this per domain.
    return "" if (empty(query))
    if (use_regex) {
        return regex_matches(query, source, matches)
    }
    literal_matches(query, source, matches)
    return ""
}

def private group_by_reference(re : Regex; source, name : string;
                               match_start, match_end : int) : string {
    // ${...} resolves numeric first (${0} whole match, ${n} group n), then named
    var numeric = !empty(name)
    var group = 0
    peek_data(name) $(bytes) {
        for (b in bytes) {
            if (!is_number(int(b))) {
                numeric = false
                break
            }
            group = group * 10 + (int(b) - '0')
        }
    }
    if (numeric) {
        return slice(source, match_start, match_end) if (group == 0)
        return "" if (group >= length(re.groups))
        let span = re.groups[group]._0
        return slice(source, span.x, span.y)
    }
    let span = re[name]
    return slice(source, span.x, span.y)
}

def private expand_match_template(re : Regex; source, replacement : string;
                                  match_start, match_end : int) : string {
    // mirrors daslib/regex expand_replacement, which is private there
    return build_string() $(var writer) {
        let replacement_length = length(replacement)
        peek_data(replacement) $(bytes) {
            var i = 0
            while (i < replacement_length) {
                let c = int(bytes[i])
                if (c != '$' || i + 1 >= replacement_length) {
                    writer |> write_char(c)
                    i++
                    continue
                }
                let next = int(bytes[i + 1])
                if (next == '$') {
                    writer |> write_char('$')
                    i += 2
                } elif (next == '&' || next == '0') {
                    writer |> write(slice(source, match_start, match_end))
                    i += 2
                } elif (next >= '1' && next <= '9') {
                    let group = next - '0'
                    if (group < length(re.groups)) {
                        let span = re.groups[group]._0
                        writer |> write(slice(source, span.x, span.y))
                    }
                    i += 2
                } elif (next == '{') {
                    let close = find(replacement, "}", i + 2)
                    if (close < 0) {
                        writer |> write_char(c)
                        i++
                    } else {
                        let name = slice(replacement, i + 2, close)
                        writer |> write(group_by_reference(re, source, name,
                            match_start, match_end))
                        i = close + 1
                    }
                } else {
                    writer |> write_char(c)
                    i++
                }
            }
        }
    }
}

def public text_find_replacements(state : TextFindState const; source : string;
                                  var texts : array<string>) {
    //! One replacement string per `state.matches` entry: the literal replace
    //! text, or (regex mode) the template expanded per match.
    texts |> clear()
    texts |> reserve(length(state.matches))
    var re : Regex
    let expand = (state.matched_regex && find(state.replace_text, "$") >= 0
        && regex_compile(re, state.matched_query, true))
    for (matched in state.matches) {
        if (expand && regex_match(re, source, matched.start_byte) >= 0) {
            texts |> push(expand_match_template(re, source, state.replace_text,
                matched.start_byte, matched.end_byte))
        } else {
            texts |> push(state.replace_text)
        }
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
    return false if (!text_find_needs_rebuild(state, revision))
    let previous_start = text_find_current(state).start_byte
    state.matched_revision = revision
    state.matched_query = state.query
    state.matched_regex = state.use_regex
    delete state.matches
    state.current = -1
    state.error = text_find_collect(state.query, state.use_regex, source,
        state.matches)
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
var private FIND_TOGGLE : table<int; ClickState>
var private FIND_REGEX : table<int; ClickState>
var private FIND_PREV : table<int; ClickState>
var private FIND_NEXT : table<int; ClickState>
var private FIND_CLOSE : table<int; ClickState>
var private FIND_COUNT : table<int; NarrativeState>
var private FIND_JOIN : table<int; EmptyMarkerState>
var private REPLACE_TOGGLE : table<int; ClickState>
var private REPLACE_INPUT : table<int; InputTextState>
var private REPLACE_ONE : table<int; ClickState>
var private REPLACE_ALL : table<int; ClickState>
var private REPLACE_JOIN : table<int; EmptyMarkerState>

def private find_query_input(strip : int; var state : TextFindState) : int2 {
    // returns (active, done) as 0/1 — the bar's Enter/Escape logic needs both
    let synced = state.pending_widget_valid
    // a refocus must land the same frame as the value (ImGui re-reads on activation)
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
        // pending write waits a frame — the input's working buffer must exist
        unsafe {
            FIND_INPUT[strip].has_pending = true
            FIND_INPUT[strip].pending_value = state.pending_widget_query
        }
        state.pending_widget_valid = false
    }
    return int2(input_active ? 1 : 0, input_done ? 1 : 0)
}

def public text_find_bar(strip : int; var state : TextFindState) {
    //! Find bar + replace row (while `state.replace_open`); call once per
    //! frame while `state.open`; `strip` separates instances. Enter steps /
    //! replaces, Ctrl+Alt+Enter replaces all, Esc closes; hosts apply requests.
    return if (!state.open)
    if (icon_button(FIND_TOGGLE[strip], (
            glyph = state.replace_open ? "chevron-down" : "chevron-right",
            tip = "Toggle replace (Ctrl+H)", size = 16.0f))) {
        state.replace_open = !state.replace_open
    }
    same_line(FIND_JOIN[strip * 8 + 4])
    let query_state = find_query_input(strip, state)
    let input_active = query_state.x != 0
    let input_done = query_state.y != 0
    same_line(FIND_JOIN[strip * 8])
    let regex_color = GetStyleColorVec4(state.use_regex
        ? ImGuiCol.ButtonActive : ImGuiCol.Button)
    with_style((ImGuiCol.Button, regex_color)) {
        if (icon_button(FIND_REGEX[strip], (glyph = "regex",
                tip = "Regular expression", size = 16.0f))) {
            state.use_regex = !state.use_regex
        }
    }
    let count = length(state.matches)
    same_line(FIND_JOIN[strip * 8 + 1])
    if (icon_button(FIND_PREV[strip], (glyph = "chevron-up",
            tip = "Previous match (Shift+Enter)", size = 16.0f))) {
        text_find_step(state, -1)
    }
    same_line(FIND_JOIN[strip * 8 + 2])
    if (icon_button(FIND_NEXT[strip], (glyph = "chevron-down",
            tip = "Next match (Enter)", size = 16.0f))) {
        text_find_step(state, 1)
    }
    same_line(FIND_JOIN[strip * 8 + 3])
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
    if (state.replace_open) {
        text_find_replace_row(strip, state)
    }
}

def private text_find_replace_row(strip : int; var state : TextFindState) {
    if (icon_button(REPLACE_TOGGLE[strip], (glyph = "chevron-down",
            tip = "Hide replace", size = 16.0f))) {
        state.replace_open = false
    }
    same_line(REPLACE_JOIN[strip * 4])
    let synced = state.pending_replace_valid
    if (state.replace_focus_pending && !synced) {
        set_keyboard_focus_here(0)
        state.replace_focus_pending = false
    }
    SetNextItemWidth(240.0f)
    input_text(REPLACE_INPUT[strip], (text = "##replace"))
    let input_active = IsItemActive()
    let input_done = IsItemDeactivated()
    if (!synced) {
        state.replace_text = REPLACE_INPUT[strip].value
    }
    if (synced) {
        // same delayed-sync dance as the query input (see text_find_bar)
        unsafe {
            REPLACE_INPUT[strip].has_pending = true
            REPLACE_INPUT[strip].pending_value = state.pending_replace_text
        }
        state.pending_replace_valid = false
    }
    same_line(REPLACE_JOIN[strip * 4 + 1])
    if (small_button(REPLACE_ONE[strip], (text = "Replace##replace_one{strip}"))) {
        text_find_request_replace(state)
    }
    same_line(REPLACE_JOIN[strip * 4 + 2])
    if (small_button(REPLACE_ALL[strip], (text = "All##replace_all{strip}"))) {
        text_find_request_replace_all(state)
    }
    if (input_done && IsKeyPressed(ImGuiKey.Escape, false)) {
        text_find_close(state)
    } elif ((input_active || input_done)
            && (IsKeyPressed(ImGuiKey.Enter, false)
                || IsKeyPressed(ImGuiKey.KeypadEnter, false))) {
        let io & = unsafe(GetIO())
        if (io.KeyCtrl && io.KeyAlt) {
            text_find_request_replace_all(state)
        } else {
            text_find_request_replace(state)
        }
        state.replace_focus_pending = true
    }
}
