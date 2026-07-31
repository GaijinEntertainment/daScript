options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_edit_complete shared

require imgui/imgui_text_source_edit public
require math
require strings
require daslib/strings_boost

//! Completion carrier: ONE async interface every provider feeds. A request
//! is (revision, caret, prefix) with a token; providers answer through
//! `text_edit_complete_offer` and stale tokens are dropped — the built-in
//! document-words+keywords provider answers inline, the compiler/LSP
//! providers answer frames later through the same rail. The best candidate
//! becomes the editor's ghost text; Tab accepts it.

struct public TextEditCompletion {
    //! Bumped per request; an offer must carry the token it answers.
    token : int = 0
    request_revision : int = -1
    request_line : int = -1
    request_column : int = -1
    prefix : string
    //! Candidates so far, best first (full words, not suffixes).
    items : array<string>
    //! The request came from an explicit trigger (`.`): the list opens
    //! immediately instead of waiting for the idle delay.
    explicit_trigger : bool = false
}

//! Idle seconds before the candidate LIST pops (the ghost is instant).
let public TEXT_EDIT_LIST_DELAY = 0.35f
//! Rows shown in the list.
let public TEXT_EDIT_LIST_ROWS = 8

def public text_edit_complete_dispose(var comp : TextEditCompletion) {
    delete comp.items
    comp = TextEditCompletion()
}

def private ident_byte(b : int) : bool {
    return is_alpha(b) || is_number(b) || b == '_' || b >= 0x80
}

def private byte_before_caret(ed : TextEditState const) : int {
    return -1 if (empty(ed.lines) || ed.cursor_line >= length(ed.lines)
        || ed.cursor_column <= 0)
    var value = -1
    peek_data(ed.lines[ed.cursor_line]) $(bytes) {
        if (ed.cursor_column <= length(bytes)) {
            value = int(bytes[ed.cursor_column - 1])
        }
    }
    return value
}

def private word_prefix_at(ed : TextEditState const) : string {
    return "" if (empty(ed.lines) || ed.cursor_line >= length(ed.lines))
    let line = ed.lines[ed.cursor_line]
    var start = ed.cursor_column
    peek_data(line) $(bytes) {
        while (start > 0 && start <= length(bytes)
                && ident_byte(int(bytes[start - 1]))) {
            start--
        }
    }
    return slice(line, start, ed.cursor_column)
}

def private collect_document_words(ed : TextEditState const; prefix : string;
                                   var items : array<string>) {
    // frequency-ranked words from the buffer itself (provider #1, instant)
    var counts : table<string; int>
    for (line in ed.lines) {
        peek_data(line) $(bytes) {
            var at = 0
            while (at < length(bytes)) {
                if (!ident_byte(int(bytes[at]))) {
                    at++
                    continue
                }
                let start = at
                while (at < length(bytes) && ident_byte(int(bytes[at]))) {
                    at++
                }
                let word = slice(line, start, at)
                if (word != prefix && starts_with(word, prefix)) {
                    counts[word]++
                }
            }
        }
    }
    var ranked : array<tuple<string; int>>
    ranked |> reserve(length(counts))
    for (word, count in keys(counts), values(counts)) {
        ranked |> push((word, count))
    }
    ranked |> sort() $(a, b) {
        return a._1 > b._1 if (a._1 != b._1)
        return length(a._0) < length(b._0) if (length(a._0) != length(b._0))
        return a._0 < b._0
    }
    items |> reserve(length(items) + length(ranked))
    for (entry in ranked) {
        items |> push(entry._0)
    }
    delete ranked
    delete counts
}

def private refresh_list(var ed : TextEditState; comp : TextEditCompletion const;
                         open_now : bool) {
    return if (!ed.list_open && !open_now)
    if (empty(comp.items)) {
        text_edit_close_list(ed)
        return
    }
    ed.list_open = true
    ed.list_prefix_bytes = length(comp.prefix)
    delete ed.list_items
    let count = min(length(comp.items), TEXT_EDIT_LIST_ROWS)
    ed.list_items |> reserve(count)
    for (index in range(count)) {
        ed.list_items |> push(comp.items[index])
    }
    if (ed.list_index >= count) {
        ed.list_index = count - 1
    }
}

def private refresh_ghost(var ed : TextEditState; comp : TextEditCompletion const) {
    var ghost = ""
    for (item in comp.items) {
        if (item != comp.prefix && starts_with(item, comp.prefix)) {
            ghost = slice(item, length(comp.prefix), length(item))
            break
        }
    }
    text_edit_set_ghost(ed, ghost)
}

def public text_edit_complete_offer(var ed : TextEditState;
                                    var comp : TextEditCompletion;
                                    token : int; items : array<string>) {
    //! Provider answer rail: candidates append (deduplicated) and the ghost
    //! refreshes — an offer for a stale token is dropped whole.
    return if (token != comp.token)
    for (item in items) {
        if (find_index(comp.items, item) < 0) {
            comp.items |> push(item)
        }
    }
    refresh_ghost(ed, comp)
    refresh_list(ed, comp, comp.explicit_trigger)
}

def public text_edit_complete_sync(var ed : TextEditState;
                                   var comp : TextEditCompletion) {
    //! Frame tick: re-request when the caret/buffer moved, run the built-in
    //! document-words + profile-keywords provider, refresh the ghost. Hosts
    //! with async providers kick them off when this bumps `comp.token`.
    var enabled = false
    if (!empty(ed.language)) {
        text_language_profile(ed.language) $(profile) {
            enabled = profile.ghost_completion
        }
    }
    let prefix = enabled && !text_edit_has_selection(ed) ? word_prefix_at(ed) : ""
    // `foo.` is an explicit completion point: empty prefix, list opens now
    let explicit_point = (enabled && empty(prefix)
        && byte_before_caret(ed) == '.')
    if (empty(prefix) && !explicit_point) {
        comp.request_revision = -1
        text_edit_close_list(ed)
        return
    }
    if (comp.request_revision == ed.revision
            && comp.request_line == ed.cursor_line
            && comp.request_column == ed.cursor_column && comp.prefix == prefix) {
        // unchanged request: the list still pops once the caret sits idle
        if (!ed.list_open && length(comp.items) > 1
                && ed.syntax_idle >= TEXT_EDIT_LIST_DELAY) {
            refresh_list(ed, comp, true)
        }
        return
    }
    comp.token++
    comp.request_revision = ed.revision
    comp.request_line = ed.cursor_line
    comp.request_column = ed.cursor_column
    comp.prefix = prefix
    comp.explicit_trigger = explicit_point
    delete comp.items
    var inscope words : array<string>
    collect_document_words(ed, prefix, words)
    text_language_profile(ed.language) $(profile) {
        for (keyword in profile.keywords) {
            if (keyword != prefix && starts_with(keyword, prefix)) {
                words |> push(keyword)
            }
        }
    }
    text_edit_complete_offer(ed, comp, comp.token, words)
}
