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
    content_view_top : float = 0.0f
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

let SYNTAX_MODE_LABELS : array<string> <- [
    "Auto", "Plain", "daScript", "C", "C++", "Markdown"
]
let SYNTAX_MODE_VALUES : array<string> <- [
    "auto", "none", "das", "c", "cpp", "markdown"
]

let CMD_FILE_OPEN = "file.open"
let CMD_FILE_SAVE = "file.save"
let CMD_FILE_SAVE_AS = "file.save_as"
let CMD_FILE_RELOAD = "file.reload"
let CMD_APP_QUIT = "app.quit"
let CMD_VIEW_MARKDOWN = "view.markdown"
let CMD_VIEW_PLAIN = "view.plain"
let CMD_VIEW_TOGGLE = "view.toggle_source"
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
    viewer.status = "No file selected."
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
    return save_path(g_documents[active], path)
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

def private define_commands() {
    // ImGuiKey's digit enumerators cannot be named directly in daslang because
    // `.0` parses as a float literal. ImGui keeps the digit row immediately
    // before A, so derive the key for Ctrl+0 from that stable enum sequence.
    let key_0 = unsafe(reinterpret<ImGuiKey>(int(ImGuiKey.A) - 10))
    g_commands.config_path = command_default_config_path("text-viewer")
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
    let has_selection = has_active && document_has_selection(g_documents[active])
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
        if (invocation.command_id == CMD_FILE_OPEN) {
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
    main_menu_bar(MAIN_MENU) {
        menu(FILE_MENU, (text = "File", enabled = true)) {
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
        }
        if (has_active && !empty(g_documents[active].source.path)) {
            separator(MENU_PATH_SEPARATOR)
            let dirty = text_document_dirty(g_documents[active].source)
            text_disabled(MENU_PATH, (text = (dirty ? "* " : "")
                + g_documents[active].source.path))
        }
    }

    dispatch_menu_clicks()
}

def private dispatch_menu_clicks() {
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

def private draw_document(var doc : TextViewerDocument) {
    draw_document_find(doc)
    let footer_height = GetFrameHeightWithSpacing()
    child(DOCUMENT_CONTENT,
          (text = "document content",
           size = float2(0.0f, -footer_height),
           child_flags = ImGuiChildFlags.None,
           window_flags = ImGuiWindowFlags.HorizontalScrollbar)) {
        doc.content_view_top = GetWindowPos().y
        doc.content_view_bottom = GetWindowPos().y + GetWindowSize().y
        if (doc.mode == TextViewMode.markdown) {
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

    let rendered = doc.mode == TextViewMode.markdown
    if (icon_button(MODE_TOGGLE,
            (glyph = rendered ? "edit" : "visible",
             tip = rendered ? "Show Markdown source" : "Render Markdown",
             size = 18.0f, weight = 1.0f))) {
        let _ = command_invoke(g_commands, CMD_VIEW_TOGGLE,
                               CommandSource.toolbar, "{doc.id}")
    }
    same_line()
    text_disabled(MODE_LABEL,
        (text = rendered ? "Rendered Markdown" : "Markdown source"))
    if (rendered) {
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
    let zoom_width = 245.0f
    same_line()
    SetCursorPosX(max(GetCursorPosX(), right_edge - zoom_width))
    text(ZOOM_LABEL, (text = "Zoom"))
    same_line()
    SetNextItemWidth(150.0f)
    edit_slider_int(unsafe(addr(doc.zoom_step)),
        (id = "TEXT_VIEWER_ZOOM", text = "##zoom", v_min = 10,
         v_max = 60, format = ""))
    same_line()
    text_disabled(ZOOM_VALUE, (text = "{zoom_percent(doc)}%"))
    same_line()
    if (small_button(ZOOM_RESET, (text = "100%"))) {
        let _ = command_invoke(g_commands, CMD_VIEW_ZOOM_RESET,
                               CommandSource.toolbar, "{doc.id}")
    }
}

def private handle_global_shortcuts() {
    command_poll_scope(g_commands, CommandScope.application)
}

def private handle_document_shortcuts(doc : TextViewerDocument const) {
    let io & = unsafe(GetIO())
    command_poll_scope(g_commands, CommandScope.document, "{doc.id}")
    command_poll_scope(g_commands, CommandScope.component, "{doc.id}")
    let command_down = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl
    if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && command_down) {
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
    let visible_title = empty(doc.source.path) ? "Text viewer" : base_name(doc.source.path)
    // Triple-hash keeps the ImGui window identity stable when a welcome
    // document is loaded from disk and its visible title becomes a filename.
    let title = "{visible_title}###text_document_{doc.id}"
    dock_window(g_document_windows[doc.id],
                (text = title, closable = true,
                 flags = ImGuiWindowFlags.NoCollapse)) {
        if (IsWindowFocused(ImGuiFocusedFlags.RootAndChildWindows)) {
            g_active_document_id = doc.id
        }
        handle_document_shortcuts(doc)
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
def text_viewer_state(_input : JsonValue?) : JsonValue? {
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
        dirty = text_document_dirty(doc.source),
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
        markdown_selection_start = md_text_selection_range(
            doc.markdown_view.selection_anchor, doc.markdown_view.selection_focus).start_byte,
        markdown_selection_end = md_text_selection_range(
            doc.markdown_view.selection_anchor, doc.markdown_view.selection_focus).end_byte,
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
    let ok = save_path(g_documents[active], path)
    return JV((ok = ok, path = g_documents[active].source.path,
               dirty = text_document_dirty(g_documents[active].source),
               document_revision = g_documents[active].source.revision,
               saved_revision = g_documents[active].source.saved_revision,
               status = g_documents[active].status))
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
                let _ = save_path(g_documents[active], selected_path)
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
