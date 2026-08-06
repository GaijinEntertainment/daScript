options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_markdown_hybrid
require imgui/imgui_markdown_model

//! Pure-logic coverage for the live-preview segmentation: no UI, no live host.

def private segments_for(t : T?; source : string; caret_line : int;
                         var segments : array<HybridSegment>) : int {
    var doc <- md_parse(source)
    t |> success(doc.ok, "fixture parses")
    var inscope starts : array<int>
    markdown_hybrid_line_starts(doc.source, starts)
    let island = markdown_hybrid_compute_segments(doc, starts, caret_line,
        length(starts), segments)
    delete doc
    return island
}

def private island_lines(t : T?; source : string; caret_line : int) : int2 {
    var inscope segments : array<HybridSegment>
    let island = segments_for(t, source, caret_line, segments)
    t |> success(island >= 0 && island < length(segments), "island index valid")
    let seg = segments[island]
    t |> success(seg.kind == HybridSegKind.source_, "island segment is source")
    t |> success(caret_line >= seg.line_first && caret_line <= seg.line_last,
        "island holds the caret line")
    return int2(seg.line_first, seg.line_last)
}

[test]
def test_hybrid_line_starts(t : T?) {
    var inscope starts : array<int>
    markdown_hybrid_line_starts("ab\ncd\n", starts)
    t |> equal(length(starts), 3, "two newlines make three lines")
    t |> equal(starts[0], 0, "first line at zero")
    t |> equal(starts[1], 3, "second line after ab\\n")
    t |> equal(starts[2], 6, "trailing empty line at the end")
    markdown_hybrid_line_starts("", starts)
    t |> equal(length(starts), 1, "empty source is one line")
}

[test]
def test_hybrid_two_paragraphs(t : T?) {
    let source = "alpha one\n\nbeta two\n"
    let first = island_lines(t, source, 0)
    t |> equal(first.x, 0, "first paragraph island starts at its line")
    t |> equal(first.y, 1, "separator blank attaches to the PRECEDING block")
    let on_blank = island_lines(t, source, 1)
    t |> equal(on_blank.x, 0, "caret on the blank line unfolds the preceding block")
    let second = island_lines(t, source, 2)
    t |> equal(second.x, 2, "second paragraph island starts at its line")
    t |> equal(second.y, 3, "trailing lines attach to the last block")
}

[test]
def test_hybrid_segment_order(t : T?) {
    var inscope segments : array<HybridSegment>
    let source = "# Title\n\n```das\nlet x = 1\n```\n\ntail\n"
    let island = segments_for(t, source, 3, segments)
    t |> equal(length(segments), 3, "rich run + island + rich run")
    t |> equal(island, 1, "island sits between the rich runs")
    t |> success(segments[0].kind == HybridSegKind.rich, "leading run is rich")
    t |> equal(segments[0].node_first, 0, "leading run starts at ordinal 0")
    t |> equal(segments[0].node_last, 0, "leading run is the heading only")
    t |> equal(segments[0].line_first, 0, "leading run starts at line 0")
    t |> equal(segments[0].line_last, 1, "leading run ends before the fence")
    t |> success(segments[2].kind == HybridSegKind.rich, "trailing run is rich")
    t |> equal(segments[2].node_first, 2, "trailing run resumes after the code")
    t |> equal(segments[2].line_first, 6, "trailing run starts at the tail")
    t |> success(segments[1].exclude_node >= 0, "island names its excluded node")
    t |> equal(segments[1].top_node, segments[1].exclude_node,
        "top-level code block excludes itself")
}

[test]
def test_hybrid_fenced_code_covers_fences(t : T?) {
    let source = "# Title\n\n```das\nlet x = 1\n```\n\ntail\n"
    let inside = island_lines(t, source, 3)
    t |> equal(inside.x, 2, "island includes the OPENING fence line")
    t |> equal(inside.y, 5, "island includes the closing fence + blank")
    let on_close = island_lines(t, source, 4)
    t |> equal(on_close.x, 2, "caret on the closing fence keeps one island")
}

[test]
def test_hybrid_fence_at_eof_without_closer(t : T?) {
    let span = island_lines(t, "```das\nlet y = 2\n", 1)
    t |> equal(span.x, 0, "unclosed fence island starts at the fence")
    t |> equal(span.y, 2, "unclosed fence island runs to the end")
}

[test]
def test_hybrid_thematic_breaks(t : T?) {
    let source = "para\n\n---\n\n***\n\nend\n"
    let first_break = island_lines(t, source, 2)
    t |> equal(first_break.x, 2, "first break owns its marker line")
    t |> equal(first_break.y, 3, "first break takes the following blank")
    let second_break = island_lines(t, source, 4)
    t |> equal(second_break.x, 4, "second break resolves past the first")
    let tail = island_lines(t, source, 6)
    t |> equal(tail.x, 6, "the end paragraph is its own island")
}

[test]
def test_hybrid_setext_underline(t : T?) {
    let span = island_lines(t, "Title\n=====\n\nbody\n", 1)
    t |> equal(span.x, 0, "setext underline belongs to the heading block")
    t |> equal(span.y, 2, "heading cover reaches the body's start")
}

[test]
def test_hybrid_nested_list_items(t : T?) {
    let source = "- alpha\n- beta\n  - inner one\n  - inner two\n- gamma\n"
    let alpha = island_lines(t, source, 0)
    t |> equal(alpha.x, 0, "first item unfolds alone")
    t |> equal(alpha.y, 0, "first item island is one line")
    let inner = island_lines(t, source, 2)
    t |> equal(inner.x, 2, "nested item unfolds at the INNER level")
    t |> equal(inner.y, 2, "nested item island is one line")
    let head = island_lines(t, source, 1)
    t |> equal(head.x, 1, "caret on the parent item's head takes the item")
    t |> equal(head.y, 3, "parent item island spans its nested subtree")
}

[test]
def test_hybrid_item_with_code_block(t : T?) {
    let source = "- head\n  ```\n  code\n  ```\n- next\n"
    let span = island_lines(t, source, 2)
    t |> equal(span.x, 1, "item's code block unfolds without the marker line")
    t |> equal(span.y, 3, "code island covers both fences")
    let head = island_lines(t, source, 0)
    t |> equal(head.x, 0, "caret on the item head unfolds the whole item")
    t |> equal(head.y, 3, "head unfold spans the item's block children")
}

[test]
def test_hybrid_quote_inner_paragraph(t : T?) {
    var inscope segments : array<HybridSegment>
    let source = "> first para\n>\n> second para\n\ntail\n"
    let island = segments_for(t, source, 0, segments)
    let seg = segments[island]
    t |> equal(seg.line_first, 0, "quote unfolds per inner paragraph")
    t |> equal(seg.line_last, 1, "first inner paragraph island")
    t |> success(seg.exclude_node != seg.top_node,
        "the excluded node is INSIDE the quote, not the quote")
    let second = island_lines(t, source, 2)
    t |> equal(second.x, 2, "second inner paragraph unfolds alone")
}

[test]
def test_hybrid_edges(t : T?) {
    var inscope segments : array<HybridSegment>
    let island = segments_for(t, "", 0, segments)
    t |> equal(length(segments), 1, "empty document is one source segment")
    t |> equal(island, 0, "empty document island index")
    let leading = island_lines(t, "\n\npara\n", 0)
    t |> equal(leading.x, 0, "leading blanks attach to the first block")
    t |> equal(leading.y, 3, "single block owns the whole buffer")
}
