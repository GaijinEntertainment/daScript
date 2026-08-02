options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_create

//! Pure-logic coverage for the creation registry: built-ins, last-wins user
//! overrides, predicate gating, context facts, and the transform shapes.

def private md_editor(source : string) : TextEditState {
    var ed = TextEditState()
    text_edit_load(ed, source)
    text_edit_set_language(ed, "markdown")
    return <- ed
}

[test]
def test_create_builtins_present(t : T?) {
    var ed = md_editor("")
    var found_bold = false
    var found_if = false
    let _b = creation_get("md.bold") $(definition) {
        found_bold = definition.shape == CreationShape.wrap
    }
    let _i = creation_get("das.if") $(definition) {
        found_if = definition.format == "daslang"
    }
    t |> success(found_bold, "md.bold ships as a wrap")
    t |> success(found_if, "das.if ships for daslang buffers")
    var inscope md_ids <- creation_ids("markdown")
    t |> success(length(md_ids) >= 12, "the markdown set is registered")
    var inscope das_ids <- creation_ids("daslang")
    t |> success(length(das_ids) >= 12, "the daslang set is registered")
    text_edit_dispose(ed)
}

[test]
def test_create_last_wins_override(t : T?) {
    var ed = md_editor("hello")
    text_edit_select(ed, 0, 0, 0, 5)
    creation_define(CreatableDefinition(id = "md.bold", title = "Bold",
        format = "markdown", shape = CreationShape.wrap, body = "__$\{SEL}__"))
    t |> success(creation_execute(ed, "md.bold"), "overridden bold executes")
    t |> equal(text_edit_text(ed), "__hello__",
        "user creation_define REPLACES the built-in (last wins)")
    // restore the shipped shape (incl. its tag) for later tests in-process
    var restored = CreatableDefinition(id = "md.bold", title = "Bold",
        format = "markdown", shape = CreationShape.wrap, body = "**$\{SEL}**",
        key = ImGuiKey.B, key_primary = true)
    restored.tags |> push("not-in-code-fence")
    creation_define(restored)
    text_edit_dispose(ed)
}

[test]
def test_create_predicate_lambda(t : T?) {
    var ed = md_editor("text here\n")
    var definition = CreatableDefinition(id = "user.rule", title = "Rule",
        format = "markdown", shape = CreationShape.insert, body = "---\n")
    definition.can_create <- @(ctx : CreationContext const) : bool {
        return ctx.blank_line
    }
    creation_define(definition)
    text_edit_set_cursor(ed, 0, 0)
    var inscope on_text <- creation_context(ed)
    var inscope allowed_on_text <- creation_available("markdown", on_text)
    t |> success(find_index(allowed_on_text, "user.rule") < 0,
        "the predicate excludes a non-blank line")
    t |> success(!creation_execute(ed, "user.rule"), "execute declines too")
    text_edit_set_cursor(ed, 1, 0)
    var inscope on_blank <- creation_context(ed)
    var inscope allowed_on_blank <- creation_available("markdown", on_blank)
    t |> success(find_index(allowed_on_blank, "user.rule") >= 0,
        "the predicate admits a blank line")
    t |> success(creation_execute(ed, "user.rule"), "execute lands there")
    t |> equal(ed.lines[1], "---", "the rule inserted")
    text_edit_dispose(ed)
}

[test]
def test_create_context_facts(t : T?) {
    var ed = md_editor("# Title\n\n- item\n```das\ncode\n```\ntail\n")
    text_edit_set_cursor(ed, 0, 2)
    var inscope heading_ctx <- creation_context(ed)
    t |> equal(heading_ctx.heading_level, 1, "heading level detected")
    t |> success(!heading_ctx.in_code_fence, "heading is outside the fence")
    text_edit_set_cursor(ed, 2, 3)
    var inscope list_ctx <- creation_context(ed)
    t |> equal(list_ctx.list_prefix, "- ", "list marker detected")
    text_edit_set_cursor(ed, 4, 0)
    var inscope fence_ctx <- creation_context(ed)
    t |> success(fence_ctx.in_code_fence, "inside the fenced block")
    var inscope fenced <- creation_available("markdown", fence_ctx)
    t |> success(find_index(fenced, "md.bold") < 0,
        "not-in-code-fence gates the markdown set inside fences")
    var das_ed = TextEditState()
    text_edit_load(das_ed, "\tlet x = 1 // note\n")
    text_edit_set_language(das_ed, "daslang")
    text_edit_set_cursor(das_ed, 0, 1)
    var inscope stmt_ctx <- creation_context(das_ed)
    t |> success(stmt_ctx.at_statement_start,
        "caret after the indent is a statement start")
    text_edit_set_cursor(das_ed, 0, 15)
    var inscope comment_ctx <- creation_context(das_ed)
    t |> success(comment_ctx.in_comment, "line-comment run detected at the caret")
    text_edit_dispose(ed)
    text_edit_dispose(das_ed)
}

[test]
def test_create_heading_cycle(t : T?) {
    var ed = md_editor("plain line")
    text_edit_set_cursor(ed, 0, 3)
    let depth = length(ed.undo_stack)
    t |> success(creation_execute(ed, "md.heading"), "cycle from plain")
    t |> equal(ed.lines[0], "# plain line", "level 1 first")
    t |> equal(length(ed.undo_stack), depth + 1, "one undo unit per transform")
    t |> success(creation_execute(ed, "md.heading"), "cycle again")
    t |> equal(ed.lines[0], "## plain line", "level 2 next")
    let undone = text_edit_undo(ed)
    t |> success(undone, "undo accepted")
    t |> equal(ed.lines[0], "# plain line", "one undo steps one level")
    var six = md_editor("###### deep")
    text_edit_set_cursor(six, 0, 0)
    t |> success(creation_execute(six, "md.heading"), "cycle past six")
    t |> equal(six.lines[0], "deep", "level 6 wraps around to plain")
    text_edit_dispose(ed)
    text_edit_dispose(six)
}

[test]
def test_create_list_and_checkbox_transforms(t : T?) {
    var ed = md_editor("alpha\nbeta\ngamma")
    text_edit_select(ed, 0, 0, 2, 5)
    t |> success(creation_execute(ed, "md.ordered_list"), "ordered from plain")
    t |> equal(ed.lines[0], "1. alpha", "renumber starts at one")
    t |> equal(ed.lines[1], "2. beta", "renumber counts down the block")
    t |> equal(ed.lines[2], "3. gamma", "renumber covers the selection")
    let sel = text_edit_selection(ed)
    t |> equal(sel.start_line, 0, "selection re-covers the block")
    t |> equal(sel.end_line, 2, "selection end at the block's last line")
    t |> success(creation_execute(ed, "md.list"), "plain list over ordered")
    t |> equal(ed.lines[1], "- beta", "ordered markers swap to dashes")
    t |> success(creation_execute(ed, "md.checkbox"), "checkbox over list")
    t |> equal(ed.lines[0], "- [ ] alpha", "unchecked box added")
    t |> success(creation_execute(ed, "md.checkbox"), "checkbox flips")
    t |> equal(ed.lines[0], "- [x] alpha", "the first line's state decides")
    text_edit_dispose(ed)
}

[test]
def test_create_quote_and_noop(t : T?) {
    var ed = md_editor("word")
    text_edit_set_cursor(ed, 0, 2)
    t |> success(creation_execute(ed, "md.quote"), "quote on")
    t |> equal(ed.lines[0], "> word", "prefix added")
    t |> equal(ed.cursor_column, 4, "caret carried by the prefix delta")
    t |> success(creation_execute(ed, "md.quote"), "quote off")
    t |> equal(ed.lines[0], "word", "prefix removed")
    text_edit_dispose(ed)
}

[test]
def test_create_wrap_degenerate_and_snippet(t : T?) {
    var ed = md_editor("")
    t |> success(creation_execute(ed, "md.bold"), "wrap without a selection")
    t |> equal(text_edit_text(ed), "****", "delimiters inserted")
    t |> equal(ed.cursor_column, 2, "caret lands between the delimiters")
    text_edit_load(ed, "")
    text_edit_set_language(ed, "markdown")
    t |> success(creation_execute(ed, "md.link"), "link snippet")
    t |> equal(text_edit_text(ed), "[](url)", "template body inserted")
    t |> success(text_edit_snippet_active(ed), "snippet session armed")
    text_edit_dispose(ed)
}

[test]
def test_create_das_statement_gate(t : T?) {
    var ed = TextEditState()
    text_edit_load(ed, "\tlet x = 1")
    text_edit_set_language(ed, "daslang")
    text_edit_set_cursor(ed, 0, 10)
    t |> success(!creation_execute(ed, "das.if"),
        "block constructs decline mid-expression")
    text_edit_set_cursor(ed, 0, 1)
    t |> success(creation_execute(ed, "das.if"), "statement start admits")
    t |> equal(ed.lines[0], "\tif (condition) \{", "head line lands")
    t |> equal(ed.lines[1], "\t\t", "body line inherits the indent")
    t |> equal(ed.lines[2], "\t\}let x = 1",
        "the caret's tail rides after the inserted body")
    t |> success(text_edit_snippet_active(ed), "condition stop armed")
    text_edit_dispose(ed)
}
