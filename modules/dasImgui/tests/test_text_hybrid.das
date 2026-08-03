options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui public
require imgui/imgui_playwright public
require daslib/fio public
require strings
require daslib/json public
require daslib/json_boost public

//! Live-preview (hybrid) editing scenarios for examples/text: unfold follows
//! the caret, rich clicks place it, typing keeps the debounce, a dirty caret
//! exit reparses immediately, and the editor state survives surface switches.

let FEATURE = "examples/text/main.das"
let DEADLOCK_GUARD_SEC = 30.0f
let HYBRID_EDITOR = "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/editor"
let HYBRID_FIXTURE = "# Hybrid probe\n\nFirst paragraph line.\n\nSecond paragraph line.\n\n- alpha\n- beta\n\nTail paragraph.\n"

def private viewer_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_state", null)
}

def private edit_text_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_edit_text", null)
}

def private hybrid_segments(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_hybrid_segments", null)
}

def private set_cursor(app : ImguiApp; line : int; column : int = 0) : JsonValue? {
    return post_command(app, "text_viewer_set_cursor",
        JV((line = line, column = column)))
}

def private open_hybrid_fixture(t : T?; app : ImguiApp; temp_dir : string) : bool {
    let fixture = path_join(temp_dir, "hybrid.md")
    t |> success(fwrite(fixture, HYBRID_FIXTURE), "hybrid fixture written")
    let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
    t |> success(opened?["ok"] ?? false, "fixture opens")
    let entered = post_command(app, "text_viewer_edit", JV((on = true)))
    t |> success(entered?["ok"] ?? false, "edit mode enters")
    let previewing = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        var state = viewer_state(app)
        return ((state?["editing"] ?? false)
            && (state?["edit_surface"] ?? "") == "preview"
            && (state?["hybrid_segment_count"] ?? 0) > 0)
    }
    t |> success(previewing, "live preview is the markdown editing default")
    return previewing
}

def private viewer_ready(t : T?; app : ImguiApp) : bool {
    let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        var state = viewer_state(app)
        return ((state?["ok"] ?? false)
            && (state?["render_revision"] ?? 0) > 0
            && (state?["window_docked"] ?? false))
    }
    t |> success(ready, "viewer starts docked and rendered")
    return ready
}

[test]
def test_hybrid_surface_three_way(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_text_hybrid_surface", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_hybrid_fixture(t, app, temp_dir))
        // footer radio drives the surface; per-site aliases target each option
        click(app, "DOCK_ROOT/g_document_windows[1]/SURFACE_MODE#0")
        let sourced = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (viewer_state(app)?["edit_surface"] ?? "") == "source"
        }
        t |> success(sourced, "footer radio selects the Source surface")
        var pane = wait_for_render(app,
            "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/EDITOR_PANE/editor",
            15.0f)
        t |> success(pane != null, "source surface shows the classic editor pane")
        let split = post_command(app, "text_viewer_command",
            JV((id = "view.surface_split")))
        t |> success(split?["ok"] ?? false, "split surface command queues")
        let splitting = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["edit_surface"] ?? "") == "split"
                && (state?["preview_stamp"] ?? 0) > 0)
        }
        t |> success(splitting, "split surface parses the side-by-side preview")
        click(app, "DOCK_ROOT/g_document_windows[1]/SURFACE_MODE#2")
        let previewed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["edit_surface"] ?? "") == "preview"
                && (state?["hybrid_segment_count"] ?? 0) > 0)
        }
        t |> success(previewed, "footer radio returns to the live preview")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_hybrid_unfold_follows_caret(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_text_hybrid_caret", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_hybrid_fixture(t, app, temp_dir))
        var state = viewer_state(app)
        let first_unfold = state?["hybrid_unfold_node"] ?? -1
        t |> equal(state?["hybrid_island_first_line"] ?? -1, 0,
            "the opening caret unfolds the heading block")
        let moved = set_cursor(app, 4)
        t |> success(moved?["ok"] ?? false, "caret command lands")
        let followed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return ((now?["hybrid_island_first_line"] ?? -1) == 4
                && (now?["hybrid_unfold_node"] ?? -1) != first_unfold)
        }
        t |> success(followed, "the island follows the caret to the new block")
        var segs = hybrid_segments(app)
        t |> success((segs?["count"] ?? 0) >= 3, "rich runs flank the island")
        let head_run = segs?["segments"]?[0]
        t |> equal(head_run?["kind"] ?? "", "rich",
            "the exited heading block renders rich again")
        t |> equal(head_run?["line_first"] ?? -1, 0, "leading rich run at line 0")
        t |> equal(head_run?["line_last"] ?? -1, 3,
            "leading rich run ends before the island")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_hybrid_click_rich_block_moves_caret(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_text_hybrid_click", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_hybrid_fixture(t, app, temp_dir))
        var state = viewer_state(app)
        let clicks_before = state?["hybrid_rich_click_revision"] ?? 0
        let island_before = state?["hybrid_island_first_line"] ?? -1
        var segs = hybrid_segments(app)
        // the trailing rich run starts with "First paragraph line." at its top
        var rich_y = -1.0f
        let count = segs?["count"] ?? 0
        for (index in range(count)) {
            let seg = segs?["segments"]?[index]
            if ((seg?["kind"] ?? "") == "rich" && (seg?["line_first"] ?? -1) >= 2) {
                rich_y = (seg?["y0"] ?? -1.0f)
                break
            }
        }
        t |> success(rich_y >= 0.0f, "a rich segment sits below the island")
        post_command(app, "imgui_mouse_pos", JV((x = 60.0f, y = rich_y + 10.0f)))
        post_command(app, "imgui_mouse_click", JV((button = 0, action = "press")))
        post_command(app, "imgui_mouse_click", JV((button = 0, action = "release")))
        let routed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return (((now?["hybrid_rich_click_revision"] ?? 0) > clicks_before)
                && (now?["edit_cursor_line"] ?? -1) >= 2
                && (now?["hybrid_island_first_line"] ?? -1) != island_before)
        }
        t |> success(routed, "a rich click moves the caret and unfolds its block")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_hybrid_typing_debounce_and_exit_snap(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_text_hybrid_type", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_hybrid_fixture(t, app, temp_dir))
        var probe = wait_for_render(app, HYBRID_EDITOR, 15.0f)
        t |> success(probe != null, "the source island renders in the hybrid")
        click(app, HYBRID_EDITOR)
        t |> success(wait_for_mouse_idle(app), "island focus click drains")
        let moved = set_cursor(app, 2)
        t |> success(moved?["ok"] ?? false, "caret lands in the paragraph island")
        var state = viewer_state(app)
        let revision_before = state?["edit_revision"] ?? -1
        let stamp_before = state?["preview_stamp"] ?? -1
        let typed = post_command(app, "imgui_key_type", JV((text = "x")))
        t |> success(typed != null, "island accepts synthesized text")
        let pending = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return (((now?["edit_revision"] ?? -1) > revision_before)
                && (now?["preview_stamp"] ?? -1) == stamp_before)
        }
        t |> success(pending, "typing edits the buffer BEFORE any reparse (debounce)")
        var mid = viewer_state(app)
        let snaps_before = mid?["hybrid_immediate_reparse_count"] ?? -1
        let exited = set_cursor(app, 9)
        t |> success(exited?["ok"] ?? false, "caret exits to the tail block")
        let snapped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return (((now?["hybrid_immediate_reparse_count"] ?? -1) > snaps_before)
                && (now?["preview_revision"] ?? -1) == (now?["edit_revision"] ?? -2)
                && (now?["hybrid_island_first_line"] ?? -1) == 9)
        }
        t |> success(snapped,
            "a dirty caret exit reparses IMMEDIATELY - the debounce is bypassed")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_hybrid_editor_state_across_surfaces(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_text_hybrid_state", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_hybrid_fixture(t, app, temp_dir))
        var probe = wait_for_render(app, HYBRID_EDITOR, 15.0f)
        t |> success(probe != null, "the source island renders")
        click(app, HYBRID_EDITOR)
        t |> success(wait_for_mouse_idle(app), "island focus click drains")
        let moved = set_cursor(app, 2)
        t |> success(moved?["ok"] ?? false, "caret in the paragraph")
        let typed = post_command(app, "imgui_key_type", JV((text = "Q")))
        t |> success(typed != null, "typed into the live preview")
        let dirtied = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return (find(buffer?["text"] ?? "", "QFirst") >= 0
                && (buffer?["undo_depth"] ?? 0) > 0)
        }
        t |> success(dirtied, "the ONE buffer holds the edit")
        let to_source = post_command(app, "text_viewer_command",
            JV((id = "view.surface_source")))
        t |> success(to_source?["ok"] ?? false, "switch to the Source surface")
        let undone = post_command(app, "text_viewer_command", JV((id = "edit.undo")))
        t |> success(undone?["ok"] ?? false, "undo command queues on Source")
        let reverted = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return find(buffer?["text"] ?? "", "QFirst") < 0
        }
        t |> success(reverted, "one journal: undo on Source reverts the preview edit")
        let back = post_command(app, "text_viewer_command",
            JV((id = "view.surface_preview")))
        t |> success(back?["ok"] ?? false, "switch back to the live preview")
        let saved = post_command(app, "text_viewer_save", null)
        t |> success(saved?["ok"] ?? false, "save works from the preview surface")
        let clean = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return !(viewer_state(app)?["dirty"] ?? true)
        }
        t |> success(clean, "one save point: dirty clears across surfaces")
        let found = post_command(app, "text_viewer_find", JV((query = "Tail")))
        t |> success(found?["ok"] ?? false, "find opens in the preview surface")
        let jumped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return ((now?["find_match_count"] ?? 0) > 0
                && (now?["edit_cursor_line"] ?? -1) == 9
                && (now?["hybrid_island_first_line"] ?? -1) == 9)
        }
        t |> success(jumped, "a find jump moves the caret and unfolds the match block")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}
