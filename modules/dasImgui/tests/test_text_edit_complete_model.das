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
