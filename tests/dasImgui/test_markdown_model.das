options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_markdown_model public
require daslib/stringify
require strings

def private first_kind(doc : RichDocument const; kind : RichNodeKind) : int {
    for (i in range(length(doc.nodes))) {
        if (doc.nodes[i].kind == kind) return i
    }
    return -1
}

def private first_text(doc : RichDocument const; value : string) : int {
    for (i in range(length(doc.nodes))) {
        let node = doc.nodes[i]
        if (node.source_exact && md_node_text(doc, i) == value) return i
    }
    return -1
}

def private first_destination(doc : RichDocument const; value : string) : int {
    for (i in range(length(doc.nodes))) {
        if (doc.nodes[i].destination == value) return i
    }
    return -1
}

def private count_kind(doc : RichDocument const; kind : RichNodeKind) : int {
    var count = 0
    for (node in doc.nodes) {
        if (node.kind == kind) count++
    }
    return count
}

[test]
def test_non_markdown_producer_uses_shared_rich_document(t : T?) {
    let source = "Literal **markers** from another format."
    var document <- rich_plain_document(source)
    t |> success(document.ok, "plain producer creates a valid shared rich document")
    t |> equal(document.nodes[document.root].kind, RichNodeKind.document,
               "non-Markdown producer feeds the same block tree")
    let paragraph = document.nodes[document.root].first_child
    let content <- md_inline_content(document, paragraph)
    t |> equal(content.text, source,
               "shared inline/layout input retains producer display text")
    var styled = false
    for (run in content.runs) {
        styled = styled || run.is_strong || run.is_emphasis || run.is_code
    }
    t |> success(!styled,
                 "Markdown-looking bytes gain no semantics without the Markdown adapter")
    rich_document_dispose(document)
}

[test]
def test_tight_list_direct_inline_content(t : T?) {
    let source = "- **Parent** item\n  - nested item\n- second item\n"
    let doc <- md_parse(source)
    t |> success(doc.ok, "tight nested list parses")
    if (!doc.ok) return

    let list = first_kind(doc, RichNodeKind.unordered_list)
    t |> success(list >= 0, "tight list is present")
    if (list < 0) return
    let item = doc.nodes[list].first_child
    t |> success(item >= 0, "tight list has a first item")
    if (item < 0) return

    let content <- md_inline_content_direct_children(doc, item)
    t |> equal(content.text, "Parent item",
               "direct inline content retains text and excludes the nested list")
    t |> success(length(content.runs) > 1,
                 "direct inline content retains strong and regular runs")
}

[test]
def test_markdown_gfm_model(t : T?) {
    let source = %stringify~# Unicode Привет 文档

Text with *emphasis*, **strong**, [named](https://example.com/a?x=1&amp;y=2 "Example &copy;"), https://dascript.org and &amp; &copy; &#x1F680;.

- [x] done
- [ ] todo

| Left | Center | Right |
| :--- | :----: | ----: |
| α | β | γ |

```das
print("hi")
```

<div>raw</div>
%%

    let doc <- md_parse(source)
    t |> success(doc.ok, "GFM document parses: {doc.error}")
    if (!doc.ok) return

    t |> success(doc.root >= 0, "document has a root")
    t |> equal(doc.nodes[doc.root].kind, RichNodeKind.document,
               "root is the Markdown document")
    t |> success(doc.nodes[doc.root].first_child >= 0,
                 "document root owns block children")
    t |> success(!doc.nodes[doc.root].source_exact,
                 "container ranges are explicitly derived, not exact")

    let unicode_text = first_text(doc, "Unicode Привет 文档")
    t |> success(unicode_text >= 0, "Unicode heading text survives unchanged")
    if (unicode_text >= 0) {
        let node = doc.nodes[unicode_text]
        t |> success(node.source_exact, "Unicode leaf has exact UTF-8 byte range")
        t |> equal(md_node_text(doc, unicode_text), "Unicode Привет 文档",
                   "exact range slices the original UTF-8 source")
    }

    t |> success(first_kind(doc, RichNodeKind.emphasis) >= 0, "emphasis is typed")
    t |> success(first_kind(doc, RichNodeKind.strong) >= 0, "strong text is typed")
    t |> success(first_kind(doc, RichNodeKind.entity) >= 0, "entity is typed")
    let entity = first_kind(doc, RichNodeKind.entity)
    if (entity >= 0) {
        t |> equal(md_node_text(doc, entity), "&amp;", "entity keeps exact source bytes")
        t |> equal(md_node_display_text(doc, entity), "&", "entity exposes decoded display text")
    }
    let paragraph = first_kind(doc, RichNodeKind.paragraph)
    let inline <- md_inline_content(doc, paragraph)
    t |> success(inline.text |> find("and & © 🚀.") >= 0,
                 "inline content decodes named and numeric entities")
    t |> success(length(inline.runs) > 1, "inline content preserves semantic runs")
    var has_strong_run = false
    var has_link_run = false
    for (run in inline.runs) {
        has_strong_run = has_strong_run || run.is_strong
        has_link_run = has_link_run || (run.is_link && run.semantic_node >= 0)
    }
    t |> success(has_strong_run, "inline runs retain inherited strong style")
    t |> success(has_link_run, "inline runs retain link semantics")

    let named_link = first_destination(doc, "https://example.com/a?x=1&y=2")
    t |> success(named_link >= 0, "named link destination is retained")
    if (named_link >= 0) {
        t |> equal(doc.nodes[named_link].title, "Example ©",
                   "link attributes decode entities")
        t |> success(!doc.nodes[named_link].is_autolink, "named link is not autolink")
    }
    let auto_link = first_destination(doc, "https://dascript.org")
    t |> success(auto_link >= 0, "bare URL becomes a link")
    if (auto_link >= 0) {
        t |> success(doc.nodes[auto_link].is_autolink, "bare URL is marked autolink")
    }

    var checked = 0
    var unchecked = 0
    for (node in doc.nodes) {
        if (node.kind == RichNodeKind.list_item && node.is_task) {
            if (node.is_checked) checked++
            else unchecked++
            t |> success(node.source.start_byte >= 0,
                         "task item retains its checkbox marker offset")
            t |> equal(node.control_source.end_byte - node.control_source.start_byte, 1,
                       "task control retains an exact one-byte marker range")
            let marker = slice(doc.source, node.control_source.start_byte,
                               node.control_source.end_byte)
            t |> success(marker == " " || marker == "x",
                         "task control range is not expanded to list contents")
        }
    }
    t |> equal(checked, 1, "checked task metadata")
    t |> equal(unchecked, 1, "unchecked task metadata")

    let table_index = first_kind(doc, RichNodeKind.table_)
    t |> success(table_index >= 0, "GFM table is typed")
    if (table_index >= 0) {
        t |> equal(doc.nodes[table_index].column_count, 3, "table column count")
        t |> equal(doc.nodes[table_index].head_row_count, 1, "table head row count")
        t |> equal(doc.nodes[table_index].body_row_count, 1, "table body row count")
    }
    var left_cells = 0
    var center_cells = 0
    var right_cells = 0
    for (node in doc.nodes) {
        if (node.kind == RichNodeKind.table_header_cell || node.kind == RichNodeKind.table_cell) {
            if (node.alignment == RichAlignment.left) left_cells++
            elif (node.alignment == RichAlignment.center) center_cells++
            elif (node.alignment == RichAlignment.right) right_cells++
        }
    }
    t |> success(left_cells > 0 && center_cells > 0 && right_cells > 0,
                 "table cell alignment metadata")

    let code = first_kind(doc, RichNodeKind.code_block)
    t |> success(code >= 0, "fenced code block is typed")
    if (code >= 0) {
        t |> equal(doc.nodes[code].info, "das", "code info string")
        t |> equal(doc.nodes[code].language, "das", "code language")
    }

    t |> equal(count_kind(doc, RichNodeKind.raw_html), 0,
               "raw HTML blocks are disabled by default")
    t |> equal(count_kind(doc, RichNodeKind.html_text), 0,
               "raw inline HTML is disabled by default")
}

[test]
def test_markdown_checkbox_projects_to_revision_checked_source_edit(t : T?) {
    let source = "- [ ] todo\r\n- [X] done\n"
    let doc <- md_parse(source)
    t |> success(doc.ok, "checkbox edit fixture parses")
    if (!doc.ok) return
    var projection <- md_rich_projection(doc, 11)
    t |> equal(length(projection.controls), 2,
               "Markdown task items become neutral toggle controls")
    if (length(projection.controls) != 2) return
    let todo = projection.controls[0]
    t |> equal(todo.source_mapping, RichSourceMapping.exact,
               "toggle exposes an exact source mapping")
    t |> equal(slice(source, todo.source.start_byte, todo.source.end_byte), " ",
               "unchecked marker source is exact")

    let event = RichControlEvent(active = true, control_id = todo.id,
        kind = RichControlKind.toggle, requested_checked = true,
        source_revision = 11)
    let action <- md_control_action(doc, projection, event)
    t |> success(action.valid, "current control event produces an edit")
    t |> equal(action.edit.replacement, "x", "toggle edit uses canonical lowercase x")
    t |> equal(action.edit.expected, " ", "toggle edit guards the original byte")

    var stale_event := event
    stale_event.source_revision = 10
    let stale <- md_control_action(doc, projection, stale_event)
    t |> equal(stale.status, TextDocumentStatus.stale_revision,
               "stale control event is rejected before source mutation")
    rich_document_projection_dispose(projection)
}

[test]
def test_markdown_options(t : T?) {
    let source = "~~old~~\n\n| A |\n|---|\n| B |\n"
    let plain <- md_parse(source, MdParseOptions(
        tables = false,
        task_lists = false,
        strikethrough = false,
        permissive_autolinks = false))
    t |> success(plain.ok, "restricted Markdown options still parse")
    t |> equal(count_kind(plain, RichNodeKind.strikethrough), 0,
               "strikethrough extension can be disabled")
    t |> equal(count_kind(plain, RichNodeKind.table_), 0,
               "table extension can be disabled")
}

[test]
def test_markdown_rejects_invalid_utf8(t : T?) {
    let invalid = string([uint8(0xC3), uint8(0x28)])
    let doc <- md_parse(invalid)
    t |> success(!doc.ok, "invalid UTF-8 is rejected before native parsing")
    t |> equal(doc.error, "Markdown source is not valid UTF-8",
               "invalid UTF-8 reports a stable diagnostic")
}

[test]
def test_markdown_plain_unicode_uris(t : T?) {
    let source = "Plain https://example.com/文档/naïve/é?q=👩‍💻 and editor://workspace/文档."
    let doc <- md_parse(source)
    t |> success(doc.ok, "plain Unicode URI document parses")
    let paragraph = first_kind(doc, RichNodeKind.paragraph)
    let inline <- md_inline_content(doc, paragraph)
    var automatic = 0
    for (run in inline.runs) {
        if (run.is_automatic_uri) {
            automatic++
            t |> success(run.semantic_node < 0,
                         "plain URI is semantic without fabricating Markdown node")
            t |> equal(slice(doc.source, run.source.start_byte, run.source.end_byte),
                       run.destination, "plain URI target maps to exact source")
        }
    }
    t |> equal(automatic, 2,
               "shared detector supplements Markdown parser for plain Unicode/custom URIs")
}

[test]
def test_markdown_image_is_atomic_with_plain_text_fallback(t : T?) {
    let source = "Before ![Unicode terminal preview](editor://assets/terminal.png \"Terminal preview\") after."
    let doc <- md_parse(source)
    t |> success(doc.ok, "image fixture parses")
    if (!doc.ok) return
    let image = first_kind(doc, RichNodeKind.image)
    t |> success(image >= 0, "image is a typed Markdown node")
    if (image < 0) return
    t |> equal(doc.nodes[image].destination, "editor://assets/terminal.png",
               "image destination is retained")
    t |> equal(doc.nodes[image].title, "Terminal preview",
               "image title is retained")

    let paragraph = first_kind(doc, RichNodeKind.paragraph)
    let inline <- md_inline_content(doc, paragraph)
    var image_runs = 0
    var image_run = RichInlineRun()
    for (run in inline.runs) {
        if (run.is_image) {
            image_runs++
            image_run = run
        }
    }
    t |> equal(image_runs, 1, "image alt subtree becomes one atomic inline run")
    if (image_runs != 1) return
    t |> equal(slice(inline.text, image_run.display.start_byte, image_run.display.end_byte),
               "Unicode terminal preview", "image displays its alt fallback")
    t |> equal(image_run.image_alt, "Unicode terminal preview",
               "accessible image alt is retained separately")
    t |> success(image_run.is_link && image_run.semantic_node == image,
                 "image retains URI activation semantics")
    t |> success(!image_run.source_exact,
                 "image never claims an unavailable full Markdown syntax range")

    var styles = MdSelectionStyleAccumulator()
    md_selection_include(styles, inline, image_run.source)
    let flags = md_selection_styles(styles)
    t |> success(flags.has_images && flags.has_links,
                 "image selection reports both image and link semantics")
}

[test]
def test_markdown_selection_source_and_styles(t : T?) {
    let source = "plain **bold** and *italic*, `code`, ~~gone~~, with [link](editor://item)."
    let doc <- md_parse(source)
    t |> success(doc.ok, "selection fixture parses")
    if (!doc.ok) return
    let paragraph = first_kind(doc, RichNodeKind.paragraph)
    let inline <- md_inline_content(doc, paragraph)

    let bold_display = inline.text |> find("bold")
    let bold_anchor = md_inline_position(inline, paragraph, bold_display, false)
    let bold_focus = md_inline_position(inline, paragraph, bold_display + 4, true)
    let bold_source = md_text_selection_range(bold_anchor, bold_focus)
    t |> equal(md_source_text(doc, bold_source), "bold",
               "display selection maps to original Markdown content bytes")

    var bold_styles = MdSelectionStyleAccumulator()
    md_selection_include(bold_styles, inline, bold_source)
    let bold_flags = md_selection_styles(bold_styles)
    t |> success(bold_flags.has_strong && !bold_flags.all_regular,
                 "strong-only selection reports semantic style")
    t |> success(!bold_flags.has_mixed_styles,
                 "one style combination is not mixed")

    let regular_start = inline.text |> find("plain")
    let regular_anchor = md_inline_position(inline, paragraph, regular_start, false)
    let regular_focus = md_inline_position(inline, paragraph, regular_start + 5, true)
    var regular_styles = MdSelectionStyleAccumulator()
    md_selection_include(regular_styles, inline,
        md_text_selection_range(regular_anchor, regular_focus))
    t |> success(md_selection_styles(regular_styles).all_regular,
                 "unstyled selection reports all_regular")

    let all_anchor = md_inline_position(inline, paragraph, 0, false)
    let all_focus = md_inline_position(inline, paragraph, length(inline.text), true)
    var all_styles = MdSelectionStyleAccumulator()
    md_selection_include(all_styles, inline,
        md_text_selection_range(all_anchor, all_focus))
    let all_flags = md_selection_styles(all_styles)
    t |> success(all_flags.has_strong && all_flags.has_emphasis
              && all_flags.has_code && all_flags.has_strikethrough
              && all_flags.has_links,
                 "cross-style selection reports every semantic style")
    t |> success(all_flags.has_mixed_styles,
                 "cross-style selection reports mixed combinations")
}

[test]
def test_markdown_selection_rich_export(t : T?) {
    let source = "**bold** and [link](editor://item) plus ![alt](img://preview \"T\") &amp;"
    let doc <- md_parse(source)
    t |> success(doc.ok, "rich-selection fixture parses")
    if (!doc.ok) return
    let paragraph = first_kind(doc, RichNodeKind.paragraph)
    let inline <- md_inline_content(doc, paragraph)
    var exported = MdSelectionExport()
    md_selection_export_include(exported, inline,
        TextSourceRange(start_byte = 0, end_byte = length(source)))

    t |> equal(exported.plain_text, "bold and link plus alt &",
               "plain export follows transformed display text")
    t |> equal(exported.markdown,
               "**bold** and [link](editor://item) plus ![alt](img://preview \"T\") &",
               "Markdown export reconstructs standalone styled syntax")
    t |> equal(exported.html,
               "<strong>bold</strong> and <a href=\"editor://item\">link</a> plus "
               + "<img src=\"img://preview\" alt=\"alt\" title=\"T\"> &amp;",
               "HTML export preserves styles, links, images, and escaping")
    let visible_uri_list = exported.uri_list |> replace("\r", "<CR>") |> replace("\n", "<LF>")
    t |> equal(visible_uri_list,
               "editor://item<CR><LF>img://preview<CR><LF>",
               "URI-list export includes link and image destinations once")
}
