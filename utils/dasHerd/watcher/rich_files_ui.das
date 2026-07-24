options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_files_ui

require imgui/imgui_harness
require imgui/imgui_icons
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
require strings

var private FILES_ROW : table<int; ToggleState>
var private FILES_INDENT : table<int; EmptyMarkerState>
var private FILES_ROW_POPUP : table<int; PopupContextItemState>
var private FILES_SEND : table<int; ToggleState>

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

def private draw_files_row(var row : int&; depth : int; name, path : string;
                           is_folder, expanded : bool) : bool {
    if (depth > 0) {
        dummy(FILES_INDENT[row], (size = float2(float(depth) * 18.0f, 1.0f)))
        same_line()
    }
    icon(is_folder ? "folder" : "edit", GetTextLineHeight() * 0.85f)
    same_line()
    let suffix = is_folder ? (expanded ? "/" : "/...") : ""
    FILES_ROW[row].value = (!is_folder
        && g_inspector.file_path == path
        && g_inspector.comparison == "working")
    let clicked = selectable(FILES_ROW[row], (text = "{name}{suffix}##wtf:{path}"))
    if (!is_folder) {
        popup_context_item(FILES_ROW_POPUP[row],
                (str_id = "wtfile:{path}", flags = ImGuiPopupFlags.MouseButtonRight)) {
            if (menu_item(FILES_SEND[row], (text = "Send to session agent",
                    shortcut = "",
                    enabled = g_client != null && g_client.connected
                        && !empty(g_client.selected_id)))) {
                g_client->post_file_focus(g_selected_repository_id,
                    g_selected_worktree_path, path)
                g_status = "File sent to session agent"
            }
        }
    }
    row++
    return clicked
}

def draw_worktree_files_window() {
    ensure_worktree_files()
    if (icon_button_draw("##wt-files-refresh", "refresh", GetTextLineHeight())) {
        g_worktree_files_identity = ""
    }
    same_line()
    text_disabled(FILES_SUMMARY, (text = g_worktree_files_loading
        ? "Reading file list..."
        : "{length(g_worktree_files)} files  {base_name(g_selected_worktree_path)}"))
    if (!empty(g_worktree_files_error)) {
        icon("warning", GetTextLineHeight())
        same_line()
        text_colored(FILES_ERROR, (color = float4(1.0f, 0.35f, 0.30f, 1.0f),
            text = g_worktree_files_error))
    }
    separator()
    child(FILES_TREE_CHILD, (text = "worktree files", size = float2(0.0f, 0.0f),
            child_flags = ImGuiChildFlags.None,
            window_flags = ImGuiWindowFlags.None)) {
        var row = 0
        var index = 0
        var stack_paths : array<string>
        let count = length(g_worktree_files)
        while (index < count) {
            let path & = unsafe(g_worktree_files[index])
            var inscope segments <- split(path, "/")
            var common = 0
            while (common < length(stack_paths)
                    && common + 1 < length(segments)) {
                let prefix = stack_paths[common]
                let name_start = common == 0 ? 0 : length(stack_paths[common - 1]) + 1
                if (slice(prefix, name_start, length(prefix)) != segments[common]) break
                common++
            }
            stack_paths |> resize(common)
            var skip_prefix = ""
            var depth = common
            while (depth + 1 < length(segments)) {
                let parent = depth == 0 ? "" : stack_paths[depth - 1]
                let dir_path = (empty(parent)
                    ? segments[depth] : "{parent}/{segments[depth]}")
                let expanded = (key_exists(g_files_expanded, dir_path)
                    && g_files_expanded[dir_path])
                if (draw_files_row(row, depth, segments[depth],
                        dir_path, true, expanded)) {
                    g_files_expanded[dir_path] = !expanded
                }
                if (!expanded && !(key_exists(g_files_expanded, dir_path)
                        && g_files_expanded[dir_path])) {
                    skip_prefix = dir_path + "/"
                    break
                }
                stack_paths |> push(dir_path)
                depth++
            }
            if (!empty(skip_prefix)) {
                index++
                while (index < count
                        && starts_with(g_worktree_files[index], skip_prefix)) {
                    index++
                }
                continue
            }
            if (draw_files_row(row, length(stack_paths),
                    segments[length(segments) - 1], path, false, false)) {
                open_worktree_file(path)
            }
            index++
        }
        if (count == 0 && !g_worktree_files_loading) {
            text_disabled("Select a worktree to browse its files")
        }
    }
}

def draw_worktree_files_dock() {
    dock_window(WORKTREE_FILES_WIN.PUBLIC, (text = "Worktree Files", closable = true,
            flags = ImGuiWindowFlags.None)) {
        draw_worktree_files_window()
    }
}
