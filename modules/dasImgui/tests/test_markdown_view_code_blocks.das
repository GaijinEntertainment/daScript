options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_markdown_view public

def private code_block_nodes(doc : RichDocument const) : array<int> {
    var nodes <- [for (index in range(length(doc.nodes))); index;
        where doc.nodes[index].kind == RichNodeKind.code_block]
    return <- nodes
}

[test]
def test_markdown_code_blocks_before_first_layout(t : T?) {
    // The node cache is sized on the first render. A document parsed but never
    // drawn (plain-text mode, or the frame before the first markdown pass) has
    // an empty cache while the model already carries every code-block node.
    let doc <- md_parse("intro\n\n```das\nlet x = 1\n```\n")
    var state = MarkdownViewState()
    t |> equal(length(code_block_nodes(doc)), 1, "the document has one code block")
    t |> equal(length(state.nodes), 0, "an undrawn view has no node cache")

    let blocks <- markdown_view_code_blocks(doc, state)
    t |> equal(length(blocks), 1, "the code block is reported without a layout")
    t |> success(!blocks[0].laid_out, "an undrawn block is reported unlaid-out")
    t |> success(!blocks[0].icons_valid,
                 "an unlaid-out block reports no hover-icon rects")
    t |> equal(blocks[0].screen_origin.x, 0.0f,
               "an unlaid-out block reports zero geometry")
}

[test]
def test_markdown_code_blocks_shorter_cache(t : T?) {
    // A reparse can grow the document past the retained cache: markdown_view_-
    // invalidate keeps the outer capacity for a same-sized replacement, so the
    // node count and the cache length disagree until the next render.
    let doc <- md_parse("```das\nlet x = 1\n```\n\ntext\n\n```das\nlet y = 2\n```\n")
    var state = MarkdownViewState()
    state.nodes |> resize(2)
    let expected <- code_block_nodes(doc)
    t |> equal(length(expected), 2, "the document has two code blocks")
    t |> success(expected[1] >= length(state.nodes), "the document outgrew the cache")

    let blocks <- markdown_view_code_blocks(doc, state)
    t |> equal(length(blocks), 2, "both code blocks survive a short cache")
    t |> equal(blocks[0].node, expected[0], "the first block keeps its node index")
    t |> equal(blocks[1].node, expected[1], "the second block keeps its node index")
    t |> success(!blocks[1].laid_out, "the block past the cache is unlaid-out")
}

[test]
def test_markdown_code_blocks_report_layout(t : T?) {
    let doc <- md_parse("```das\nlet x = 1\n```\n")
    var state = MarkdownViewState()
    state.nodes |> resize(length(doc.nodes))
    let node = code_block_nodes(doc)[0]
    var entry & = unsafe(state.nodes[node])
    entry.icon_rects_valid = true
    entry.screen_origin = float2(12.0f, 34.0f)
    entry.fold_icon_rect = float4(10.0f, 20.0f, 30.0f, 40.0f)
    entry.copy_icon_rect = float4(50.0f, 60.0f, 70.0f, 80.0f)
    t |> success(markdown_view_set_code_collapsed(state, node, true),
                 "the block collapses")

    let blocks <- markdown_view_code_blocks(doc, state)
    t |> equal(length(blocks), 1, "one code block")
    t |> success(blocks[0].laid_out, "a cached block is reported laid out")
    t |> success(blocks[0].collapsed, "collapsed state is reported")
    t |> success(blocks[0].icons_valid, "laid-out icons are reported")
    t |> equal(blocks[0].screen_origin.y, 34.0f, "screen origin is reported")
    t |> equal(blocks[0].fold_icon_center.x, 20.0f,
               "fold icon center is the rect midpoint")
    t |> equal(blocks[0].copy_icon_center.y, 70.0f,
               "copy icon center is the rect midpoint")
}
