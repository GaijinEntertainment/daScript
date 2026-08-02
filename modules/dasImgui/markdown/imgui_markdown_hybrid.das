options gen2
options _comment_hygiene = true
options indenting = 4
// custom-widget layer: raw child windows + drawlist composition by design
options _allow_imgui_legacy = true

module imgui_markdown_hybrid shared

require imgui public
require imgui/imgui_text_source_edit public
require imgui/imgui_markdown_view public
require imgui/imgui_markdown_tree_sitter public
require math
require strings

//! Live-preview compositor: ONE `TextEditState` buffer partitioned into an
//! ordered list of line segments — rich blocks drawn by the markdown view,
//! the caret block drawn as editable source in place (Obsidian-style).
//! Host contract: sole content of a scrolling child window (no horizontal
//! scrollbar there — the source island scrolls itself); `doc` parsed from the
//! buffer's text with the host owning the reparse debounce.

enum public HybridSegKind {
    source_
    rich
}

struct public HybridSegment {
    kind : HybridSegKind = HybridSegKind.rich
    line_first : int = 0
    line_last : int = 0 // inclusive, parse-time line domain
    node_first : int = -1 // rich run: first top-level block ordinal
    node_last : int = -1 // rich run: last top-level block ordinal
    top_node : int = -1 // source island: top-level block node index
    exclude_node : int = -1 // source island: innermost unfolded node index
    screen_y0 : float = 0.0f // stamped at render (tests + caret follow)
    screen_y1 : float = 0.0f
}

struct public HybridEditState {
    segments : array<HybridSegment>
    segments_revision : int = 0
    seg_document_stamp : int = -1 // host parse stamp the segments were built for
    seg_edit_revision : int = -1 // ed.revision at build
    seg_caret_line : int = -1
    seg_line_count : int = 0 // parse-time line count -> live delta shifting
    source_segment : int = -1 // index into segments
    rich_click_revision : int = 0
    line_starts : array<int> // byte offset of each parse-time line start
}

struct public HybridEditResult {
    edit : imgui_text_source_edit::TextEditResult
    view : MarkdownViewResult = MarkdownViewResult()
    //! The caret left the island while edits were pending: the host must
    //! reparse THIS frame (bypassing the debounce) so the exited block snaps
    //! rich with fresh content next frame.
    reparse_needed : bool = false
}

def public markdown_hybrid_dispose(var hybrid : HybridEditState) {
    delete hybrid.segments
    delete hybrid.line_starts
    hybrid.source_segment = -1
    hybrid.seg_document_stamp = -1
    hybrid.seg_edit_revision = -1
    hybrid.seg_caret_line = -1
    hybrid.seg_line_count = 0
}

// ===== Segmentation (pure — no ImGui, model-testable) =====

def public markdown_hybrid_line_starts(source : string; var starts : array<int>) {
    //! Byte offset of every line start in `source` (LF domain).
    starts |> clear()
    starts |> push(0)
    peek_data(source) $(bytes) {
        for (index in range(length(bytes))) {
            if (int(bytes[index]) == '\n') {
                starts |> push(index + 1)
            }
        }
    }
}

def private line_of(line_starts : array<int> const; byte_offset : int) : int {
    // last line whose start <= byte_offset
    var lo = 0
    var hi = length(line_starts) - 1
    while (lo < hi) {
        let mid = (lo + hi + 1) / 2
        if (line_starts[mid] <= byte_offset) {
            lo = mid
        } else {
            hi = mid - 1
        }
    }
    return lo
}

def private line_is_blank(source : string; line_starts : array<int> const;
                          line : int) : bool {
    let start = line_starts[line]
    let end_ = (line + 1 < length(line_starts)
        ? line_starts[line + 1] - 1 : length(source))
    var blank = true
    peek_data(source) $(bytes) {
        for (index in range(start, min(end_, length(bytes)))) {
            let value = int(bytes[index])
            if (value != ' ' && value != '\t' && value != '\r') {
                blank = false
                break
            }
        }
    }
    return blank
}

struct private BlockLineSpan {
    node : int = -1
    first : int = -1
    last : int = -1
}

def private raw_line_span(doc : RichDocument const;
                          line_starts : array<int> const;
                          node_index : int) : int2 {
    // (-1,-1) = no usable source range (thematic_break, empty fence)
    let node = doc.nodes[node_index]
    return int2(-1, -1) if (node.source.start_byte < 0
        || node.source.end_byte <= node.source.start_byte)
    var first = line_of(line_starts, node.source.start_byte)
    let last = line_of(line_starts,
        max(node.source.end_byte - 1, node.source.start_byte))
    // a fenced code block's source is content-only: widen to the opening fence
    if (node.kind == RichNodeKind.code_block && node.marker != 0 && first > 0) {
        first--
    }
    return int2(first, last)
}

def private child_line_spans(doc : RichDocument const; source : string;
                             line_starts : array<int> const;
                             parent, from_line, to_line : int;
                             var spans : array<BlockLineSpan>) {
    // sourceless children (thematic_break) take the next non-blank line
    spans |> clear()
    var child = doc.nodes[parent].first_child
    var cursor = from_line
    while (child >= 0) {
        if (!md_node_kind_is_inline(doc.nodes[child].kind)) {
            let raw = raw_line_span(doc, line_starts, child)
            var first = raw.x
            var last = raw.y
            if (first < 0) {
                var probe = cursor
                while (probe < to_line
                        && line_is_blank(source, line_starts, probe)) {
                    probe++
                }
                first = min(probe, to_line)
                last = first
            }
            first = max(first, cursor)
            last = max(last, first)
            spans |> push(BlockLineSpan(node = child, first = first, last = last))
            cursor = last + 1
        }
        child = doc.nodes[child].next_sibling
    }
}

def private sweep_cover(var spans : array<BlockLineSpan>;
                        from_line, to_line : int) {
    // total partition of [from..to]; separator lines attach to the PRECEDING block
    let count = length(spans)
    var start_line = from_line
    for (index in range(count)) {
        let next_first = index + 1 < count ? spans[index + 1].first : to_line + 1
        spans[index].first = start_line
        spans[index].last = clamp(next_first - 1, start_line, to_line)
        start_line = spans[index].last + 1
    }
    if (count > 0) {
        spans[count - 1].last = to_line
    }
}

def private descend_block(doc : RichDocument const; source : string;
                          line_starts : array<int> const;
                          node, cover_first, cover_last, caret_line : int) : int3 {
    // innermost unfold: (exclude_node, island_first, island_last)
    let kind = doc.nodes[node].kind
    if (kind == RichNodeKind.quote || kind == RichNodeKind.unordered_list
            || kind == RichNodeKind.ordered_list) {
        var inscope spans : array<BlockLineSpan>
        child_line_spans(doc, source, line_starts, node,
            cover_first, cover_last, spans)
        sweep_cover(spans, cover_first, cover_last)
        for (span in spans) {
            if (caret_line >= span.first && caret_line <= span.last) {
                return descend_block(doc, source, line_starts, span.node,
                    span.first, span.last, caret_line)
            }
        }
        return int3(node, cover_first, cover_last)
    }
    if (kind == RichNodeKind.list_item) {
        // caret on the marker/inline-head lines unfolds the whole item
        var inscope spans : array<BlockLineSpan>
        child_line_spans(doc, source, line_starts, node,
            cover_first, cover_last, spans)
        if (!empty(spans) && caret_line >= spans[0].first) {
            sweep_cover(spans, spans[0].first, cover_last)
            for (span in spans) {
                if (caret_line >= span.first && caret_line <= span.last) {
                    return descend_block(doc, source, line_starts, span.node,
                        span.first, span.last, caret_line)
                }
            }
        }
        return int3(node, cover_first, cover_last)
    }
    return int3(node, cover_first, cover_last)
}

def public markdown_hybrid_compute_segments(doc : RichDocument const;
                                            line_starts : array<int> const;
                                            caret_line, line_count : int;
                                            var segments : array<HybridSegment>) : int {
    //! Pure segmentation: partition lines [0..line_count-1] into rich runs and
    //! ONE source island holding `caret_line`. Returns the island's index.
    segments |> clear()
    var inscope tops : array<BlockLineSpan>
    if (doc.root >= 0 && line_count > 0) {
        child_line_spans(doc, doc.source, line_starts, doc.root,
            0, line_count - 1, tops)
    }
    if (empty(tops)) {
        segments |> push(HybridSegment(kind = HybridSegKind.source_,
            line_first = 0, line_last = max(line_count - 1, 0)))
        return 0
    }
    sweep_cover(tops, 0, line_count - 1)
    var top_ordinal = length(tops) - 1
    for (index in range(length(tops))) {
        if (caret_line >= tops[index].first && caret_line <= tops[index].last) {
            top_ordinal = index
            break
        }
    }
    let top = tops[top_ordinal]
    let hit = descend_block(doc, doc.source, line_starts, top.node,
        top.first, top.last, clamp(caret_line, top.first, top.last))
    if (top_ordinal > 0) {
        segments |> push(HybridSegment(kind = HybridSegKind.rich,
            line_first = 0, line_last = top.first - 1,
            node_first = 0, node_last = top_ordinal - 1))
    }
    let island = length(segments)
    segments |> push(HybridSegment(kind = HybridSegKind.source_,
        line_first = hit.y, line_last = hit.z,
        top_node = top.node, exclude_node = hit.x))
    if (top_ordinal < length(tops) - 1) {
        segments |> push(HybridSegment(kind = HybridSegKind.rich,
            line_first = top.last + 1, line_last = line_count - 1,
            node_first = top_ordinal + 1, node_last = length(tops) - 1))
    }
    return island
}

// ===== The widget =====

def private hybrid_edit_line_height(edit_style : TextSourceViewStyle) : float {
    // mirror of the editor's own metrics so the island child fits exactly
    let zoom = clamp(edit_style.zoom, 0.5f, 3.0f)
    let font_size = (edit_style.fonts.size_px > 0.0f
        ? edit_style.fonts.size_px : GetFontSize()) * zoom
    return max(font_size,
        GetTextLineHeight() * font_size / max(GetFontSize(), 1.0f))
}

def private hybrid_sync_segments(var hybrid : HybridEditState;
                                 ed : TextEditState const;
                                 doc : RichDocument const;
                                 document_stamp : int;
                                 var result : HybridEditResult) {
    // a DIRTY caret exit keeps old segments one frame and demands the reparse
    let delta = length(ed.lines) - hybrid.seg_line_count
    var caret_inside = false
    let island = hybrid.source_segment
    if (island >= 0 && island < length(hybrid.segments)) {
        let seg & = unsafe(hybrid.segments[island])
        caret_inside = (ed.cursor_line >= seg.line_first
            && ed.cursor_line <= seg.line_last + delta)
    }
    let stamp_changed = hybrid.seg_document_stamp != document_stamp
    return if (!stamp_changed && caret_inside)
    if (!stamp_changed && ed.revision != hybrid.seg_edit_revision) {
        result.reparse_needed = true
        return
    }
    markdown_hybrid_line_starts(doc.source, hybrid.line_starts)
    let line_count = length(hybrid.line_starts)
    hybrid.source_segment = markdown_hybrid_compute_segments(doc,
        hybrid.line_starts, clamp(ed.cursor_line, 0, max(line_count - 1, 0)),
        line_count, hybrid.segments)
    hybrid.segments_revision++
    hybrid.seg_document_stamp = document_stamp
    hybrid.seg_edit_revision = ed.revision
    hybrid.seg_caret_line = ed.cursor_line
    hybrid.seg_line_count = line_count
}

def private hybrid_draw_island(var ed : TextEditState;
                               var edit_style : TextSourceViewStyle;
                               var registry : CommandRegistry;
                               seg : HybridSegment;
                               delta : int; line_height, view_height : float;
                               var result : HybridEditResult) : float {
    // returns the island child's screen top (caret-follow anchor)
    let line_last = min(seg.line_last + delta, length(ed.lines) - 1)
    let island_lines = max(line_last - seg.line_first + 1, 1)
    let island_h = (float(island_lines) * line_height
        + GetStyle().ScrollbarSize + GetStyle().WindowPadding.y)
    let top = GetCursorScreenPos().y
    PushStyleVar(ImGuiStyleVar.WindowPadding, float2(0.0f, 0.0f))
    let open = BeginChild("##hybrid_island", float2(0.0f, island_h),
        ImGuiChildFlags.None, ImGuiWindowFlags.HorizontalScrollbar)
    if (open) {
        let spec = TextEditSegmentSpec(line_first = seg.line_first,
            line_last = line_last, primary = true, advance_clocks = true,
            owns_scroll = false,
            follow_x = ed.follow_revision != ed.followed_revision,
            dead_area = false, register_alias = true,
            view_height = view_height)
        result.edit = text_source_edit_segment("editor", ed, edit_style,
            registry, spec)
    }
    EndChild()
    PopStyleVar()
    return top
}

def public markdown_hybrid_edit(id : string; var ed : TextEditState; // nolint:STYLE038
                                doc : RichDocument const;
                                var view : MarkdownViewState;
                                var provider : MarkdownTreeSitterState;
                                var hybrid : HybridEditState;
                                var edit_style : TextSourceViewStyle;
                                var md_style : MarkdownViewStyle;
                                var registry : CommandRegistry;
                                document_stamp : int) : HybridEditResult {
    //! Draw + drive the hybrid live preview for one frame. `doc` must be the
    //! parse of the buffer text at `document_stamp`; honor `reparse_needed`
    //! the same host frame.
    var result = HybridEditResult()
    hybrid_sync_segments(hybrid, ed, doc, document_stamp, result)
    let view_height = GetWindowHeight()
    let view_top = GetWindowPos().y
    let selection_before = view.selection_revision
    markdown_tree_sitter_prepare(doc, provider, view, document_stamp)
    if (!markdown_view_begin(id, doc, view, md_style, document_stamp)) {
        // unparseable document: honest fallback is the classic full editor
        result.edit = text_source_edit_segment("editor", ed, edit_style,
            registry, TextEditSegmentSpec())
        return <- result
    }
    let line_height = hybrid_edit_line_height(edit_style)
    let delta = length(ed.lines) - hybrid.seg_line_count
    var island_top = 0.0f
    var island_first = 0
    for (index in range(length(hybrid.segments))) {
        var seg & = unsafe(hybrid.segments[index])
        seg.screen_y0 = GetCursorScreenPos().y
        if (seg.kind == HybridSegKind.rich) {
            markdown_view_top_blocks(doc, view, md_style, result.view,
                seg.node_first, seg.node_last)
        } elif (seg.top_node >= 0) {
            markdown_view_block_before(doc, seg.top_node, seg.exclude_node,
                view, md_style, result.view)
            island_top = hybrid_draw_island(ed, edit_style, registry, seg,
                delta, line_height, view_height, result)
            island_first = seg.line_first
            markdown_view_block_after(doc, seg.top_node, seg.exclude_node,
                view, md_style, result.view)
        } else {
            // whole-buffer source segment (empty/unstructured document)
            island_top = hybrid_draw_island(ed, edit_style, registry, seg,
                delta, line_height, view_height, result)
            island_first = seg.line_first
        }
        seg.screen_y1 = GetCursorScreenPos().y
    }
    markdown_view_end(doc, view, md_style, result.view)
    if (view.selection_revision != selection_before
            && view.selection_focus.valid
            && view.selection_focus.source_byte >= 0) {
        // a rich click is a caret move; the island follows on re-segmentation
        let target = text_edit_position_of_byte(ed,
            view.selection_focus.source_byte)
        text_edit_set_cursor(ed, target.x, target.y)
        markdown_view_clear_selection(view)
        hybrid.rich_click_revision++
        if (ed.revision != hybrid.seg_edit_revision) {
            result.reparse_needed = true
        }
    }
    if (ed.follow_revision != ed.followed_revision) {
        let caret_inside = (ed.cursor_line >= island_first
            && ed.cursor_line - island_first < hybrid.seg_line_count + delta)
        if (caret_inside) {
            let caret_y = (island_top
                + float(ed.cursor_line - island_first) * line_height)
            if (caret_y < view_top + view_height * 0.10f
                    || caret_y + line_height > view_top + view_height * 0.90f) {
                SetScrollY(max(GetScrollY() + caret_y
                    - (view_top + view_height * 0.35f), 0.0f))
            }
            ed.followed_revision = ed.follow_revision
        }
    }
    if (ed.pending_scroll_lines != 0) {
        SetScrollY(max(GetScrollY()
            + float(ed.pending_scroll_lines) * line_height, 0.0f))
        ed.pending_scroll_lines = 0
    }
    return <- result
}
