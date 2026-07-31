options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_edit_syntax

//! Pure-logic coverage for the full-parse refinement tier.

[test]
def test_apply_syntax_maps_and_splits(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "abc\ndef\nghi")
    ed.language = "daslang"
    // "bc" on line 0; "ef\ngh" spans lines 1-2 and must split at the newline
    var tokens <- [
        SyntaxToken(start_byte = 1, end_byte = 3, role = SyntaxRole.type_),
        SyntaxToken(start_byte = 5, end_byte = 10, role = SyntaxRole.string_)]
    text_edit_apply_syntax(ed, tokens, ed.revision)
    t |> equal(length(ed.line_styles), 3, "entries align with the buffer")
    t |> equal(ed.line_styles[0].runs[0].start_byte, 1, "line 0 run start maps")
    t |> equal(int(ed.line_styles[0].runs[0].style), int(TextSourceSyntaxStyle.type_),
        "type role maps to the type style")
    t |> equal(ed.line_styles[1].runs[0].start_byte, 1, "line 1 piece starts at e")
    t |> equal(ed.line_styles[1].runs[0].end_byte, 3, "line 1 piece runs to line end")
    t |> equal(ed.line_styles[2].runs[0].start_byte, 0, "line 2 piece starts the line")
    t |> equal(ed.line_styles[2].runs[0].end_byte, 2, "line 2 piece covers gh")
    t |> equal(ed.syntax_revision, ed.revision, "the parse revision is recorded")
    delete tokens
    text_edit_dispose(ed)
}

[test]
def test_parse_styles_survive_same_line_edits(t : T?) {
    // Boris's flicker catch: typing re-lexes the line and dropped
    // parse-refined colors to plain until the reparse - blue/white/blue.
    var ed = TextEditState()
    text_edit_load(ed, "def foo() \{\n\tfoo(bar)\n\}")
    text_edit_set_language(ed, "das")
    var tokens <- [
        SyntaxToken(start_byte = 13, end_byte = 16, role = SyntaxRole.function_),
        SyntaxToken(start_byte = 17, end_byte = 20, role = SyntaxRole.variable)]
    text_edit_apply_syntax(ed, tokens, ed.revision)
    t |> equal(int(ed.line_styles[1].runs[0].style),
        int(TextSourceSyntaxStyle.function_), "the parse styled foo")
    // typing at the line end keeps both runs in place - no flicker
    text_edit_set_cursor(ed, 1, 9)
    text_edit_type(ed, ";")
    t |> equal(length(ed.line_styles[1].runs), 2, "both parse runs survive the edit")
    t |> equal(ed.line_styles[1].runs[0].start_byte, 1, "foo stays put before the edit")
    // typing INSIDE foo cuts that run; bar shifts right and keeps its color
    text_edit_set_cursor(ed, 1, 2)
    text_edit_type(ed, "z")
    t |> equal(length(ed.line_styles[1].runs), 1, "the cut run drops until reparse")
    t |> equal(int(ed.line_styles[1].runs[0].style),
        int(TextSourceSyntaxStyle.variable), "bar keeps its parse style")
    t |> equal(ed.line_styles[1].runs[0].start_byte, 6, "bar shifted by the insert")
    delete tokens
    text_edit_dispose(ed)
}

[test]
def test_syntax_sync_debounce_and_refine(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "def foo() \{\n\tlet x = 1\n\}")
    text_edit_set_language(ed, "das")
    t |> success(!text_edit_syntax_sync(ed), "under the debounce nothing runs")
    ed.syntax_idle = 1.0f
    if (!syntax_language_available("daslang")) {
        t |> success(true, "grammar library absent - lexical tier stands")
        text_edit_dispose(ed)
        return
    }
    t |> success(text_edit_syntax_sync(ed), "idle buffer parses")
    t |> equal(ed.syntax_revision, ed.revision, "parse marks the revision")
    var refined = false
    for (run in ed.line_styles[0].runs) {
        if (run.style == TextSourceSyntaxStyle.function_) {
            refined = true
        }
    }
    t |> success(refined, "the parse refines foo to a function style")
    ed.syntax_idle = 1.0f
    t |> success(!text_edit_syntax_sync(ed), "same revision does not re-parse")
    text_edit_insert(ed, "z")
    t |> success(ed.syntax_revision != ed.revision, "an edit makes the parse stale")
    t |> success(!text_edit_syntax_sync(ed), "the edit reset the idle clock")
    text_edit_dispose(ed)
}
