options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_rich_das

//! Pure-logic coverage for the rich-das provider: classification, grouping,
//! fragment stripping, caret mapping, and the adoption cache.

let FIXTURE = "// # Title\n// intro text\n\nlet x = 1 // trailing\n\n\t// indented note\n/*\nBlock body\n*/\nlet y = 2\n"

def private das_editor(source : string) : TextEditState {
    var ed = TextEditState()
    text_edit_load(ed, source)
    text_edit_set_language(ed, "daslang")
    return <- ed
}

[test]
def test_rich_das_classification(t : T?) {
    var ed = das_editor(FIXTURE)
    t |> success(rich_das_classify_line(ed, 0) == RichDasLineClass.full_comment,
        "a // line is a full comment")
    t |> success(rich_das_classify_line(ed, 2) == RichDasLineClass.blank,
        "an empty line is blank")
    t |> success(rich_das_classify_line(ed, 3) == RichDasLineClass.code,
        "code with a trailing comment stays code")
    t |> success(rich_das_classify_line(ed, 5) == RichDasLineClass.full_comment,
        "an indented // line is a full comment")
    t |> success(rich_das_classify_line(ed, 7) == RichDasLineClass.full_comment,
        "a block-comment interior line is a full comment")
    t |> success(rich_das_classify_line(ed, 9) == RichDasLineClass.code,
        "plain code is code")
    var plain = TextEditState()
    text_edit_load(plain, "// not das")
    t |> success(rich_das_classify_line(plain, 0) == RichDasLineClass.code,
        "non-daslang buffers never classify rich (fail closed)")
    text_edit_dispose(ed)
    text_edit_dispose(plain)
}

[test]
def test_rich_das_grouping(t : T?) {
    var ed = das_editor(FIXTURE)
    var inscope spans : array<RichDasGroupSpan>
    rich_das_collect_groups(ed, spans)
    t |> equal(length(spans), 3, "adjacency + indent + blanks form three groups")
    t |> equal(spans[0].first_line, 0, "header group starts at the top")
    t |> equal(spans[0].last_line, 1, "blank line splits the header group")
    t |> equal(spans[1].first_line, 5, "indent change starts a new group")
    t |> equal(spans[1].last_line, 5, "the indented note stands alone")
    t |> equal(spans[2].first_line, 6, "self-contained /* */ forms a group")
    t |> equal(spans[2].last_line, 8, "the block group covers its closer")
    text_edit_dispose(ed)
}

[test]
def test_rich_das_block_comment_poisoning(t : T?) {
    var opener = das_editor("x = 1 /* note\nstill note\n*/\nlet z = 3\n")
    var inscope opener_spans : array<RichDasGroupSpan>
    rich_das_collect_groups(opener, opener_spans)
    t |> equal(length(opener_spans), 0,
        "a MIXED opener poisons the whole block span")
    var closer = das_editor("/* body\ninner\n*/ let z = 3\n")
    var inscope closer_spans : array<RichDasGroupSpan>
    rich_das_collect_groups(closer, closer_spans)
    t |> equal(length(closer_spans), 0,
        "a MIXED closer poisons the whole block span")
    // a blank interior line poisons too: the tail would be an interior-only
    // island, which the grouping rules forbid outright
    var gapped = das_editor("/* alpha\n\nbeta */\nlet z = 3\n")
    var inscope gapped_spans : array<RichDasGroupSpan>
    rich_das_collect_groups(gapped, gapped_spans)
    t |> equal(length(gapped_spans), 0,
        "a blank inside a pure block comment keeps the whole span source")
    text_edit_dispose(opener)
    text_edit_dispose(closer)
    text_edit_dispose(gapped)
}

[test]
def test_rich_das_fragment_strip(t : T?) {
    var ed = das_editor(FIXTURE)
    var inscope spans : array<RichDasGroupSpan>
    rich_das_collect_groups(ed, spans)
    var inscope strip : array<int>
    let header = rich_das_fragment(ed, spans[0], strip)
    t |> equal(header, "# Title\nintro text",
        "// + ONE space strip per line")
    t |> equal(strip[0], 3, "strip map records the head bytes")
    let note = rich_das_fragment(ed, spans[1], strip)
    t |> equal(note, "indented note", "indent + prefix stripped")
    t |> equal(strip[0], 4, "tab indent counts into the strip")
    let block_ = rich_das_fragment(ed, spans[2], strip)
    t |> equal(block_, "\nBlock body\n",
        "block delimiters strip; interior lines keep their text")
    var spaced = das_editor("//     let sample = 1\n")
    var inscope spaced_spans : array<RichDasGroupSpan>
    rich_das_collect_groups(spaced, spaced_spans)
    let code_frag = rich_das_fragment(spaced, spaced_spans[0], strip)
    t |> equal(code_frag, "    let sample = 1",
        "ONLY one space strips - md indented-code survives")
    text_edit_dispose(ed)
    text_edit_dispose(spaced)
}

[test]
def test_rich_das_caret_mapping(t : T?) {
    var ed = das_editor(FIXTURE)
    var st = RichDasState()
    let _ = rich_das_sync(st, ed, true)
    t |> equal(length(st.groups), 3, "sync builds the groups")
    // "intro text" starts at fragment byte 8 of the header group
    let target = rich_das_caret_target(st, 0, 8)
    t |> equal(target.x, 1, "fragment line maps to the exact source line")
    t |> equal(target.y, 3, "column re-adds the stripped head bytes")
    rich_das_dispose(st)
    text_edit_dispose(ed)
}

[test]
def test_rich_das_adoption_cache(t : T?) {
    var ed = das_editor(FIXTURE)
    var st = RichDasState()
    let _first = rich_das_sync(st, ed, true)
    t |> equal(st.parse_count, 3, "first sync parses every group")
    let same = rich_das_sync(st, ed, true)
    t |> success(!same, "same revision syncs are no-ops")
    // edit INSIDE the header group: only that fragment reparses
    text_edit_set_cursor(ed, 0, 5)
    text_edit_insert(ed, "X")
    let _second = rich_das_sync(st, ed, true)
    t |> equal(st.parse_count, 4, "one changed group = ONE reparse")
    t |> equal(length(st.groups), 3, "group count stable")
    // edit in CODE between groups: nothing reparses
    text_edit_set_cursor(ed, 9, 0)
    text_edit_insert(ed, "let w = 4\n")
    let _third = rich_das_sync(st, ed, true)
    t |> equal(st.parse_count, 4, "code edits between groups reparse nothing")
    // a NEW comment line below code becomes a new group (a line adjacent
    // to an existing group would MERGE into it - the adjacency rule)
    text_edit_set_cursor(ed, 10, 0)
    text_edit_insert(ed, "// fresh note\n")
    let _fourth = rich_das_sync(st, ed, true)
    t |> equal(length(st.groups), 4, "a new comment line forms a new group")
    t |> equal(st.parse_count, 5, "only the new group parses")
    rich_das_dispose(st)
    text_edit_dispose(ed)
}
