options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_file_dialog shared

require imgui public
require imgui/imgui_boost_runtime public
require imgui/imgui_boost_v2 public
require imgui/imgui_widgets_builtin public
require imgui/imgui_containers_builtin public
require imgui/imgui_table_builtin public
require daslib/fio
require strings
require daslib/strings_boost

//! Native, reusable file dialog built entirely from the boost DSL — so every
//! interactive element registers on the snapshot registry and the whole dialog
//! is `imgui_playwright`-drivable (a bound C++ file dialog, rendering raw ImGui,
//! would be invisible to it). Mirrors ImGuiFileDialog's core UX: directory
//! navigation (clickable breadcrumb + ".."), a sortable Name/Size/Type table,
//! an extension-filter dropdown, multi-select (open mode), a filename field
//! (save/create), and an overwrite-confirm step.
//!
//! Immediate-mode: `file_dialog_open(cfg)` queues the modal, then call
//! `file_dialog_draw()` EVERY frame (at the top level of `update()`, outside any
//! window, so the registry path stays a clean `FILEDLG/...`). It returns the
//! per-frame result; read the chosen path(s) with `file_dialog_selection()`.
//! A modal is exclusive, so the widget state is a module singleton (one dialog
//! at a time).

// ===== Public types =====

enum FileDialogResult {
    none        //!< still open this frame, or not open
    confirmed   //!< committed a valid selection this frame
    cancelled   //!< cancelled / closed this frame
}

enum FileDialogMode {
    open        //!< pick existing file(s) — multi-select per `max_selection`
    save        //!< type/confirm a name; overwrite-confirm if it exists
    create      //!< "New" — type a name (same name-entry path as save)
    pick_dir    //!< choose a directory — the list shows folders only, OK confirms the selected (or current) folder
}

struct FileDialogFilter {
    caption : string    //!< shown in the filter dropdown, e.g. "Images (*.png *.jpg)" (`label` is reserved in gen2)
    ext : string        //!< extension(s) without the dot, ';'-separated ("png;jpg;jpeg"); "" = all files
}

struct FileDialogConfig {
    title : string = "Select File"
    start_dir : string = ""                 //!< absolute dir to open at; "" = cwd
    filters : array<FileDialogFilter>       //!< empty => all files
    mode : FileDialogMode = FileDialogMode.open
    max_selection : int = 1                 //!< open mode; 0 = unlimited
    default_name : string = ""              //!< save/create: pre-filled filename
}

// ===== Internal state =====

struct private Entry {
    name : string
    is_dir : bool
    size : uint64
    ext : string    //!< dotted extension (".das"), "" if none / dir — drives the Type cell + Type sort
    mtime : clock   //!< last-modified time — drives the Modified cell + Modified sort
}

struct private DialogState {
    title : string
    cur_dir : string
    start_dir : string
    mode : FileDialogMode = FileDialogMode.open
    max_selection : int = 1
    filters : array<FileDialogFilter>
    filter_labels : array<string>           // cached for the combo (no per-frame alloc)
    filter_idx : int = 0
    entries : array<Entry>                  // current dir, filtered
    view : array<int>                       // indices into entries, sorted
    selection : array<int>                  // selected entry indices
    anchor : int = -1                       // shift-range anchor (entry index)
    crumbs : array<string>                  // ancestor paths, root-first
    result : array<string>                  // confirmed absolute path(s)
    pending_leaf : string                   // name awaiting overwrite-confirm
    sort_col : int = 1                       // column_user_id (COL_NAME)
    sort_desc : bool = false
    open : bool = false
    error : string
    dir_error : string                      // error shown inside the New Folder sub-modal
    path_focused : bool = false             // PATH_FIELD held focus last frame (routes Enter: navigate vs confirm)
}

var private g_fd : DialogState = DialogState()
var private g_last_dir : string   //!< dir of the last confirmed selection — reused as the default start_dir within a session (app may persist it via file_dialog_last_dir / file_dialog_set_last_dir)

let private COL_NAME = 1u
let private COL_SIZE = 2u
let private COL_TYPE = 3u
let private COL_DATE = 4u

// Indexed widget-state tables (the dynamic-row / dynamic-button idiom).
var private ROW : table<int; ClickState>
var private SIZE_CELL : table<int; NarrativeState>
var private TYPE_CELL : table<int; NarrativeState>
var private DATE_CELL : table<int; NarrativeState>
var private CRUMB : table<int; ClickState>
var private CRUMB_SL : table<int; EmptyMarkerState>

// ===== Helpers =====

def private active_ext() : string {
    if (g_fd.filter_idx >= 0 && g_fd.filter_idx < length(g_fd.filters)) {
        return g_fd.filters[g_fd.filter_idx].ext
    }
    return ""
}

// "foo.json" matches the pre-split pattern list (["json"] / ["png","jpg"]) iff it ends in exactly
// ".<one of them>" (via fio's replace_extension — never hand-rolled rfind/slice); empty list = all files.
// The caller splits the active filter ONCE so scan_dir doesn't re-split (and leak) it per file.
def private matches_ext(name : string; pats : array<string>) : bool {
    if (empty(pats)) {
        return true
    }
    let base = replace_extension(name, "")
    for (e in pats) {
        if (!empty(e) && "{base}.{e}" == name) {
            return true
        }
    }
    return false
}

def private has_ext(name : string; ext : string) : bool {
    if (empty(ext)) {
        return true
    }
    var pats <- split(ext, ";")
    let ok = matches_ext(name, pats)
    delete pats
    return ok
}

def private ensure_ext(name : string; ext : string) : string {
    if (empty(ext) || has_ext(name, ext)) {
        return name
    }
    var parts <- split(ext, ";")
    // append the first NON-EMPTY pattern; a spec like ";png" or ";" must not yield "name."
    var leaf = name
    for (p in parts) {
        if (!empty(p)) {
            leaf = "{name}.{p}"
            break
        }
    }
    delete parts
    return leaf
}

//! Build a filter list from a C#-style pipe spec — alternating caption / extension
//! list: ``"daslang|das|Images|png;jpg;jpeg|All files|"``. Each extension list is
//! ';'-separated (matching `FileDialogFilter.ext`); an empty list means all files.
def public file_filters(spec : string) : array<FileDialogFilter> {
    var parts <- split(spec, "|")
    // ceil-pair: an odd part count (user dropped the final '|') keeps the trailing caption
    // as an all-files filter ("") rather than silently truncating it off the list.
    var out <- [for (i in range((length(parts) + 1) / 2));
        FileDialogFilter(caption = parts[i * 2],
                         ext = i * 2 + 1 < length(parts) ? parts[i * 2 + 1] : "")]
    delete parts
    return <- out
}

// Human-readable size with one decimal. uint interpolates as hex, so the
// tenths are computed with integer math and rendered through int (decimal).
def private fmt_size(b : uint64) : string {
    if (b < 1024ul) {
        return "{int(b)} B"
    } elif (b < 1048576ul) {
        let t = int(b * 10ul / 1024ul)
        return "{t / 10}.{t % 10} KB"
    } elif (b < 1073741824ul) {
        let t = int(b * 10ul / 1048576ul)
        return "{t / 10}.{t % 10} MB"
    }
    let t = int64(b * 10ul / 1073741824ul)   // int64: the GB tier is unbounded (int32 overflows >~214 TB)
    return "{t / 10}.{t % 10} GB"
}

def private is_valid_filename(name : string) : bool {
    if (empty(name) || name == "." || name == "..") {
        return false
    }
    var ok = true
    peek_data(name) $(bytes) {
        for (c in bytes) {
            if (c == uint8(47) || c == uint8(92)) {     // '/' or '\' — no path separators
                ok = false
            }
        }
    }
    return ok
}

def private scan_dir() {
    g_fd.entries |> clear()
    let ext = active_ext()
    let dirs_only = g_fd.mode == FileDialogMode.pick_dir
    // Split the active filter's pattern list ONCE here, not per file in matches_ext; guard empty ext (split("", ";") yields [""] not [], which would break the "all files" case).
    var pats : array<string>
    if (!empty(ext)) {
        pats <- split(ext, ";")
    }
    dir(g_fd.cur_dir) $(nm) {
        if (nm == "." || nm == "..") {
            return
        }
        let fs = stat(path_join(g_fd.cur_dir, nm))
        if (!fs.is_valid) {
            return
        }
        if (fs.is_dir) {
            g_fd.entries |> push(Entry(name = nm, is_dir = true, size = 0ul, ext = "", mtime = fs.mtime))
        } elif (!dirs_only && matches_ext(nm, pats)) {
            g_fd.entries |> push(Entry(name = nm, is_dir = false, size = fs.size, ext = extension(nm), mtime = fs.mtime))
        }
    }
    delete pats
}

def private compare_entry(a : Entry; b : Entry) : int {
    var ord = 0
    if (g_fd.sort_col == int(COL_SIZE)) {
        if (a.size < b.size) {
            ord = -1
        } elif (a.size > b.size) {
            ord = 1
        }
    } elif (g_fd.sort_col == int(COL_TYPE)) {
        if (a.ext < b.ext) {
            ord = -1
        } elif (a.ext > b.ext) {
            ord = 1
        }
    } elif (g_fd.sort_col == int(COL_DATE)) {
        if (a.mtime < b.mtime) {
            ord = -1
        } elif (a.mtime > b.mtime) {
            ord = 1
        }
    } else {
        if (a.name < b.name) {
            ord = -1
        } elif (a.name > b.name) {
            ord = 1
        }
    }
    return ord
}

def private sort_view() {
    g_fd.view |> sort() $(ia, ib) {
        let a = g_fd.entries[ia]
        let b = g_fd.entries[ib]
        if (a.is_dir != b.is_dir) {
            return a.is_dir                  // dirs first, always
        }
        var ord = compare_entry(a, b)
        if (g_fd.sort_desc) {
            ord = -ord
        }
        if (ord != 0) {
            return ord < 0
        }
        return a.name < b.name               // stable tiebreak
    }
}

def private rebuild_view() {
    g_fd.view <- [for (i in range(length(g_fd.entries))); i]
    sort_view()
}

def private rebuild_crumbs() {
    var tmp : array<string>
    tmp |> reserve(64)
    var p = g_fd.cur_dir
    for (_ in range(64)) {
        tmp |> push(p)
        let par = parent(p)
        if (par == p) {
            break
        }
        p = par
    }
    g_fd.crumbs <- [for (i in range(length(tmp))); tmp[length(tmp) - 1 - i]]
    delete tmp
}

def private navigate_to(d : string) {
    g_fd.cur_dir = normalize(d)
    g_fd.selection |> clear()
    g_fd.anchor = -1
    g_fd.error = ""
    // keep the editable path field in sync with where we are
    PATH_FIELD.pending_value = g_fd.cur_dir
    PATH_FIELD.has_pending = true
    scan_dir()
    rebuild_view()
    rebuild_crumbs()
}

def private is_selected(idx : int) : bool {
    for (s in g_fd.selection) {
        if (s == idx) {
            return true
        }
    }
    return false
}

def private apply_click(idx : int; ctrl : bool; shift : bool) {
    let multi = g_fd.mode == FileDialogMode.open && g_fd.max_selection != 1
    if (!multi || (!ctrl && !shift)) {
        g_fd.selection |> clear()
        g_fd.selection |> push(idx)
        g_fd.anchor = idx
        return
    }
    if (shift && g_fd.anchor >= 0) {
        var lo = -1
        var hi = -1
        for (vi in range(length(g_fd.view))) {
            if (g_fd.view[vi] == g_fd.anchor) {
                lo = vi
            }
            if (g_fd.view[vi] == idx) {
                hi = vi
            }
        }
        if (lo > hi) {
            let t = lo; lo = hi; hi = t
        }
        if (lo >= 0) {
            g_fd.selection <- [for (vi in range(lo, hi + 1)); g_fd.view[vi]]
        } else {
            g_fd.selection |> clear()
        }
    } else {
        // ctrl-toggle: drop idx if present, else add it
        let was_sel = is_selected(idx)
        var ns <- [for (s in g_fd.selection); s; where s != idx]
        if (!was_sel) {
            ns |> push(idx)
        }
        g_fd.selection <- ns
        g_fd.anchor = idx
    }
    // clamp to max_selection (keep the most recent N)
    if (g_fd.max_selection > 0 && length(g_fd.selection) > g_fd.max_selection) {
        let start = length(g_fd.selection) - g_fd.max_selection
        g_fd.selection <- [for (k in range(start, length(g_fd.selection))); g_fd.selection[k]]
    }
}

def private name_exists(leaf : string) : bool {
    for (e in g_fd.entries) {
        if (!e.is_dir && e.name == leaf) {
            return true
        }
    }
    return false
}

def private confirm_single(path : string) : FileDialogResult {
    g_fd.result |> clear()
    g_fd.result |> push(path)
    FILEDLG.pending_close = true
    g_fd.open = false
    g_last_dir = g_fd.cur_dir   // remember the folder so the next open starts here, not at cwd
    return FileDialogResult.confirmed
}

def private confirm_multi(var paths : array<string>) : FileDialogResult {
    g_fd.result <- paths
    FILEDLG.pending_close = true
    g_fd.open = false
    g_last_dir = g_fd.cur_dir
    return FileDialogResult.confirmed
}

def private try_confirm() : FileDialogResult {
    g_fd.error = ""
    if (g_fd.mode == FileDialogMode.pick_dir) {
        // a selected folder, else the directory we're currently browsing
        if (length(g_fd.selection) == 1 && g_fd.entries[g_fd.selection[0]].is_dir) {
            return confirm_single(path_join(g_fd.cur_dir, g_fd.entries[g_fd.selection[0]].name))
        }
        return confirm_single(g_fd.cur_dir)
    }
    if (g_fd.mode == FileDialogMode.open) {
        var paths <- [for (s in g_fd.selection);
            path_join(g_fd.cur_dir, g_fd.entries[s].name);
            where !g_fd.entries[s].is_dir]
        if (!empty(paths)) {
            return confirm_multi(paths)
        }
        if (length(g_fd.selection) == 1 && g_fd.entries[g_fd.selection[0]].is_dir) {
            navigate_to(path_join(g_fd.cur_dir, g_fd.entries[g_fd.selection[0]].name))
            return FileDialogResult.none
        }
        g_fd.error = "select a file"
        return FileDialogResult.none
    }
    // save / create (name mode)
    let raw = NAME_FIELD.value
    if (!is_valid_filename(raw)) {
        g_fd.error = "invalid name"
        return FileDialogResult.none
    }
    let leaf = ensure_ext(raw, active_ext())
    if (name_exists(leaf)) {
        g_fd.pending_leaf = leaf
        OVERWRITE.pending_open = true
        return FileDialogResult.none
    }
    return confirm_single(path_join(g_fd.cur_dir, leaf))
}

// ===== Public API =====

//! Queue the dialog to open next frame and snapshot `start_dir`'s listing.
def public file_dialog_open(cfg : FileDialogConfig) {
    g_fd.title = cfg.title
    g_fd.mode = cfg.mode
    g_fd.max_selection = cfg.max_selection
    g_fd.filters := cfg.filters
    g_fd.filter_labels <- [for (f in cfg.filters); f.caption]
    g_fd.filter_idx = 0
    FILTER_COMBO.value = empty(cfg.filters) ? -1 : 0
    // start at the explicit start_dir, else the last folder used this session, else cwd
    let fallback = empty(g_last_dir) ? getcwd() : g_last_dir
    g_fd.start_dir = normalize(empty(cfg.start_dir) ? fallback : cfg.start_dir)
    g_fd.cur_dir = g_fd.start_dir
    g_fd.selection |> clear()
    g_fd.result |> clear()
    g_fd.anchor = -1
    g_fd.sort_col = int(COL_NAME)
    g_fd.sort_desc = false
    g_fd.error = ""
    g_fd.dir_error = ""
    g_fd.path_focused = false
    g_fd.open = true
    NAME_FIELD.pending_value = cfg.default_name   // "" for open/pick_dir callers; pre-fill for save/create
    NAME_FIELD.has_pending = true
    PATH_FIELD.pending_value = g_fd.cur_dir
    PATH_FIELD.has_pending = true
    ROW |> clear()
    SIZE_CELL |> clear()
    TYPE_CELL |> clear()
    DATE_CELL |> clear()
    CRUMB |> clear()
    CRUMB_SL |> clear()
    scan_dir()
    rebuild_view()
    rebuild_crumbs()
    FILEDLG.pending_open = true
}

//! The folder of the last confirmed selection this session ("" if none yet). Persist
//! it to app config on exit, then restore via file_dialog_set_last_dir on startup to
//! make the dialog reopen at the user's last folder across runs.
def public file_dialog_last_dir() : string {
    return g_last_dir
}

//! Seed the remembered start folder (e.g. from saved app config on startup). An empty
//! string clears it (back to cwd-default).
def public file_dialog_set_last_dir(dir : string) {
    g_last_dir = empty(dir) ? "" : normalize(dir)
}

//! Render the modal if open and return this frame's result. Call every frame.
def public file_dialog_draw() : FileDialogResult {
    var result = FileDialogResult.none
    // A row double-click navigates / confirms, both of which rebuild
    // entries/view — deferred out of the row loop (mutating mid-iteration
    // invalidates the loop's indices). Captured here, applied after the table.
    var nav_to = ""
    var confirm_path = ""
    SetNextWindowSize(ImVec2(720.0f, 520.0f), ImGuiCond.FirstUseEver)
    popup_modal(FILEDLG, (text = g_fd.title, closable = false, flags = ImGuiWindowFlags.None)) {
        // PATH_FIELD's focus is sampled at the END of the body; capture last frame's
        // value now so the Enter handler (also at the end) can route Enter to path-nav
        // vs confirm even though the input deactivates itself on the Enter keystroke.
        let was_path_focused = g_fd.path_focused
        // --- breadcrumb / nav ---
        if (button(UP, (text = ".."))) {
            nav_to = parent(g_fd.cur_dir)
        }
        for (ci in range(length(g_fd.crumbs))) {
            let seg = g_fd.crumbs[ci]
            let lbl = base_name(seg)
            same_line(CRUMB_SL[ci])
            if (button(CRUMB[ci], (text = empty(lbl) ? seg : lbl))) {
                nav_to = seg            // deferred: navigate_to rebuilds crumbs — can't run mid-loop
            }
        }
        // --- editable path + drive jump + new folder ---
        input_text(PATH_FIELD, (text = "##path"))
        g_fd.path_focused = IsItemActive()
        same_line()
        if (button(GO, (text = "Go"))) {
            let p = normalize(PATH_FIELD.value)
            if (stat(p).is_dir) {
                nav_to = p
            } else {
                g_fd.error = "no such directory"
            }
        }
        same_line()
        if (button(NEWFOLDER, (text = "New Folder"))) {
            g_fd.dir_error = ""
            NEWDIR_NAME.pending_value = ""
            NEWDIR_NAME.has_pending = true
            NEWDIR.pending_open = true
        }
        // --- filter (extensions are meaningless in directory-pick mode) ---
        if (!empty(g_fd.filter_labels) && g_fd.mode != FileDialogMode.pick_dir) {
            combo(FILTER_COMBO, (text = "##filter", items := g_fd.filter_labels))
            if (FILTER_COMBO.value != g_fd.filter_idx && FILTER_COMBO.value >= 0) {
                g_fd.filter_idx = FILTER_COMBO.value
                // selection holds indices into `entries`; scan_dir rebuilds it, so old
                // indices would dangle (OOB / wrong row in try_confirm). Clear them.
                g_fd.selection |> clear()
                g_fd.anchor = -1
                scan_dir()
                rebuild_view()
            }
        }
        separator(NAV_SEP)
        // --- file table ---
        let tflags = (ImGuiTableFlags.BordersOuter | ImGuiTableFlags.RowBg |
                      ImGuiTableFlags.Resizable | ImGuiTableFlags.ScrollY |
                      ImGuiTableFlags.Sortable)
        data_table(FILE_TABLE, (text = "##files", columns = 4, flags = tflags,
                                outer_size = float2(0.0f, 340.0f), inner_width = 0.0f)) {
            table_setup_scroll_freeze(0, 1)
            table_setup_column("Name", ImGuiTableColumnFlags.DefaultSort | ImGuiTableColumnFlags.WidthStretch, 0.0f, COL_NAME)
            table_setup_column("Size", ImGuiTableColumnFlags.WidthFixed, 90.0f, COL_SIZE)
            table_setup_column("Type", ImGuiTableColumnFlags.WidthFixed, 80.0f, COL_TYPE)
            table_setup_column("Modified", ImGuiTableColumnFlags.WidthFixed, 130.0f, COL_DATE)
            table_headers_row()
            sort_specs() $(specs) {
                if (!empty(specs)) {
                    g_fd.sort_col = int(specs[0].column_user_id)
                    g_fd.sort_desc = specs[0].sort_direction == ImGuiSortDirection.Descending
                }
                sort_view()
            }
            for (idx in g_fd.view) {
                let e = g_fd.entries[idx]
                table_next_row()
                table_set_column_index(0)
                if (selectable_label(ROW[idx], (text = e.name, selected = is_selected(idx),
                                                flags = ImGuiSelectableFlags.SpanAllColumns | ImGuiSelectableFlags.AllowDoubleClick))) {
                    if (IsMouseDoubleClicked(ImGuiMouseButton.Left)) {
                        if (e.is_dir) {
                            nav_to = path_join(g_fd.cur_dir, e.name)
                        } else {
                            confirm_path = path_join(g_fd.cur_dir, e.name)
                        }
                    } else {
                        let io & = unsafe(GetIO())
                        apply_click(idx, io.KeyCtrl, io.KeyShift)
                        if (g_fd.mode != FileDialogMode.open && !e.is_dir) {
                            NAME_FIELD.pending_value = e.name
                            NAME_FIELD.has_pending = true
                        }
                    }
                }
                table_set_column_index(1)
                text(SIZE_CELL[idx], (text = e.is_dir ? "" : fmt_size(e.size)))
                table_set_column_index(2)
                text(TYPE_CELL[idx], (text = e.is_dir ? "Folder" : (empty(e.ext) ? "File" : e.ext)))
                table_set_column_index(3)
                text(DATE_CELL[idx], (text = format_time(e.mtime, "%Y-%m-%d %H:%M")))
            }
        }
        // Apply the deferred row action now the loop is finished (safe to rebuild).
        if (!empty(nav_to)) {
            navigate_to(nav_to)
        } elif (!empty(confirm_path)) {
            if (g_fd.mode == FileDialogMode.open) {
                result = confirm_single(confirm_path)
            } else {
                // save/create: a double-clicked file exists by definition, so it must pass
                // the same overwrite gate as the name+OK path instead of confirming directly.
                g_fd.pending_leaf = base_name(confirm_path)
                NAME_FIELD.pending_value = g_fd.pending_leaf
                NAME_FIELD.has_pending = true
                OVERWRITE.pending_open = true
            }
        }
        separator(BOTTOM_SEP)
        // --- bottom bar (name field only in the type-a-name modes) ---
        if (g_fd.mode == FileDialogMode.save || g_fd.mode == FileDialogMode.create) {
            input_text(NAME_FIELD, (text = "Name"))
        }
        if (!empty(g_fd.error)) {
            text(ERR_LABEL, (text = g_fd.error))
        }
        let ok_label = (g_fd.mode == FileDialogMode.open ? "Open" :
                        g_fd.mode == FileDialogMode.pick_dir ? "Select Folder" :
                        g_fd.mode == FileDialogMode.save ? "Save" : "Create")
        if (button(BTN_OK, (text = ok_label))) {
            result = try_confirm()
        }
        same_line()
        if (button(BTN_CANCEL, (text = "Cancel"))) {
            FILEDLG.pending_close = true
            g_fd.open = false
            result = FileDialogResult.cancelled
        }
        // --- overwrite-confirm (save / create) ---
        SetNextWindowSize(ImVec2(360.0f, 0.0f), ImGuiCond.FirstUseEver)
        popup_modal(OVERWRITE, (text = "Overwrite?", closable = false, flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            text(OW_MSG, (text = "'{g_fd.pending_leaf}' already exists. Overwrite?"))
            separator(OW_SEP)
            if (button(OW_YES, (text = "Overwrite"))) {
                OVERWRITE.pending_close = true
                result = confirm_single(path_join(g_fd.cur_dir, g_fd.pending_leaf))
            }
            same_line()
            if (button(OW_NO, (text = "Cancel"))) {
                OVERWRITE.pending_close = true
            }
        }
        // --- new folder (NEWDIR_NAME flagless so the Create button reads the live
        // value; Enter is caught here instead of via EnterReturnsTrue, same reason
        // as NAME_FIELD) ---
        SetNextWindowSize(ImVec2(360.0f, 0.0f), ImGuiCond.FirstUseEver)
        popup_modal(NEWDIR, (text = "New Folder", closable = false, flags = ImGuiWindowFlags.AlwaysAutoResize)) {
            input_text(NEWDIR_NAME, (text = "Name"))
            if (!empty(g_fd.dir_error)) {
                text(NEWDIR_ERR, (text = g_fd.dir_error))
            }
            separator(NEWDIR_SEP)
            let enter = IsKeyPressed(ImGuiKey.Enter, false) || IsKeyPressed(ImGuiKey.KeypadEnter, false)
            if (button(NEWDIR_OK, (text = "Create")) || enter) {
                let nm = NEWDIR_NAME.value
                if (!is_valid_filename(nm)) {
                    g_fd.dir_error = "invalid folder name"
                } else {
                    var err = ""
                    if (mkdir_rec(path_join(g_fd.cur_dir, nm), err)) {
                        NEWDIR.pending_close = true
                        g_fd.dir_error = ""
                        navigate_to(g_fd.cur_dir)   // rescan so the new folder shows up
                    } else {
                        g_fd.dir_error = empty(err) ? "could not create folder" : err
                    }
                }
            }
            same_line()
            if (button(NEWDIR_CANCEL, (text = "Cancel"))) {
                NEWDIR.pending_close = true
                g_fd.dir_error = ""
            }
        }
        // --- keyboard: Enter confirms (or navigates when the path field had focus),
        // Backspace goes up a directory when no text field is being edited ---
        let sub_open = OVERWRITE.open || NEWDIR.open
        if (!sub_open) {
            if (IsKeyPressed(ImGuiKey.Escape, false)) {
                // ESC cancels the dialog (explicit — a focused name/path field would otherwise
                // eat the keystroke and the modal wouldn't auto-close). Mirrors the Cancel button.
                FILEDLG.pending_close = true
                g_fd.open = false
                result = FileDialogResult.cancelled
            } elif (IsKeyPressed(ImGuiKey.Enter, false) || IsKeyPressed(ImGuiKey.KeypadEnter, false)) {
                if (was_path_focused) {
                    let p = normalize(PATH_FIELD.value)
                    if (stat(p).is_dir) {
                        navigate_to(p)
                    } else {
                        g_fd.error = "no such directory"
                    }
                } else {
                    result = try_confirm()
                }
            } elif (!IsAnyItemActive() && IsKeyPressed(ImGuiKey.Backspace, false)) {
                navigate_to(parent(g_fd.cur_dir))
            }
        }
    }
    // ESC / external close with no committed result this frame ⇒ cancel
    if (g_fd.open && !FILEDLG.open && result == FileDialogResult.none) {
        g_fd.open = false
        result = FileDialogResult.cancelled
    }
    return result
}

//! Confirmed absolute path(s). Valid on the `confirmed` frame and until reopened.
def public file_dialog_selection() : array<string> {
    var out : array<string>
    out := g_fd.result
    return <- out
}

//! First confirmed path (convenience for single-select callers).
def public file_dialog_selected_path() : string {
    return empty(g_fd.result) ? "" : g_fd.result[0]
}

def public file_dialog_current_dir() : string => g_fd.cur_dir

def public file_dialog_is_open() : bool => g_fd.open
