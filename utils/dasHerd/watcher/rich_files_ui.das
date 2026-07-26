options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_files_ui

require imgui/imgui_harness
require imgui/imgui_icons
require imgui/imgui_drawlist_builtin
require imgui/imgui_docking_builtin
require daslib/safe_addr
require daslib/json_boost
require daslib/strings_boost
require live/live_commands
require ./watcher_core.das
require ./repository_core.das
require ./rich_state.das
require ./rich_net.das
require ./rich_inspector_ui.das
require math
require strings

var private FILES_ROW : table<int; ToggleState>
var private FILES_INDENT : table<int; EmptyMarkerState>
var private FILES_ROW_POPUP : table<int; PopupContextItemState>
var private FILES_SEND : table<int; ToggleState>
var private FILES_INDENT_JOIN : table<int; EmptyMarkerState>
var private FILES_ICON_JOIN : table<int; EmptyMarkerState>

let private PROJECT_SORT_LABELS <- ["Folders first", "Name", "Largest", "Newest"]

struct private ProjectNode {
    name : string
    path : string
    parent : int = -1
    is_folder : bool
    hidden : bool
    dirty : bool
    // A nested git repository (git lists it as one "dir/" entry) — its
    // contents are invisible to this repository's file listing.
    external : bool
    stat_size : int64
    stat_mtime : int64
    children : array<int>
}

var private g_project_nodes : array<ProjectNode>
var private g_project_index : table<string; int>
var private g_project_model_identity = ""
var private g_project_file_count = 0
var private g_project_filter_key = ""
var private g_project_filter_matches : array<int>

def ensure_worktree_files() {
    if (g_client == null || !g_client.connected
            || empty(g_selected_repository_id)
            || empty(g_selected_worktree_path)) return
    let identity = "{g_selected_repository_id}\n{g_selected_worktree_path}"
    if (identity == g_worktree_files_identity || g_worktree_files_loading) return
    g_client->request_worktree_files(
        g_selected_repository_id, g_selected_worktree_path)
}

def private open_worktree_file(path : string) {
    if (g_client == null) return
    ui_event("worktree-file-click {path}")
    g_client->inspect_file(g_selected_repository_id,
        g_selected_worktree_path, path, "working")
    FILE_INSPECTOR_WIN.open = true
    FILE_INSPECTOR_WIN.pending_raise = true
}

def private project_folder_node(parent_index : int; name, path : string) : int {
    if (key_exists(g_project_index, path)) return g_project_index?[path] ?? 0
    let index = length(g_project_nodes)
    g_project_nodes |> emplace(ProjectNode(name = name, path = path,
        parent = parent_index, is_folder = true,
        hidden = starts_with(name, ".")))
    g_project_index[path] = index
    g_project_nodes[parent_index].children |> push(index)  // nolint:PERF006 -- per-folder child lists are small and unknowable up front
    return index
}

def private project_stats_needed : bool {
    return (g_ui_settings.project_show_size || g_ui_settings.project_show_date
        || g_ui_settings.project_sort >= 2)
}

def private project_fill_stats() {
    for (node in g_project_nodes) {
        if (node.is_folder) continue
        let st = stat(path_join(g_selected_worktree_path, node.path))
        if (!st.is_valid) continue
        node.stat_size = int64(st.size)
        node.stat_mtime = int64(st.mtime)
        // Folders aggregate the sizes and freshest mtime of their subtree.
        var parent = node.parent
        while (parent >= 0) {
            g_project_nodes[parent].stat_size += node.stat_size
            g_project_nodes[parent].stat_mtime = max(
                g_project_nodes[parent].stat_mtime, node.stat_mtime)
            parent = g_project_nodes[parent].parent
        }
    }
}

def private project_mark_dirty() {
    if (g_client == null) return
    for (repository in g_client.repositories) {
        if (repository.record.id != g_selected_repository_id) continue
        for (file in repository.files) {
            if (file.worktree_path != g_selected_worktree_path) continue
            // Status reports untracked/nested directories with a trailing
            // slash; the index keys carry none.
            var status_path = file.path
            if (ends_with(status_path, "/")) {
                status_path = slice(status_path, 0, length(status_path) - 1)
            }
            if (!key_exists(g_project_index, status_path)) continue
            var index = g_project_index?[status_path] ?? 0
            while (index >= 0) {
                if (g_project_nodes[index].dirty) break
                g_project_nodes[index].dirty = true
                index = g_project_nodes[index].parent
            }
        }
    }
}

def private project_child_before(a, b : int) : bool {
    let na & = unsafe(g_project_nodes[a])
    let nb & = unsafe(g_project_nodes[b])
    let mode = g_ui_settings.project_sort
    if (mode != 1 && na.is_folder != nb.is_folder) return na.is_folder
    if (mode == 2 && na.stat_size != nb.stat_size) return na.stat_size > nb.stat_size
    if (mode == 3 && na.stat_mtime != nb.stat_mtime) return na.stat_mtime > nb.stat_mtime
    return na.name < nb.name
}

def private project_sort_children() {
    for (node in g_project_nodes) {
        if (empty(node.children)) continue
        var order := node.children
        sort(order) $(a, b) => project_child_before(a, b)
        node.children <- order
    }
}

def private project_repository_revision : int64 {
    if (g_client != null) {
        for (repository in g_client.repositories) {
            if (repository.record.id == g_selected_repository_id) {
                return repository.revision
            }
        }
    }
    return 0l
}

def private ensure_project_model() {
    let identity = ("{g_worktree_files_identity}\n{project_repository_revision()}\n" +
        "{g_ui_settings.project_sort}\n{project_stats_needed()}")
    if (identity == g_project_model_identity) return
    g_project_model_identity = identity
    g_project_filter_key = ""
    delete g_project_nodes
    delete g_project_index
    g_project_nodes |> reserve(length(g_worktree_files)
        + length(g_worktree_files) / 8 + 16)
    g_project_nodes |> emplace(ProjectNode(name = "", path = "", is_folder = true))
    g_project_file_count = 0
    for (path in g_worktree_files) {
        // A nested git repository (or fully untracked directory) lists as a
        // single "dir/" entry: build the folder chain, add no file node.
        if (ends_with(path, "/")) {
            var inscope dir_segments <- split(slice(path, 0, length(path) - 1), "/")
            var dir_parent = 0
            var dir_prefix = ""
            for (segment in dir_segments) {
                dir_prefix = (empty(dir_prefix) ? segment
                    : "{dir_prefix}/{segment}")
                dir_parent = project_folder_node(dir_parent,
                    segment, dir_prefix)
            }
            g_project_nodes[dir_parent].external = true
            continue
        }
        g_project_file_count++
        var inscope segments <- split(path, "/")
        var parent = 0
        var prefix = ""
        for (depth in range(length(segments) - 1)) {
            prefix = empty(prefix) ? segments[depth] : "{prefix}/{segments[depth]}"
            parent = project_folder_node(parent, segments[depth], prefix)
        }
        let name = segments[length(segments) - 1]
        let index = length(g_project_nodes)
        g_project_nodes |> emplace(ProjectNode(name = name, path = path,
            parent = parent, hidden = starts_with(name, ".")))
        g_project_index[path] = index
        g_project_nodes[parent].children |> push(index)  // nolint:PERF006 -- per-folder child lists are small and unknowable up front
    }
    if (project_stats_needed()) {
        project_fill_stats()
    }
    project_mark_dirty()
    project_sort_children()
}

def private project_name_color(node : ProjectNode const) : uint {
    let alpha = node.hidden ? 0.55f : 1.0f
    return (node.is_folder
        ? GetColorU32(token_path_dir_color(alpha))
        : GetColorU32(token_path_name_color(alpha)))
}

def private project_size_label(size : int64) : string {
    if (size >= 10485760l) return "{size / 1048576l} MB"
    if (size >= 10240l) return "{size / 1024l} KB"
    return "{size} B"
}

def private project_age_label(mtime : int64) : string {
    let age = int64(get_clock()) - mtime
    if (age < 60l) return "now"
    if (age < 3600l) return "{age / 60l}m"
    if (age < 86400l) return "{age / 3600l}h"
    if (age < 2592000l) return "{age / 86400l}d"
    return "{age / 2592000l}mo"
}

// Right-aligned size/modified columns plus the amber dirty marker, drawn
// over the row like the changelist stats columns.
def private decorate_project_row(node : ProjectNode const;
                                 name_position : float2) {
    let row_max = GetItemRectMax()
    let row_right = git_visible_row_right(row_max.x)
    let dirty_color = GetColorU32(token_warning_color())
    with_window_drawlist() $(var dl) {
        let name_text = node.name + (node.is_folder ? "/" : "")
        dl |> add_text(name_position, project_name_color(node), name_text)
        var marker_x = name_position.x + CalcTextSize(name_text).x + 4.0f
        // Tag only a portal ROOT — every folder inside stays external for
        // recursive listing, but repeating the tag would be noise.
        if (node.external && node.parent >= 0
                && !g_project_nodes[node.parent].external) {
            let repo_tag = "(repo)"
            dl |> add_text(float2(marker_x, name_position.y),
                GetColorU32(ImGuiCol.TextDisabled), repo_tag)
            marker_x += CalcTextSize(repo_tag).x + 4.0f
        }
        if (node.dirty) {
            dl |> add_text(float2(marker_x, name_position.y),
                dirty_color, "*")
        }
        var column_x = row_right
        if (g_ui_settings.project_show_date && node.stat_mtime > 0l) {
            let age_text = project_age_label(node.stat_mtime)
            column_x -= 52.0f
            dl |> add_text(float2(column_x, name_position.y),
                GetColorU32(ImGuiCol.TextDisabled), age_text)
        }
        if (g_ui_settings.project_show_size && node.stat_size > 0l) {
            let size_text = project_size_label(node.stat_size)
            column_x -= CalcTextSize(size_text).x + 12.0f
            dl |> add_text(float2(column_x, name_position.y),
                GetColorU32(ImGuiCol.TextDisabled), size_text)
        }
    }
}

def private draw_project_row(var row : int&; node_index, depth : int) : bool {
    let node & = unsafe(g_project_nodes[node_index])
    if (depth > 0) {
        dummy(FILES_INDENT[row], (size = float2(float(depth) * 18.0f, 1.0f)))
        same_line(FILES_INDENT_JOIN[row])
    }
    icon(node.is_folder ? "folder" : "edit", GetTextLineHeight() * 0.85f)
    same_line(FILES_ICON_JOIN[row])
    FILES_ROW[row].value = (!node.is_folder
        && g_inspector.file_path == node.path
        && g_inspector.comparison == "working")
    var clicked = false
    with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
        clicked = selectable(FILES_ROW[row],
            (text = "{node.name}##wtf:{node.path}"))
    }
    let row_min = GetItemRectMin()
    let text_y = row_min.y + max(0.0f,
        (GetItemRectMax().y - row_min.y - GetTextLineHeight()) * 0.5f)
    decorate_project_row(node,
        float2(row_min.x + GetStyle().FramePadding.x, text_y))
    if (!node.is_folder) {
        popup_context_item(FILES_ROW_POPUP[row],
                (str_id = "wtfile:{node.path}", flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(FILES_SEND[row], (text = "Send to session agent",
                    shortcut = "",
                    enabled = g_client != null && g_client.connected
                        && !empty(g_client.selected_id)))) {
                g_client->post_file_focus(g_selected_repository_id,
                    g_selected_worktree_path, node.path)
                g_status = "File sent to session agent"
            }
        }
    }
    row++
    return clicked
}

// Note 37: a nested repository is one opaque "dir/" entry in the parent's
// listing. Expanding it lists the filesystem directly — a local portal into
// its contents. Folder children stay external so the portal recurses; .git
// stays out of the tree.
def private populate_external_folder(node_index : int) {
    var inscope names : array<string>
    {
        let full = path_join(g_selected_worktree_path,
            g_project_nodes[node_index].path)
        dir(full) $(name) {
            if (name != "." && name != ".." && name != ".git") {
                names |> push(clone_string(name))
            }
        }
    }
    for (name in names) {
        let child_path = "{g_project_nodes[node_index].path}/{name}"
        let st = stat(path_join(g_selected_worktree_path, child_path))
        let is_dir = st.is_valid && st.is_dir
        let child_index = length(g_project_nodes)
        g_project_nodes |> emplace(ProjectNode(name = name, path = child_path,
            parent = node_index, is_folder = is_dir,
            hidden = starts_with(name, "."), external = is_dir,
            stat_size = (st.is_valid && !is_dir ? int64(st.size) : 0l),
            stat_mtime = (st.is_valid ? int64(st.mtime) : 0l)))
        g_project_index[child_path] = child_index
        g_project_nodes[node_index].children |> push(child_index)
    }
    var order := g_project_nodes[node_index].children
    sort(order) $(a, b) => project_child_before(a, b)
    g_project_nodes[node_index].children <- order
}

def private draw_project_tree() {
    var row = 0
    // Explicit stack: (node, depth) pairs, children pushed in reverse so the
    // sorted order draws top-down.
    var inscope stack : array<tuple<int; int>>
    stack |> reserve(64)
    let root_children & = unsafe(g_project_nodes[0].children)
    for (index in range(length(root_children))) {
        stack |> push(root_children[length(root_children) - 1 - index] => 0)
    }
    while (!empty(stack)) {
        let top = stack[length(stack) - 1]
        let node_index = top._0
        let depth = top._1
        stack |> pop()
        let is_folder = g_project_nodes[node_index].is_folder
        let path = g_project_nodes[node_index].path
        let expanded = (is_folder && key_exists(g_files_expanded, path)
            && g_files_expanded[path])
        if (draw_project_row(row, node_index, depth)) {
            if (is_folder) {
                g_files_expanded[path] = !expanded
            } else {
                open_worktree_file(path)
            }
        }
        if (is_folder && expanded) {
            if (g_project_nodes[node_index].external
                    && empty(g_project_nodes[node_index].children)) {
                populate_external_folder(node_index)
            }
            let children & = unsafe(g_project_nodes[node_index].children)
            for (index in range(length(children))) {
                stack |> push(children[length(children) - 1 - index] => depth + 1)
            }
        }
    }
    if (row == 0 && !g_worktree_files_loading) {
        text_disabled("Select a worktree to browse its files")
    }
}

def private ensure_project_filter(filter : string; include_sub : bool) {
    let key = "{filter}\n{include_sub}\n{g_project_model_identity}"
    if (key == g_project_filter_key) return
    g_project_filter_key = key
    delete g_project_filter_matches
    g_project_filter_matches |> reserve(64)
    let needle = to_lower(filter)
    for (index in range(length(g_project_nodes))) {
        let node & = unsafe(g_project_nodes[index])
        if (node.is_folder
                || (!include_sub && node.parent != 0)
                || find(to_lower(node.name), needle) < 0) continue
        g_project_filter_matches |> push(index)
    }
}

def private draw_project_filter_results(filter : string; include_sub : bool) {
    ensure_project_filter(filter, include_sub)
    var row = 0
    list_clipper(length(g_project_filter_matches)) $(start, end_) {
        row = start
        for (match_index in range(start, end_)) {
            let node_index = g_project_filter_matches[match_index]
            let node & = unsafe(g_project_nodes[node_index])
            FILES_ROW[row].value = (g_inspector.file_path == node.path
                && g_inspector.comparison == "working")
            var clicked = false
            with_style((ImGuiCol.Text, float4(0.0f, 0.0f, 0.0f, 0.0f))) {
                clicked = selectable(FILES_ROW[row],
                    (text = "{node.path}##wtff:{node.path}"))
            }
            let row_min = GetItemRectMin()
            let text_y = row_min.y + max(0.0f,
                (GetItemRectMax().y - row_min.y - GetTextLineHeight()) * 0.5f)
            draw_git_path_text(node.path,
                float2(row_min.x + GetStyle().FramePadding.x, text_y),
                float2(git_visible_row_right(GetItemRectMax().x), GetItemRectMax().y),
                false)
            if (clicked) open_worktree_file(node.path)
            row++
        }
    }
    if (empty(g_project_filter_matches)) {
        text_disabled("No files match the filter")
    }
}

def draw_project_files() {
    ensure_worktree_files()
    ensure_project_model()
    if (icon_button(FILES_REFRESH, (glyph = "refresh",
            tip = "Re-read the worktree file list",
            size = GetTextLineHeight()))) {
        g_worktree_files_identity = ""
    }
    same_line()
    text_disabled(FILES_SUMMARY, (text = g_worktree_files_loading
        ? "Reading file list..."
        : "{g_project_file_count} files  {base_name(g_selected_worktree_path)}"))
    same_line()
    if (small_button(FILES_SORT_CYCLE,
            (text = "Sort: {PROJECT_SORT_LABELS[clamp(g_ui_settings.project_sort, 0, 3)]}"))) {
        g_ui_settings.project_sort = (g_ui_settings.project_sort + 1) % 4
        settings_save()
    }
    same_line()
    FILES_SHOW_SIZE.value = g_ui_settings.project_show_size
    if (checkbox(FILES_SHOW_SIZE, (text = "size"))) {
        g_ui_settings.project_show_size = FILES_SHOW_SIZE.value
        settings_save()
    }
    same_line()
    FILES_SHOW_DATE.value = g_ui_settings.project_show_date
    if (checkbox(FILES_SHOW_DATE, (text = "modified"))) {
        g_ui_settings.project_show_date = FILES_SHOW_DATE.value
        settings_save()
    }
    SetNextItemWidth(220.0f)
    input_text_growable(FILES_FILTER, (text = "Filter"))
    same_line()
    checkbox(FILES_FILTER_SUBFOLDERS, (text = "subfolders"))
    separator()
    child(FILES_TREE_CHILD, (text = "worktree files", size = float2(0.0f, 0.0f),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.None)) {
        let filter = strip(FILES_FILTER.value)
        if (!empty(filter)) {
            draw_project_filter_results(filter, FILES_FILTER_SUBFOLDERS.value)
        } else {
            draw_project_tree()
        }
    }
}

def files_ui_init() {
    FILES_FILTER.capacity = 256
    FILES_FILTER_SUBFOLDERS.value = true
}

def project_model_counts(var folders, files, dirty : int&) {
    folders = 0
    files = 0
    dirty = 0
    for (node in g_project_nodes) {
        if (node.is_folder) {
            folders++
        } else {
            files++
        }
        if (node.dirty) dirty++
    }
    folders = max(0, folders - 1)
}

def project_filter_text : string => strip(FILES_FILTER.value)

def project_filter_match_count : int => length(g_project_filter_matches)
