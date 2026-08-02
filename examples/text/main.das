options gen2
options gc
options persistent_heap
options indenting = 4

require imgui/imgui_harness
require imgui/imgui_file_dialog
require imgui/imgui_fonts
require imgui/imgui_icons
require imgui/imgui_commands
require imgui/imgui_markdown_tree_sitter
require imgui/imgui_document_model
require imgui/imgui_text_find
require imgui/imgui_text_source_view
require imgui/imgui_text_source_edit
require imgui/imgui_text_edit_syntax
require imgui/imgui_text_edit_complete
require imgui/imgui_text_tree_sitter
require imgui/imgui_theme_daslang
require daslib/clargs
require daslib/fio
require daslib/json
require daslib/json_boost
require daslib/module_path
require daslib/safe_addr
require daslib/stringify
require live/live_commands
require math
require strings

//! Small user-facing Unicode text and Markdown viewer.
//!
//! Interpreted:
//!   daslang examples/text/main.das -- README.md
//!   daslang examples/text/main.das -- --plain LICENSE
//!
//! A standalone executable accepts the same path without the `--` separator.

enum TextViewMode {
    markdown
    plain_text
}

enum ViewerFileDialogAction {
    none
    open
    save_as
}

struct TextViewerOpenArgs {
    path : string
}

struct TextViewerModeArgs {
    mode : string
}

struct TextViewerZoomArgs {
    percent : int
}

struct TextViewerSourceRangeArgs {
    text : string
}

struct TextViewerCheckboxArgs {
    index : int = 0
}

struct TextViewerCommandArgs {
    id : string
}

struct TextViewerFindArgs {
    query : string
    regex : bool = false
}

struct TextViewerFindStepArgs {
    delta : int = 1
}

struct TextViewerFoldArgs {
    index : int = -1
    collapsed : bool = true
}

struct TextViewerEditArgs {
    on : bool = true
}

struct TextViewerDocument {
    id : int
    pending_dock : bool = true
    source : TextDocumentSource = TextDocumentSource()
    document : RichDocument = RichDocument()
    projection : RichDocumentProjection = RichDocumentProjection()
    markdown_view : MarkdownViewState = MarkdownViewState()
    markdown_syntax : MarkdownTreeSitterState = MarkdownTreeSitterState()
    source_document : TextSourceDocument = TextSourceDocument()
    source_view : TextSourceViewState = TextSourceViewState()
    source_result : TextSourceViewResult = TextSourceViewResult()
    status : string = "Open a UTF-8 text or Markdown file."
    mode : TextViewMode = TextViewMode.markdown
    mode_override : int = -1
    syntax_mode : int = 0
    zoom_step : int = 20       // 10..60 maps to 50%..300% in exact 5% steps
    find : TextFindState = TextFindState()
    find_applied_jump : int = 0
    find_scroll_byte : int = -1
    find_scroll_markdown : bool = false
    edit : TextEditState = TextEditState()
    edit_complete : TextEditCompletion = TextEditCompletion()
    editing : bool = false
    read_only : bool = false
    // markdown live preview while editing — separate from the view-mode
    // markdown state so viewer scroll/folds survive an editing session
    preview_document : RichDocument = RichDocument()
    preview_view : MarkdownViewState = MarkdownViewState()
    preview_syntax : MarkdownTreeSitterState = MarkdownTreeSitterState()
    preview_revision : int = -1
    preview_stamp : int = 0
    preview_follow : bool = false
    preview_caret_applied : int = 0
    preview_caret_byte : int = -1
    preview_caret_stamp : int = -1
    disk_mtime : clock
    disk_size : uint64
    disk_check_accum : float = 0.0f
    external_change : bool = false
    content_view_top : float = 0.0f
    window_scroll_max : float = 0.0f
    footer_overflow : float = 0.0f
    content_view_bottom : float = 0.0f
}

var private g_fonts = FontRoleSet()
var private g_font_sources : array<string>
var private g_documents : array<TextViewerDocument>
var private g_document_windows : table<int; DockWindowState>
var private g_active_document_id = 0
var private g_next_document_id = 1
var private g_initial_path : string
var private g_initial_mode_override = -1
var private g_last_open_error : string
var private g_open_clicks = 0
var private g_reload_clicks = 0
var private g_quit_clicks = 0
var private g_markdown_clicks = 0
var private g_plain_clicks = 0
var private g_save_clicks = 0
var private g_save_as_clicks = 0
var private g_file_dialog_action = ViewerFileDialogAction.none
var private g_commands = CommandRegistry()
var private g_bindings_editor = CommandBindingsEditorState()
var private g_last_command : string
var private g_command_revision = 0
var private g_edit_copy_clicks = 0
var private g_edit_select_all_clicks = 0
var private g_edit_clear_clicks = 0
var private g_edit_bindings_clicks = 0
var private g_edit_find_clicks = 0
var private g_edit_find_next_clicks = 0
var private g_edit_find_prev_clicks = 0
var private g_file_new_clicks = 0
var private g_edit_mode_clicks = 0
var private g_edit_undo_clicks = 0
var private g_edit_redo_clicks = 0

let FOOTER_ICON_SIZE = 18.0f

let SYNTAX_MODE_LABELS : array<string> <- [
    "Auto", "Plain", "daScript", "C", "C++", "Markdown"
]
let SYNTAX_MODE_VALUES : array<string> <- [
    "auto", "none", "das", "c", "cpp", "markdown"
]

let CMD_FILE_NEW = "file.new"
let CMD_FILE_OPEN = "file.open"
let CMD_FILE_SAVE = "file.save"
let CMD_FILE_SAVE_AS = "file.save_as"
let CMD_FILE_RELOAD = "file.reload"
let CMD_APP_QUIT = "app.quit"
let CMD_VIEW_MARKDOWN = "view.markdown"
let CMD_VIEW_PLAIN = "view.plain"
let CMD_VIEW_TOGGLE = "view.toggle_source"
// outside the "edit." prefix: the editor widget drains every queued "edit.*"
// invocation, so a mode toggle named edit.* would be eaten while editing
let CMD_EDIT_MODE = "view.edit_mode"
let CMD_VIEW_ZOOM_IN = "view.zoom_in"
let CMD_VIEW_ZOOM_OUT = "view.zoom_out"
let CMD_VIEW_ZOOM_RESET = "view.zoom_reset"
let CMD_EDIT_COPY = "edit.copy"
let CMD_EDIT_SELECT_ALL = "edit.select_all"
let CMD_EDIT_CLEAR_SELECTION = "edit.clear_selection"
let CMD_EDIT_BINDINGS = "preferences.edit_bindings"
let CMD_FIND = "find.open"
let CMD_FIND_NEXT = "find.next"
let CMD_FIND_PREV = "find.prev"

def private mode_name(doc : TextViewerDocument const) : string {
    return doc.mode == TextViewMode.markdown ? "markdown" : "plain"
}

def private zoom_percent(doc : TextViewerDocument const) : int {
    return doc.zoom_step * 5
}

def private set_zoom_percent(var doc : TextViewerDocument; percent : int) {
    doc.zoom_step = clamp((percent + 2) / 5, 10, 60)
}

def private active_document_index() : int {
    let index = find_index_if(g_documents) $(doc) => doc.id == g_active_document_id
    return index >= 0 ? index : (!empty(g_documents) ? 0 : -1)
}

def private markdown_path(path : string) : bool {
    let lower = to_lower(path)
    return ends_with(lower, ".md") || ends_with(lower, ".markdown")
}

def private set_mode(var doc : TextViewerDocument; mode : TextViewMode) {
    if (doc.mode == mode) return
    doc.mode = mode
    markdown_view_clear_selection(doc.markdown_view)
    text_source_view_clear_selection(doc.source_view)
}

def private document_dirty(doc : TextViewerDocument const) : bool {
    return text_document_dirty(doc.source) || (doc.editing && doc.edit.dirty)
}

def private edit_language_for(path : string) : string {
    return markdown_path(path) ? "markdown" : extension(path)
}

def private record_disk_stamp(var doc : TextViewerDocument) {
    doc.external_change = false
    doc.disk_check_accum = 0.0f
    return if (empty(doc.source.path))
    let info = stat(doc.source.path)
    if (info.is_valid) {
        doc.disk_mtime = info.mtime
        doc.disk_size = info.size
    }
}

def private watch_external_change(var doc : TextViewerDocument) {
    // detect-and-warn only: a dirty buffer is never auto-reloaded (E5 ruling)
    return if (empty(doc.source.path) || doc.external_change)
    let io & = unsafe(GetIO())
    doc.disk_check_accum += io.DeltaTime
    return if (doc.disk_check_accum < 2.0f)
    doc.disk_check_accum = 0.0f
    let info = stat(doc.source.path)
    if (!info.is_valid) {
        // deletion is an external change too - Save re-creates the file
        doc.external_change = true
        doc.status = "File no longer exists on disk - Save re-creates it."
        return
    }
    if (info.mtime != doc.disk_mtime || info.size != doc.disk_size) {
        doc.external_change = true
        doc.status = (document_dirty(doc)
            ? "File changed on disk - saving will overwrite it; Reload discards your edits."
            : "File changed on disk - use Reload to refresh.")
    }
}

def private seed_editor_from_source(var doc : TextViewerDocument) {
    text_edit_load(doc.edit, doc.source.text)
    text_edit_set_language(doc.edit, edit_language_for(doc.source.path))
    text_edit_set_file_name(doc.edit,
        empty(doc.source.path) ? "untitled" : base_name(doc.source.path))
    doc.preview_revision = -1
}

def private sync_edit_to_document(var doc : TextViewerDocument) : bool {
    let text = text_edit_source(doc.edit)
    return false if (text == doc.source.text)
    doc.source.text = text
    doc.source.revision++
    rebuild_projection(doc)
    return true
}

def private enter_edit_mode(var doc : TextViewerDocument) : bool {
    if (doc.editing) return true
    if (doc.read_only) {
        doc.status = "This document is read-only."
        return false
    }
    seed_editor_from_source(doc)
    // carry the viewer selection into the editor when the byte domains agree
    // (the editor is LF-joined; a CRLF file's view offsets do not map 1:1)
    if (!doc.edit.crlf) {
        var start_byte = -1
        var end_byte = -1
        if (doc.mode == TextViewMode.markdown) {
            if (doc.markdown_view.selection_anchor.valid
                    && doc.markdown_view.selection_focus.valid) {
                let span = md_text_selection_range(doc.markdown_view.selection_anchor,
                    doc.markdown_view.selection_focus)
                start_byte = span.start_byte
                end_byte = span.end_byte
            }
        } else {
            let selection = text_source_view_selection(doc.source_view)
            start_byte = selection.start_byte
            end_byte = selection.end_byte
        }
        if (start_byte >= 0 && end_byte >= start_byte) {
            let span = text_edit_range_of_bytes(doc.edit, start_byte, end_byte)
            text_edit_select(doc.edit, span.start_line, span.start_column,
                span.end_line, span.end_column)
        }
    }
    doc.editing = true
    // pre-arm the editor's keyboard: the widget reclaims item focus and
    // drains typed chars the same frame it first draws
    doc.edit.focused = true
    doc.status = "Editing."
    return true
}

def private release_preview(var doc : TextViewerDocument) {
    markdown_view_invalidate(doc.preview_view)
    delete doc.preview_document.nodes
    doc.preview_document = RichDocument()
    doc.preview_revision = -1
}

def private exit_edit_mode(var doc : TextViewerDocument) {
    return if (!doc.editing)
    let _ = sync_edit_to_document(doc)
    doc.editing = false
    release_preview(doc)
    doc.status = "Viewing."
}

def private release_document_content(var doc : TextViewerDocument) {
    markdown_view_invalidate(doc.markdown_view)
    delete doc.document.nodes
    doc.document = RichDocument()
    rich_document_projection_dispose(doc.projection)
    text_source_view_invalidate(doc.source_view)
    text_source_document_dispose(doc.source_document)
}

def private dispose_document(var doc : TextViewerDocument) {
    markdown_tree_sitter_dispose(doc.markdown_syntax)
    markdown_view_dispose(doc.markdown_view)
    text_source_view_dispose(doc.source_view)
    text_find_dispose(doc.find)
    text_edit_dispose(doc.edit)
    text_edit_complete_dispose(doc.edit_complete)
    release_preview(doc)
    markdown_view_dispose(doc.preview_view)
    markdown_tree_sitter_dispose(doc.preview_syntax)
    release_document_content(doc)
}

def private install_document(var viewer : TextViewerDocument;
                             var next_source : TextDocumentSource) {
    var next_document <- md_parse(next_source.text)
    var next_projection <- md_rich_projection(next_document, next_source.revision)
    var next_source_document <- (markdown_path(next_source.path)
        ? markdown_source_document(next_source.text, next_source.revision)
        : tree_sitter_source_document(next_source.text,
                                      extension(next_source.path),
                                      next_source.revision))
    if (markdown_path(next_source.path)) {
        tree_sitter_collect_folds(next_source_document, "markdown", next_source.text)
    }
    release_document_content(viewer)
    viewer.source <- next_source
    viewer.document <- next_document
    viewer.projection <- next_projection
    viewer.source_document <- next_source_document

    if (viewer.mode_override == int(TextViewMode.markdown)) {
        viewer.mode = TextViewMode.markdown
    } elif (viewer.mode_override == int(TextViewMode.plain_text)) {
        viewer.mode = TextViewMode.plain_text
    } else {
        viewer.mode = markdown_path(viewer.source.path) ? TextViewMode.markdown : TextViewMode.plain_text
    }
    viewer.status = (viewer.document.ok
        ? "Loaded {length(text_document_bytes(viewer.source))} UTF-8 bytes."
        : viewer.document.error)
}

def private rebuild_projection(var viewer : TextViewerDocument) {
    var next_document <- md_parse(viewer.source.text)
    var next_projection <- md_rich_projection(next_document, viewer.source.revision)
    var next_source_document <- (markdown_path(viewer.source.path)
        ? markdown_source_document(viewer.source.text, viewer.source.revision)
        : tree_sitter_source_document(viewer.source.text,
                                      extension(viewer.source.path),
                                      viewer.source.revision))
    if (markdown_path(viewer.source.path)) {
        tree_sitter_collect_folds(next_source_document, "markdown", viewer.source.text)
    }
    release_document_content(viewer)
    viewer.document <- next_document
    viewer.projection <- next_projection
    viewer.source_document <- next_source_document
}

def private install_source_text(var viewer : TextViewerDocument;
                                path, text : string) : bool {
    var next_source = TextDocumentSource()
    let status = text_document_install_text(
        next_source, text, path, viewer.source.revision + 1)
    if (status != TextDocumentStatus.ok) return false
    install_document(viewer, next_source)
    return true
}

def private load_path(var viewer : TextViewerDocument; path : string) : bool {
    if (empty(path)) {
        viewer.status = "No file path was provided."
        return false
    }
    if (!fexist(path)) {
        viewer.status = "File does not exist: {path}"
        return false
    }
    let file_info = stat(path)
    if (!file_info.is_valid || file_info.is_dir) {
        viewer.status = "Path is not a regular file: {path}"
        return false
    }
    let bytes = fread(path)
    var next_source = TextDocumentSource()
    let source_status = text_document_install_bytes(
        next_source, bytes, path, viewer.source.revision + 1)
    if (source_status != TextDocumentStatus.ok) {
        viewer.status = "File is not valid UTF-8: {path}"
        return false
    }
    install_document(viewer, next_source)
    viewer.read_only = false
    record_disk_stamp(viewer)
    if (viewer.editing) {
        // explicit load/reload replaces the edit buffer (and its history)
        seed_editor_from_source(viewer)
    }
    return true
}

def private save_path(var viewer : TextViewerDocument; path : string) : bool {
    if (empty(path)) {
        viewer.status = "No save path was provided."
        return false
    }
    if (!fwrite(path, text_document_bytes(viewer.source))) {
        viewer.status = "Could not save: {path}"
        return false
    }
    text_document_mark_saved(viewer.source, path)
    viewer.status = "Saved {length(text_document_bytes(viewer.source))} UTF-8 bytes."
    return true
}

def private save_document(var viewer : TextViewerDocument; path : string) : bool {
    if (viewer.editing) {
        let _ = sync_edit_to_document(viewer)
    }
    return false if (!save_path(viewer, path))
    record_disk_stamp(viewer)
    viewer.read_only = false
    if (viewer.editing) {
        text_edit_mark_saved(viewer.edit)
        // Save As can change the extension - keep the language honest
        text_edit_set_language(viewer.edit, edit_language_for(viewer.source.path))
        text_edit_set_file_name(viewer.edit, base_name(viewer.source.path))
    }
    return true
}

def private apply_control_event(var viewer : TextViewerDocument;
                                event : RichControlEvent const) : bool {
    let action <- md_control_action(viewer.document, viewer.projection, event)
    if (!action.valid) {
        viewer.status = "Checkbox action was rejected ({int(action.status)})."
        return false
    }
    var old_anchor := viewer.markdown_view.selection_anchor
    var old_focus := viewer.markdown_view.selection_focus
    let changed <- text_document_apply_edit(viewer.source, action.edit)
    if (!changed.applied) {
        viewer.status = "Checkbox edit was rejected ({int(changed.status)})."
        return false
    }
    rebuild_projection(viewer)
    if (old_anchor.valid && old_anchor.inline_node >= 0
            && old_anchor.inline_node < length(viewer.document.nodes)) {
        old_anchor.source_byte = text_position_after_edit(
            old_anchor.source_byte, action.edit, TextPositionBias.before)
        viewer.markdown_view.selection_anchor = old_anchor
    }
    if (old_focus.valid && old_focus.inline_node >= 0
            && old_focus.inline_node < length(viewer.document.nodes)) {
        old_focus.source_byte = text_position_after_edit(
            old_focus.source_byte, action.edit, TextPositionBias.after)
        viewer.markdown_view.selection_focus = old_focus
    }
    viewer.status = "Modified."
    return true
}

def private install_welcome_document(var viewer : TextViewerDocument) {
    let welcome = %stringify~# Text viewer

Open a UTF-8 file from **File / Open...**, or pass its path on the command line.

- `.md` and `.markdown` files open as rendered Markdown.
- Other files open as regular text.
- Switch modes at any time with **View** or the buttons below.
- Use the bottom-right slider or Ctrl+mouse-wheel to zoom.
%%
    let _ = install_source_text(viewer, "", welcome)
    viewer.mode = TextViewMode.markdown
    viewer.read_only = true
    viewer.status = "No file selected."
}

def private add_untitled_document() {
    let active = active_document_index()
    let inherited_zoom_step = active >= 0 ? g_documents[active].zoom_step : 20
    var viewer = TextViewerDocument(id = g_next_document_id,
                                    zoom_step = inherited_zoom_step)
    g_next_document_id++
    let _ = install_source_text(viewer, "", "")
    viewer.mode = TextViewMode.plain_text
    viewer.status = "New document."
    g_active_document_id = viewer.id
    g_document_windows[viewer.id] = DockWindowState(open = true)
    g_documents |> emplace(viewer)
    let index = active_document_index()
    if (index >= 0) {
        let _entered = enter_edit_mode(g_documents[index])
    }
}

def private add_document(path : string; mode_override : int = -1;
                         welcome_on_failure : bool = false) : bool {
    let active = active_document_index()
    let inherited_zoom_step = active >= 0 ? g_documents[active].zoom_step : 20
    var viewer = TextViewerDocument(id = g_next_document_id,
                                    mode_override = mode_override,
                                    zoom_step = inherited_zoom_step)
    g_next_document_id++
    if (empty(path)) {
        install_welcome_document(viewer)
    } elif (!load_path(viewer, path)) {
        g_last_open_error = viewer.status
        if (!welcome_on_failure) {
            dispose_document(viewer)
            return false
        }
        let error = viewer.status
        install_welcome_document(viewer)
        viewer.status = error
    }
    g_active_document_id = viewer.id
    g_document_windows[viewer.id] = DockWindowState(open = true)
    g_documents |> emplace(viewer)
    return true
}

def private open_file_dialog() {
    let active = active_document_index()
    var start_dir = ""
    if (active >= 0 && !empty(g_documents[active].source.path)) {
        start_dir = dir_name(g_documents[active].source.path)
    }
    g_file_dialog_action = ViewerFileDialogAction.open
    file_dialog_open(FileDialogConfig(
        title = "Open text or Markdown",
        start_dir = start_dir,
        filters <- file_filters(
            "Text and Markdown|md;markdown;txt;rst;das|Markdown|md;markdown|All files|"),
        mode = FileDialogMode.open,
        max_selection = 1))
}

def private save_file_dialog() {
    let active = active_document_index()
    if (active < 0) return
    let path = g_documents[active].source.path
    g_file_dialog_action = ViewerFileDialogAction.save_as
    file_dialog_open(FileDialogConfig(
        title = "Save text or Markdown",
        start_dir = empty(path) ? "" : dir_name(path),
        default_name = empty(path) ? "document.md" : base_name(path),
        filters <- file_filters(
            "Markdown|md;markdown|Text|txt;rst;das|All files|"),
        mode = FileDialogMode.save,
        max_selection = 1))
}

def private save_active_document(bool_save_as : bool = false) : bool {
    let active = active_document_index()
    if (active < 0) return false
    let path = g_documents[active].source.path
    if (bool_save_as || empty(path)) {
        save_file_dialog()
        return false
    }
    return save_document(g_documents[active], path)
}

def private setup_default_layout(dock_id : uint) {
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let viewport = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, viewport.Size)
    DockBuilderFinish(dock_id)
}

def private parse_command_line() {
    let args <- get_user_args()
    for (arg in args) {
        if (arg == "--plain") {
            g_initial_mode_override = int(TextViewMode.plain_text)
        } elif (arg == "--markdown") {
            g_initial_mode_override = int(TextViewMode.markdown)
        } elif (!starts_with(arg, "--") && empty(g_initial_path)) {
            g_initial_path = arg
        }
    }
}

def private define_find_commands() {
    command_define(g_commands, CommandDefinition(
        id = CMD_FIND, title = "Find...", category = "Edit",
        scope = CommandScope.document),
        command_binding(CMD_FIND, ImGuiKey.F, primary = true,
                        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_FIND_NEXT, title = "Find Next", category = "Edit",
        scope = CommandScope.document),
        command_binding(CMD_FIND_NEXT, ImGuiKey.F3,
                        scope = CommandScope.document, repeat = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_FIND_PREV, title = "Find Previous", category = "Edit",
        scope = CommandScope.document),
        command_binding(CMD_FIND_PREV, ImGuiKey.F3, shift = true,
                        scope = CommandScope.document, repeat = true))
}

def private define_file_commands() {
    command_define(g_commands, CommandDefinition(
        id = CMD_FILE_NEW, title = "New", category = "File",
        description = "Create an empty editable document."),
        command_binding(CMD_FILE_NEW, ImGuiKey.N, primary = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_FILE_OPEN, title = "Open...", category = "File",
        description = "Open a file in a new docked document."),
        command_binding(CMD_FILE_OPEN, ImGuiKey.O, primary = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_FILE_SAVE, title = "Save", category = "File"),
        command_binding(CMD_FILE_SAVE, ImGuiKey.S, primary = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_FILE_SAVE_AS, title = "Save As...", category = "File"),
        command_binding(CMD_FILE_SAVE_AS, ImGuiKey.S, primary = true, shift = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_FILE_RELOAD, title = "Reload", category = "File",
        scope = CommandScope.document),
        command_binding(CMD_FILE_RELOAD, ImGuiKey.R, primary = true,
                        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_APP_QUIT, title = "Quit", category = "Application"),
        command_binding(CMD_APP_QUIT, ImGuiKey.Q, primary = true))
}

def private define_commands() {
    // ImGuiKey's digit enumerators cannot be named directly in daslang because
    // `.0` parses as a float literal. ImGui keeps the digit row immediately
    // before A, so derive the key for Ctrl+0 from that stable enum sequence.
    let key_0 = unsafe(reinterpret<ImGuiKey>(int(ImGuiKey.A) - 10))
    g_commands.config_path = command_default_config_path("text-viewer")
    define_file_commands()
    command_define(g_commands, CommandDefinition(
        id = CMD_VIEW_MARKDOWN, title = "Markdown", category = "View",
        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_VIEW_PLAIN, title = "Plain text", category = "View",
        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_VIEW_TOGGLE, title = "Toggle rendered/source", category = "View",
        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_EDIT_MODE, title = "Edit Mode", category = "View",
        description = "Toggle between viewing and editing.",
        scope = CommandScope.document),
        command_binding(CMD_EDIT_MODE, ImGuiKey.E, primary = true,
                        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_VIEW_ZOOM_IN, title = "Zoom In", category = "View",
        scope = CommandScope.document),
        command_binding(CMD_VIEW_ZOOM_IN, ImGuiKey.Equal, primary = true,
                        scope = CommandScope.document, repeat = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_VIEW_ZOOM_OUT, title = "Zoom Out", category = "View",
        scope = CommandScope.document),
        command_binding(CMD_VIEW_ZOOM_OUT, ImGuiKey.Minus, primary = true,
                        scope = CommandScope.document, repeat = true))
    command_define(g_commands, CommandDefinition(
        id = CMD_VIEW_ZOOM_RESET, title = "Reset Zoom", category = "View",
        scope = CommandScope.document),
        command_binding(CMD_VIEW_ZOOM_RESET, key_0, primary = true,
                        scope = CommandScope.document))
    command_define(g_commands, CommandDefinition(
        id = CMD_EDIT_COPY, title = "Copy", category = "Edit",
        scope = CommandScope.component),
        command_binding(CMD_EDIT_COPY, ImGuiKey.C, primary = true,
                        scope = CommandScope.component))
    command_define(g_commands, CommandDefinition(
        id = CMD_EDIT_SELECT_ALL, title = "Select All", category = "Edit",
        scope = CommandScope.component),
        command_binding(CMD_EDIT_SELECT_ALL, ImGuiKey.A, primary = true,
                        scope = CommandScope.component))
    command_define(g_commands, CommandDefinition(
        id = CMD_EDIT_CLEAR_SELECTION, title = "Clear Selection", category = "Edit",
        scope = CommandScope.component))
    define_find_commands()
    command_define(g_commands, CommandDefinition(
        id = CMD_EDIT_BINDINGS, title = "Edit Bindings...", category = "Preferences"))
    // after the app's own definitions (its find.open / edit.copy shapes win the
    // idempotence race), before the config load (so user overrides apply)
    text_edit_register_commands(g_commands)
    if (!harness_is_headless() && fexist(g_commands.config_path)) {
        let _ = command_load_bindings(g_commands)
    }
}

def private document_has_selection(doc : TextViewerDocument const) : bool {
    if (doc.mode == TextViewMode.markdown) {
        return (doc.markdown_view.selection_anchor.valid
            && doc.markdown_view.selection_focus.valid
            && doc.markdown_view.selection_anchor.source_byte != doc.markdown_view.selection_focus.source_byte)
    }
    let selection = text_source_view_selection(doc.source_view)
    return selection.start_byte >= 0 && selection.end_byte > selection.start_byte
}

def private update_command_state() {
    let active = active_document_index()
    let has_active = active >= 0
    let has_path = has_active && !empty(g_documents[active].source.path)
    let editing = has_active && g_documents[active].editing
    // while editing, Copy/Clear track the EDITOR's selection - the app's
    // enabled state gates the editor's own Ctrl+C poll
    let has_selection = has_active && (editing
        ? text_edit_has_selection(g_documents[active].edit)
        : document_has_selection(g_documents[active]))
    command_set_enabled(g_commands, CMD_FILE_SAVE, has_active)
    command_set_enabled(g_commands, CMD_FILE_SAVE_AS, has_active)
    command_set_enabled(g_commands, CMD_FILE_RELOAD, has_path)
    command_set_enabled(g_commands, CMD_VIEW_MARKDOWN, has_active)
    command_set_enabled(g_commands, CMD_VIEW_PLAIN, has_active)
    command_set_enabled(g_commands, CMD_VIEW_TOGGLE, has_active)
    command_set_enabled(g_commands, CMD_VIEW_ZOOM_IN, has_active)
    command_set_enabled(g_commands, CMD_VIEW_ZOOM_OUT, has_active)
    command_set_enabled(g_commands, CMD_VIEW_ZOOM_RESET, has_active)
    command_set_enabled(g_commands, CMD_EDIT_COPY, has_selection)
    command_set_enabled(g_commands, CMD_EDIT_SELECT_ALL, has_active)
    command_set_enabled(g_commands, CMD_EDIT_CLEAR_SELECTION, has_selection)
    command_set_enabled(g_commands, CMD_FIND, has_active)
    command_set_enabled(g_commands, CMD_FIND_NEXT, has_active)
    command_set_enabled(g_commands, CMD_FIND_PREV, has_active)
    command_set_selected(g_commands, CMD_VIEW_MARKDOWN,
        has_active && g_documents[active].mode == TextViewMode.markdown)
    command_set_selected(g_commands, CMD_VIEW_PLAIN,
        has_active && g_documents[active].mode == TextViewMode.plain_text)
    command_set_enabled(g_commands, CMD_EDIT_MODE,
        has_active && !g_documents[active].read_only)
    command_set_selected(g_commands, CMD_EDIT_MODE, editing)
}

def private dispatch_document_command(command_id : string; active : int) : bool {
    if (active < 0) return false
    if (command_id == CMD_VIEW_MARKDOWN) {
        set_mode(g_documents[active], TextViewMode.markdown)
    } elif (command_id == CMD_VIEW_PLAIN) {
        set_mode(g_documents[active], TextViewMode.plain_text)
    } elif (command_id == CMD_VIEW_TOGGLE) {
        set_mode(g_documents[active], g_documents[active].mode == TextViewMode.markdown
            ? TextViewMode.plain_text : TextViewMode.markdown)
    } elif (command_id == CMD_VIEW_ZOOM_IN) {
        g_documents[active].zoom_step = min(g_documents[active].zoom_step + 1, 60)
    } elif (command_id == CMD_VIEW_ZOOM_OUT) {
        g_documents[active].zoom_step = max(g_documents[active].zoom_step - 1, 10)
    } elif (command_id == CMD_VIEW_ZOOM_RESET) {
        set_zoom_percent(g_documents[active], 100)
    } elif (command_id == CMD_EDIT_COPY) {
        if (g_documents[active].mode == TextViewMode.markdown) {
            markdown_view_request_copy(g_documents[active].markdown_view)
        } else {
            text_source_view_request_copy(g_documents[active].source_view)
        }
    } elif (command_id == CMD_EDIT_SELECT_ALL) {
        if (g_documents[active].mode == TextViewMode.markdown) {
            let _ = markdown_view_select_all(g_documents[active].markdown_view)
        } else {
            text_source_view_select_all(g_documents[active].source_document,
                                        g_documents[active].source_view)
        }
    } elif (command_id == CMD_EDIT_CLEAR_SELECTION) {
        if (g_documents[active].mode == TextViewMode.markdown) {
            markdown_view_clear_selection(g_documents[active].markdown_view)
        } else {
            text_source_view_clear_selection(g_documents[active].source_view)
        }
    } elif (command_id == CMD_EDIT_MODE) {
        if (g_documents[active].editing) {
            exit_edit_mode(g_documents[active])
        } else {
            let _ = enter_edit_mode(g_documents[active])
        }
    } elif (command_id == CMD_FIND) {
        text_find_open(g_documents[active].find)
    } elif (command_id == CMD_FIND_NEXT) {
        text_find_step(g_documents[active].find, 1)
    } elif (command_id == CMD_FIND_PREV) {
        text_find_step(g_documents[active].find, -1)
    } else {
        return false
    }
    return true
}

def private dispatch_commands() {
    var invocation = CommandInvocation()
    while (command_take(g_commands, invocation)) {
        g_last_command = invocation.command_id
        g_command_revision++
        let active = active_document_index()
        if (invocation.command_id == CMD_FILE_NEW) {
            add_untitled_document()
        } elif (invocation.command_id == CMD_FILE_OPEN) {
            open_file_dialog()
        } elif (invocation.command_id == CMD_FILE_SAVE) {
            let _ = save_active_document()
        } elif (invocation.command_id == CMD_FILE_SAVE_AS) {
            let _ = save_active_document(true)
        } elif (invocation.command_id == CMD_FILE_RELOAD && active >= 0
                && !empty(g_documents[active].source.path)) {
            let path = g_documents[active].source.path
            let _ = load_path(g_documents[active], path)
        } elif (invocation.command_id == CMD_APP_QUIT) {
            request_exit()
        } elif (invocation.command_id == CMD_EDIT_BINDINGS) {
            g_bindings_editor.open = true
        } else {
            let _ = dispatch_document_command(invocation.command_id, active)
        }
    }
}

def private draw_menu_bar() {
    let active = active_document_index()
    let has_active = active >= 0
    let editing = has_active && g_documents[active].editing
    main_menu_bar(MAIN_MENU) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
            menu_label(FILE_NEW, (text = "New",
                                  shortcut = command_shortcut_text(g_commands, CMD_FILE_NEW)))
            menu_label(FILE_OPEN, (text = "Open...",
                                   shortcut = command_shortcut_text(g_commands, CMD_FILE_OPEN)))
            menu_label(FILE_SAVE, (text = "Save",
                                   shortcut = command_shortcut_text(g_commands, CMD_FILE_SAVE),
                                   enabled = has_active))
            menu_label(FILE_SAVE_AS, (text = "Save As...",
                                      shortcut = command_shortcut_text(g_commands, CMD_FILE_SAVE_AS),
                                      enabled = has_active))
            menu_label(FILE_RELOAD, (text = "Reload",
                                     shortcut = command_shortcut_text(g_commands, CMD_FILE_RELOAD),
                                     enabled = has_active
                                         && !empty(g_documents[active].source.path)))
            separator(FILE_SEPARATOR)
            menu_label(FILE_QUIT, (text = "Quit",
                                   shortcut = command_shortcut_text(g_commands, CMD_APP_QUIT)))
        }
        menu(EDIT_MENU, (text = "Edit", enabled = true)) {
            menu_label(EDIT_UNDO, (text = "Undo",
                shortcut = command_shortcut_text(g_commands, "edit.undo"),
                enabled = editing && text_edit_can_undo(g_documents[active].edit)))
            menu_label(EDIT_REDO, (text = "Redo",
                shortcut = command_shortcut_text(g_commands, "edit.redo"),
                enabled = editing && text_edit_can_redo(g_documents[active].edit)))
            separator(EDIT_UNDO_SEPARATOR)
            menu_label(EDIT_COPY, (text = "Copy",
                shortcut = command_shortcut_text(g_commands, CMD_EDIT_COPY),
                enabled = command_enabled(g_commands, CMD_EDIT_COPY)))
            menu_label(EDIT_SELECT_ALL, (text = "Select All",
                shortcut = command_shortcut_text(g_commands, CMD_EDIT_SELECT_ALL),
                enabled = has_active))
            menu_label(EDIT_CLEAR_SELECTION, (text = "Clear Selection", shortcut = "",
                enabled = command_enabled(g_commands, CMD_EDIT_CLEAR_SELECTION)))
            separator(EDIT_FIND_SEPARATOR)
            menu_label(EDIT_FIND, (text = "Find...",
                shortcut = command_shortcut_text(g_commands, CMD_FIND),
                enabled = has_active))
            menu_label(EDIT_FIND_NEXT, (text = "Find Next",
                shortcut = command_shortcut_text(g_commands, CMD_FIND_NEXT),
                enabled = has_active))
            menu_label(EDIT_FIND_PREV, (text = "Find Previous",
                shortcut = command_shortcut_text(g_commands, CMD_FIND_PREV),
                enabled = has_active))
            separator(EDIT_SEPARATOR)
            menu_label(EDIT_BINDINGS, (text = "Edit Bindings...", shortcut = ""))
        }
        menu(VIEW_MENU, (text = "View", enabled = true)) {
            menu_label(VIEW_MARKDOWN, (text = "Markdown", shortcut = "",
                                      selected = has_active &&
                                          g_documents[active].mode == TextViewMode.markdown))
            menu_label(VIEW_PLAIN, (text = "Plain text", shortcut = "",
                                   selected = has_active &&
                                       g_documents[active].mode == TextViewMode.plain_text))
            separator(VIEW_EDIT_SEPARATOR)
            menu_label(VIEW_EDIT_MODE, (text = "Edit Mode",
                shortcut = command_shortcut_text(g_commands, CMD_EDIT_MODE),
                enabled = has_active && !g_documents[active].read_only,
                selected = editing))
        }
        if (has_active && !empty(g_documents[active].source.path)) {
            separator(MENU_PATH_SEPARATOR)
            let dirty = document_dirty(g_documents[active])
            text_disabled(MENU_PATH, (text = (dirty ? "* " : "")
                + g_documents[active].source.path))
        }
    }

    dispatch_menu_clicks()
}

def private dispatch_menu_clicks() {
    if (FILE_NEW.click_count > g_file_new_clicks) {
        g_file_new_clicks = FILE_NEW.click_count
        let _ = command_invoke(g_commands, CMD_FILE_NEW, CommandSource.menu)
    }
    if (FILE_OPEN.click_count > g_open_clicks) {
        g_open_clicks = FILE_OPEN.click_count
        open_file_dialog()
    }
    if (FILE_SAVE.click_count > g_save_clicks) {
        g_save_clicks = FILE_SAVE.click_count
        let _ = command_invoke(g_commands, CMD_FILE_SAVE, CommandSource.menu)
    }
    if (FILE_SAVE_AS.click_count > g_save_as_clicks) {
        g_save_as_clicks = FILE_SAVE_AS.click_count
        let _ = command_invoke(g_commands, CMD_FILE_SAVE_AS, CommandSource.menu)
    }
    if (FILE_RELOAD.click_count > g_reload_clicks) {
        g_reload_clicks = FILE_RELOAD.click_count
        let _ = command_invoke(g_commands, CMD_FILE_RELOAD, CommandSource.menu)
    }
    if (FILE_QUIT.click_count > g_quit_clicks) {
        g_quit_clicks = FILE_QUIT.click_count
        request_exit()
    }
    if (VIEW_MARKDOWN.click_count > g_markdown_clicks) {
        g_markdown_clicks = VIEW_MARKDOWN.click_count
        let _ = command_invoke(g_commands, CMD_VIEW_MARKDOWN, CommandSource.menu)
    }
    if (VIEW_PLAIN.click_count > g_plain_clicks) {
        g_plain_clicks = VIEW_PLAIN.click_count
        let _ = command_invoke(g_commands, CMD_VIEW_PLAIN, CommandSource.menu)
    }
    if (EDIT_UNDO.click_count > g_edit_undo_clicks) {
        g_edit_undo_clicks = EDIT_UNDO.click_count
        let _ = command_invoke(g_commands, "edit.undo", CommandSource.menu)
    }
    if (EDIT_REDO.click_count > g_edit_redo_clicks) {
        g_edit_redo_clicks = EDIT_REDO.click_count
        let _ = command_invoke(g_commands, "edit.redo", CommandSource.menu)
    }
    if (VIEW_EDIT_MODE.click_count > g_edit_mode_clicks) {
        g_edit_mode_clicks = VIEW_EDIT_MODE.click_count
        let _ = command_invoke(g_commands, CMD_EDIT_MODE, CommandSource.menu)
    }
    if (EDIT_COPY.click_count > g_edit_copy_clicks) {
        g_edit_copy_clicks = EDIT_COPY.click_count
        let _ = command_invoke(g_commands, CMD_EDIT_COPY, CommandSource.menu)
    }
    if (EDIT_SELECT_ALL.click_count > g_edit_select_all_clicks) {
        g_edit_select_all_clicks = EDIT_SELECT_ALL.click_count
        let _ = command_invoke(g_commands, CMD_EDIT_SELECT_ALL, CommandSource.menu)
    }
    if (EDIT_CLEAR_SELECTION.click_count > g_edit_clear_clicks) {
        g_edit_clear_clicks = EDIT_CLEAR_SELECTION.click_count
        let _ = command_invoke(g_commands, CMD_EDIT_CLEAR_SELECTION, CommandSource.menu)
    }
    if (EDIT_FIND.click_count > g_edit_find_clicks) {
        g_edit_find_clicks = EDIT_FIND.click_count
        let _ = command_invoke(g_commands, CMD_FIND, CommandSource.menu)
    }
    if (EDIT_FIND_NEXT.click_count > g_edit_find_next_clicks) {
        g_edit_find_next_clicks = EDIT_FIND_NEXT.click_count
        let _ = command_invoke(g_commands, CMD_FIND_NEXT, CommandSource.menu)
    }
    if (EDIT_FIND_PREV.click_count > g_edit_find_prev_clicks) {
        g_edit_find_prev_clicks = EDIT_FIND_PREV.click_count
        let _ = command_invoke(g_commands, CMD_FIND_PREV, CommandSource.menu)
    }
    if (EDIT_BINDINGS.click_count > g_edit_bindings_clicks) {
        g_edit_bindings_clicks = EDIT_BINDINGS.click_count
        let _ = command_invoke(g_commands, CMD_EDIT_BINDINGS, CommandSource.menu)
    }
}

def private draw_document_find(var doc : TextViewerDocument) {
    return if (!doc.find.open)
    text_find_bar(doc.id, doc.find)
    let _ = text_find_rebuild(doc.find, doc.source.text, doc.source.revision)
    if (doc.find.jump_revision != doc.find_applied_jump) {
        doc.find_applied_jump = doc.find.jump_revision
        let found = text_find_current(doc.find)
        if (found.start_byte >= 0) {
            if (doc.mode == TextViewMode.markdown) {
                doc.find_scroll_markdown = markdown_view_select_source_range(
                    doc.markdown_view, found.start_byte, found.end_byte)
            } else {
                let _expanded = text_source_view_expand_for_byte(
                    doc.source_document, doc.source_view, found.start_byte)
                text_source_view_select_range(doc.source_document, doc.source_view,
                                              found.start_byte, found.end_byte)
                doc.find_scroll_byte = found.start_byte
            }
        }
    }
    if (doc.find.open && IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)
            && IsKeyPressed(ImGuiKey.Escape, false)) {
        text_find_close(doc.find)
    }
}

def private apply_find_scroll(var doc : TextViewerDocument) {
    // No GetScrollMaxY clamp: it reports LAST frame's range (0 on a fresh
    // layout, silently pinning the jump to the top); ImGui clamps SetScrollY
    // against the real content height at frame end.
    return if (doc.find_scroll_byte < 0)
    let line_top = text_source_view_line_top(doc.source_view, doc.find_scroll_byte)
    if (line_top >= 0.0f) {
        let height = GetWindowSize().y
        let local_top = line_top - GetScrollY()
        if (local_top < height * 0.1f || local_top > height * 0.9f) {
            SetScrollY(max(0.0f, line_top - height * 0.35f))
        }
    }
    doc.find_scroll_byte = -1
}

def private apply_markdown_find_scroll(var doc : TextViewerDocument) {
    // The view's own caret-visible pass bails when the focus position misses
    // a fragment boundary and only edge-scrolls; a find jump places the
    // selection focus in the comfortable band instead.
    return if (!doc.find_scroll_markdown)
    doc.find_scroll_markdown = false
    let focus = markdown_view_selection_focus_screen_y(doc.markdown_view)
    return if (!focus.valid)
    let view_top = GetWindowPos().y
    let height = GetWindowSize().y
    if (focus.y < view_top + height * 0.1f
            || focus.y > view_top + height * 0.9f) {
        SetScrollY(max(0.0f, GetScrollY() + focus.y
            - (view_top + height * 0.35f)))
    }
}

def private refresh_edit_preview(var doc : TextViewerDocument) {
    // debounce on the editor's own idle clock; the first parse is immediate
    return if (doc.preview_revision == doc.edit.revision
        || (doc.preview_revision >= 0 && doc.edit.syntax_idle < 0.2f))
    var next_document <- md_parse(text_edit_text(doc.edit))
    markdown_view_invalidate(doc.preview_view)
    delete doc.preview_document.nodes
    doc.preview_document <- next_document
    doc.preview_revision = doc.edit.revision
    doc.preview_stamp++
    doc.preview_caret_applied = 0
}

def private apply_preview_caret(var doc : TextViewerDocument) {
    // the preview follows the CURSOR, not just the scroll (E5 ruling). The
    // position resolves READ-ONLY - the preview's selection is never touched
    // (even a zero-width selection painted arbitrary spans: the anchor/focus
    // resolution flavors land on different runs around unrendered syntax)
    return if (doc.preview_revision != doc.edit.revision)
    if (doc.preview_caret_applied != doc.edit.follow_revision
            || doc.preview_caret_stamp != doc.preview_stamp) {
        doc.preview_caret_applied = doc.edit.follow_revision
        doc.preview_caret_stamp = doc.preview_stamp
        // walk back for end-of-document, where nothing snaps forward
        let caret = text_edit_byte_of_position(doc.edit,
            doc.edit.cursor_line, doc.edit.cursor_column)
        var anchor = caret
        var tries = 0
        doc.preview_caret_byte = -1
        while (tries < 64) {
            if (markdown_view_source_line(doc.preview_view, anchor).valid) {
                doc.preview_caret_byte = anchor
                break
            }
            break if (anchor == 0)
            anchor--
            tries++
        }
        doc.preview_follow = true
    }
    return if (doc.preview_caret_byte < 0)
    let line = markdown_view_source_line_tint(doc.preview_view,
        doc.preview_caret_byte, TextSourceViewStyle().current_line_color)
    return if (!doc.preview_follow || !line.valid)
    doc.preview_follow = false
    let view_top = GetWindowPos().y
    let height = GetWindowSize().y
    if (line.y < view_top + height * 0.1f || line.y > view_top + height * 0.9f) {
        SetScrollY(max(0.0f, GetScrollY() + line.y - (view_top + height * 0.35f)))
    }
}

def private draw_document_editor(var doc : TextViewerDocument) {
    let side_by_side = doc.mode == TextViewMode.markdown
    let ui_style & = GetStyle()
    let avail = GetContentRegionAvail()
    let editor_width = (side_by_side
        ? floor((avail.x - ui_style.ItemSpacing.x) * 0.5f) : 0.0f)
    child(EDITOR_PANE, (text = "editor pane",
          size = float2(editor_width, 0.0f),
          child_flags = ImGuiChildFlags.None,
          window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        var edit_style = TextSourceViewStyle(
            fonts = g_fonts,
            zoom = float(zoom_percent(doc)) / 100.0f,
            write_system_clipboard = !harness_is_headless())
        let _ = text_source_edit("editor", doc.edit, edit_style, g_commands)
    }
    if (side_by_side) {
        same_line()
        child(PREVIEW_PANE, (text = "markdown preview",
              size = float2(0.0f, 0.0f),
              child_flags = ImGuiChildFlags.None,
              window_flags = ImGuiWindowFlags.None)) {
            refresh_edit_preview(doc)
            var style = MarkdownViewStyle(
                fonts = g_fonts,
                typography = MarkdownTypography(
                    inline_code_scale = 0.84f,
                    code_block_scale = 0.90f),
                zoom = float(zoom_percent(doc)) / 100.0f,
                write_system_clipboard = false)
            let preview_result <- markdown_view_tree_sitter(
                "edit preview", doc.preview_document, doc.preview_syntax,
                doc.preview_view, style, doc.preview_stamp)
            if (preview_result.control_event.active) {
                doc.status = "Preview checkboxes are read-only - edit the source."
            }
            apply_preview_caret(doc)
        }
    }
}

def private draw_document(var doc : TextViewerDocument) {
    if (!doc.editing) {
        draw_document_find(doc)
    }
    // The footer's icon button stands taller than a text frame; reserving only
    // GetFrameHeightWithSpacing() overflows the dock window by the difference,
    // growing a phantom second scrollbar beside the content child's.
    let ui_style & = GetStyle()
    let footer_height = max(GetFrameHeightWithSpacing(),
        FOOTER_ICON_SIZE + ui_style.FramePadding.y * 2.0f + ui_style.ItemSpacing.y)
    // While editing, the find bar draws BELOW the editor (the demo order): the
    // editor's FocusItem reclaim runs at its own draw, so a bar drawn earlier
    // in the frame would lose the replace-input focus race every frame.
    var bar_rows = 0.0f
    if (doc.editing && doc.find.open) {
        bar_rows = doc.find.replace_open ? 2.0f : 1.0f
    }
    let reserved = footer_height + GetFrameHeightWithSpacing() * bar_rows
    child(DOCUMENT_CONTENT,
          (text = "document content",
           size = float2(0.0f, -reserved),
           child_flags = ImGuiChildFlags.None,
           window_flags = doc.editing ? ImGuiWindowFlags.None
               : ImGuiWindowFlags.HorizontalScrollbar)) {
        doc.content_view_top = GetWindowPos().y
        doc.content_view_bottom = GetWindowPos().y + GetWindowSize().y
        if (doc.editing) {
            draw_document_editor(doc)
        } elif (doc.mode == TextViewMode.markdown) {
            var style = MarkdownViewStyle(
                fonts = g_fonts,
                typography = MarkdownTypography(
                    inline_code_scale = 0.84f,
                    code_block_scale = 0.90f),
                zoom = float(zoom_percent(doc)) / 100.0f,
                write_system_clipboard = !harness_is_headless())
            let view_result <- markdown_view_tree_sitter(
                "text viewer", doc.document, doc.markdown_syntax,
                doc.markdown_view, style, doc.source.revision)
            if (view_result.control_event.active) {
                let _ = apply_control_event(doc, view_result.control_event)
            }
            apply_markdown_find_scroll(doc)
        } else {
            var source_style = TextSourceViewStyle(
                fonts = g_fonts,
                zoom = float(zoom_percent(doc)) / 100.0f,
                wrap = false,
                write_system_clipboard = !harness_is_headless())
            doc.source_result <- text_source_view("text source", doc.source_document,
                                                  doc.source_view, source_style)
            apply_find_scroll(doc)
        }
    }

    if (doc.editing) {
        // bar after the editor, then the sync chain - the demo order
        if (doc.find.open) {
            text_find_bar(doc.id, doc.find)
        }
        text_edit_find_sync(doc.edit, doc.find, g_commands)
        let _ = text_edit_syntax_sync(doc.edit)
        text_edit_complete_sync(doc.edit, doc.edit_complete)
    }
    draw_document_footer(doc)
}

def private footer_mode_label(doc : TextViewerDocument const) : string {
    let rendered = doc.mode == TextViewMode.markdown
    var caption = ""
    if (doc.editing) {
        caption = rendered ? "Editing side-by-side" : "Editing source"
    } else {
        caption = rendered ? "Rendered Markdown" : "Markdown source"
    }
    if (doc.read_only) {
        caption += " - read-only"
    }
    return caption
}

def private draw_document_footer(var doc : TextViewerDocument) {
    let ui_style & = GetStyle()
    let rendered = doc.mode == TextViewMode.markdown
    if (icon_button(MODE_TOGGLE,
            (glyph = rendered ? "edit" : "visible",
             tip = rendered ? "Show Markdown source" : "Render Markdown",
             size = FOOTER_ICON_SIZE, weight = 1.0f))) {
        let _ = command_invoke(g_commands, CMD_VIEW_TOGGLE,
                               CommandSource.toolbar, "{doc.id}")
    }
    if (!doc.read_only) {
        same_line()
        if (small_button(EDIT_TOGGLE, (text = doc.editing ? "Done" : "Edit"))) {
            let _ = command_invoke(g_commands, CMD_EDIT_MODE,
                                   CommandSource.toolbar, "{doc.id}")
        }
    }
    same_line()
    text_disabled(MODE_LABEL, (text = footer_mode_label(doc)))
    if (doc.editing) {
        same_line()
        text_disabled(EDIT_POSITION,
            (text = "Ln {doc.edit.cursor_line + 1}, Col {doc.edit.cursor_column + 1}"))
    }
    if (doc.external_change) {
        same_line()
        text_disabled(EXTERNAL_WARNING, (text = "(!) changed on disk"))
    }
    if (rendered && !doc.editing) {
        same_line()
        SetNextItemWidth(115.0f)
        if (edit_combo(unsafe(addr(doc.syntax_mode)),
                (id = "TEXT_VIEWER_SYNTAX", text = "Syntax",
                 items <- SYNTAX_MODE_LABELS))) {
            let index = clamp(doc.syntax_mode, 0, length(SYNTAX_MODE_VALUES) - 1)
            let _ = markdown_tree_sitter_set_default(
                doc.markdown_syntax, SYNTAX_MODE_VALUES[index])
        }
    }
    same_line()
    text_disabled(STATUS, (text = doc.status))

    let right_edge = GetCursorPosX() + GetContentRegionAvail().x
    let zoom_value_text = "{zoom_percent(doc)}%"
    // Measured, not hardcoded: the cluster width varies with the zoom value
    // text, and an underestimate pushed the reset button past the window edge.
    let zoom_width = (CalcTextSize("Zoom").x + 150.0f
        + CalcTextSize(zoom_value_text).x
        + CalcTextSize("100%").x + ui_style.FramePadding.x * 2.0f
        + ui_style.ItemSpacing.x * 3.0f)
    same_line()
    SetCursorPosX(max(GetCursorPosX(), right_edge - zoom_width))
    text(ZOOM_LABEL, (text = "Zoom"))
    same_line()
    SetNextItemWidth(150.0f)
    edit_slider_int(unsafe(addr(doc.zoom_step)),
        (id = "TEXT_VIEWER_ZOOM", text = "##zoom", v_min = 10,
         v_max = 60, format = ""))
    same_line()
    text_disabled(ZOOM_VALUE, (text = zoom_value_text))
    same_line()
    if (small_button(ZOOM_RESET, (text = "100%"))) {
        let _ = command_invoke(g_commands, CMD_VIEW_ZOOM_RESET,
                               CommandSource.toolbar, "{doc.id}")
    }
    doc.footer_overflow = max(0.0f,
        (GetItemRectMax().x - GetWindowPos().x) - right_edge)
}

def private handle_global_shortcuts() {
    command_poll_scope(g_commands, CommandScope.application)
}

def private handle_view_typing(var doc : TextViewerDocument) {
    // the ruling: typing in the viewer makes it an editor - the first printable
    // keystroke enters edit mode and lands in the buffer
    return if (doc.editing || doc.read_only
        || !IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows))
    let io & = unsafe(GetIO())
    return if (io.WantTextInput || io.KeyCtrl || io.KeyAlt)
    var any_printable = false
    for (value in io.InputQueueCharacters) {
        let codepoint = int(value)
        if (codepoint >= 0x20 && codepoint != 0x7f && codepoint <= 0x10ffff) {
            any_printable = true
        }
    }
    return if (!any_printable)
    // enter only - the still-queued chars route through the editor widget's
    // own drain later this frame (a manual replay here would double-type)
    let _ = enter_edit_mode(doc)
}

def private handle_document_shortcuts(doc : TextViewerDocument const) {
    let io & = unsafe(GetIO())
    command_poll_scope(g_commands, CommandScope.document, "{doc.id}")
    if (!doc.editing) {
        // while editing, the editor widget owns the component scope - a second
        // poll here would double-fire shared chords like Ctrl+C
        command_poll_scope(g_commands, CommandScope.component, "{doc.id}")
    }
    // io.KeyCtrl is logical — ImGui maps Cmd onto it under ConfigMacOSXBehaviors.
    if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
        if (io.MouseWheel > 0.0f) {
            let _ = command_invoke(g_commands, CMD_VIEW_ZOOM_IN,
                                   CommandSource.programmatic, "{doc.id}")
        } elif (io.MouseWheel < 0.0f) {
            let _ = command_invoke(g_commands, CMD_VIEW_ZOOM_OUT,
                                   CommandSource.programmatic, "{doc.id}")
        }
    }
}

def private draw_document_window(var doc : TextViewerDocument; dock_id : uint) {
    if (doc.pending_dock && dock_id != 0u) {
        unsafe {
            g_document_windows[doc.id].pending_dock_to = dock_id
        }
        doc.pending_dock = false
    }
    let base_title = (empty(doc.source.path)
        ? (doc.read_only ? "Text viewer" : "Untitled") : base_name(doc.source.path))
    let visible_title = (document_dirty(doc) ? "* " : "") + base_title
    // Triple-hash keeps the ImGui window identity stable when a welcome
    // document is loaded from disk and its visible title becomes a filename.
    let title = "{visible_title}###text_document_{doc.id}"
    // The content child is the only intended scroller; the window itself must
    // never scroll (a scrolled window would carry the footer off-screen).
    dock_window(g_document_windows[doc.id],
                (text = title, closable = true,
                 flags = ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoScrollbar
                     | ImGuiWindowFlags.NoScrollWithMouse)) {
        if (IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)) {
            g_active_document_id = doc.id
        }
        // Nonzero means the dock window itself overflowed (a phantom second
        // scrollbar); the content child is the only intended scroller.
        doc.window_scroll_max = GetScrollMaxY()
        handle_document_shortcuts(doc)
        handle_view_typing(doc)
        watch_external_change(doc)
        draw_document(doc)
    }
}

def private remove_closed_documents() {
    var remaining = length(g_documents)
    while (remaining > 0) {
        remaining--
        let index = remaining
        let id = g_documents[index].id
        if (!g_document_windows[id].open) {
            let removed_active = g_documents[index].id == g_active_document_id
            dispose_document(g_documents[index])
            g_documents |> erase(index)
            g_document_windows |> erase(id)
            if (removed_active) {
                g_active_document_id = !empty(g_documents) ? g_documents[0].id : 0
            }
        }
    }
}

[live_command(description = "Inspect the text viewer without a screenshot.")]
def text_viewer_state(_input : JsonValue?) : JsonValue? { // nolint:STYLE038 — one JV state snapshot, a field per line; splitting scatters the wire shape
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open",
                               document_count = 0, active_document_id = 0))
    let doc & = unsafe(g_documents[active])
    let window = g_document_windows[doc.id]
    return JV((
        ok = doc.document.ok,
        document_count = length(g_documents),
        active_document_id = doc.id,
        window_open = window.open,
        window_docked = window.docked,
        window_dock_id = window.dock_id,
        dockspace_id = DOCK_ROOT.dock_id,
        path = doc.source.path,
        byte_count = length(text_document_bytes(doc.source)),
        mode = mode_name(doc),
        zoom_percent = zoom_percent(doc),
        zoom_step = doc.zoom_step,
        document_revision = doc.source.revision,
        saved_revision = doc.source.saved_revision,
        dirty = document_dirty(doc),
        source_dirty = text_document_dirty(doc.source),
        editing = doc.editing,
        read_only = doc.read_only,
        edit_dirty = doc.edit.dirty,
        edit_revision = doc.edit.revision,
        edit_cursor_line = doc.edit.cursor_line,
        edit_cursor_column = doc.edit.cursor_column,
        edit_line_count = text_edit_line_count(doc.edit),
        edit_can_undo = text_edit_can_undo(doc.edit),
        edit_can_redo = text_edit_can_redo(doc.edit),
        external_change = doc.external_change,
        preview_revision = doc.preview_revision,
        preview_stamp = doc.preview_stamp,
        preview_selection_revision = doc.preview_view.selection_revision,
        preview_focus_valid = doc.preview_caret_byte >= 0,
        preview_focus_y = markdown_view_source_line(
            doc.preview_view, max(doc.preview_caret_byte, 0)).y,
        preview_selection_start = md_text_selection_range(
            doc.preview_view.selection_anchor, doc.preview_view.selection_focus).start_byte,
        preview_selection_end = md_text_selection_range(
            doc.preview_view.selection_anchor, doc.preview_view.selection_focus).end_byte,
        has_utf8_bom = doc.source.has_utf8_bom,
        checkbox_count = length(doc.projection.controls),
        control_activation_revision = doc.markdown_view.control_activation_revision,
        activated_control_id = doc.markdown_view.activated_control_id,
        render_revision = doc.markdown_view.render_revision,
        source_render_revision = doc.source_view.render_revision,
        source_layout_hits = doc.source_view.layout_hits,
        source_layout_misses = doc.source_view.layout_misses,
        source_marker_count = doc.source_document.marker_count,
        source_cell_count = length(doc.source_view.cells),
        source_selection_revision = doc.source_view.selection_revision,
        source_selection_commit_revision = doc.source_view.selection_commit_revision,
        source_selection_dragging = doc.source_view.selection_dragging,
        source_selection_start = doc.source_result.selection.source.start_byte,
        source_selection_end = doc.source_result.selection.source.end_byte,
        source_selection_has_markers = doc.source_result.selection.styles.has_markers,
        markdown_selection_copy_revision = doc.markdown_view.selection_copy_revision,
        source_selection_copy_revision = doc.source_view.selection_copy_revision,
        markdown_selection_revision = doc.markdown_view.selection_revision,
        markdown_selection_screen_valid = doc.markdown_view.selection_screen_valid,
        markdown_selection_screen_y0 = doc.markdown_view.selection_screen_bounds.y,
        markdown_selection_screen_y1 = doc.markdown_view.selection_screen_bounds.w,
        markdown_focus_screen_valid = markdown_view_selection_focus_screen_y(
            doc.markdown_view).valid,
        markdown_focus_screen_y = markdown_view_selection_focus_screen_y(
            doc.markdown_view).y,
        source_selection_screen_valid = doc.source_view.selection_screen_valid,
        source_selection_screen_y0 = doc.source_view.selection_screen_bounds.y,
        source_selection_screen_y1 = doc.source_view.selection_screen_bounds.w,
        content_view_top = doc.content_view_top,
        content_view_bottom = doc.content_view_bottom,
        window_scroll_max_y = doc.window_scroll_max,
        footer_overflow_x = doc.footer_overflow,
        markdown_selection_start = md_text_selection_range(
            doc.markdown_view.selection_anchor, doc.markdown_view.selection_focus).start_byte,
        markdown_selection_end = md_text_selection_range(
            doc.markdown_view.selection_anchor, doc.markdown_view.selection_focus).end_byte,
        fold_count = length(doc.source_document.folds),
        fold_collapsed_count = text_source_view_collapsed_count(doc.source_view),
        fold_revision = doc.source_view.fold_revision,
        visible_line_count = length(doc.source_view.layout.lines),
        md_code_collapsed_count = markdown_view_collapsed_count(doc.markdown_view),
        md_fold_revision = doc.markdown_view.fold_revision,
        code_copy_revision = doc.markdown_view.code_copy_revision,
        code_copy_node = doc.markdown_view.code_copy_node,
        code_copy_bytes = length(doc.markdown_view.code_copy_text),
        find_open = doc.find.open,
        find_query = doc.find.query,
        find_regex = doc.find.use_regex,
        find_match_count = length(doc.find.matches),
        find_current = doc.find.current,
        find_error = doc.find.error,
        find_jump_revision = doc.find.jump_revision,
        find_applied_jump = doc.find_applied_jump,
        command_revision = g_command_revision,
        last_command = g_last_command,
        bindings_editor_open = g_bindings_editor.open,
        bindings_config_path = g_commands.config_path,
        bindings_revision = g_commands.revision,
        status = doc.status))
}

[live_command(description = "Find source text and inspect its exact cell-aligned screen range.")]
def text_viewer_source_range(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    var doc & = unsafe(g_documents[active])
    let args = from_JV(input, type<TextViewerSourceRangeArgs>)
    let start_byte = find(doc.source.text, args.text)
    if (start_byte < 0) {
        return JV((ok = false, error = "source text not found", text = args.text))
    }
    let end_byte = start_byte + length(args.text)
    var found = false
    var all_markers = true
    var columns = 0
    var matched_cells = 0
    var marker_cells = 0
    var syntax_cells = 0
    var first_cell_start = -1
    var last_cell_end = -1
    var rect = float4()
    for (cell in doc.source_view.cells) {
        if (cell.source.end_byte <= start_byte || cell.source.start_byte >= end_byte) continue
        let screen = float4(
            doc.source_view.screen_origin.x + cell.rect.x,
            doc.source_view.screen_origin.y + cell.rect.y,
            doc.source_view.screen_origin.x + cell.rect.z,
            doc.source_view.screen_origin.y + cell.rect.w)
        if (!found) {
            rect = screen
            found = true
        } else {
            rect.x = min(rect.x, screen.x)
            rect.y = min(rect.y, screen.y)
            rect.z = max(rect.z, screen.z)
            rect.w = max(rect.w, screen.w)
        }
        columns += cell.columns
        matched_cells++
        marker_cells += cell.style_id == 1 ? 1 : 0
        syntax_cells += cell.style_id > 1 ? 1 : 0
        if (first_cell_start < 0) {
            first_cell_start = cell.source.start_byte
        }
        last_cell_end = cell.source.end_byte
        all_markers = all_markers && cell.style_id == 1
    }
    return JV((ok = found, text = args.text,
        source_start = start_byte, source_end = end_byte,
        columns = columns, matched_cells = matched_cells,
        marker_cells = marker_cells, syntax_cells = syntax_cells,
        first_cell_start = first_cell_start,
        last_cell_end = last_cell_end,
        all_markers = found && all_markers,
        cell_width = doc.source_view.key.cell_width,
        x0 = rect.x, y0 = rect.y, x1 = rect.z, y1 = rect.w))
}

[live_command(description = "Open a UTF-8 file in the text viewer.")]
def text_viewer_open(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TextViewerOpenArgs>)
    let active = active_document_index()
    if (active < 0) {
        let ok = add_document(args.path)
        let created = active_document_index()
        return JV((ok = ok, path = created >= 0 ? g_documents[created].source.path : "",
                   status = created >= 0 ? g_documents[created].status : g_last_open_error,
                   document_revision = created >= 0
                       ? g_documents[created].source.revision : 0))
    }
    let ok = load_path(g_documents[active], args.path)
    return JV((ok = ok, path = g_documents[active].source.path,
               status = g_documents[active].status,
               document_revision = g_documents[active].source.revision))
}

[live_command(description = "Open a UTF-8 file as another docked document.")]
def text_viewer_open_window(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TextViewerOpenArgs>)
    let ok = add_document(args.path)
    let active = active_document_index()
    return JV((ok = ok, document_count = length(g_documents),
               active_document_id = active >= 0 ? g_documents[active].id : 0,
               path = active >= 0 ? g_documents[active].source.path : "",
               zoom_percent = active >= 0 ? zoom_percent(g_documents[active]) : 0,
               status = ok && active >= 0
                   ? g_documents[active].status : g_last_open_error))
}

[live_command(description = "Inspect one rendered Markdown checkbox and its screen bounds.")]
def text_viewer_checkbox(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerCheckboxArgs>)
    var doc & = unsafe(g_documents[active])
    if (args.index < 0 || args.index >= length(doc.projection.controls)) {
        return JV((ok = false, error = "checkbox index is out of range"))
    }
    let control = doc.projection.controls[args.index]
    if (control.payload < 0 || control.payload >= length(doc.markdown_view.nodes)) {
        return JV((ok = false, error = "checkbox has not been laid out"))
    }
    let cache & = unsafe(doc.markdown_view.nodes[control.payload])
    return JV((ok = cache.control_screen_valid,
        index = args.index, control_id = control.id, checked = control.checked,
        source_start = control.source.start_byte, source_end = control.source.end_byte,
        document_revision = doc.source.revision,
        x0 = cache.control_screen_rect.x, y0 = cache.control_screen_rect.y,
        x1 = cache.control_screen_rect.z, y1 = cache.control_screen_rect.w))
}

[live_command(description = "Save the active document exactly; optional path performs Save As.")]
def text_viewer_save(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerOpenArgs>)
    let path = empty(args.path) ? g_documents[active].source.path : args.path
    let ok = save_document(g_documents[active], path)
    return JV((ok = ok, path = g_documents[active].source.path,
               dirty = document_dirty(g_documents[active]),
               document_revision = g_documents[active].source.revision,
               saved_revision = g_documents[active].source.saved_revision,
               status = g_documents[active].status))
}

[live_command(description = "Enter or leave edit mode in the active document.")]
def text_viewer_edit(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerEditArgs>)
    var doc & = unsafe(g_documents[active])
    var ok = true
    if (args.on) {
        ok = enter_edit_mode(doc)
    } else {
        exit_edit_mode(doc)
    }
    return JV((ok = ok, editing = doc.editing, read_only = doc.read_only,
               status = doc.status))
}

[live_command(description = "Read the active edit buffer (LF-joined) and cursor state.")]
def text_viewer_edit_text(_input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let doc & = unsafe(g_documents[active])
    if (!doc.editing) return JV((ok = false, error = "not editing"))
    return JV((ok = true, text = text_edit_text(doc.edit),
               cursor_line = doc.edit.cursor_line,
               cursor_column = doc.edit.cursor_column,
               dirty = doc.edit.dirty,
               revision = doc.edit.revision,
               undo_depth = length(doc.edit.undo_stack),
               redo_depth = length(doc.edit.redo_stack)))
}

[live_command(description = "Switch between markdown and plain text modes.")]
def text_viewer_set_mode(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerModeArgs>)
    let requested = to_lower(args.mode)
    if (requested == "markdown") {
        set_mode(g_documents[active], TextViewMode.markdown)
    } elif (requested == "plain" || requested == "text") {
        set_mode(g_documents[active], TextViewMode.plain_text)
    } else {
        return JV((ok = false, error = "mode must be 'markdown' or 'plain'"))
    }
    return JV((ok = true, mode = mode_name(g_documents[active])))
}

[live_command(description = "Set the fixed-step viewer zoom percentage.")]
def text_viewer_set_zoom(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerZoomArgs>)
    set_zoom_percent(g_documents[active], args.percent)
    return JV((ok = true, zoom_percent = zoom_percent(g_documents[active])))
}

[live_command(description = "List rendered code-block nodes and their collapsed state.")]
def text_viewer_md_code_blocks(_input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let doc & = unsafe(g_documents[active])
    var found <- markdown_view_code_blocks(doc.document, doc.markdown_view)
    var blocks <- [for (code in found); JV((node = code.node,
        collapsed = code.collapsed,
        laid_out = code.laid_out,
        screen_x = code.screen_origin.x,
        screen_y = code.screen_origin.y,
        icons_valid = code.icons_valid,
        fold_icon_x = code.fold_icon_center.x,
        fold_icon_y = code.fold_icon_center.y,
        copy_icon_x = code.copy_icon_center.x,
        copy_icon_y = code.copy_icon_center.y))]
    return JV((ok = true, count = length(blocks), blocks = JV(blocks)))
}

struct TextViewerMdFoldArgs {
    node : int = -1
    collapsed : bool = true
}

[live_command(description = "Collapse or expand one rendered code block by node index.")]
def text_viewer_md_fold(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerMdFoldArgs>)
    var doc & = unsafe(g_documents[active])
    let changed = markdown_view_set_code_collapsed(
        doc.markdown_view, args.node, args.collapsed)
    return JV((ok = changed, node = args.node, collapsed = args.collapsed,
               md_fold_revision = doc.markdown_view.fold_revision,
               md_code_collapsed_count = markdown_view_collapsed_count(doc.markdown_view)))
}

[live_command(description = "Copy one rendered code block to the clipboard by node index.")]
def text_viewer_md_copy_code(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerMdFoldArgs>)
    var doc & = unsafe(g_documents[active])
    let copied = markdown_view_copy_code_block(doc.markdown_view, args.node,
        !harness_is_headless())
    return JV((ok = copied,
               code_copy_revision = doc.markdown_view.code_copy_revision,
               code_copy_node = doc.markdown_view.code_copy_node,
               code_copy_bytes = length(doc.markdown_view.code_copy_text)))
}

[live_command(description = "List fold regions of the active document's source view.")]
def text_viewer_folds(_input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let doc & = unsafe(g_documents[active])
    var regions <- [for (index in range(length(doc.source_document.folds)));
        JV((index = index,
            kind = doc.source_document.folds[index].kind,
            header_row = doc.source_document.folds[index].header_row,
            end_row = doc.source_document.folds[index].end_row,
            start_byte = doc.source_document.folds[index].start_byte,
            end_byte = doc.source_document.folds[index].end_byte,
            collapsed = text_source_view_fold_collapsed(
                doc.source_document, doc.source_view, index),
            handle_valid = text_source_view_fold_handle_pos(
                doc.source_document, doc.source_view, index).valid,
            handle_x = text_source_view_fold_handle_pos(
                doc.source_document, doc.source_view, index).x,
            handle_y = text_source_view_fold_handle_pos(
                doc.source_document, doc.source_view, index).y))]
    return JV((ok = true, fold_count = length(doc.source_document.folds),
               folds = JV(regions)))
}

[live_command(description = "Collapse or expand one fold region by index.")]
def text_viewer_fold(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerFoldArgs>)
    var doc & = unsafe(g_documents[active])
    let changed = text_source_view_set_fold(
        doc.source_document, doc.source_view, args.index, args.collapsed)
    return JV((ok = changed, index = args.index, collapsed = args.collapsed,
               fold_revision = doc.source_view.fold_revision,
               fold_collapsed_count = text_source_view_collapsed_count(doc.source_view)))
}

[live_command(description = "Open find in the active document with a query; regex optional.")]
def text_viewer_find(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerFindArgs>)
    var doc & = unsafe(g_documents[active])
    text_find_set_query(doc.find, args.query, args.regex)
    let _ = text_find_rebuild(doc.find, doc.source.text, doc.source.revision)
    return JV((ok = true, query = doc.find.query, regex = doc.find.use_regex,
               match_count = length(doc.find.matches),
               current = doc.find.current, error = doc.find.error,
               jump_revision = doc.find.jump_revision))
}

[live_command(description = "Step the active document's find selection by delta.")]
def text_viewer_find_step(input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    let args = from_JV(input, type<TextViewerFindStepArgs>)
    var doc & = unsafe(g_documents[active])
    text_find_step(doc.find, args.delta)
    return JV((ok = true, match_count = length(doc.find.matches),
               current = doc.find.current,
               jump_revision = doc.find.jump_revision))
}

[live_command(description = "Close find in the active document.")]
def text_viewer_find_close(_input : JsonValue?) : JsonValue? {
    let active = active_document_index()
    if (active < 0) return JV((ok = false, error = "no document is open"))
    text_find_close(g_documents[active].find)
    return JV((ok = true))
}

[live_command(description = "Invoke a semantic viewer command by stable ID.")]
def text_viewer_command(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TextViewerCommandArgs>)
    let queued = command_invoke(g_commands, args.id, CommandSource.live_api)
    return JV((ok = queued, id = args.id, command_revision = g_command_revision))
}

[live_command(description = "Inspect semantic commands and current bindings.")]
def text_viewer_commands(_input : JsonValue?) : JsonValue? {
    var definitions <- [for (definition in g_commands.definitions); JV((
            id = definition.id,
            title = definition.title,
            category = definition.category,
            scope = int(definition.scope),
            enabled = definition.enabled,
            selected = definition.selected,
            shortcut = command_shortcut_text(g_commands, definition.id)))]
    return JV((ok = true, revision = g_commands.revision,
               saved_revision = g_commands.saved_revision,
               config_path = g_commands.config_path,
               status = g_commands.status,
               commands = JV(definitions)))
}

[export]
def init() {
    harness_init("daScript text viewer", 1100, 780)
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
    g_fonts = load_unicode_font_roles(18.0f, g_font_sources)
    apply_daslang_theme(GetStyle())
    parse_command_line()
    define_commands()
    let _ = add_document(g_initial_path, g_initial_mode_override, true)
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()

    command_begin_frame(g_commands)
    update_command_state()
    draw_menu_bar()
    handle_global_shortcuts()
    dockspace(DOCK_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!DOCK_ROOT.has_initial_layout && DOCK_ROOT.dock_id != 0u) {
            setup_default_layout(DOCK_ROOT.dock_id)
            DOCK_ROOT.has_initial_layout = true
        }
        for (index in range(length(g_documents))) {
            unsafe {
                draw_document_window(g_documents[index], DOCK_ROOT.dock_id)
            }
        }
    }

    command_bindings_editor(g_commands, g_bindings_editor)
    dispatch_commands()

    let dialog_result = file_dialog_draw()
    if (dialog_result == FileDialogResult.confirmed) {
        let selected_path = file_dialog_selected_path()
        if (g_file_dialog_action == ViewerFileDialogAction.save_as) {
            let active = active_document_index()
            if (active >= 0) {
                let _ = save_document(g_documents[active], selected_path)
            }
        } else {
            let _ = add_document(selected_path)
        }
        g_file_dialog_action = ViewerFileDialogAction.none
    } elif (dialog_result == FileDialogResult.cancelled) {
        let active = active_document_index()
        if (active >= 0) {
            g_documents[active].status = (g_file_dialog_action == ViewerFileDialogAction.save_as
                    ? "Save As cancelled."
                    : "Open cancelled.")
        }
        g_file_dialog_action = ViewerFileDialogAction.none
    }

    remove_closed_documents()

    harness_end_frame()
}

[export]
def shutdown() {
    for (doc in g_documents) {
        dispose_document(doc)
    }
    delete g_documents
    delete g_document_windows
    command_registry_dispose(g_commands)
    harness_shutdown()
    delete g_font_sources
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
