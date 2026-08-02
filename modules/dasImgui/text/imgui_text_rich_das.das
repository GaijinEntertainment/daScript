options gen2
options _comment_hygiene = true
options indenting = 4
// custom-widget layer: raw child windows by design (same as the compositor)
options _allow_imgui_legacy = true

module imgui_text_rich_das shared

require imgui public
require imgui/imgui_text_source_edit public
require imgui/imgui_markdown_hybrid public
require imgui/imgui_markdown_model
require math
require strings

//! The SECOND hybrid format: a daslang buffer whose COMMENT GROUPS render as
//! markdown fragments while everything else stays source (the inverse ratio
//! of the markdown live preview). Classification reads the editor's own
//! lexical runs — never a second comment lexer.

// imgui_document_model declares a colliding TextEditResult; pin the editor's
typedef RichDasEditResult = imgui_text_source_edit::TextEditResult

enum public RichDasLineClass {
    code
    full_comment
    blank
}

struct public RichDasGroupSpan {
    first_line : int = 0
    last_line : int = 0 // inclusive
    indent : string // exact leading-whitespace prefix (byte-identical rule)
}

struct public RichDasGroup {
    span : RichDasGroupSpan = RichDasGroupSpan()
    fragment : string
    strip : array<int> // per-line bytes stripped from the head (caret map)
    doc : RichDocument = RichDocument()
    view : MarkdownViewState = MarkdownViewState()
    doc_revision : int = 0
    selection_seen : int = 0
}

struct public RichDasState {
    groups : array<RichDasGroup>
    synced_revision : int = -1 // ed.revision the groups were built for
    synced_line_count : int = 0
    parse_serial : int = 0
    parse_count : int = 0 // total fragment reparses (test/telemetry rail)
}

def public rich_das_dispose(var st : RichDasState) {
    for (index in range(length(st.groups))) {
        markdown_view_dispose(st.groups[index].view)
        delete st.groups[index].doc.nodes
    }
    delete st.groups
    st.synced_revision = -1
    st.synced_line_count = 0
}

// ===== Classification + grouping (pure over TextEditState) =====

def public rich_das_classify_line(ed : TextEditState const;
                                  row : int) : RichDasLineClass {
    //! `full_comment` = every non-whitespace byte sits inside a comment run;
    //! mixed code+comment lines are `code` (they never render rich).
    return RichDasLineClass.code if (ed.language != "daslang"
        || row >= length(ed.lines) || row >= length(ed.line_styles))
    let line = ed.lines[row]
    var any_solid = false
    var covered = true
    peek_data(line) $(bytes) {
        for (index in range(length(bytes))) {
            let value = int(bytes[index])
            continue if (value == ' ' || value == '\t' || value == '\r')
            any_solid = true
            var in_comment = false
            for (run in ed.line_styles[row].runs) {
                if (index >= run.start_byte && index < run.end_byte
                        && run.style == TextSourceSyntaxStyle.comment) {
                    in_comment = true
                    break
                }
            }
            if (!in_comment) {
                covered = false
                break
            }
        }
    }
    return RichDasLineClass.blank if (!any_solid)
    return covered ? RichDasLineClass.full_comment : RichDasLineClass.code
}

def private line_indent(ed : TextEditState const; row : int) : string {
    return leading_whitespace(ed.lines[row])
}

def private begins_in_block(ed : TextEditState const; row : int) : bool {
    return row < length(ed.line_styles) && ed.line_styles[row].in_block_comment
}

def public rich_das_collect_groups(ed : TextEditState const; // nolint:STYLE038
                                   var spans : array<RichDasGroupSpan>) {
    //! Maximal runs of adjacent full-comment lines with byte-identical indent.
    //! Blank lines split; a `/* */` with a MIXED opener or closer line poisons
    //! its whole span (interior-only rich islands are not allowed).
    spans |> clear()
    return if (ed.language != "daslang")
    let total = length(ed.lines)
    var row = 0
    while (row < total) {
        if (rich_das_classify_line(ed, row) != RichDasLineClass.full_comment) {
            row++
            continue
        }
        // a continuation whose opener line was mixed code stays source
        if (begins_in_block(ed, row) && (row == 0
                || rich_das_classify_line(ed, row - 1) != RichDasLineClass.full_comment)) {
            while (row < total && begins_in_block(ed, row)) {
                row++
            }
            continue
        }
        let indent = line_indent(ed, row)
        let first = row
        var last = row
        while (last + 1 < total
                && rich_das_classify_line(ed, last + 1) == RichDasLineClass.full_comment
                && line_indent(ed, last + 1) == indent) {
            last++
        }
        if (last + 1 < total && begins_in_block(ed, last + 1)) {
            // the run's tail opens a block comment that CLOSES on a mixed
            // line below: poison the whole candidate
            row = last + 1
            continue
        }
        spans |> push(RichDasGroupSpan(first_line = first, last_line = last,
            indent = indent))
        row = last + 1
    }
}

def public rich_das_fragment(ed : TextEditState const; span : RichDasGroupSpan;
                             var strip : array<int>) : string {
    //! The group's markdown text: indent + one comment prefix + at most ONE
    //! space stripped per line (deeper spaces survive as md indentation).
    //! `strip[i]` = head bytes removed from source line `first_line + i`.
    strip |> clear()
    return build_string() $(var writer) {
        for (row in range(span.first_line, span.last_line + 1)) {
            let line = ed.lines[row]
            var at = min(length(span.indent), length(line))
            var rest = slice(line, at, length(line))
            if (begins_in_block(ed, row)) {
                pass // block-comment interior: indent strip only
            } elif (starts_with(rest, "//!")) {
                at += 3
            } elif (starts_with(rest, "//")) {
                at += 2
            } elif (starts_with(rest, "/*")) {
                at += 2
            }
            rest = slice(line, at, length(line))
            if (starts_with(rest, " ")) {
                at++
                rest = slice(line, at, length(line))
            }
            var tail = length(line)
            if (ends_with(rest, "*/")) {
                tail -= 2
                if (tail > at && slice(line, tail - 1, tail) == " ") {
                    tail--
                }
            }
            strip |> push(at)
            if (row > span.first_line) {
                writer |> write("\n")
            }
            writer |> write(slice(line, at, max(tail, at)))
        }
    }
}

def public rich_das_caret_target(st : RichDasState const; group_index : int;
                                 fragment_byte : int) : int2 {
    //! Map a fragment-local byte to (source line, column) via the strip map.
    //! Lines are exact; columns are approximate under inline markup.
    return int2(0, 0) if (group_index < 0 || group_index >= length(st.groups))
    let group & = unsafe(st.groups[group_index])
    var fragment_line = 0
    var line_start = 0
    let clamped = clamp(fragment_byte, 0, length(group.fragment))
    peek_data(group.fragment) $(bytes) {
        for (index in range(min(clamped, length(bytes)))) {
            if (int(bytes[index]) == '\n') {
                fragment_line++
                line_start = index + 1
            }
        }
    }
    let source_line = group.span.first_line + fragment_line
    let head = fragment_line < length(group.strip) ? group.strip[fragment_line] : 0
    return int2(source_line, head + (clamped - line_start))
}

// ===== Sync (per-group cache with content adoption) =====

def public rich_das_sync(var st : RichDasState; ed : TextEditState const; // nolint:STYLE038
                         immediate : bool) : bool {
    //! Rebuild the group list; unchanged fragments ADOPT their old parse and
    //! layout, so editing one group reparses only that group and code edits
    //! between groups reparse nothing. Debounced unless `immediate`.
    return false if (ed.revision == st.synced_revision
        || (!immediate && st.synced_revision >= 0 && ed.syntax_idle < 0.2f))
    var inscope spans : array<RichDasGroupSpan>
    rich_das_collect_groups(ed, spans)
    var next_groups : array<RichDasGroup>
    next_groups |> reserve(length(spans))
    var old_index = 0
    for (span in spans) {
        var group = RichDasGroup(span = span)
        group.fragment = rich_das_fragment(ed, span, group.strip)
        var adopted = -1
        for (probe in range(old_index, length(st.groups))) {
            if (st.groups[probe].fragment == group.fragment) {
                adopted = probe
                break
            }
        }
        if (adopted >= 0) {
            group.doc <- st.groups[adopted].doc
            group.view <- st.groups[adopted].view
            group.doc_revision = st.groups[adopted].doc_revision
            group.selection_seen = st.groups[adopted].selection_seen
            old_index = adopted + 1
        } else {
            group.doc <- md_parse(group.fragment)
            st.parse_serial++
            st.parse_count++
            group.doc_revision = st.parse_serial
        }
        next_groups |> emplace(group)
    }
    // unadopted old groups dispose; adopted ones were moved out (now empty)
    for (index in range(length(st.groups))) {
        markdown_view_dispose(st.groups[index].view)
        delete st.groups[index].doc.nodes
    }
    delete st.groups
    st.groups <- next_groups
    st.synced_revision = ed.revision
    st.synced_line_count = length(ed.lines)
    return true
}

// ===== Segments + the driver =====

def public rich_das_segments(st : RichDasState const; line_count : int;
                             var segments : array<HybridSegment>) {
    //! Caret-agnostic partition: source runs between groups, one rich segment
    //! per group (`node_first` carries the group index). The driver demotes
    //! the caret's segment to source at draw time.
    segments |> clear()
    segments |> reserve(length(st.groups) * 2 + 1)
    var cursor = 0
    for (index in range(length(st.groups))) {
        let span = st.groups[index].span
        if (span.first_line > cursor) {
            segments |> push(HybridSegment(kind = HybridSegKind.source_,
                line_first = cursor, line_last = span.first_line - 1))
        }
        segments |> push(HybridSegment(kind = HybridSegKind.rich,
            line_first = span.first_line, line_last = span.last_line,
            node_first = index, node_last = index))
        cursor = span.last_line + 1
    }
    if (cursor <= line_count - 1 || empty(segments)) {
        segments |> push(HybridSegment(kind = HybridSegKind.source_,
            line_first = cursor, line_last = max(line_count - 1, cursor)))
    }
}

def private rich_das_segment_of(segments : array<HybridSegment> const;
                                line : int) : int {
    for (index in range(length(segments))) {
        if (line >= segments[index].line_first
                && line <= segments[index].line_last) {
            return index
        }
    }
    return max(length(segments) - 1, 0)
}

def private rich_das_route_click(var ed : TextEditState; var st : RichDasState;
                                 group_index : int) {
    var group & = unsafe(st.groups[group_index])
    return if (group.view.selection_revision == group.selection_seen)
    group.selection_seen = group.view.selection_revision
    return if (!group.view.selection_focus.valid
        || group.view.selection_focus.source_byte < 0)
    let target = rich_das_caret_target(st, group_index,
        group.view.selection_focus.source_byte)
    markdown_view_clear_selection(group.view)
    text_edit_set_cursor(ed, target.x, target.y)
}

def private rich_das_draw_source(var ed : TextEditState; // nolint:STYLE038
                                 var edit_style : TextSourceViewStyle;
                                 var registry : CommandRegistry;
                                 seg : HybridSegment; index : int;
                                 primary, advance_clocks : bool;
                                 line_height, view_height : float;
                                 var result : RichDasEditResult) : float {
    // returns the child's screen top (the caret-follow anchor when primary)
    let rows = seg.line_last - seg.line_first + 1
    var island_h = float(rows) * line_height
    if (primary) {
        island_h += GetStyle().ScrollbarSize
    }
    let top = GetCursorScreenPos().y
    PushStyleVar(ImGuiStyleVar.WindowPadding, float2(0.0f, 0.0f))
    PushID(index)
    let open_ = BeginChild("##rich_das_source", float2(0.0f, island_h),
        ImGuiChildFlags.None,
        primary ? ImGuiWindowFlags.HorizontalScrollbar
            : ImGuiWindowFlags.NoScrollbar | ImGuiWindowFlags.NoScrollWithMouse)
    if (open_) {
        let spec = TextEditSegmentSpec(line_first = seg.line_first,
            line_last = seg.line_last, primary = primary,
            advance_clocks = advance_clocks, owns_scroll = false,
            follow_x = primary && ed.follow_revision != ed.followed_revision,
            dead_area = false, register_alias = primary,
            view_height = view_height)
        let seg_result = text_source_edit_segment("editor", ed, edit_style,
            registry, spec)
        if (primary) {
            result = seg_result
        }
    }
    EndChild()
    PopID()
    PopStyleVar()
    return top
}

def public rich_das_hybrid_edit(var ed : TextEditState; // nolint:STYLE038
                                var st : RichDasState;
                                var hybrid : HybridEditState;
                                var edit_style : TextSourceViewStyle;
                                var md_style : MarkdownViewStyle;
                                var registry : CommandRegistry) : RichDasEditResult {
    //! One frame of the rich-das hybrid: comment groups rich, the rest (and
    //! the caret's group) as source. Host contract matches the md compositor:
    //! sole content of a scrolling child, no horizontal scrollbar there.
    var result : RichDasEditResult = default<RichDasEditResult>
    // line-count changes and dirty caret-exits resync NOW (adoption keeps it
    // cheap); same-line-count edits inside a group ride the debounce
    if (length(ed.lines) != st.synced_line_count) {
        let _ = rich_das_sync(st, ed, true)
    } else {
        let _ = rich_das_sync(st, ed, false)
    }
    rich_das_segments(st, length(ed.lines), hybrid.segments)
    var caret_segment = rich_das_segment_of(hybrid.segments, ed.cursor_line)
    if (caret_segment != hybrid.source_segment
            && ed.revision != st.synced_revision) {
        let _ = rich_das_sync(st, ed, true)
        rich_das_segments(st, length(ed.lines), hybrid.segments)
        caret_segment = rich_das_segment_of(hybrid.segments, ed.cursor_line)
    }
    hybrid.source_segment = caret_segment
    hybrid.seg_caret_line = ed.cursor_line
    hybrid.seg_edit_revision = st.synced_revision
    hybrid.seg_line_count = st.synced_line_count
    let view_height = GetWindowHeight()
    let view_top = GetWindowPos().y
    let line_height = markdown_hybrid_line_height(edit_style)
    var clocks_pending = true
    var island_top = 0.0f
    var island_first = 0
    for (index in range(length(hybrid.segments))) {
        var seg & = unsafe(hybrid.segments[index])
        seg.screen_y0 = GetCursorScreenPos().y
        let primary = index == caret_segment
        if (seg.kind == HybridSegKind.source_ || primary) {
            let top = rich_das_draw_source(ed, edit_style, registry, seg,
                index, primary, clocks_pending, line_height, view_height,
                result)
            clocks_pending = false
            if (primary) {
                island_top = top
                island_first = seg.line_first
            }
        } else {
            let group_index = seg.node_first
            var group & = unsafe(st.groups[group_index])
            let view_result <- rich_document_view("rich_das_{group_index}",
                group.doc, group.view, md_style, group.doc_revision)
            rich_das_route_click(ed, st, group_index)
        }
        seg.screen_y1 = GetCursorScreenPos().y
    }
    if (ed.follow_revision != ed.followed_revision) {
        let caret_y = (island_top
            + float(ed.cursor_line - island_first) * line_height)
        if (caret_y < view_top + view_height * 0.10f
                || caret_y + line_height > view_top + view_height * 0.90f) {
            SetScrollY(max(GetScrollY() + caret_y
                - (view_top + view_height * 0.35f), 0.0f))
        }
        ed.followed_revision = ed.follow_revision
    }
    if (ed.pending_scroll_lines != 0) {
        SetScrollY(max(GetScrollY()
            + float(ed.pending_scroll_lines) * line_height, 0.0f))
        ed.pending_scroll_lines = 0
    }
    return result
}
