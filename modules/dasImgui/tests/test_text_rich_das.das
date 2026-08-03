options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui public
require imgui/imgui_playwright public
require daslib/fio public
require daslib/json public
require daslib/json_boost public
require math
require strings

//! Rich-das end-to-end in examples/text: comment groups render rich in the
//! Preview surface, the caret's group unfolds to source, edits refresh only
//! the touched fragment.

let FEATURE = "examples/text/main.das"
let DEADLOCK_GUARD_SEC = 30.0f
let RICH_DAS_FIXTURE = "// # Doc header\n// Intro paragraph.\n\nlet q = 1 // trailing note\n\ndef foo() \{\n\tpass\n\}\n\n// Tail note\n"

def private viewer_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_state", null)
}

def private real_click_at(app : ImguiApp; x, y : float) {
    post_command(app, "imgui_mouse_pos", JV((x = x, y = y)))
    post_command(app, "imgui_mouse_click", JV((button = 0, action = "press")))
    post_command(app, "imgui_mouse_click", JV((button = 0, action = "release")))
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

def private open_rich_das(t : T?; app : ImguiApp; temp_dir : string) : bool {
    let fixture = path_join(temp_dir, "rich.das")
    t |> success(fwrite(fixture, RICH_DAS_FIXTURE), "das fixture written")
    let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
    t |> success(opened?["ok"] ?? false, "fixture opens")
    let entered = post_command(app, "text_viewer_edit", JV((on = true)))
    t |> success(entered?["ok"] ?? false, "edit mode enters")
    let surfaced = post_command(app, "text_viewer_command",
        JV((id = "view.surface_preview")))
    t |> success(surfaced?["ok"] ?? false, "preview surface selects for das")
    let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        var state = viewer_state(app)
        return ((state?["editing"] ?? false)
            && (state?["edit_surface"] ?? "") == "preview"
            && (state?["rich_das_group_count"] ?? 0) == 2)
    }
    t |> success(ready, "both comment groups register in the preview")
    return ready
}

def private open_long_rich(t : T?; app : ImguiApp; temp_dir : string) : bool {
    // three comment groups around two 60-line code runs - long enough that
    // segments scroll in and out of the viewport
    let fixture = path_join(temp_dir, "long_rich.das")
    let text = ("// # Header\n// Intro.\n\n" + repeat("let x = 1\n", 60)
        + "\n// Mid note\n\n" + repeat("let y = 2\n", 60) + "// Tail note\n")
    t |> success(fwrite(fixture, text), "long das fixture written")
    let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
    t |> success(opened?["ok"] ?? false, "fixture opens")
    let entered = post_command(app, "text_viewer_edit", JV((on = true)))
    t |> success(entered?["ok"] ?? false, "edit mode enters")
    let surfaced = post_command(app, "text_viewer_command",
        JV((id = "view.surface_preview")))
    t |> success(surfaced?["ok"] ?? false, "preview surface selects")
    let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        var state = viewer_state(app)
        return ((state?["editing"] ?? false)
            && (state?["rich_das_group_count"] ?? 0) == 3)
    }
    t |> success(ready, "three comment groups register")
    return ready
}

def private chord(app : ImguiApp; key : int) {
    post_command(app, "imgui_key_press", JV((key = int(ImGuiKey.LeftCtrl))))
    post_command(app, "imgui_key_press", JV((key = key)))
    post_command(app, "imgui_key_release", JV((key = key)))
    post_command(app, "imgui_key_release", JV((key = int(ImGuiKey.LeftCtrl))))
}

def private segment_of_line(app : ImguiApp; line : int) : JsonValue const? {
    var segs = post_command(app, "text_viewer_hybrid_segments", null)
    for (index in range(segs?["count"] ?? 0)) {
        let seg = segs?["segments"]?[index]
        if ((seg?["line_first"] ?? -1) <= line
                && (seg?["line_last"] ?? -1) >= line) {
            return seg
        }
    }
    return null
}

[test]
def test_rich_das_cross_segment_click_no_jump(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    // play round 3: a click on ANOTHER source segment used to be hijacked by
    // the primary (caret clamped to its first line) and the follow scrolled
    // "god knows where" off a stale anchor
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_rich_click", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_long_rich(t, app, temp_dir))
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 100, column = 0)))
        t |> success(moved?["ok"] ?? false, "caret parks deep in code run two")
        // scroll until line 20 (code run ONE - not the primary) is on screen
        post_command(app, "imgui_mouse_pos", JV((x = 400.0f, y = 300.0f)))
        var row_y = -1.0f
        var line_height = 18.0f
        for (_step in range(12)) {
            let seg = segment_of_line(app, 20)
            continue if (seg == null)
            let rows = float((seg?["line_last"] ?? 1) - (seg?["line_first"] ?? 0) + 1)
            line_height = ((seg?["y1"] ?? 0.0f) - (seg?["y0"] ?? 0.0f)) / max(rows, 1.0f)
            let y = ((seg?["y0"] ?? 0.0f)
                + float(20 - (seg?["line_first"] ?? 0)) * line_height)
            if (y > 80.0f && y < 640.0f) {
                row_y = y
                break
            }
            post_command(app, "imgui_mouse_scroll",
                JV((x = 0.0f, y = y < 80.0f ? 8.0f : -8.0f)))
        }
        t |> success(row_y >= 0.0f, "line 20's row scrolls into view")
        let seg_before = segment_of_line(app, 20)
        real_click_at(app, 300.0f, row_y + line_height * 0.5f)
        let landed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (viewer_state(app)?["edit_cursor_line"] ?? -1) == 20
        }
        t |> success(landed, "the click lands the caret on the clicked line")
        let seg_after = segment_of_line(app, 20)
        let drift = abs((seg_after?["y0"] ?? -999.0f) - (seg_before?["y0"] ?? 0.0f))
        t |> success(drift < 2.0f, "a click on a visible row never scrolls")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_rich_das_keys_after_ctrl_end(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    // play round 3: after Ctrl+End the caret's segment-child owns neither nav
    // focus nor (once scrolled out) a drawn body - keys must survive both
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_rich_keys", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_long_rich(t, app, temp_dir))
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 3, column = 0)))
        t |> success(moved?["ok"] ?? false, "caret parks in code run one")
        chord(app, int(ImGuiKey.End))
        let at_end = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["edit_cursor_line"] ?? -1)
                == (state?["edit_line_count"] ?? 0) - 1)
        }
        t |> success(at_end, "Ctrl+End jumps to the buffer end")
        chord(app, int(ImGuiKey.Home))
        let at_start = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (viewer_state(app)?["edit_cursor_line"] ?? -1) == 0
        }
        t |> success(at_start, "Ctrl+Home works right after - no click needed")
        post_command(app, "imgui_key_press", JV((key = int(ImGuiKey.DownArrow))))
        post_command(app, "imgui_key_release", JV((key = int(ImGuiKey.DownArrow))))
        let stepped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (viewer_state(app)?["edit_cursor_line"] ?? -1) > 0
        }
        t |> success(stepped, "plain arrows stay alive too")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_rich_das_groups_and_unfold(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_rich_das_groups", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_rich_das(t, app, temp_dir))
        // caret starts at line 0 inside the HEADER group: it renders as source
        var state = viewer_state(app)
        t |> equal(state?["hybrid_island_first_line"] ?? -1, 0,
            "the caret's group is demoted to source")
        t |> success((state?["hybrid_segment_count"] ?? 0) >= 3,
            "source runs + rich groups partition the buffer")
        // caret into code: BOTH groups render rich
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 5, column = 0)))
        t |> success(moved?["ok"] ?? false, "caret into the code run")
        let refolded = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return ((now?["hybrid_island_first_line"] ?? -1) == 2
                && (now?["rich_das_group_count"] ?? 0) == 2)
        }
        t |> success(refolded, "leaving the group snaps it back rich")
        // caret into the tail group: it unfolds alone
        let tail = post_command(app, "text_viewer_set_cursor",
            JV((line = 9, column = 3)))
        t |> success(tail?["ok"] ?? false, "caret into the tail note")
        let unfolded = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return ((now?["hybrid_island_first_line"] ?? -1) == 9
                && (now?["hybrid_island_last_line"] ?? -1) == 9)
        }
        t |> success(unfolded, "the tail group unfolds alone at the caret")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_rich_das_edit_refreshes_one_fragment(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_rich_das_edit", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_rich_das(t, app, temp_dir))
        var probe = wait_for_render(app,
            "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/editor", 15.0f)
        t |> success(probe != null, "the caret island renders")
        click(app, "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/editor")
        t |> success(wait_for_mouse_idle(app), "island focus click drains")
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 0, column = 5)))
        t |> success(moved?["ok"] ?? false, "caret inside the header group")
        var before = viewer_state(app)
        let parses_before = before?["rich_das_parse_count"] ?? -1
        let typed = post_command(app, "imgui_key_type", JV((text = "X")))
        t |> success(typed != null, "typed into the unfolded group")
        let refreshed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var now = viewer_state(app)
            return ((now?["rich_das_synced_revision"] ?? -1)
                    == (now?["edit_revision"] ?? -2)
                && (now?["rich_das_parse_count"] ?? -1) == parses_before + 1)
        }
        t |> success(refreshed,
            "the debounced resync reparses exactly ONE fragment")
        t |> equal(viewer_state(app)?["rich_das_group_count"] ?? -1, 2,
            "mixed/trailing-comment code lines never join the groups")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}
