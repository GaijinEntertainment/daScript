options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_document_model shared

require imgui/imgui_text_flow public
require daslib/utf8_utils
require math
require strings

//! View-independent source storage and edit transactions for text-heavy UI.
//!
//! `TextDocumentSource.text` is the canonical editable UTF-8 content.  A UTF-8
//! BOM is retained as file metadata instead of being passed through parsers, so
//! parser offsets remain content-relative while an untouched save remains
//! byte-for-byte identical.  Rendered/rich documents are projections of this
//! source and must never be serialized to save a file.

enum public TextDocumentStatus {
    ok
    invalid_utf8
    stale_revision
    invalid_range
    unexpected_source
    invalid_action
}

enum public TextPositionBias {
    before
    after
}

struct public TextDocumentSource {
    text : string
    path : string
    has_utf8_bom : bool = false
    revision : int = 0
    saved_revision : int = 0
}

struct public TextEdit {
    source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    replacement : string
    expected : string
    verify_expected : bool = false
    base_revision : int = -1
}

struct public TextEditResult {
    status : TextDocumentStatus = TextDocumentStatus.ok
    applied : bool = false
    revision : int = -1
    removed : string
    inverse : TextEdit = TextEdit()
}

enum public RichNodeKind {
    document
    quote
    unordered_list
    ordered_list
    list_item
    thematic_break
    heading
    code_block
    raw_html
    paragraph
    table_
    table_head
    table_body
    table_row
    table_header_cell
    table_cell
    emphasis
    strong
    link
    image
    inline_code
    strikethrough
    latex_math
    latex_math_display
    wiki_link
    underline
    text
    null_character
    hard_break
    soft_break
    entity
    code_text
    html_text
    latex_text
    unknown
}

enum public RichAlignment {
    default_
    left
    center
    right
}

struct public RichNode {
    kind : RichNodeKind = RichNodeKind.unknown
    parent : int = -1
    first_child : int = -1
    last_child : int = -1
    next_sibling : int = -1
    source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    source_exact : bool = false
    control_source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    level : int = 0
    marker : int = 0
    start_number : int = 0
    alignment : RichAlignment = RichAlignment.default_
    column_count : int = 0
    head_row_count : int = 0
    body_row_count : int = 0
    is_tight : bool = false
    is_task : bool = false
    is_checked : bool = false
    is_autolink : bool = false
    literal : string
    destination : string
    title : string
    info : string
    language : string
}

struct public RichDocument {
    //! Format adapters produce this flat semantic tree. `source` always points
    //! back to the canonical TextDocumentSource text for exact selection/edit
    //! anchors; generated documents may instead use literal leaves.
    source : string
    nodes : array<RichNode>
    root : int = -1
    ok : bool = false
    error : string
}

struct public RichInlineRun {
    display : TextSourceRange
    source : TextSourceRange
    source_exact : bool = false
    node_index : int = -1
    semantic_node : int = -1
    is_strong : bool = false
    is_emphasis : bool = false
    is_code : bool = false
    is_strikethrough : bool = false
    is_link : bool = false
    is_automatic_uri : bool = false
    is_image : bool = false
    image_alt : string
    destination : string
    title : string
}

struct public RichInlineContent {
    text : string
    runs : array<RichInlineRun>
}

struct public RichTextPosition {
    valid : bool = false
    inline_node : int = -1
    display_byte : int = -1
    source_byte : int = -1
    trailing : bool = false
}

enum public RichControlKind {
    none
    toggle
}

enum public RichSourceMapping {
    none
    derived
    exact
}

struct public RichControl {
    id : int = -1
    kind : RichControlKind = RichControlKind.none
    source : TextSourceRange = TextSourceRange(start_byte = -1, end_byte = -1)
    source_mapping : RichSourceMapping = RichSourceMapping.none
    checked : bool = false
    payload : int = -1
}

struct public RichDocumentProjection {
    //! This small first projection seam intentionally starts with controls.
    //! Blocks and inline runs can migrate behind the same revision/source-map
    //! contract without changing source ownership or edit semantics.
    source_revision : int = -1
    controls : array<RichControl>
}

struct public RichControlEvent {
    active : bool = false
    control_id : int = -1
    kind : RichControlKind = RichControlKind.none
    requested_checked : bool = false
    source_revision : int = -1
}

struct public RichControlAction {
    status : TextDocumentStatus = TextDocumentStatus.ok
    valid : bool = false
    edit : TextEdit = TextEdit()
}

def private utf8_bom() : string {
    return "\xEF\xBB\xBF"
}

def public text_document_install_bytes(var document : TextDocumentSource;
                                       bytes, path : string;
                                       revision : int = 1) : TextDocumentStatus {
    let has_bom = length(bytes) >= 3 && contains_utf8_bom(bytes)
    let content = has_bom ? slice(bytes, 3) : bytes
    if (!is_utf8_string_valid(content)) return TextDocumentStatus.invalid_utf8
    document.text = content
    document.path = path
    document.has_utf8_bom = has_bom
    document.revision = max(revision, 1)
    document.saved_revision = document.revision
    return TextDocumentStatus.ok
}

def public text_document_install_text(var document : TextDocumentSource;
                                      text, path : string;
                                      revision : int = 1) : TextDocumentStatus {
    if (!is_utf8_string_valid(text)) return TextDocumentStatus.invalid_utf8
    document.text = text
    document.path = path
    document.has_utf8_bom = false
    document.revision = max(revision, 1)
    document.saved_revision = document.revision
    return TextDocumentStatus.ok
}

def public text_document_bytes(document : TextDocumentSource const) : string {
    return document.has_utf8_bom ? utf8_bom() + document.text : document.text
}

def public text_document_dirty(document : TextDocumentSource const) : bool {
    return document.revision != document.saved_revision
}

def public text_document_mark_saved(var document : TextDocumentSource;
                                    path : string = "") {
    if (!empty(path)) {
        document.path = path
    }
    document.saved_revision = document.revision
}

def public text_document_apply_edit(var document : TextDocumentSource;
                                    edit : TextEdit const) : TextEditResult {
    var result = TextEditResult(revision = document.revision)
    if (edit.base_revision >= 0 && edit.base_revision != document.revision) {
        result.status = TextDocumentStatus.stale_revision
        return <- result
    }
    let start_byte = edit.source.start_byte
    let end_byte = edit.source.end_byte
    if (start_byte < 0 || end_byte < start_byte || end_byte > length(document.text)) {
        result.status = TextDocumentStatus.invalid_range
        return <- result
    }
    let removed = slice(document.text, start_byte, end_byte)
    if (edit.verify_expected && removed != edit.expected) {
        result.status = TextDocumentStatus.unexpected_source
        return <- result
    }
    let prefix = start_byte > 0 ? slice(document.text, 0, start_byte) : ""
    let suffix = end_byte < length(document.text) ? slice(document.text, end_byte) : ""
    let old_revision = document.revision
    document.text = prefix + edit.replacement + suffix
    document.revision++
    result.status = TextDocumentStatus.ok
    result.applied = true
    result.revision = document.revision
    result.removed = removed
    result.inverse = TextEdit(
        source = TextSourceRange(start_byte = start_byte,
                                 end_byte = start_byte + length(edit.replacement)),
        replacement = removed,
        expected = edit.replacement,
        verify_expected = true,
        base_revision = old_revision + 1)
    return <- result
}

def public text_position_after_edit(position : int; edit : TextEdit const;
                                    bias : TextPositionBias = TextPositionBias.after) : int {
    let start_byte = edit.source.start_byte
    let end_byte = edit.source.end_byte
    let delta = length(edit.replacement) - (end_byte - start_byte)
    if (position < start_byte) return position
    if (position > end_byte) return position + delta
    if (position == start_byte && bias == TextPositionBias.before) return start_byte
    return start_byte + length(edit.replacement)
}

def public text_range_after_edit(source : TextSourceRange const; edit : TextEdit const;
                                 start_bias : TextPositionBias = TextPositionBias.before;
                                 end_bias : TextPositionBias = TextPositionBias.after) : TextSourceRange {
    return TextSourceRange(
        start_byte = text_position_after_edit(source.start_byte, edit, start_bias),
        end_byte = text_position_after_edit(source.end_byte, edit, end_bias))
}

def public rich_document_projection_dispose(var projection : RichDocumentProjection) {
    delete projection.controls
    projection = RichDocumentProjection()
}

def public rich_plain_document(source : string) : RichDocument {
    //! Minimal non-Markdown producer used by plain/rich viewers and tests. It
    //! deliberately assigns no Markdown semantics: marker-looking text such as
    //! `**literal**` remains a regular exact source leaf.
    var document = RichDocument(source = source)
    if (!is_utf8_string_valid(source)) {
        document.error = "Rich document source is not valid UTF-8"
        return <- document
    }
    let source_range = TextSourceRange(start_byte = 0, end_byte = length(source))
    document.nodes <- [
        RichNode(
            kind = RichNodeKind.document,
            first_child = 1, last_child = 1,
            source = source_range),
        RichNode(
            kind = RichNodeKind.paragraph,
            parent = 0, first_child = 2, last_child = 2,
            source = source_range),
        RichNode(
            kind = RichNodeKind.text,
            parent = 1,
            source = source_range,
            source_exact = true)
    ]
    document.root = 0
    document.ok = true
    return <- document
}

def public rich_document_dispose(var document : RichDocument) {
    delete document.nodes
    document = RichDocument()
}

def public rich_document_control(projection : RichDocumentProjection const;
                                 control_id : int) : RichControl {
    for (control in projection.controls) {
        if (control.id == control_id) return control
    }
    return RichControl()
}
