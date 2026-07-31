options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_edit_complete

//! Pure-logic coverage for the completion carrier + document-words provider.

[test]
def test_complete_document_words_ghost(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "greeting greeting grease\ngre")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    t |> equal(comp.prefix, "gre", "the word before the caret is the prefix")
    t |> equal(ed.ghost_text, "eting", "the most frequent word wins the ghost")
    t |> equal(comp.items[1], "grease", "runners-up stay in the list")
    // Tab accepts the ghost instead of indenting
    text_edit_tab(ed)
    t |> equal(ed.lines[1], "greeting", "Tab accepts the ghost")
    t |> success(empty(ed.ghost_text), "accepting clears the ghost")
    text_edit_complete_sync(ed, comp)
    // caret movement away clears the ghost and the next sync re-requests
    text_edit_move_home(ed)
    t |> success(empty(ed.ghost_text), "moving clears the ghost")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_keywords_and_kill_switch(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "ret")
    text_edit_set_language(ed, "das")
    text_edit_move_end(ed)
    text_edit_complete_sync(ed, comp)
    t |> equal(ed.ghost_text, "urn", "profile keywords complete too")
    // plain profile has the ghost kill switch on
    text_edit_load(ed, "returned ret")
    text_edit_set_language(ed, "txt")
    text_edit_move_end(ed)
    text_edit_complete_sync(ed, comp)
    t |> success(empty(ed.ghost_text), "plain text never ghosts")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_list_flow(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "grease greet greed\ngre")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    t |> success(!ed.list_open, "the list waits for the idle delay")
    ed.syntax_idle = 1.0f
    text_edit_complete_sync(ed, comp)
    t |> success(ed.list_open, "idle pops the list")
    t |> equal(length(ed.list_items), 3, "all three candidates show")
    t |> equal(ed.list_index, -1, "the keyboard is still in the editor")
    // Down enters, Down steps, Up walks back out
    t |> success(text_edit_list_next(ed), "Down enters the list")
    t |> equal(ed.list_index, 0, "at the first row")
    let _ = text_edit_list_next(ed)
    t |> equal(ed.list_index, 1, "Down steps")
    let _2 = text_edit_list_prev(ed)
    t |> success(text_edit_list_prev(ed), "Up walks back out")
    t |> equal(ed.list_index, -1, "back in the editor, list still visible")
    t |> success(!text_edit_list_accept(ed), "Enter with no row selected declines")
    // enter a row and accept it
    let _3 = text_edit_list_next(ed)
    let _4 = text_edit_list_next(ed)
    t |> success(text_edit_list_accept(ed), "Enter accepts the selected row")
    t |> equal(ed.lines[1], "greet", "the second-ranked candidate landed")
    t |> success(!ed.list_open, "accepting closes the list")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_explicit_dot_trigger(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "foo bar baz\nfoo.")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    t |> success(ed.list_open, "a dot opens the list immediately")
    t |> success(length(ed.list_items) >= 3, "all document words offer after the dot")
    t |> equal(ed.list_prefix_bytes, 0, "empty prefix - the whole word inserts")
    text_edit_close_list(ed)
    t |> success(!ed.list_open, "Escape path closes it")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_offer_token_contract(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "alp")
    text_edit_set_language(ed, "das")
    text_edit_move_end(ed)
    text_edit_complete_sync(ed, comp)
    let live_token = comp.token
    var inscope stale <- ["alpine"]
    text_edit_complete_offer(ed, comp, live_token - 1, stale)
    t |> success(find_index(comp.items, "alpine") < 0, "stale offers are dropped whole")
    var inscope fresh <- ["alphabet"]
    text_edit_complete_offer(ed, comp, live_token, fresh)
    t |> success(find_index(comp.items, "alphabet") >= 0, "live offers append")
    t |> equal(ed.ghost_text, "habet", "the async offer refreshes the ghost")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}
