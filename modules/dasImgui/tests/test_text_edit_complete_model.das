options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_edit_complete
require imgui/imgui_text_edit_provider_das
require strings

//! Pure-logic coverage for the completion carrier + document-words provider.

[test]
def test_complete_document_words_ghost(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "greeting greeting grease\ngr")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp) // baseline: syncs run per frame
    text_edit_type(ed, "e")
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
    text_edit_load(ed, "re")
    text_edit_set_language(ed, "das")
    text_edit_move_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "t")
    text_edit_complete_sync(ed, comp)
    t |> equal(ed.ghost_text, "urn", "profile keywords complete too")
    // plain profile has the ghost kill switch on
    text_edit_load(ed, "returned re")
    text_edit_set_language(ed, "txt")
    text_edit_move_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "t")
    text_edit_complete_sync(ed, comp)
    t |> success(empty(ed.ghost_text), "plain text never ghosts")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_list_flow(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "grease greet greed\ngr")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "e")
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
def test_complete_navigation_stays_silent(t : T?) {
    // Boris's live catch: arrowing into a word must not ghost or pop the
    // list - only TYPED requests offer.
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "grease greet greed\ngrease again")
    text_edit_set_language(ed, "das")
    text_edit_set_cursor(ed, 1, 3) // mid-word, by navigation
    ed.syntax_idle = 10.0f // long-idle buffer, the old failure mode
    text_edit_complete_sync(ed, comp)
    t |> success(empty(ed.ghost_text), "no ghost from navigation")
    t |> success(!ed.list_open, "no list from navigation")
    text_edit_complete_sync(ed, comp)
    t |> success(!ed.list_open, "the idle clock alone never opens it")
    // navigating right after a dot is quiet too
    text_edit_load(ed, "foo bar\nfoo.")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    ed.syntax_idle = 10.0f
    text_edit_complete_sync(ed, comp)
    t |> success(!ed.list_open, "navigating to a dot does not trigger")
    // typing still offers
    text_edit_set_cursor(ed, 0, 0)
    text_edit_type(ed, "f")
    text_edit_complete_sync(ed, comp)
    t |> success(!empty(ed.ghost_text), "typing ghosts as before")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_escape_dismissal_sticks(t : T?) {
    // Boris's live catch #2: Esc closed the list but the same idle request
    // reopened it next frame - the menu looked immovable.
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "grease greet greed\ngr")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "e")
    text_edit_complete_sync(ed, comp)
    ed.syntax_idle = 1.0f
    text_edit_complete_sync(ed, comp)
    t |> success(ed.list_open, "typed + idle opens the list")
    text_edit_close_list(ed) // the Escape path
    text_edit_complete_sync(ed, comp)
    text_edit_complete_sync(ed, comp)
    t |> success(!ed.list_open, "dismissal sticks across syncs")
    text_edit_type(ed, "e") // "gree"
    text_edit_complete_sync(ed, comp)
    ed.syntax_idle = 1.0f
    text_edit_complete_sync(ed, comp)
    t |> success(ed.list_open, "the next typed request may open again")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_explicit_dot_trigger(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "foo bar baz\nfoo")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, ".")
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
def test_das_provider_compiles_symbols(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    var provider = TextEditDasProvider()
    text_edit_load(ed, "def alpha_one() \{\}\ndef alpha_two() \{\}\nvar alpha_global = 1")
    text_edit_set_language(ed, "das")
    // hand-built request isolates the provider: document words never ran
    comp.token = 5
    comp.request_revision = ed.revision
    comp.prefix = "alp"
    comp.typed = true
    text_edit_das_provider_sync(ed, comp, provider)
    t |> equal(provider.sent_token, 5, "the worker got the request")
    var found = false
    var waited = 0
    while (!found && waited < 500) {
        sleep(20u)
        text_edit_das_provider_sync(ed, comp, provider)
        found = find_index(comp.items, "alpha_one") >= 0
        waited++
    }
    t |> success(found, "compiled function names arrive through the offer rail")
    t |> success(find_index(comp.items, "alpha_two") >= 0, "every function offers")
    t |> success(find_index(comp.items, "alpha_global") >= 0, "globals offer too")
    t |> equal(ed.ghost_text, "ha_one", "the offer refreshed the ghost")
    text_edit_das_provider_stop(provider)
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_das_issue_parsing(t : T?) {
    var out : array<TextEditDiagnostic>
    let issues = ("error[30151]: syntax error, unexpected end of file\n"
        + "text_edit_completion_overlay.das:3:19\n"
        + "var alpha_global = 1\n"
        + "error[30341]: no matching functions\n"
        + "text_edit_completion_overlay.das:1:5\n")
    text_edit_parse_das_issues(issues, "text_edit_completion_overlay.das", out)
    t |> equal(length(out), 2, "both findings parse")
    t |> equal(out[0].line, 2, "line converts to 0-based")
    t |> equal(out[0].column, 18, "column converts to 0-based")
    t |> success(find(out[0].message, "syntax error") >= 0, "the message rides along")
    t |> equal(out[1].line, 0, "the second finding pins its own line")
    delete out
}

[test]
def test_das_provider_reports_diagnostics(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    var provider = TextEditDasProvider()
    text_edit_load(ed, "def broken( \{")
    text_edit_set_language(ed, "das")
    ed.syntax_idle = 1.0f // the idle diagnostics kick, no completion needed
    text_edit_das_provider_sync(ed, comp, provider)
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.compiling),
        "the kick marks the buffer compiling")
    var waited = 0
    while (ed.compile_status == TextEditCompileStatus.compiling && waited < 500) {
        sleep(20u)
        text_edit_das_provider_sync(ed, comp, provider)
        waited++
    }
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.failed),
        "a broken buffer reports failed")
    t |> success(!empty(ed.diagnostics), "the findings underline")
    // fix the buffer: the next idle compile turns it ok and clears findings
    text_edit_load(ed, "def fine() \{\}")
    text_edit_set_language(ed, "das")
    ed.syntax_idle = 1.0f
    text_edit_das_provider_sync(ed, comp, provider)
    waited = 0
    while (ed.compile_status != TextEditCompileStatus.ok && waited < 500) {
        sleep(20u)
        text_edit_das_provider_sync(ed, comp, provider)
        waited++
    }
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.ok),
        "a fixed buffer reports ok")
    t |> success(empty(ed.diagnostics), "the findings clear")
    text_edit_das_provider_stop(provider)
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_offer_token_contract(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "al")
    text_edit_set_language(ed, "das")
    text_edit_move_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "p")
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
