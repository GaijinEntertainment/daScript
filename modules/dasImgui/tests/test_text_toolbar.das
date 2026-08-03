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

//! REAL-CLICK coverage for the live-preview creation flows — the class the
//! rail-driven tests missed (Boris, play round 2): clicks on blank separator
//! lines, off-glyph clicks, and TOOLBAR buttons pressed while the island
//! editor draws (the FocusItem reclaim used to cancel their press).

let FEATURE = "examples/text/main.das"
let DEADLOCK_GUARD_SEC = 30.0f
let TOOLBAR_FIXTURE = "# Title line\n\nFirst paragraph body text.\n\nSecond paragraph body text.\n"

def private viewer_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_state", null)
}

def private edit_text_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_edit_text", null)
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

def private open_toolbar_fixture(t : T?; app : ImguiApp;
                                 temp_dir : string) : bool {
    let fixture = path_join(temp_dir, "toolbar.md")
    t |> success(fwrite(fixture, TOOLBAR_FIXTURE), "fixture written")
    let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
    t |> success(opened?["ok"] ?? false, "fixture opens")
    let entered = post_command(app, "text_viewer_edit", JV((on = true)))
    t |> success(entered?["ok"] ?? false, "edit mode enters")
    let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        var state = viewer_state(app)
        return ((state?["editing"] ?? false)
            && (state?["edit_surface"] ?? "") == "preview"
            && (state?["hybrid_segment_count"] ?? 0) > 0)
    }
    t |> success(ready, "live preview up")
    return ready
}

[test]
def test_toolbar_image_between_paragraphs(t : T?) {
    // Boris's exact flow: click a paragraph, click the blank line after it,
    // click Img on the toolbar - all REAL clicks
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_toolbar_img", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_toolbar_fixture(t, app, temp_dir))
        // Boris's sequence verbatim: with the caret still in the TITLE
        // island, click the SPACING GAP between the two rich paragraphs
        var bands = post_command(app, "text_viewer_block_bands", null)
        var gap_y = -1.0f
        var previous_bottom = -1.0f
        for (index in range(bands?["count"] ?? 0)) {
            let band = bands?["bands"]?[index]
            let y0 = band?["y0"] ?? -1.0f
            continue if (y0 < 0.0f)
            if (previous_bottom >= 0.0f && y0 - previous_bottom > 4.0f) {
                gap_y = (previous_bottom + y0) * 0.5f
                break
            }
            previous_bottom = band?["y1"] ?? -1.0f
        }
        t |> success(gap_y >= 0.0f, "a spacing gap exists between rich blocks")
        real_click_at(app, 400.0f, gap_y)
        let on_blank = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return ((buffer?["cursor_line"] ?? -1) == 3
                && (buffer?["cursor_column"] ?? -1) == 0)
        }
        t |> success(on_blank, "a gap click lands the caret on the separator line")
        // the toolbar Img button - a REAL alias click while the island draws
        click(app, "DOCK_ROOT/g_document_windows[1]/TOOLBAR_ITEM[6]")
        let inserted = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return (find(buffer?["text"] ?? "", "![alt](src)") >= 0
                && (buffer?["snippet_active"] ?? false))
        }
        t |> success(inserted,
            "the toolbar inserts the image AT the blank line, session armed")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

let FENCE_FIXTURE = "# Title\n\nLead paragraph.\n\n```text\ncode line\n```\n\nTail paragraph.\n"

[test]
def test_toolbar_image_below_fence(t : T?) {
    // regression pin (Boris, play round 2): a gap click BELOW a code fence
    // parked the caret ON the closing ``` line (node source ranges exclude
    // it), so Img/List declined in-fence with only a silent footer status
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_toolbar_fence", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        let fixture = path_join(temp_dir, "fence.md")
        t |> success(fwrite(fixture, FENCE_FIXTURE), "fixture written")
        let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
        t |> success(opened?["ok"] ?? false, "fixture opens")
        let entered = post_command(app, "text_viewer_edit", JV((on = true)))
        t |> success(entered?["ok"] ?? false, "edit mode enters")
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["editing"] ?? false)
                && (state?["hybrid_segment_count"] ?? 0) > 0)
        }
        t |> success(ready, "live preview up")
        // the LAST inter-band gap: between the fence block and the tail
        // paragraph
        var bands = post_command(app, "text_viewer_block_bands", null)
        var gap_y = -1.0f
        var previous_bottom = -1.0f
        for (index in range(bands?["count"] ?? 0)) {
            let band = bands?["bands"]?[index]
            let y0 = band?["y0"] ?? -1.0f
            continue if (y0 < 0.0f)
            if (previous_bottom >= 0.0f && y0 - previous_bottom > 4.0f) {
                gap_y = (previous_bottom + y0) * 0.5f
            }
            previous_bottom = band?["y1"] ?? -1.0f
        }
        t |> success(gap_y >= 0.0f, "a gap exists below the fence block")
        real_click_at(app, 400.0f, gap_y)
        let after_fence = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return ((buffer?["cursor_line"] ?? -1) == 7
                && (buffer?["cursor_column"] ?? -1) == 0)
        }
        t |> success(after_fence,
            "the caret lands on the blank AFTER the closing fence, not on it")
        click(app, "DOCK_ROOT/g_document_windows[1]/TOOLBAR_ITEM[6]")
        let inserted = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return (find(buffer?["text"] ?? "", "![alt](src)") >= 0
                && (buffer?["snippet_active"] ?? false))
        }
        t |> success(inserted, "Img inserts below the fence, session armed")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_toolbar_buttons_fire_over_reclaim(t : T?) {
    // the FocusItem reclaim regression pin: toolbar buttons are submitted
    // BEFORE the island editor and must still complete their press
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_toolbar_press", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_toolbar_fixture(t, app, temp_dir))
        // +Section from the default caret (heading island)
        click(app, "DOCK_ROOT/g_document_windows[1]/TOOLBAR_ITEM[0]")
        let sectioned = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "## Section") >= 0
        }
        t |> success(sectioned, "+Section fires while the island editor draws")
        // heading cycle on the section line the snippet left us on
        click(app, "DOCK_ROOT/g_document_windows[1]/TOOLBAR_ITEM[1]")
        let cycled = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "### Section") >= 0
        }
        t |> success(cycled, "a second toolbar press also completes")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_click_off_glyph_and_dead_strip(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_toolbar_click", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_toolbar_fixture(t, app, temp_dir))
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 4, column = 0)))
        t |> success(moved?["ok"] ?? false, "caret parks in the second paragraph")
        // off-glyph: mid-window click on the RICH title line, far right of
        // its text, must land the caret at that line's end
        var segs = post_command(app, "text_viewer_hybrid_segments", null)
        let head = segs?["segments"]?[0]
        t |> equal(head?["kind"] ?? "", "rich", "title renders rich")
        real_click_at(app, 700.0f, (head?["y0"] ?? 0.0f) + 12.0f)
        let landed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return ((buffer?["cursor_line"] ?? -1) == 0
                && (buffer?["cursor_column"] ?? -1) == 12)
        }
        t |> success(landed, "an off-glyph rich click snaps to the line end")
        // the island re-segments to the clicked block one frame later - gate
        // on it BEFORE probing geometry (the bbox would be the old island's)
        let refolded = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["hybrid_island_first_line"] ?? -1) == 0
                && (state?["hybrid_island_last_line"] ?? -1) == 1)
        }
        t |> success(refolded, "the island re-segments to the clicked block")
        // dead strip: click BELOW the island's last row, inside its child
        // (anchor on the editor ITEM bbox - the child extends past it)
        var probe = wait_for_render(app,
            "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/editor", 15.0f)
        t |> success(probe != null, "the island editor renders")
        // wait_for_render returns the SNAPSHOT; extract the widget entry
        var entry = find_widget(probe,
            "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/editor")
        let strip_y = (entry?["bbox"]?["w"] ?? 0.0f) + 6.0f
        // atomic travel+click: a plain pos+press pair can interleave with the
        // previous click's synth timeline
        post_command(app, "imgui_mouse_click_at",
            JV((x = 300.0f, y = strip_y)))
        let clamped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (edit_text_state(app)?["cursor_line"] ?? -1) == 1
        }
        t |> success(clamped, "a dead-strip click clamps to the nearest line")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

let TASKS_FIXTURE = "# Tasks\n\n- [ ] first task item\n- [x] second task item\n\nClosing paragraph body text.\n"

def private real_drag(app : ImguiApp; x0, y0, x1, y1 : float) {
    post_command(app, "imgui_mouse_pos", JV((x = x0, y = y0)))
    post_command(app, "imgui_mouse_click", JV((button = 0, action = "press")))
    post_command(app, "imgui_mouse_pos",
        JV((x = (x0 + x1) * 0.5f, y = (y0 + y1) * 0.5f)))
    post_command(app, "imgui_mouse_pos", JV((x = x1, y = y1)))
    post_command(app, "imgui_mouse_click", JV((button = 0, action = "release")))
}

def private open_viewing(t : T?; app : ImguiApp; path, text : string) : bool {
    t |> success(fwrite(path, text), "fixture written")
    let opened = post_command(app, "text_viewer_open", JV((path = path)))
    t |> success(opened?["ok"] ?? false, "fixture opens")
    post_command(app, "text_viewer_edit", JV((on = false)))
    let viewing = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        return !(viewer_state(app)?["editing"] ?? true)
    }
    t |> success(viewing, "starts viewing")
    return viewing
}

[test]
def test_view_click_enters_editing(t : T?) {
    // Boris: "i need to press edit all the time. should be like never.
    // if i click somewhere - its for edit."
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_click_edit", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_viewing(t, app,
            path_join(temp_dir, "clicked.md"), TOOLBAR_FIXTURE))
        // a real click on the first paragraph enters edit mode AT that line
        var bands = post_command(app, "text_viewer_block_bands", null)
        let band = bands?["bands"]?[1]
        let target_y = ((band?["y0"] ?? 0.0f) + (band?["y1"] ?? 0.0f)) * 0.5f
        real_click_at(app, 120.0f, target_y)
        let entered = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return ((viewer_state(app)?["editing"] ?? false)
                && (edit_text_state(app)?["cursor_line"] ?? -1) == 2)
        }
        t |> success(entered, "a view click enters edit mode on the clicked line")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_view_drag_and_checkbox_stay_viewing(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_click_view", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_viewing(t, app,
            path_join(temp_dir, "tasks.md"), TASKS_FIXTURE))
        // drag-select across the closing paragraph: selection, NOT editing
        var bands = post_command(app, "text_viewer_block_bands", null)
        let band = bands?["bands"]?[2]
        let row_y = ((band?["y0"] ?? 0.0f) + (band?["y1"] ?? 0.0f)) * 0.5f
        real_drag(app, 60.0f, row_y, 220.0f, row_y)
        let selected = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["markdown_selection_end"] ?? -1)
                > (state?["markdown_selection_start"] ?? -1))
        }
        t |> success(selected, "a view drag selects text")
        t |> success(!(viewer_state(app)?["editing"] ?? true),
            "a drag never enters edit mode")
        // a real checkbox click toggles the marker, NOT editing
        var box = post_command(app, "text_viewer_checkbox", JV((index = 0)))
        t |> success(box?["ok"] ?? false, "checkbox rect is live")
        real_click_at(app,
            ((box?["x0"] ?? 0.0f) + (box?["x1"] ?? 0.0f)) * 0.5f,
            ((box?["y0"] ?? 0.0f) + (box?["y1"] ?? 0.0f)) * 0.5f)
        let toggled = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = post_command(app, "text_viewer_checkbox", JV((index = 0)))
            return state?["checked"] ?? false
        }
        t |> success(toggled, "the checkbox click toggles the marker")
        t |> success(!(viewer_state(app)?["editing"] ?? true),
            "a checkbox click never enters edit mode")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

let CRLF_FIXTURE = "# Title line\r\n\r\nFirst paragraph body text.\r\n\r\nSecond paragraph body text.\r\n"

[test]
def test_click_to_edit_crlf_caret(t : T?) {
    // the caret carry maps view bytes by line/column, so a CRLF checkout
    // still lands the caret on the clicked line (byte offsets would drift)
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_click_crlf", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_viewing(t, app,
            path_join(temp_dir, "crlf.md"), CRLF_FIXTURE))
        var bands = post_command(app, "text_viewer_block_bands", null)
        let band = bands?["bands"]?[1]
        let target_y = ((band?["y0"] ?? 0.0f) + (band?["y1"] ?? 0.0f)) * 0.5f
        real_click_at(app, 120.0f, target_y)
        let entered = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return ((viewer_state(app)?["editing"] ?? false)
                && (edit_text_state(app)?["cursor_line"] ?? -1) == 2)
        }
        t |> success(entered, "a CRLF view click still enters on the clicked line")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_live_reload_keeps_documents(t : T?) {
    // the [before_reload]/[after_reload] document persistence: a reload keeps
    // the open doc, its unsaved buffer, the caret, and edit mode
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_reload_docs", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_toolbar_fixture(t, app, temp_dir))
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 2, column = 3)))
        t |> success(moved?["ok"] ?? false, "caret parks in the paragraph")
        let typed = post_command(app, "imgui_key_type", JV((text = "zz")))
        t |> success(typed != null, "typed unsaved content")
        let dirty = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "zz") >= 0
        }
        t |> success(dirty, "the buffer carries the edit")
        reload(app)
        let restored = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return ((viewer_state(app)?["editing"] ?? false)
                && find(buffer?["text"] ?? "", "zz") >= 0
                && (buffer?["cursor_line"] ?? -1) == 2)
        }
        t |> success(restored,
            "reload keeps the doc, dirty buffer, caret, and edit mode")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_escape_exits_editing(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_toolbar_escape", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_toolbar_fixture(t, app, temp_dir))
        post_command(app, "imgui_key_press", JV((key = int(ImGuiKey.Escape))))
        post_command(app, "imgui_key_release", JV((key = int(ImGuiKey.Escape))))
        let viewing = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return !(viewer_state(app)?["editing"] ?? true)
        }
        t |> success(viewing, "Escape with nothing to cancel leaves edit mode")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}
