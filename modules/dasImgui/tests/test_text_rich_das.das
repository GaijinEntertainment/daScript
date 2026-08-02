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

//! Rich-das end-to-end in examples/text: comment groups render rich in the
//! Preview surface, the caret's group unfolds to source, edits refresh only
//! the touched fragment.

let FEATURE = "examples/text/main.das"
let DEADLOCK_GUARD_SEC = 30.0f
let RICH_DAS_FIXTURE = "// # Doc header\n// Intro paragraph.\n\nlet q = 1 // trailing note\n\ndef foo() \{\n\tpass\n\}\n\n// Tail note\n"

def private viewer_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_state", null)
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
