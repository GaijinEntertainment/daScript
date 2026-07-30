options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_markdown_model shared

require imgui
require imgui/imgui_document_model public
require imgui/imgui_text_flow public
require daslib/utf8_utils
require math
require strings

//! View-independent Markdown document model.
//!
//! MD4C owns CommonMark/GFM recognition and emits SAX-style events through a
//! deliberately small native bridge. This module turns those events into a
//! flat, index-addressed tree suitable for viewers, editors, search, and tests.
//! Exact ranges identify bytes in `RichDocument.source`; container ranges are
//! conservative unions of their descendants and therefore marked non-exact.

struct public MdParseOptions {
    tables : bool = true
    task_lists : bool = true
    strikethrough : bool = true
    permissive_autolinks : bool = true
    raw_html : bool = false
}

bitfield public MdSelectionStyles {
    //! Semantic styles present in the selected display text. `all_regular`
    //! means every contributing run is unstyled; `has_mixed_styles` means at
    //! least two distinct combinations of the other style bits occur.
    all_regular
    has_strong
    has_emphasis
    has_code
    has_strikethrough
    has_links
    has_images
    has_mixed_styles
}

struct public MdSelectionStyleAccumulator {
    //! View-independent accumulator: callers can include one or many inline
    //! contents, which is how a viewer aggregates a cross-block selection.
    styles : MdSelectionStyles
    first_mask : int = -1
    matched_runs : int = 0
    regular_only : bool = true
}

struct public MdSelectionExport {
    //! Platform-neutral representations of one source-mapped selection. All follow rendered inline
    //! runs (entities stay transformed); Markdown syntax is reconstructed around partial styled
    //! selections so copied bold/link text stays valid when pasted alone.
    plain_text : string
    markdown : string
    html : string
    uri_list : string
    block_count : int = 0
}

def private block_kind(kind : int) : RichNodeKind {
    if (kind == 0) return RichNodeKind.document
    if (kind == 1) return RichNodeKind.quote
    if (kind == 2) return RichNodeKind.unordered_list
    if (kind == 3) return RichNodeKind.ordered_list
    if (kind == 4) return RichNodeKind.list_item
    if (kind == 5) return RichNodeKind.thematic_break
    if (kind == 6) return RichNodeKind.heading
    if (kind == 7) return RichNodeKind.code_block
    if (kind == 8) return RichNodeKind.raw_html
    if (kind == 9) return RichNodeKind.paragraph
    if (kind == 10) return RichNodeKind.table_
    if (kind == 11) return RichNodeKind.table_head
    if (kind == 12) return RichNodeKind.table_body
    if (kind == 13) return RichNodeKind.table_row
    if (kind == 14) return RichNodeKind.table_header_cell
    if (kind == 15) return RichNodeKind.table_cell
    return RichNodeKind.unknown
}

def private span_kind(kind : int) : RichNodeKind {
    if (kind == 0) return RichNodeKind.emphasis
    if (kind == 1) return RichNodeKind.strong
    if (kind == 2) return RichNodeKind.link
    if (kind == 3) return RichNodeKind.image
    if (kind == 4) return RichNodeKind.inline_code
    if (kind == 5) return RichNodeKind.strikethrough
    if (kind == 6) return RichNodeKind.latex_math
    if (kind == 7) return RichNodeKind.latex_math_display
    if (kind == 8) return RichNodeKind.wiki_link
    if (kind == 9) return RichNodeKind.underline
    return RichNodeKind.unknown
}

def private text_kind(kind : int) : RichNodeKind {
    if (kind == 0) return RichNodeKind.text
    if (kind == 1) return RichNodeKind.null_character
    if (kind == 2) return RichNodeKind.hard_break
    if (kind == 3) return RichNodeKind.soft_break
    if (kind == 4) return RichNodeKind.entity
    if (kind == 5) return RichNodeKind.code_text
    if (kind == 6) return RichNodeKind.html_text
    if (kind == 7) return RichNodeKind.latex_text
    return RichNodeKind.unknown
}

def private md_alignment(value : int) : RichAlignment {
    if (value == 1) return RichAlignment.left
    if (value == 2) return RichAlignment.center
    if (value == 3) return RichAlignment.right
    return RichAlignment.default_
}

def private parser_flags(parse_options : MdParseOptions const) : int {
    var flags = 0u
    if (parse_options.permissive_autolinks) {
        flags |= 0x0004u // MD_FLAG_PERMISSIVEURLAUTOLINKS
        flags |= 0x0008u // MD_FLAG_PERMISSIVEEMAILAUTOLINKS
        flags |= 0x0400u // MD_FLAG_PERMISSIVEWWWAUTOLINKS
    }
    if (!parse_options.raw_html) {
        flags |= 0x0020u // MD_FLAG_NOHTMLBLOCKS
        flags |= 0x0040u // MD_FLAG_NOHTMLSPANS
    }
    if (parse_options.tables) {
        flags |= 0x0100u
    }
    if (parse_options.strikethrough) {
        flags |= 0x0200u
    }
    if (parse_options.task_lists) {
        flags |= 0x0800u
    }
    return int(flags)
}

def private attach_node(var doc : RichDocument; var stack : array<int>;
                        var node : RichNode) : int {
    let index = length(doc.nodes)
    if (!empty(stack)) {
        let parent = stack[length(stack) - 1]
        node.parent = parent
        if (doc.nodes[parent].first_child < 0) {
            doc.nodes[parent].first_child = index
        } else {
            doc.nodes[doc.nodes[parent].last_child].next_sibling = index
        }
        doc.nodes[parent].last_child = index
    }
    doc.nodes |> push(<- node)
    return index
}

def private apply_block_detail(var node : RichNode; kind, flags, offset, size,
                               aux0, aux1 : int; text, text2 : string) {
    if (kind == 2) {
        node.is_tight = flags != 0
        node.marker = aux0
    } elif (kind == 3) {
        node.is_tight = flags != 0
        node.start_number = aux0
        node.marker = aux1
    } elif (kind == 4) {
        node.is_task = flags != 0
        node.is_checked = node.is_task && aux0 != int(' ')
        node.marker = aux0
        if (node.is_task && offset >= 0) {
            node.control_source = TextSourceRange(
                start_byte = offset, end_byte = offset + 1)
            node.source = node.control_source
        }
    } elif (kind == 6) {
        node.level = aux0
    } elif (kind == 7) {
        node.marker = aux0
        node.info := text
        node.language := text2
    } elif (kind == 10) {
        node.column_count = aux0
        node.head_row_count = aux1
        node.body_row_count = size
    } elif (kind == 14 || kind == 15) {
        node.alignment = md_alignment(aux0)
    }
}

def private apply_span_detail(var node : RichNode; kind, flags : int;
                              text, text2 : string) {
    if (kind == 2 || kind == 3) {
        node.destination := text
        node.title := text2
        node.is_autolink = kind == 2 && flags != 0
    } elif (kind == 8) {
        node.destination := text
    }
}

def private include_range(var node : RichNode; source_range : TextSourceRange const) {
    if (source_range.start_byte < 0 || source_range.end_byte < source_range.start_byte) return
    if (node.source.start_byte < 0) {
        node.source = source_range
    } else {
        node.source.start_byte = min(node.source.start_byte, source_range.start_byte)
        node.source.end_byte = max(node.source.end_byte, source_range.end_byte)
    }
}

def private close_node(var doc : RichDocument; var stack : array<int>) {
    if (empty(stack)) return
    let index = stack[length(stack) - 1]
    stack |> pop()
    var child = doc.nodes[index].first_child
    while (child >= 0) {
        include_range(doc.nodes[index], doc.nodes[child].source)
        child = doc.nodes[child].next_sibling
    }
}

def public md_node_text(doc : RichDocument const; node_index : int) : string {
    //! Return the exact Markdown source bytes represented by a leaf.
    if (node_index < 0 || node_index >= length(doc.nodes)) return ""
    let node = doc.nodes[node_index]
    if (node.source_exact && node.source.start_byte >= 0
            && node.source.end_byte <= length(doc.source)) {
        return slice(doc.source, node.source.start_byte, node.source.end_byte)
    }
    return node.literal
}

def public md_node_display_text(doc : RichDocument const; node_index : int) : string {
    //! Return display-ready text. Entities and null characters retain their
    //! exact source range while exposing MD4C's decoded/replacement text here.
    if (node_index < 0 || node_index >= length(doc.nodes)) return ""
    let node = doc.nodes[node_index]
    if (!empty(node.literal)) return node.literal
    return md_node_text(doc, node_index)
}

def private collect_inline(doc : RichDocument const; node_index : int;
                           var content : RichInlineContent;
                           strong, emphasis, code, strike : bool;
                           semantic_node : int) {
    if (node_index < 0 || node_index >= length(doc.nodes)) return
    let node = doc.nodes[node_index]
    let next_strong = strong || node.kind == RichNodeKind.strong
    let next_emphasis = emphasis || node.kind == RichNodeKind.emphasis
    let next_code = (code || node.kind == RichNodeKind.inline_code
        || node.kind == RichNodeKind.code_block)
    let next_strike = strike || node.kind == RichNodeKind.strikethrough

    if (node.kind == RichNodeKind.image) {
        // Keep replaced content atomic; the image's child subtree is flattened only for its plain-text alt.
        var alt_content = RichInlineContent()
        var child = node.first_child
        while (child >= 0) {
            collect_inline(doc, child, alt_content, false, false,
                           false, false, -1)
            child = doc.nodes[child].next_sibling
        }
        var alt := alt_content.text
        delete alt_content.runs
        let display_alt = empty(alt) ? "Image" : alt
        let start = length(content.text)
        content.text = content.text + display_alt
        content.runs |> push(RichInlineRun(
            display = TextSourceRange(
                start_byte = start, end_byte = start + length(display_alt)),
            source = node.source,
            source_exact = false,
            node_index = node_index,
            semantic_node = node_index,
            is_strong = next_strong,
            is_emphasis = next_emphasis,
            is_code = next_code,
            is_strikethrough = next_strike,
            is_link = true,
            is_image = true,
            image_alt = alt,
            destination = node.destination,
            title = node.title))
        return
    }

    var next_semantic = semantic_node
    if (node.kind == RichNodeKind.link) {
        next_semantic = node_index
    }

    let is_leaf = (node.kind == RichNodeKind.text
        || node.kind == RichNodeKind.null_character
        || node.kind == RichNodeKind.hard_break
        || node.kind == RichNodeKind.soft_break
        || node.kind == RichNodeKind.entity
        || node.kind == RichNodeKind.code_text
        || node.kind == RichNodeKind.html_text
        || node.kind == RichNodeKind.latex_text)
    if (is_leaf) {
        var value = md_node_display_text(doc, node_index)
        if (node.kind == RichNodeKind.soft_break) {
            value = " "
        } elif (node.kind == RichNodeKind.hard_break) {
            value = "\n"
        }
        let start = length(content.text)
        content.text = content.text + value
        var run = RichInlineRun(
            display = TextSourceRange(start_byte = start, end_byte = start + length(value)),
            source = node.source,
            source_exact = node.source_exact,
            node_index = node_index,
            semantic_node = next_semantic,
            is_strong = next_strong,
            is_emphasis = next_emphasis,
            is_code = next_code,
            is_strikethrough = next_strike,
            is_link = next_semantic >= 0)
        if (next_semantic >= 0) {
            run.destination := doc.nodes[next_semantic].destination
            run.title := doc.nodes[next_semantic].title
        }
        content.runs |> push(<- run)
        return
    }

    var child = node.first_child
    while (child >= 0) {
        collect_inline(doc, child, content, next_strong, next_emphasis,
                       next_code, next_strike, next_semantic)
        child = doc.nodes[child].next_sibling
    }
}

def private split_plain_uris(doc : RichDocument const; var content : RichInlineContent) {
    var split_runs : array<RichInlineRun>
    for (i in range(length(content.runs))) {
        var base <- content.runs[i]
        let can_detect = (!base.is_link && base.source_exact
            && base.node_index >= 0
            && doc.nodes[base.node_index].kind == RichNodeKind.text)
        if (!can_detect) {
            split_runs |> push(<- base)
            continue
        }
        let value = slice(content.text, base.display.start_byte, base.display.end_byte)
        let uris <- text_uris(value)
        if (empty(uris)) {
            split_runs |> push(<- base)
            continue
        }
        var cursor = 0
        for (uri in uris) {
            if (uri.source.start_byte > cursor) {
                var before := base
                before.display = TextSourceRange(
                    start_byte = base.display.start_byte + cursor,
                    end_byte = base.display.start_byte + uri.source.start_byte)
                before.source = TextSourceRange(
                    start_byte = base.source.start_byte + cursor,
                    end_byte = base.source.start_byte + uri.source.start_byte)
                split_runs |> push(<- before)
            }
            var linked := base
            linked.display = TextSourceRange(
                start_byte = base.display.start_byte + uri.source.start_byte,
                end_byte = base.display.start_byte + uri.source.end_byte)
            linked.source = TextSourceRange(
                start_byte = base.source.start_byte + uri.source.start_byte,
                end_byte = base.source.start_byte + uri.source.end_byte)
            linked.is_link = true
            linked.is_automatic_uri = true
            linked.semantic_node = -1
            let display_uri = slice(value, uri.source.start_byte, uri.source.end_byte)
            linked.destination := (uri.kind == TextUriKind.www
                ? "https://" + display_uri : display_uri)
            split_runs |> push(<- linked)
            cursor = uri.source.end_byte
        }
        if (cursor < length(value)) {
            var after := base
            after.display = TextSourceRange(
                start_byte = base.display.start_byte + cursor,
                end_byte = base.display.end_byte)
            after.source = TextSourceRange(
                start_byte = base.source.start_byte + cursor,
                end_byte = base.source.end_byte)
            split_runs |> push(<- after)
        }
    }
    content.runs <- split_runs
}

def public md_inline_content(doc : RichDocument const; node_index : int) : RichInlineContent {
    //! Flatten one inline-bearing subtree into display text plus semantic/style
    //! runs. This remains independent of ImGui and is shared by future viewer,
    //! editor, accessibility, and copy/export paths.
    var content = RichInlineContent()
    collect_inline(doc, node_index, content, false, false, false, false, -1)
    split_plain_uris(doc, content)
    return <- content
}

def public md_node_kind_is_inline(kind : RichNodeKind) : bool {
    //! True for span/text nodes that participate in one inline layout.
    return (kind == RichNodeKind.emphasis
        || kind == RichNodeKind.strong
        || kind == RichNodeKind.link
        || kind == RichNodeKind.image
        || kind == RichNodeKind.inline_code
        || kind == RichNodeKind.strikethrough
        || kind == RichNodeKind.latex_math
        || kind == RichNodeKind.latex_math_display
        || kind == RichNodeKind.wiki_link
        || kind == RichNodeKind.underline
        || kind == RichNodeKind.text
        || kind == RichNodeKind.null_character
        || kind == RichNodeKind.hard_break
        || kind == RichNodeKind.soft_break
        || kind == RichNodeKind.entity
        || kind == RichNodeKind.code_text
        || kind == RichNodeKind.html_text
        || kind == RichNodeKind.latex_text
        || kind == RichNodeKind.unknown)
}

def public md_inline_content_direct_children(
                                             doc : RichDocument const; node_index : int) : RichInlineContent {
    //! Flatten only direct inline children of a block container. MD4C emits
    //! tight-list inline nodes directly under list_item, without a paragraph.
    var content = RichInlineContent()
    if (node_index < 0 || node_index >= length(doc.nodes)) return <- content
    var child = doc.nodes[node_index].first_child
    while (child >= 0) {
        if (md_node_kind_is_inline(doc.nodes[child].kind)) {
            collect_inline(doc, child, content, false, false, false, false, -1)
        }
        child = doc.nodes[child].next_sibling
    }
    split_plain_uris(doc, content)
    return <- content
}

def private md_run_style_mask(run : RichInlineRun const) : int {
    var mask = 0
    if (run.is_strong) {
        mask = mask | 1
    }
    if (run.is_emphasis) {
        mask = mask | 2
    }
    if (run.is_code) {
        mask = mask | 4
    }
    if (run.is_strikethrough) {
        mask = mask | 8
    }
    if (run.is_link) {
        mask = mask | 16
    }
    if (run.is_image) {
        mask = mask | 32
    }
    return mask
}

def public md_inline_position(content : RichInlineContent const;
                              inline_node, display_byte : int;
                              trailing : bool = false) : RichTextPosition {
    //! Convert a grapheme-boundary display caret to original Markdown bytes. At a style boundary
    //! `trailing` selects the run on the left, else the right is preferred; transformed runs
    //! (e.g. an entity displayed as one glyph) snap to the whole source token.
    if (empty(content.runs)) return RichTextPosition()
    let at = clamp(display_byte, 0, length(content.text))
    var chosen = -1
    if (trailing) {
        for (i in range(length(content.runs))) {
            let run = content.runs[i]
            if (run.display.start_byte < at && at <= run.display.end_byte) {
                chosen = i
                break
            }
        }
    } else {
        for (i in range(length(content.runs))) {
            let run = content.runs[i]
            if (run.display.start_byte <= at && at < run.display.end_byte) {
                chosen = i
                break
            }
        }
    }
    if (chosen < 0) {
        chosen = trailing ? length(content.runs) - 1 : 0
    }

    let run = content.runs[chosen]
    let display_size = run.display.end_byte - run.display.start_byte
    let source_size = run.source.end_byte - run.source.start_byte
    var source_byte = run.source.start_byte
    if (run.source_exact && display_size == source_size) {
        source_byte += clamp(at - run.display.start_byte, 0, display_size)
    } elif (at >= run.display.end_byte) {
        source_byte = run.source.end_byte
    } elif (at > run.display.start_byte && trailing) {
        source_byte = run.source.end_byte
    }
    return RichTextPosition(
        valid = source_byte >= 0,
        inline_node = inline_node,
        display_byte = at,
        source_byte = source_byte,
        trailing = trailing)
}

def public md_text_selection_range(anchor, focus : RichTextPosition const) : TextSourceRange {
    if (!anchor.valid || !focus.valid) {
        return TextSourceRange(start_byte = -1, end_byte = -1)
    }
    return TextSourceRange(
        start_byte = min(anchor.source_byte, focus.source_byte),
        end_byte = max(anchor.source_byte, focus.source_byte))
}

def public md_selection_include(var accumulator : MdSelectionStyleAccumulator;
                                content : RichInlineContent const;
                                source : TextSourceRange const) {
    //! Include semantic runs overlapping an original-source selection.
    if (source.start_byte < 0 || source.end_byte <= source.start_byte) return
    for (run in content.runs) {
        if (run.source.start_byte < 0 || run.source.end_byte <= source.start_byte
                || run.source.start_byte >= source.end_byte) continue
        let mask = md_run_style_mask(run)
        if (accumulator.first_mask < 0) {
            accumulator.first_mask = mask
        } elif (accumulator.first_mask != mask) {
            accumulator.styles.has_mixed_styles = true
        }
        accumulator.matched_runs++
        accumulator.regular_only = accumulator.regular_only && mask == 0
        accumulator.styles.has_strong = accumulator.styles.has_strong || run.is_strong
        accumulator.styles.has_emphasis = accumulator.styles.has_emphasis || run.is_emphasis
        accumulator.styles.has_code = accumulator.styles.has_code || run.is_code
        accumulator.styles.has_strikethrough = (accumulator.styles.has_strikethrough
            || run.is_strikethrough)
        accumulator.styles.has_links = accumulator.styles.has_links || run.is_link
        accumulator.styles.has_images = accumulator.styles.has_images || run.is_image
    }
}

def public md_selection_styles(accumulator : MdSelectionStyleAccumulator const) : MdSelectionStyles {
    var styles = accumulator.styles
    styles.all_regular = accumulator.matched_runs > 0 && accumulator.regular_only
    return styles
}

def public md_selected_display_range(run : RichInlineRun const;
                                     source : TextSourceRange const) : TextSourceRange {
    //! Map a source selection onto rendered bytes. Transformed entities and
    //! other inexact runs remain atomic because splitting them would invent a
    //! source position the parser never supplied.
    let a = max(source.start_byte, run.source.start_byte)
    let b = min(source.end_byte, run.source.end_byte)
    if (run.source.start_byte < 0 || b <= a) {
        return TextSourceRange(start_byte = -1, end_byte = -1)
    }
    let display_size = run.display.end_byte - run.display.start_byte
    let source_size = run.source.end_byte - run.source.start_byte
    if (run.source_exact && display_size == source_size) {
        return TextSourceRange(
            start_byte = run.display.start_byte + a - run.source.start_byte,
            end_byte = run.display.start_byte + b - run.source.start_byte)
    }
    return run.display
}

def private md_html_text(value : string) : string {
    return value |> replace("&", "&amp;") |> replace("<", "&lt;") |> replace(">", "&gt;")
}

def private md_html_attribute(value : string) : string {
    return value |> md_html_text() |> replace("\"", "&quot;") |> replace("'", "&#39;")
}

def private md_markdown_text(value : string) : string {
    var result = value |> replace("\\", "\\\\")
    result = result |> replace("`", "\\`")
    result = result |> replace("*", "\\*")
    result = result |> replace("_", "\\_")
    result = result |> replace("[", "\\[")
    result = result |> replace("]", "\\]")
    result = result |> replace("~", "\\~")
    return result
}

def private md_markdown_destination(value : string) : string {
    return value |> replace("\\", "\\\\") |> replace(")", "\\)")
}

def private md_export_uri(var value : string&; uri : string) {
    if (empty(uri)) return
    let line = uri + "\r\n"
    if ((value |> find(line)) < 0) {
        value = value + line
    }
}

def public md_selection_export_include(var result : MdSelectionExport;
                                       content : RichInlineContent const;
                                       source : TextSourceRange const) {
    //! Add one inline block to a rich selection export. Call blocks in document
    //! order; only blocks overlapping `source` contribute separators.
    var block_plain = ""
    var block_markdown = ""
    var block_html = ""
    for (run in content.runs) {
        let display = md_selected_display_range(run, source)
        if (display.start_byte < 0 || display.end_byte <= display.start_byte) continue
        let shown = slice(content.text, display.start_byte, display.end_byte)
        block_plain = block_plain + shown
        if (run.is_link || run.is_image) {
            md_export_uri(result.uri_list, run.destination)
        }
        if (run.is_image) {
            var image_html = "<img src=\"{run.destination |> md_html_attribute()}\" alt=\"{shown |> md_html_attribute()}\""
            if (!empty(run.title)) {
                image_html = image_html + " title=\"{run.title |> md_html_attribute()}\""
            }
            block_html = block_html + image_html + ">"
            var image_markdown = "![{shown |> md_markdown_text()}]({run.destination |> md_markdown_destination()}"
            if (!empty(run.title)) {
                let escaped_title = run.title |> replace("\"", "\\\"")
                image_markdown = image_markdown + " \"" + escaped_title + "\""
            }
            block_markdown = block_markdown + image_markdown + ")"
            continue
        }
        var markdown_text = run.is_code ? shown : (shown |> md_markdown_text())
        if (run.is_code) {
            markdown_text = "`" + markdown_text + "`"
        }
        if (run.is_strikethrough) {
            markdown_text = "~~" + markdown_text + "~~"
        }
        if (run.is_emphasis) {
            markdown_text = "*" + markdown_text + "*"
        }
        if (run.is_strong) {
            markdown_text = "**" + markdown_text + "**"
        }
        if (run.is_link) {
            markdown_text = "[" + markdown_text + "](" + (run.destination |> md_markdown_destination()) + ")"
        }
        block_markdown = block_markdown + markdown_text
        var prefix = ""
        var suffix = ""
        if (run.is_link) {
            prefix = prefix + "<a href=\"{run.destination |> md_html_attribute()}\">"
            suffix = "</a>" + suffix
        }
        if (run.is_strong) { prefix = prefix + "<strong>"; suffix = "</strong>" + suffix }
        if (run.is_emphasis) { prefix = prefix + "<em>"; suffix = "</em>" + suffix }
        if (run.is_strikethrough) { prefix = prefix + "<del>"; suffix = "</del>" + suffix }
        if (run.is_code) { prefix = prefix + "<code>"; suffix = "</code>" + suffix }
        block_html = block_html + prefix + (shown |> md_html_text()) + suffix
    }
    if (empty(block_plain) && empty(block_html)) return
    if (result.block_count > 0) {
        result.plain_text = result.plain_text + "\n"
        result.markdown = result.markdown + "\n"
        result.html = result.html + "<br>\n"
    }
    result.plain_text = result.plain_text + block_plain
    result.markdown = result.markdown + block_markdown
    result.html = result.html + block_html
    result.block_count++
}

def public md_source_text(doc : RichDocument const; source : TextSourceRange const) : string {
    //! Slice a validated original-source range, useful for comment anchors and
    //! persistence. This returns Markdown bytes, not flattened display text.
    if (source.start_byte < 0 || source.end_byte < source.start_byte
            || source.end_byte > length(doc.source)) return ""
    return slice(doc.source, source.start_byte, source.end_byte)
}

def public md_parse(source : string;
                    parse_options : MdParseOptions = MdParseOptions()) : RichDocument {
    var doc = RichDocument()
    doc.source := source
    if (!is_utf8_string_valid(source)) {
        doc.error = "Markdown source is not valid UTF-8"
        return <- doc
    }

    var stack : array<int>
    var callback_error = false
    var status : int = _markdown_parse_events(doc.source, length(doc.source),
        parser_flags(parse_options)) $(var event, kind, flags, offset, size,
                                       aux0, aux1 : int; text, text2 : string) {
            if (event == 0) { // enter block
                var node = RichNode(kind = block_kind(kind))
                apply_block_detail(node, kind, flags, offset, size,
                                   aux0, aux1, text, text2)
                let index = attach_node(doc, stack, node)
                if (kind == 0) {
                    doc.root = index
                }
                stack |> push(index)
            } elif (event == 1) { // leave block
                close_node(doc, stack)
            } elif (event == 2) { // enter span
                var node = RichNode(kind = span_kind(kind))
                apply_span_detail(node, kind, flags, text, text2)
                let index = attach_node(doc, stack, node)
                stack |> push(index)
            } elif (event == 3) { // leave span
                close_node(doc, stack)
            } elif (event == 4) { // text
                var node = RichNode(kind = text_kind(kind))
                if (offset >= 0 && size >= 0 && offset + size <= length(doc.source)) {
                    node.source = TextSourceRange(
                        start_byte = offset, end_byte = offset + size)
                    node.source_exact = true
                } else {
                    node.literal := text
                }
                if (node.kind == RichNodeKind.entity || node.kind == RichNodeKind.null_character) {
                    node.literal := text
                }
                attach_node(doc, stack, node)
            } elif (event == 5) { // debug/error diagnostic
                callback_error = true
                doc.error := text
            }
        }

    if (status != 0) {
        if (empty(doc.error)) {
            doc.error = "MD4C parse failed with status {status}"
        }
        return <- doc
    }
    if (callback_error) return <- doc
    if (!empty(stack)) {
        doc.error = "Markdown parser returned an unbalanced event stream"
        return <- doc
    }
    if (doc.root < 0) {
        doc.error = "Markdown parser did not emit a document root"
        return <- doc
    }
    doc.ok = true
    return <- doc
}

def public md_rich_projection(doc : RichDocument const;
                              source_revision : int) : RichDocumentProjection {
    //! Markdown is currently the first producer of the format-neutral control
    //! projection. Block and inline migration can happen independently of this
    //! interaction/edit contract.
    var projection = RichDocumentProjection(source_revision = source_revision)
    if (!doc.ok) return <- projection
    for (node_index in range(length(doc.nodes))) {
        let node = doc.nodes[node_index]
        if (node.kind != RichNodeKind.list_item || !node.is_task) continue
        var mapping = RichSourceMapping.none
        if (node.control_source.start_byte >= 0
                && node.control_source.end_byte == node.control_source.start_byte + 1) {
            mapping = RichSourceMapping.exact
        }
        projection.controls |> push(RichControl(
            id = node_index,
            kind = RichControlKind.toggle,
            source = node.control_source,
            source_mapping = mapping,
            checked = node.is_checked,
            payload = node_index))
    }
    return <- projection
}

def public md_control_action(doc : RichDocument const;
                             projection : RichDocumentProjection const;
                             event : RichControlEvent const) : RichControlAction {
    var action = RichControlAction()
    if (!event.active || event.kind != RichControlKind.toggle) {
        action.status = TextDocumentStatus.invalid_action
        return <- action
    }
    if (event.source_revision != projection.source_revision) {
        action.status = TextDocumentStatus.stale_revision
        return <- action
    }
    let control = rich_document_control(projection, event.control_id)
    if (control.kind != RichControlKind.toggle
            || control.source_mapping != RichSourceMapping.exact) {
        action.status = TextDocumentStatus.invalid_action
        return <- action
    }
    if (control.source.start_byte < 0
            || control.source.end_byte > length(doc.source)) {
        action.status = TextDocumentStatus.invalid_range
        return <- action
    }
    let current = slice(doc.source, control.source.start_byte, control.source.end_byte)
    let valid_marker = current == " " || current == "x" || current == "X"
    if (!valid_marker || (current != " ") == event.requested_checked) {
        action.status = TextDocumentStatus.unexpected_source
        return <- action
    }
    action.status = TextDocumentStatus.ok
    action.valid = true
    action.edit = TextEdit(
        source = control.source,
        replacement = event.requested_checked ? "x" : " ",
        expected = current,
        verify_expected = true,
        base_revision = event.source_revision)
    return <- action
}
