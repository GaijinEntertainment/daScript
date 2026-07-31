options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_edit_complete
require imgui/imgui_text_edit_provider_das
require imgui/imgui_text_edit_provider_cpp
require daslib/fio
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
    let issues = ("error[30838]: can't locate variable 'zzz'\n"
        + "text_edit_completion_overlay.das:2:10\n"
        + "var bad = zzz\n"
        + "          ^^^\n"
        + "error[30341]: no matching functions\n"
        + "text_edit_completion_overlay.das:1:5\n")
    text_edit_parse_das_issues(issues, "text_edit_completion_overlay.das", out)
    t |> equal(length(out), 2, "both findings parse")
    t |> equal(out[0].line, 1, "line converts to 0-based")
    t |> equal(out[0].column, 10, "printed columns are ALREADY 0-based")
    t |> equal(out[0].end_column, 13, "the caret width becomes the range end")
    t |> equal(out[0].file, "", "the edited buffer's findings carry no path")
    t |> success(find(out[0].message, "can't locate") >= 0, "the message rides along")
    t |> equal(out[1].line, 0, "the second finding pins its own line")
    t |> equal(out[1].end_column, out[1].column, "no caret line means unknown width")
    // a finding in a REQUIRED file keeps its path
    var foreign : array<TextEditDiagnostic>
    text_edit_parse_das_issues("error[30001]: broken elsewhere\n"
        + "daslib/foo.das:7:4\n", "text_edit_completion_overlay.das", foreign)
    t |> equal(length(foreign), 1, "foreign findings parse too")
    t |> equal(foreign[0].file, "daslib/foo.das", "and keep their path")
    delete foreign
    delete out
}

[test]
def test_das_provider_reports_diagnostics(t : T?) {
    var ed = TextEditState()
    var comp = TextEditCompletion()
    var provider = TextEditDasProvider()
    text_edit_load(ed, "def broken( \{")
    text_edit_set_language(ed, "das")
    text_edit_set_file_name(ed, "hello.das") // errors anchor on the real name
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
    t |> equal(ed.diagnostics[0].file, "",
        "a named buffer's own findings still carry no path")
    // fix the buffer: the next idle compile turns it ok and clears findings
    // (the array literal is gen2-only - proves options-less buffers parse gen2)
    text_edit_load(ed, "var g <- [1, 2, 3]\ndef fine() \{\}")
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
def test_multiline_diagnostic_spans(t : T?) {
    // squiggles across lines: first row from the column, middle rows whole,
    // last row to the end column; nothing outside; foreign files never span
    var ed = TextEditState()
    text_edit_load(ed, "aaaa\nbbbb\ncccc\ndddd")
    let diag = TextEditDiagnostic(line = 1, column = 2, end_line = 3,
        end_column = 1, file = "", message = "spans")
    t |> equal(text_edit_diagnostic_row_span(ed, diag, 0), int2(-1, -1),
        "no span above the range")
    t |> equal(text_edit_diagnostic_row_span(ed, diag, 1), int2(2, 4),
        "first row runs from the start column to line end")
    t |> equal(text_edit_diagnostic_row_span(ed, diag, 2), int2(0, 4),
        "middle rows cover the whole line")
    t |> equal(text_edit_diagnostic_row_span(ed, diag, 3), int2(0, 1),
        "last row stops at the end column")
    t |> equal(text_edit_diagnostic_row_span(ed, diag, 4), int2(-1, -1),
        "no span below the range")
    // width-unknown single position falls back to the word
    let point = TextEditDiagnostic(line = 0, column = 1, end_line = 0,
        end_column = 1, file = "", message = "point")
    t |> equal(text_edit_diagnostic_row_span(ed, point, 0), int2(1, 4),
        "unknown width underlines from the position to the word end")
    // foreign findings never underline this buffer
    let foreign = TextEditDiagnostic(line = 1, column = 0, end_line = 1,
        end_column = 4, file = "daslib/foo.das", message = "elsewhere")
    t |> equal(text_edit_diagnostic_row_span(ed, foreign, 1), int2(-1, -1),
        "foreign files draw nothing here")
    text_edit_dispose(ed)
}

[test]
def test_complete_statement_start_prefers_keywords(t : T?) {
    // Boris: typing re at statement start should offer return FIRST
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "retro retro retro\n\tr")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "e")
    text_edit_complete_sync(ed, comp)
    t |> equal(comp.items[0], "recover", "statement start ranks keywords first")
    t |> success(find_index(comp.items, "return") >= 0, "return is up there")
    t |> success(comp.item_tiers[find_index(comp.items, "retro")] == 1,
        "document words rank below")
    // mid-expression the same prefix ranks words first
    text_edit_load(ed, "retro retro retro\nx = r")
    text_edit_set_language(ed, "das")
    text_edit_move_document_end(ed)
    text_edit_complete_sync(ed, comp)
    text_edit_type(ed, "e")
    text_edit_complete_sync(ed, comp)
    t |> equal(comp.items[0], "retro", "mid-expression ranks document words first")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_rank_tiers(t : T?) {
    // Boris: ranking was backwards - compiled symbols arrived last and sat
    // at the bottom. Tiers: 0 compiled beats 1 doc words beats 2 keywords.
    var ed = TextEditState()
    var comp = TextEditCompletion()
    text_edit_load(ed, "re")
    text_edit_set_language(ed, "das")
    comp.token = 1
    comp.request_revision = ed.revision
    comp.prefix = "re"
    comp.typed = true
    var inscope keywords <- ["return"]
    text_edit_complete_offer(ed, comp, 1, keywords, 2)
    var inscope words <- ["retro"]
    text_edit_complete_offer(ed, comp, 1, words, 1)
    var inscope compiled <- ["real_fn"]
    text_edit_complete_offer(ed, comp, 1, compiled, 0)
    t |> equal(comp.items[0], "real_fn", "compiled symbols rank first")
    t |> equal(comp.items[1], "retro", "document words second")
    t |> equal(comp.items[2], "return", "keywords last")
    t |> equal(ed.ghost_text, "al_fn", "the ghost takes the best tier")
    // a duplicate arriving at a better tier promotes
    var inscope promo <- ["retro"]
    text_edit_complete_offer(ed, comp, 1, promo, 0)
    t |> equal(comp.item_tiers[1], 0, "the doc word promoted to compiled")
    t |> equal(length(comp.items), 3, "promotion does not duplicate")
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_clang_output_parsing(t : T?) {
    var out : array<TextEditDiagnostic>
    let output = ("C:/tmp/_check_1.cpp:2:13: error: use of undeclared identifier 'yyy'\n"
        + "    2 |     int x = yyy;\n"
        + "      |             ^~~\n"
        + "C:/tmp/_check_1.cpp:3:13: error: expected ';' after return statement\n"
        + "    3 |     return 0\n"
        + "      |             ^\n"
        + "include/some.h:9:1: warning: something odd\n"
        + "2 errors generated.\n")
    text_edit_parse_clang_output(output, "C:/tmp/_check_1.cpp", out)
    t |> equal(length(out), 3, "errors and warnings parse")
    t |> equal(out[0].line, 1, "clang lines convert to 0-based")
    t |> equal(out[0].column, 12, "clang columns convert to 0-based")
    t |> equal(out[0].end_column, 15, "caret plus tildes size the range")
    t |> equal(out[0].file, "", "the checked buffer carries no path")
    t |> equal(out[1].end_column, 13, "a lone caret is width one")
    t |> equal(out[2].file, "include/some.h", "header findings keep their path")
    delete out
}

[test]
def test_clang_output_parsing_posix_and_fatal(t : T?) {
    // the location split counts colons, so a drive-letter path and a POSIX one
    // must both parse; a missing #include only ever says "fatal error"
    var out : array<TextEditDiagnostic>
    let output = ("/tmp/_check_7.cpp:1:10: fatal error: 'nope.h' file not found\n"
        + "    1 | #include \"nope.h\"\n"
        + "      |          ^~~~~~~~\n"
        + "/usr/include/stdio.h:4:2: error: broken header\n"
        + "1 error generated.\n")
    text_edit_parse_clang_output(output, "/tmp/_check_7.cpp", out)
    t |> equal(length(out), 2, "POSIX paths parse")
    t |> success(find(out[0].message, "file not found") >= 0,
        "fatal errors are findings too")
    t |> equal(out[0].line, 0, "the fatal-error line converts to 0-based")
    t |> equal(out[0].column, 9, "and so does its column")
    t |> equal(out[0].end_column, 17, "the caret run sizes the include span")
    t |> equal(out[0].file, "", "the checked buffer carries no path")
    t |> equal(out[1].file, "/usr/include/stdio.h", "foreign POSIX paths survive")
    t |> equal(out[1].line, 3, "and pin their own line")
    delete out
}

[test]
def test_cpp_check_argv_selects_language(t : T?) {
    // valid C fails the C++ front end, so a `c` buffer must not go through it
    var inscope cpp_argv <- text_edit_cpp_check_argv("clang", "cpp", "/tmp/x.cpp")
    t |> equal(cpp_argv[1], "-fsyntax-only", "the check is a syntax-only round trip")
    t |> equal(cpp_argv[3], "c++", "cpp buffers check as C++")
    t |> equal(cpp_argv[4], "/tmp/x.cpp", "the scratch file is the last argument")
    t |> equal(text_edit_cpp_scratch_suffix("cpp"), ".cpp", "in a .cpp scratch")
    var inscope c_argv <- text_edit_cpp_check_argv("clang", "c", "/tmp/x.c")
    t |> equal(c_argv[3], "c", "c buffers check as C")
    t |> equal(text_edit_cpp_scratch_suffix("c"), ".c", "in a .c scratch")
    t |> success(!text_edit_cpp_compiler_available(""),
        "an empty compiler is never available")
    t |> success(!text_edit_cpp_compiler_available("dasimgui_no_such_clang"),
        "and neither is a name that does not run")
}

[test]
def test_cpp_provider_reports_diagnostics(t : T?) {
    let compiler = text_edit_cpp_compiler_discover()
    // fexist alone would skip forever wherever clang lives on PATH
    if (!text_edit_cpp_compiler_available(compiler)) {
        t |> success(true, "no clang on this box - provider skipped")
        return
    }
    var ed = TextEditState()
    var provider = TextEditCppProvider()
    text_edit_load(ed, "int main() \{\n    int x = yyy;\n\}")
    text_edit_set_language(ed, "cpp")
    text_edit_set_file_name(ed, "hello.cpp")
    ed.syntax_idle = 1.0f
    text_edit_cpp_provider_sync(ed, provider)
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.compiling),
        "the kick marks the buffer compiling")
    var waited = 0
    while (ed.compile_status == TextEditCompileStatus.compiling && waited < 750) {
        sleep(20u)
        text_edit_cpp_provider_sync(ed, provider)
        waited++
    }
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.failed),
        "a broken buffer reports failed")
    t |> success(!empty(ed.diagnostics), "clang findings arrive")
    t |> equal(ed.diagnostics[0].line, 1, "the finding pins the yyy line")
    t |> equal(ed.diagnostics[0].file, "", "buffer findings carry no path")
    text_edit_load(ed, "static int add(int a, int b) \{\n    return a + b;\n\}")
    text_edit_set_language(ed, "cpp")
    ed.syntax_idle = 1.0f
    text_edit_cpp_provider_sync(ed, provider)
    waited = 0
    while (ed.compile_status != TextEditCompileStatus.ok && waited < 750) {
        sleep(20u)
        text_edit_cpp_provider_sync(ed, provider)
        waited++
    }
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.ok),
        "a clean buffer reports ok")
    t |> success(empty(ed.diagnostics), "the findings clear")
    text_edit_cpp_provider_stop(provider)
    text_edit_dispose(ed)
}

[test]
def test_cpp_provider_c_buffer_uses_c_dialect(t : T?) {
    let compiler = text_edit_cpp_compiler_discover()
    if (!text_edit_cpp_compiler_available(compiler)) {
        t |> success(true, "no clang on this box - provider skipped")
        return
    }
    var ed = TextEditState()
    var provider = TextEditCppProvider()
    // legal C, rejected by C++: void* converts to any object pointer in C only
    text_edit_load(ed, "int main(void) \{ void *v = 0; int *p = v; return p != 0; \}")
    text_edit_set_language(ed, "c")
    text_edit_set_file_name(ed, "hello.c")
    ed.syntax_idle = 1.0f
    text_edit_cpp_provider_sync(ed, provider)
    var waited = 0
    while (ed.compile_status == TextEditCompileStatus.compiling && waited < 750) {
        sleep(20u)
        text_edit_cpp_provider_sync(ed, provider)
        waited++
    }
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.ok),
        "a c buffer checks as C, not C++")
    t |> success(empty(ed.diagnostics), "and reports nothing")
    text_edit_cpp_provider_stop(provider)
    text_edit_dispose(ed)
}

[test]
def test_das_provider_drops_stale_results(t : T?) {
    // a compile finishing after the buffer moved must not paint its findings
    // onto the newer content
    var ed = TextEditState()
    var comp = TextEditCompletion()
    var provider = TextEditDasProvider()
    text_edit_load(ed, "def broken( \{")
    text_edit_set_language(ed, "das")
    ed.syntax_idle = 1.0f
    text_edit_das_provider_sync(ed, comp, provider) // kicks at this revision
    text_edit_type(ed, "\}") // the buffer moves on while the compile runs
    ed.syntax_idle = 0.0f // ... and stays busy, so no re-kick races the drop
    var waited = 0
    while (provider.compiled_revision != -1 && waited < 500) {
        sleep(20u)
        text_edit_das_provider_sync(ed, comp, provider)
        waited++
    }
    t |> equal(provider.compiled_revision, -1, "the stale result drained and dropped")
    t |> equal(int(ed.compile_status), int(TextEditCompileStatus.compiling),
        "it never reported a status for content it never saw")
    t |> success(empty(ed.diagnostics), "and never underlined the newer buffer")
    text_edit_das_provider_stop(provider)
    text_edit_complete_dispose(comp)
    text_edit_dispose(ed)
}

[test]
def test_complete_list_survives_keywordless_prefix(t : T?) {
    // the keyword tier is offered first and is often empty; an empty offer used
    // to close the open list, so every such keystroke flickered it shut
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
    text_edit_type(ed, "e") // "gree" - no daslang keyword starts with it
    text_edit_complete_sync(ed, comp)
    t |> success(ed.list_open, "a keyword-less prefix keeps the list open")
    t |> equal(length(ed.list_items), 2, "showing the narrowed candidates")
    // a prefix nothing matches still closes it
    text_edit_type(ed, "zz")
    text_edit_complete_sync(ed, comp)
    t |> success(!ed.list_open, "no candidates at all closes the list")
    t |> success(empty(ed.ghost_text), "and clears the ghost")
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
