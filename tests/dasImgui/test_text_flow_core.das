options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_flow public
require strings

//! Headless contract tests for the shared Markdown/editor/terminal text layer.
//! These intentionally do not create an ImGui context: layout consumes a
//! measurement callback and emits source-mapped fragments.

[test]
def test_text_flow_graphemes(t : T?) {
    // e + COMBINING ACUTE, woman-technologist ZWJ sequence, and a two-RI flag
    // are each one caret/wrap unit despite occupying multiple UTF-8 scalars.
    let source = "Aé👩‍💻🇺🇸B"
    let g <- text_graphemes(source)
    t |> equal(length(g), 5, "extended UI grapheme grouping")
    t |> equal(text_next_grapheme_boundary(source, g[1].source.start_byte),
               g[1].source.end_byte,
               "right movement crosses the complete grapheme")
    t |> equal(text_previous_grapheme_boundary(source, g[1].source.end_byte),
               g[1].source.start_byte,
               "left movement crosses the complete grapheme")
    if (length(g) != 5) return

    t |> equal(g[1].source.start_byte, 1, "combining cluster starts at e")
    t |> equal(g[1].source.end_byte, 4, "combining cluster includes U+0301")
    t |> equal(g[2].source.start_byte, 4, "ZWJ emoji starts after combining cluster")
    t |> equal(g[2].source.end_byte, 15, "ZWJ emoji is one 11-byte cluster")
    t |> equal(g[3].source.start_byte, 15, "flag starts after ZWJ emoji")
    t |> equal(g[3].source.end_byte, 23, "regional-indicator pair is one cluster")
}

[test]
def test_text_flow_terminal_cell_widths(t : T?) {
    t |> equal(text_cell_width("A"), 1, "ASCII occupies one source cell")
    t |> equal(text_cell_width("é"), 1,
               "combining grapheme occupies one source cell")
    t |> equal(text_cell_width("中文"), 4, "CJK graphemes occupy two cells")
    t |> equal(text_cell_width("👩‍💻"), 2,
               "ZWJ color emoji occupies two cells as one grapheme")
    t |> equal(text_cell_width("🇺🇸"), 2,
               "regional-indicator flag occupies two cells")
    t |> equal(text_cell_width("©️"), 2,
               "variation-selector emoji presentation occupies two cells")
}

[test]
def test_text_flow_empty_source_lines_keep_byte_positions(t : T?) {
    let source = "a\n\nb\n"
    let spans = [TextFlowSpan(
        source = TextSourceRange(start_byte = 0, end_byte = length(source)),
        style_id = 0)]
    let layout <- layout_text_flow_styled_ranges(source, spans, 100.0f, 10.0f,
        $(_text : string; _start_byte, _end_byte, _style_id : int) : float {
            return 8.0f
        })
    t |> equal(length(layout.lines), 4,
               "two hard breaks plus terminal newline produce four source lines")
    if (length(layout.lines) != 4) return
    t |> equal(layout.lines[1].source.start_byte, 2,
               "middle empty line maps to its newline byte")
    t |> equal(layout.lines[3].source.start_byte, length(source),
               "final empty caret line maps to source end")
}

[test]
def test_text_flow_uri_fragments(t : T?) {
    let prefix = "Open "
    let uri = "https://example.com/文档/a-long-name?q=é&mode=full"
    let suffix = " safely."
    let source = prefix + uri + suffix
    let uri_start = length(prefix)
    let uri_end = uri_start + length(uri)

    let spans = [
        TextFlowSpan(
            source = TextSourceRange(start_byte = 0, end_byte = uri_start),
            style_id = 0),
        TextFlowSpan(
            source = TextSourceRange(start_byte = uri_start, end_byte = uri_end),
            style_id = 1, semantic_id = 42, uri_wrap = true,
            trailing_control_id = 7,
            trailing_control_size = float2(16.0f, 16.0f)),
        TextFlowSpan(
            source = TextSourceRange(start_byte = uri_end, end_byte = length(source)),
            style_id = 0)
    ]

    // Deterministic fake font: every grapheme advances 8 px. It makes the core
    // test independent of the machine's atlas while exercising Unicode input.
    let layout <- layout_text_flow(source, spans, 96.0f, 18.0f,
        $(s : string) : float {
            return float(length(text_graphemes(s))) * 8.0f
        })

    t |> success(length(layout.lines) > 1, "long URI wraps to multiple lines")
    var control_count = 0
    var uri_fragment_count = 0
    var all_lines_fit = true
    for (line in layout.lines) {
        if (line.width > 96.01f) {
            all_lines_fit = false
        }
    }
    for (fragment in layout.fragments) {
        if (fragment.is_control) {
            control_count++
            t |> equal(fragment.control_id, 7, "reserved control preserves its identity")
            t |> equal(fragment.semantic_id, 42, "reserved control belongs to the URI")
            t |> equal(fragment.source.start_byte, uri_end, "control anchors after URI source")
            t |> equal(fragment.source.end_byte, uri_end, "control consumes no source bytes")
        } elif (fragment.semantic_id == 42) {
            uri_fragment_count++
            t |> success(fragment.source.start_byte >= uri_start
                       && fragment.source.end_byte <= uri_end,
                       "URI visual fragment maps into URI source")
        }
    }
    t |> success(all_lines_fit, "greedy wrapping respects available width")
    t |> success(uri_fragment_count > 1,
                 "one semantic URI produces multiple visual fragments")
    t |> equal(control_count, 1, "copy control reserves exactly one inline slot")
}

[test]
def test_text_flow_leading_and_trailing_controls(t : T?) {
    let source = "alt"
    let spans = [TextFlowSpan(
        source = TextSourceRange(start_byte = 0, end_byte = length(source)),
        style_id = 9, semantic_id = 23,
        leading_control_id = 23, leading_control_kind = 2,
        leading_control_size = float2(12.0f, 16.0f),
        trailing_control_id = 23, trailing_control_kind = 1,
        trailing_control_size = float2(10.0f, 16.0f))]
    let layout <- layout_text_flow(source, spans, 100.0f, 18.0f,
        $(s : string) : float {
            return float(length(text_graphemes(s))) * 8.0f
        })
    t |> equal(length(layout.fragments), 3,
               "leading control, text, and trailing control are distinct fragments")
    if (length(layout.fragments) != 3) return
    let leading = layout.fragments[0]
    let text_fragment = layout.fragments[1]
    let trailing = layout.fragments[2]
    t |> success(leading.is_control && leading.control_kind == 2,
                 "leading consumer-defined control kind is preserved")
    t |> equal(leading.source.start_byte, 0, "leading control anchors before text")
    t |> equal(leading.source.end_byte, 0, "leading control consumes no source bytes")
    t |> success(!text_fragment.is_control && text_fragment.source.end_byte == length(source),
                 "text remains source mapped between controls")
    t |> success(trailing.is_control && trailing.control_kind == 1,
                 "trailing consumer-defined control kind is preserved")
    t |> equal(trailing.source.start_byte, length(source),
               "trailing control anchors after text")

    let narrow <- layout_text_flow(source, spans, 16.0f, 18.0f,
        $(s : string) : float {
            return float(length(text_graphemes(s))) * 8.0f
        })
    t |> success(length(narrow.lines) > 1,
                 "narrow image fallback wraps remaining alt text")
    if (!empty(narrow.fragments)) {
        t |> success(narrow.fragments[0].is_control
                   && length(narrow.fragments) > 1
                   && narrow.fragments[1].line_index == narrow.fragments[0].line_index,
                     "leading placeholder never orphans from the first grapheme")
    }
}

[test]
def test_plain_text_uri_detection(t : T?) {
    let source = "See https://example.com/文档/naïve/é?q=👩‍💻, editor://workspace/文档 and WWW.Example.org/path. Note: ordinary text."
    let uris <- text_uris(source)
    t |> equal(length(uris), 3, "scheme, custom scheme, and www URI detection")
    if (length(uris) != 3) return
    t |> equal(slice(source, uris[0].source.start_byte, uris[0].source.end_byte),
               "https://example.com/文档/naïve/é?q=👩‍💻",
               "Unicode URI body and grapheme bytes are retained")
    t |> equal(slice(source, uris[1].source.start_byte, uris[1].source.end_byte),
               "editor://workspace/文档", "custom slash-form URI")
    t |> equal(slice(source, uris[2].source.start_byte, uris[2].source.end_byte),
               "WWW.Example.org/path", "www URI is case-insensitive")
    t |> equal(uris[2].kind, TextUriKind.www, "www target requests scheme prefix")
}

[test]
def test_text_flow_style_aware_measurement(t : T?) {
    let source = "regular WIDE"
    let spans = [
        TextFlowSpan(source = TextSourceRange(start_byte = 0, end_byte = 8),
                     style_id = 0),
        TextFlowSpan(source = TextSourceRange(start_byte = 8, end_byte = length(source)),
                     style_id = 7)
    ]
    var saw_regular = false
    var saw_wide = false
    let layout <- layout_text_flow_styled(source, spans, 80.0f, 18.0f,
        $(value : string; style_id : int) : float {
            if (style_id == 0) {
                saw_regular = true
            }
            if (style_id == 7) {
                saw_wide = true
            }
            return float(length(text_graphemes(value))) * (style_id == 7 ? 20.0f : 6.0f)
        })
    t |> success(saw_regular && saw_wide, "measurement receives each run's style id")
    var preserved_wide_style = false
    for (fragment in layout.fragments) {
        preserved_wide_style = preserved_wide_style || fragment.style_id == 7
    }
    t |> success(preserved_wide_style, "layout fragments retain measured style")
    t |> success(length(layout.lines) > 1,
                 "wider role metrics change wrapping rather than only rendering")
}

[test]
def test_text_flow_hit_test_grapheme_boundaries(t : T?) {
    let source = "AéB"
    let spans = [TextFlowSpan(
        source = TextSourceRange(start_byte = 0, end_byte = length(source)),
        style_id = 3)]
    let layout <- layout_text_flow_styled_ranges(source, spans, 100.0f, 18.0f,
        $(_text : string; _start_byte, _end_byte, _style_id : int) : float {
            return 10.0f
        })
    let before_cluster = text_flow_hit_test_ranges(source, layout, float2(12.0f, 9.0f),
        $(_text : string; _start_byte, _end_byte, _style_id : int) : float {
            return 10.0f
        })
    let after_cluster = text_flow_hit_test_ranges(source, layout, float2(16.0f, 9.0f),
        $(_text : string; _start_byte, _end_byte, _style_id : int) : float {
            return 10.0f
        })

    t |> success(before_cluster.valid && after_cluster.valid,
                 "points inside text produce carets")
    t |> equal(before_cluster.byte_offset, 1,
               "left half snaps before the combining grapheme")
    t |> equal(after_cluster.byte_offset, 4,
               "right half snaps after the complete combining grapheme")
    t |> success(!before_cluster.trailing && !after_cluster.trailing,
                 "interior boundary reports leading affinity for the next grapheme")
}
