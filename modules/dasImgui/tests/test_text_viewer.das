options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/fio public
require strings
require daslib/json public
require daslib/json_boost public

//! Event-driven smoke coverage for the user-facing text viewer. Deadlines are
//! deadlock/crash guards only; every transition waits for semantic app state.

let FEATURE = "examples/text/main.das"
let DEADLOCK_GUARD_SEC = 30.0f

def private viewer_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_state", null)
}

def private source_range(app : ImguiApp; text : string) : JsonValue? {
    return post_command(app, "text_viewer_source_range", JV((text = text)))
}

[test]
def test_text_viewer_open_modes_and_zoom(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        var initial : JsonValue?
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            initial = viewer_state(app)
            return ((initial?["ok"] ?? false)
                && (initial?["render_revision"] ?? 0) > 0
                && (initial?["window_docked"] ?? false)
                && (initial?["window_dock_id"] ?? 0) != 0
                && (initial?["window_dock_id"] ?? 0)
                    == (initial?["dockspace_id"] ?? -1))
        }
        t |> success(ready,
                     "welcome Markdown fills the central dockspace as an ordinary window")
        if (!ready) return

        let old_revision = initial?["document_revision"] ?? 0
        let fixture = "{get_das_root()}/examples/text/README.md"
        let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
        t |> success(opened?["ok"] ?? false, "live open accepts a UTF-8 Markdown file")

        let file_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["path"] ?? "") == fixture
                && (state?["document_revision"] ?? 0) > old_revision
                && (state?["mode"] ?? "") == "markdown"
                && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(file_observed, "loaded Markdown becomes the rendered document")
        if (!file_observed) return

        let plain_mode = post_command(app, "text_viewer_set_mode", JV((mode = "plain")))
        t |> success(plain_mode?["ok"] ?? false, "mode command accepts plain text")
        let plain_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["mode"] ?? "") == "plain"
                && (state?["source_render_revision"] ?? 0) > 0
                && (state?["source_marker_count"] ?? 0) > 0
                && (state?["source_cell_count"] ?? 0) > 0
                && (state?["source_layout_hits"] ?? 0) > 0)
        }
        t |> success(plain_observed,
                     "bottom mode control switches to styled selectable source")
        if (!plain_observed) return

        let marker = source_range(app, "**")
        t |> success(marker?["ok"] ?? false,
                     "Markdown delimiter range is inspectable without a screenshot")
        t |> success(marker?["all_markers"] ?? false,
                     "both strong delimiter bytes use marker styling")
        t |> equal(marker?["matched_cells"] ?? 0, 2,
                   "two marker bytes map to two source cells")
        t |> equal(marker?["marker_cells"] ?? 0, 2,
                   "matched cells retain marker style")
        t |> equal(marker?["first_cell_start"] ?? -1,
                   marker?["source_start"] ?? -2,
                   "first marker cell starts at requested byte")
        t |> equal(marker?["last_cell_end"] ?? -1,
                   marker?["source_end"] ?? -2,
                   "last marker cell ends at requested byte")
        t |> equal(marker?["columns"] ?? 0, 2,
                   "two ASCII marker bytes retain two monospace cells")

        let emoji = source_range(app, "👩‍💻")
        t |> success(emoji?["ok"] ?? false, "color emoji source range is inspectable")
        t |> equal(emoji?["columns"] ?? 0, 2,
                   "ZWJ color emoji reserves exactly two source cells")

        let marker_start = marker?["source_start"] ?? -1
        let marker_end = marker?["source_end"] ?? -1
        let marker_x0 = marker?["x0"] ?? 0.0f
        let marker_x1 = marker?["x1"] ?? 0.0f
        let marker_y = ((marker?["y0"] ?? 0.0f) + (marker?["y1"] ?? 0.0f)) * 0.5f
        var before_selection = viewer_state(app)
        let commit_before = before_selection?["source_selection_commit_revision"] ?? 0
        post_command(app, "imgui_mouse_pos", JV((x = marker_x0 + 1.0f, y = marker_y)))
        post_command(app, "imgui_mouse_click", JV((button = 0, action = "press")))
        let press_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return viewer_state(app)?["source_selection_dragging"] ?? false
        }
        t |> success(press_observed, "source selection press is acknowledged")
        post_command(app, "imgui_mouse_pos", JV((x = marker_x1 - 1.0f, y = marker_y)))
        let marker_selected = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["source_selection_start"] ?? -1) == marker_start
                && (state?["source_selection_end"] ?? -1) == marker_end
                && (state?["source_selection_has_markers"] ?? false))
        }
        t |> success(marker_selected,
                     "source selection retains exact marker byte offsets and style")
        post_command(app, "imgui_mouse_click", JV((button = 0, action = "release")))
        let release_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return (!(state?["source_selection_dragging"] ?? true)
                && (state?["source_selection_commit_revision"] ?? 0) > commit_before)
        }
        t |> success(release_observed, "source selection release is acknowledged")

        let input_snapshot = post_command(app, "imgui_snapshot", null)
        let input_primary = (input_snapshot?["io"]?["config_macos_behaviors"] ?? false) ? "Super" : "Ctrl"
        let copy_before = viewer_state(app)?["source_selection_copy_revision"] ?? 0
        post_command(app, "imgui_key_chord", JV((mods = [input_primary], key = "C")))
        t |> success(wait_for_key_idle(app, 4.0f), "platform-primary copy chord completes")
        let copy_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["source_selection_copy_revision"] ?? 0) > copy_before
                && (state?["last_command"] ?? "") == "edit.copy")
        }
        t |> success(copy_observed,
                     "copy binding routes through a semantic command into the source view")

        post_command(app, "imgui_key_chord", JV((mods = [input_primary], key = "A")))
        t |> success(wait_for_key_idle(app, 4.0f),
                     "platform-primary select-all chord completes")
        let select_all_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["source_selection_start"] ?? -1) == 0
                && (state?["source_selection_end"] ?? -1) == (state?["byte_count"] ?? -2)
                && (state?["last_command"] ?? "") == "edit.select_all")
        }
        t |> success(select_all_observed,
                     "select-all binding invokes the component API without embedded keyboard logic")

        let zoomed = post_command(app, "text_viewer_set_zoom", JV((percent = 137)))
        t |> equal(zoomed?["zoom_percent"] ?? 0, 135,
                   "zoom command snaps to a five-percent step")
        let zoom_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (viewer_state(app)?["zoom_percent"] ?? 0) == 135
        }
        t |> success(zoom_observed, "snapped zoom reaches rendered app state")

        var before_reset = viewer_state(app)
        let command_before = before_reset?["command_revision"] ?? 0
        let zoom_snapshot = post_command(app, "imgui_snapshot", null)
        let primary_mod = (zoom_snapshot?["io"]?["config_macos_behaviors"] ?? false) ? "Super" : "Ctrl"
        post_command(app, "imgui_key_chord", JV((mods = [primary_mod], key = "0")))
        t |> success(wait_for_key_idle(app, 4.0f),
                     "platform-primary reset chord completes")
        let reset_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["zoom_percent"] ?? 0) == 100
                && (state?["last_command"] ?? "") == "view.zoom_reset"
                && (state?["command_revision"] ?? 0) > command_before)
        }
        t |> success(reset_observed,
                     "keyboard binding invokes the semantic zoom-reset command")
        let rezoomed = post_command(app, "text_viewer_set_zoom", JV((percent = 135)))
        t |> equal(rezoomed?["zoom_percent"] ?? 0, 135,
                   "zoom is restored before testing new-document inheritance")

        let markdown_mode = post_command(app, "text_viewer_set_mode", JV((mode = "markdown")))
        t |> success(markdown_mode?["ok"] ?? false, "mode command accepts Markdown")
        let markdown_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["mode"] ?? "") == "markdown"
                && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(markdown_observed, "bottom mode control returns to Markdown")

        let opened_window = post_command(app, "text_viewer_open_window",
                                         JV((path = fixture)))
        t |> success(opened_window?["ok"] ?? false,
                     "another document opens inside the running viewer")
        t |> equal(opened_window?["document_count"] ?? 0, 2,
                   "in-process open adds a second dockable document")
        t |> equal(opened_window?["zoom_percent"] ?? 0, 135,
                   "new document inherits the active document zoom")
        let second_document_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["document_count"] ?? 0) == 2
                && (state?["zoom_percent"] ?? 0) == 135)
        }
        t |> success(second_document_observed,
                     "the viewer retains both documents at inherited zoom")

        let bindings_queued = post_command(app, "text_viewer_command",
                                           JV((id = "preferences.edit_bindings")))
        t |> success(bindings_queued?["ok"] ?? false,
                     "live API invokes the same semantic command surface")
        let bindings_open = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["bindings_editor_open"] ?? false)
                && (state?["last_command"] ?? "") == "preferences.edit_bindings")
        }
        t |> success(bindings_open,
                     "edit-bindings command opens its UI window")

        close_widget(app, "DOCK_ROOT/g_document_windows[2]")
        let one_document_left = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return (viewer_state(app)?["document_count"] ?? -1) == 1
        }
        t |> success(one_document_left,
                     "new document uses the ordinary closable window lifecycle")

        close_widget(app, "DOCK_ROOT/g_document_windows[1]")
        let all_documents_closed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return (!(state?["ok"] ?? true)
                && (state?["document_count"] ?? -1) == 0)
        }
        t |> success(all_documents_closed,
                     "closing the initial document leaves an empty dockspace")
    }
}

[test]
def test_text_viewer_checkbox_save_reload_roundtrip(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    var temp_error : string
    let temp_dir = create_temp_directory("das_text_viewer_roundtrip", temp_error)
    t |> success(empty(temp_error) && !empty(temp_dir),
                 "roundtrip test receives a dedicated temporary directory")
    if (!empty(temp_error) || empty(temp_dir)) return
    let fixture = path_join(temp_dir, "checkbox.md")
    let bom = "\xEF\xBB\xBF"
    let original = bom + "# Tasks\r\n\r\n- [ ] first\r\n- [X] second\n"
    let expected = bom + "# Tasks\r\n\r\n- [x] first\r\n- [X] second\n"
    t |> success(fwrite(fixture, original), "mixed-ending BOM fixture is written")

    with_imgui_app(FEATURE) $(app) {
        let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
        t |> success(opened?["ok"] ?? false, "checkbox fixture opens")

        var initial : JsonValue?
        var checkbox : JsonValue?
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            initial = viewer_state(app)
            checkbox = post_command(app, "text_viewer_checkbox", JV((index = 0)))
            return ((initial?["ok"] ?? false)
                && (initial?["checkbox_count"] ?? 0) == 2
                && (initial?["has_utf8_bom"] ?? false)
                && !(initial?["dirty"] ?? true)
                && (checkbox?["ok"] ?? false))
        }
        t |> success(ready, "checkbox projection and screen geometry become observable")

        if (ready) {
            // Command responses are transient live-API values; snapshot the
            // geometry before issuing any later commands.
            let checkbox_width_before = ((checkbox?["x1"] ?? 0.0f)
                - (checkbox?["x0"] ?? 0.0f))
            let checkbox_height_before = ((checkbox?["y1"] ?? 0.0f)
                - (checkbox?["y0"] ?? 0.0f))
            let no_op_save = post_command(app, "text_viewer_save", JV((path = fixture)))
            t |> success(no_op_save?["ok"] ?? false, "no-op save succeeds")
            t |> equal(fread(fixture), original,
                       "no-op app save preserves BOM, spelling, and mixed endings byte-for-byte")

            let old_revision = initial?["document_revision"] ?? 0
            let old_activation = initial?["control_activation_revision"] ?? 0
            let x = ((checkbox?["x0"] ?? 0.0f) + (checkbox?["x1"] ?? 0.0f)) * 0.5f
            let y = ((checkbox?["y0"] ?? 0.0f) + (checkbox?["y1"] ?? 0.0f)) * 0.5f
            post_command(app, "imgui_mouse_pos", JV((x = x, y = y)))
            post_command(app, "imgui_mouse_click", JV((button = 0, action = "press")))
            post_command(app, "imgui_mouse_click", JV((button = 0, action = "release")))

            let toggled = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
                var state = viewer_state(app)
                var current = post_command(app, "text_viewer_checkbox", JV((index = 0)))
                return ((state?["document_revision"] ?? 0) == old_revision + 1
                    && (state?["control_activation_revision"] ?? 0) > old_activation
                    && (state?["dirty"] ?? false)
                    && (current?["ok"] ?? false)
                    && (current?["checked"] ?? false))
            }
            t |> success(toggled,
                         "click emits a control event, edits source, and reparses the projection")

            if (toggled) {
                let toggled_checkbox = post_command(
                    app, "text_viewer_checkbox", JV((index = 0)))
                let width_after = ((toggled_checkbox?["x1"] ?? 0.0f)
                    - (toggled_checkbox?["x0"] ?? 0.0f))
                let height_after = ((toggled_checkbox?["y1"] ?? 0.0f)
                    - (toggled_checkbox?["y0"] ?? 0.0f))
                t |> equal(width_after, checkbox_width_before,
                           "checked and unchecked controls reserve identical width")
                t |> equal(height_after, checkbox_height_before,
                           "checked and unchecked controls reserve identical height")
                let saved = post_command(app, "text_viewer_save", JV((path = fixture)))
                t |> success(saved?["ok"] ?? false, "edited source saves")
                t |> success(!(saved?["dirty"] ?? true), "successful save advances saved revision")
                t |> equal(fread(fixture), expected,
                           "checkbox save changes exactly its marker byte")

                let reloaded = post_command(app, "text_viewer_open", JV((path = fixture)))
                t |> success(reloaded?["ok"] ?? false, "saved document reloads")
                let reload_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
                    var state = viewer_state(app)
                    var current = post_command(app, "text_viewer_checkbox", JV((index = 0)))
                    return ((state?["has_utf8_bom"] ?? false)
                        && !(state?["dirty"] ?? true)
                        && (current?["ok"] ?? false)
                        && (current?["checked"] ?? false))
                }
                t |> success(reload_observed,
                             "saved checkbox state survives a fresh load/projection roundtrip")
            }
        }
    }
    rmdir_rec_result(temp_dir)
}

[test]
def test_text_viewer_find_both_modes(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["ok"] ?? false) && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(ready, "welcome document renders before find drives it")
        if (!ready) return

        let fixture = "{get_das_root()}/examples/text/syntax_highlighting.md"
        let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
        t |> success(opened?["ok"] ?? false, "syntax fixture opens for find coverage")
        let markdown_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["path"] ?? "") == fixture
                && (state?["mode"] ?? "") == "markdown"
                && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(markdown_ready, "fixture renders as Markdown")
        if (!markdown_ready) return

        // Case-insensitive literal find in RENDERED markdown selects the match.
        let found = post_command(app, "text_viewer_find", JV((query = "tree-sitter")))
        t |> success(found?["ok"] ?? false, "find command is accepted")
        t |> success((found?["match_count"] ?? 0) >= 2,
                     "case-insensitive literal matches Tree-sitter occurrences")
        let markdown_jumped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_open"] ?? false)
                && (state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["markdown_selection_end"] ?? -1)
                    > (state?["markdown_selection_start"] ?? -1))
        }
        t |> success(markdown_jumped, "find selects the current match in rendered Markdown")

        let selection_before = viewer_state(app)?["markdown_selection_start"] ?? -1
        let stepped = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        t |> equal(stepped?["current"] ?? -1, 1, "find next advances the current match")
        let markdown_moved = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["markdown_selection_start"] ?? -1) != selection_before
                && (state?["markdown_selection_start"] ?? -1) >= 0)
        }
        t |> success(markdown_moved, "find next moves the rendered selection")

        // Plain mode: the same find surface selects exact source bytes.
        let plain_mode = post_command(app, "text_viewer_set_mode", JV((mode = "plain")))
        t |> success(plain_mode?["ok"] ?? false, "plain mode accepts the switch")
        let plain_found = post_command(app, "text_viewer_find", JV((query = "require")))
        t |> success((plain_found?["match_count"] ?? 0) >= 1,
                     "literal find matches inside the plain source")
        let plain_jumped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            let start = state?["source_selection_start"] ?? -1
            let end = state?["source_selection_end"] ?? -1
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && start >= 0 && end - start == length("require"))
        }
        t |> success(plain_jumped, "find selects exactly the matched bytes in source view")

        // Regex option: pattern matches, invalid pattern reports without matches.
        let rx = post_command(app, "text_viewer_find",
                              JV((query = "requ[a-z]+e", regex = true)))
        t |> success((rx?["match_count"] ?? 0) >= 1, "regex find matches require")
        t |> equal(rx?["error"] ?? "?", "", "valid regex reports no error")
        let bad = post_command(app, "text_viewer_find", JV((query = "[", regex = true)))
        t |> equal(bad?["match_count"] ?? -1, 0, "invalid regex yields no matches")
        t |> success(!empty(bad?["error"] ?? ""), "invalid regex reports an error")

        let closed = post_command(app, "text_viewer_find_close", null)
        t |> success(closed?["ok"] ?? false, "find close command is accepted")
        let close_observed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return !(viewer_state(app)?["find_open"] ?? true)
        }
        t |> success(close_observed, "find bar reports closed")

        // .das support: opening a daslang file colors it via tree-sitter.
        let das_fixture = "{get_das_root()}/examples/text/main.das"
        let das_opened = post_command(app, "text_viewer_open", JV((path = das_fixture)))
        t |> success(das_opened?["ok"] ?? false, ".das file opens")
        let das_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["path"] ?? "") == das_fixture
                && (state?["mode"] ?? "") == "plain"
                && (state?["source_render_revision"] ?? 0) > 0
                && (state?["source_cell_count"] ?? 0) > 0)
        }
        t |> success(das_ready, ".das file renders as plain source")
        if (!das_ready) return
        let das_keyword = source_range(app, "options gen2")
        t |> success(das_keyword?["ok"] ?? false, ".das keyword range is inspectable")
        t |> success((das_keyword?["syntax_cells"] ?? 0) > 0,
                     ".das source carries tree-sitter syntax styling")
    }
}

[test]
def test_text_viewer_find_scrolls_to_match(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    var temp_error : string
    let temp_dir = create_temp_directory("das_text_viewer_find_scroll", temp_error)
    t |> success(empty(temp_error) && !empty(temp_dir),
                 "scroll test receives a dedicated temporary directory")
    if (!empty(temp_error) || empty(temp_dir)) return
    let fixture = path_join(temp_dir, "long.md")
    let body = build_string() $(var writer) {
        writer |> write("# Scroll fixture\n\nzebra opens the document.\n\n")
        for (index in range(200)) {
            writer |> write("Paragraph number {index} keeps the page long.\n\n")
        }
        writer |> write("zebra closes the document.\n\n")
        writer |> write("```\nzebra hides inside a code fence.\n```\n")
    }
    t |> success(fwrite(fixture, body), "long fixture is written")

    with_imgui_app(FEATURE) $(app) {
        let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
        t |> success(opened?["ok"] ?? false, "long fixture opens")
        let markdown_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["path"] ?? "") == fixture
                && (state?["mode"] ?? "") == "markdown"
                && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(markdown_ready, "long fixture renders as Markdown")
        if (!markdown_ready) return

        let found = post_command(app, "text_viewer_find", JV((query = "zebra")))
        t |> equal(found?["match_count"] ?? 0, 3, "all distant matches are found")
        let first_visible = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["markdown_selection_screen_valid"] ?? false)
                && (state?["markdown_selection_screen_y0"] ?? -1.0f)
                    >= (state?["content_view_top"] ?? 1e9f)
                && (state?["markdown_selection_screen_y1"] ?? 1e9f)
                    <= (state?["content_view_bottom"] ?? -1.0f))
        }
        t |> success(first_visible, "first match is on screen in rendered Markdown")

        let _ = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        let far_visible = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["markdown_selection_screen_valid"] ?? false)
                && (state?["markdown_selection_screen_y0"] ?? -1.0f)
                    >= (state?["content_view_top"] ?? 1e9f)
                && (state?["markdown_selection_screen_y1"] ?? 1e9f)
                    <= (state?["content_view_bottom"] ?? -1.0f))
        }
        t |> success(far_visible,
                     "find next scrolls the far match into the rendered viewport")

        // The third match lives inside a code fence: no inline selection
        // paint exists there, so the jump anchors on the focus node's flow
        // line instead (the fallback under test).
        let _fence = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        let fence_visible = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["markdown_focus_screen_valid"] ?? false)
                && (state?["markdown_focus_screen_y"] ?? -1.0f)
                    >= (state?["content_view_top"] ?? 1e9f)
                && (state?["markdown_focus_screen_y"] ?? 1e9f)
                    <= (state?["content_view_bottom"] ?? -1.0f))
        }
        t |> success(fence_visible,
                     "find next scrolls a code-fence match into the rendered viewport")

        let plain_mode = post_command(app, "text_viewer_set_mode", JV((mode = "plain")))
        t |> success(plain_mode?["ok"] ?? false, "plain mode accepts the switch")
        let _2 = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        let plain_first = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["source_selection_screen_valid"] ?? false)
                && (state?["source_selection_screen_y0"] ?? -1.0f)
                    >= (state?["content_view_top"] ?? 1e9f)
                && (state?["source_selection_screen_y1"] ?? 1e9f)
                    <= (state?["content_view_bottom"] ?? -1.0f))
        }
        t |> success(plain_first, "wrapped match is on screen in source view")

        let _3 = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        let plain_far = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["source_selection_screen_valid"] ?? false)
                && (state?["source_selection_screen_y0"] ?? -1.0f)
                    >= (state?["content_view_top"] ?? 1e9f)
                && (state?["source_selection_screen_y1"] ?? 1e9f)
                    <= (state?["content_view_bottom"] ?? -1.0f))
        }
        t |> success(plain_far, "find next scrolls the far match into the source viewport")
    }
    rmdir_rec_result(temp_dir)
}

[test]
def test_text_viewer_find_icons_and_reopen(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["ok"] ?? false) && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(ready, "welcome document renders")
        if (!ready) return
        let fixture = "{get_das_root()}/examples/text/syntax_highlighting.md"
        let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
        t |> success(opened?["ok"] ?? false, "fixture opens")

        let found = post_command(app, "text_viewer_find", JV((query = "tree-sitter")))
        t |> success((found?["match_count"] ?? 0) >= 2, "query matches")
        let bar = "DOCK_ROOT/g_document_windows[1]"
        let bar_rendered = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var snap = snapshot(app)
            return widget_rendered(snap, "{bar}/FIND_NEXT[1]")
        }
        t |> success(bar_rendered, "find bar renders")
        if (!bar_rendered) return

        // Every bar button is an icon button with the intended glyph family.
        var snap = snapshot(app)
        t |> equal(find_widget(snap, "{bar}/FIND_PREV[1]")?["kind"] ?? "?",
                   "icon_button", "previous-match is an icon button")
        t |> equal(find_widget(snap, "{bar}/FIND_NEXT[1]")?["kind"] ?? "?",
                   "icon_button", "next-match is an icon button")
        t |> equal(find_widget(snap, "{bar}/FIND_CLOSE[1]")?["kind"] ?? "?",
                   "icon_button", "close is an icon button")
        t |> equal(find_widget(snap, "{bar}/FIND_REGEX[1]")?["kind"] ?? "?",
                   "icon_button", "regex toggle is an icon button")

        // Close, clear the selection, reopen through the Ctrl+F command path:
        // the current match must come back selected.
        let _step = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        var recorded_start = -1
        var recorded_end = -1
        let selected = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            recorded_start = state?["markdown_selection_start"] ?? -1
            recorded_end = state?["markdown_selection_end"] ?? -1
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && recorded_end > recorded_start && recorded_start >= 0)
        }
        t |> success(selected, "stepped match is selected before the close")
        if (!selected) return

        let closed = post_command(app, "text_viewer_find_close", null)
        t |> success(closed?["ok"] ?? false, "find closes")
        let _clear = post_command(app, "text_viewer_command",
                                  JV((id = "edit.clear_selection")))
        let cleared = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return (!(state?["find_open"] ?? true)
                && (state?["markdown_selection_end"] ?? 0)
                    <= (state?["markdown_selection_start"] ?? 0))
        }
        t |> success(cleared, "selection clears while find is closed")

        let _reopen = post_command(app, "text_viewer_command", JV((id = "find.open")))
        let restored = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_open"] ?? false)
                && (state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && (state?["markdown_selection_start"] ?? -1) == recorded_start
                && (state?["markdown_selection_end"] ?? -1) == recorded_end)
        }
        t |> success(restored, "reopening find re-selects the current match")
        var after = viewer_state(app)
        t |> equal(after?["find_current"] ?? -1, 1, "current match survives the close")
        t |> equal(after?["find_match_count"] ?? -1, found?["match_count"] ?? -2,
                   "match count survives the close")
    }
}

[test]
def test_text_viewer_folding_and_code_copy(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["ok"] ?? false) && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(ready, "welcome document renders")
        if (!ready) return

        // ---- das source folding ----
        let das_fixture = "{get_das_root()}/examples/text/main.das"
        let opened = post_command(app, "text_viewer_open", JV((path = das_fixture)))
        t |> success(opened?["ok"] ?? false, ".das fixture opens")
        var lines_before = 0
        let das_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            lines_before = state?["visible_line_count"] ?? 0
            return ((state?["mode"] ?? "") == "plain"
                && (state?["source_render_revision"] ?? 0) > 0
                && (state?["fold_count"] ?? 0) > 20 && lines_before > 0)
        }
        t |> success(das_ready, ".das source reports tree-sitter fold regions")
        if (!das_ready) return

        let found = post_command(app, "text_viewer_find", JV((query = "harness_init")))
        t |> success((found?["match_count"] ?? 0) >= 1, "target text is found")
        var match_start = -1
        let selected = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            match_start = state?["source_selection_start"] ?? -1
            return ((state?["find_applied_jump"] ?? -1) == (state?["find_jump_revision"] ?? -2)
                && match_start >= 0)
        }
        t |> success(selected, "match is selected before the collapse")
        if (!selected) return

        var folds = post_command(app, "text_viewer_folds", null)
        let fold_count = folds?["fold_count"] ?? 0
        var target_fold = -1
        var target_size = int(0x7fffffff)
        for (index in range(fold_count)) {
            let fold = folds?["folds"]?[index]
            let start = fold?["start_byte"] ?? -1
            let end = fold?["end_byte"] ?? -1
            if (match_start >= start && match_start < end && end - start < target_size) {
                target_fold = fold?["index"] ?? -1
                target_size = end - start
            }
        }
        t |> success(target_fold >= 0, "a fold region contains the match")
        if (target_fold < 0) return

        let collapsed = post_command(app, "text_viewer_fold",
                                     JV((index = target_fold, collapsed = true)))
        t |> success(collapsed?["ok"] ?? false, "collapse command is accepted")
        let hidden = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["fold_collapsed_count"] ?? 0) == 1
                && (state?["visible_line_count"] ?? lines_before) < lines_before)
        }
        t |> success(hidden, "collapsing hides the region's body lines")

        let _step = post_command(app, "text_viewer_find_step", JV((delta = 1)))
        let expanded = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["fold_collapsed_count"] ?? 1) == 0
                && (state?["visible_line_count"] ?? 0) == lines_before
                && (state?["source_selection_screen_valid"] ?? false)
                && (state?["source_selection_screen_y0"] ?? -1.0f)
                    >= (state?["content_view_top"] ?? 1e9f)
                && (state?["source_selection_screen_y1"] ?? 1e9f)
                    <= (state?["content_view_bottom"] ?? -1.0f))
        }
        t |> success(expanded,
                     "find jump auto-expands the fold and shows the match")

        // ---- markdown code-block collapse + copy ----
        let md_fixture = "{get_das_root()}/examples/text/syntax_highlighting.md"
        let md_opened = post_command(app, "text_viewer_open", JV((path = md_fixture)))
        t |> success(md_opened?["ok"] ?? false, "markdown fixture opens")
        let md_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["path"] ?? "") == md_fixture
                && (state?["mode"] ?? "") == "markdown"
                && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(md_ready, "markdown fixture renders")
        if (!md_ready) return

        var blocks = post_command(app, "text_viewer_md_code_blocks", null)
        let block_count = blocks?["count"] ?? 0
        t |> success(block_count >= 3, "fixture carries several code blocks")
        for (index in range(block_count)) {
            let node = blocks?["blocks"]?[index]?["node"] ?? -1
            let _fold = post_command(app, "text_viewer_md_fold",
                                     JV((node = node, collapsed = true)))
        }
        let md_hidden = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return (state?["md_code_collapsed_count"] ?? 0) == block_count
        }
        t |> success(md_hidden, "every code block collapses")

        let md_found = post_command(app, "text_viewer_find", JV((query = "recipient")))
        t |> success((md_found?["match_count"] ?? 0) >= 1,
                     "query hits text inside a collapsed block")
        let md_expanded = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["md_code_collapsed_count"] ?? block_count) < block_count
                && (state?["markdown_selection_end"] ?? -1)
                    > (state?["markdown_selection_start"] ?? -1))
        }
        t |> success(md_expanded,
                     "find jump expands the collapsed block and selects the match")

        let first_node = blocks?["blocks"]?[0]?["node"] ?? -1
        let copied = post_command(app, "text_viewer_md_copy_code",
                                  JV((node = first_node)))
        t |> success(copied?["ok"] ?? false, "copy command is accepted")
        t |> equal(copied?["code_copy_revision"] ?? 0, 1, "copy action is observable")
        t |> success((copied?["code_copy_bytes"] ?? 0) > 0,
                     "copied text carries the block's content")
    }
}

[test]
def test_text_viewer_fold_clicks_and_no_reflow(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["ok"] ?? false) && (state?["render_revision"] ?? 0) > 0)
        }
        t |> success(ready, "welcome document renders")
        if (!ready) return

        // ---- rendered markdown: click the hover chrome itself ----
        let md_fixture = "{get_das_root()}/examples/text/syntax_highlighting.md"
        let opened = post_command(app, "text_viewer_open", JV((path = md_fixture)))
        t |> success(opened?["ok"] ?? false, "markdown fixture opens")
        var first_node = -1
        var hover_x = -1.0f
        var hover_y = -1.0f
        let md_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var blocks = post_command(app, "text_viewer_md_code_blocks", null)
            first_node = blocks?["blocks"]?[0]?["node"] ?? -1
            hover_x = (blocks?["blocks"]?[0]?["screen_x"] ?? -100.0f) + 40.0f
            hover_y = (blocks?["blocks"]?[0]?["screen_y"] ?? -100.0f) + 30.0f
            return first_node >= 0 && hover_x > 0.0f && hover_y > 0.0f
        }
        t |> success(md_ready, "first code block reports its screen origin")
        if (!md_ready) return

        // Hovering the block content makes the chrome appear and stamps rects.
        post_command(app, "imgui_mouse_pos", JV((x = hover_x, y = hover_y)))
        var fold_x = -1.0f
        var fold_y = -1.0f
        var copy_x = -1.0f
        var copy_y = -1.0f
        let icons_shown = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var blocks = post_command(app, "text_viewer_md_code_blocks", null)
            fold_x = blocks?["blocks"]?[0]?["fold_icon_x"] ?? -1.0f
            fold_y = blocks?["blocks"]?[0]?["fold_icon_y"] ?? -1.0f
            copy_x = blocks?["blocks"]?[0]?["copy_icon_x"] ?? -1.0f
            copy_y = blocks?["blocks"]?[0]?["copy_icon_y"] ?? -1.0f
            return ((blocks?["blocks"]?[0]?["icons_valid"] ?? false)
                && fold_x > 0.0f && copy_x > fold_x)
        }
        t |> success(icons_shown, "hover reveals the fold and copy icons")
        if (!icons_shown) return

        // THE regression: clicking the fold icon on an EXPANDED block must
        // collapse it (the active-item hover gate used to drop the release).
        post_command(app, "imgui_mouse_click_at",
                     JV((x = fold_x, y = fold_y, button = 0, travel_ms = 80.0f)))
        let collapsed_by_click = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var blocks = post_command(app, "text_viewer_md_code_blocks", null)
            return (blocks?["blocks"]?[0]?["collapsed"] ?? false)
        }
        t |> success(collapsed_by_click, "clicking the fold icon collapses the block")

        // The collapsed bar reports icons too; its chevron expands it back.
        var bar_fold_x = -1.0f
        var bar_fold_y = -1.0f
        let bar_icons = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var blocks = post_command(app, "text_viewer_md_code_blocks", null)
            bar_fold_x = blocks?["blocks"]?[0]?["fold_icon_x"] ?? -1.0f
            bar_fold_y = blocks?["blocks"]?[0]?["fold_icon_y"] ?? -1.0f
            return (blocks?["blocks"]?[0]?["icons_valid"] ?? false) && bar_fold_x > 0.0f
        }
        t |> success(bar_icons, "collapsed bar reports its icons")
        post_command(app, "imgui_mouse_click_at",
                     JV((x = bar_fold_x, y = bar_fold_y, button = 0, travel_ms = 80.0f)))
        let expanded_by_click = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var blocks = post_command(app, "text_viewer_md_code_blocks", null)
            return !(blocks?["blocks"]?[0]?["collapsed"] ?? true)
        }
        t |> success(expanded_by_click, "clicking the bar icon expands the block")

        // Copy icon click routes to the copy action.
        post_command(app, "imgui_mouse_pos", JV((x = hover_x, y = hover_y)))
        let icons_again = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var blocks = post_command(app, "text_viewer_md_code_blocks", null)
            copy_x = blocks?["blocks"]?[0]?["copy_icon_x"] ?? -1.0f
            copy_y = blocks?["blocks"]?[0]?["copy_icon_y"] ?? -1.0f
            return (blocks?["blocks"]?[0]?["icons_valid"] ?? false) && copy_x > 0.0f
        }
        t |> success(icons_again, "expanded block re-reports its icons")
        post_command(app, "imgui_mouse_click_at",
                     JV((x = copy_x, y = copy_y, button = 0, travel_ms = 80.0f)))
        let copied = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["code_copy_revision"] ?? 0) >= 1
                && (state?["code_copy_bytes"] ?? 0) > 0)
        }
        t |> success(copied, "clicking the copy icon captures the block text")

        // ---- das source: gutter chevron click + toggle-without-reflow ----
        let das_fixture = "{get_das_root()}/examples/text/main.das"
        let das_opened = post_command(app, "text_viewer_open", JV((path = das_fixture)))
        t |> success(das_opened?["ok"] ?? false, ".das fixture opens")
        var handle_x = -1.0f
        var handle_y = -1.0f
        var handle_index = -1
        var lines_before = 0
        let das_ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            lines_before = state?["visible_line_count"] ?? 0
            if ((state?["mode"] ?? "") != "plain" || lines_before == 0) return false
            let view_top = state?["content_view_top"] ?? 0.0f
            let view_bottom = state?["content_view_bottom"] ?? 0.0f
            var folds = post_command(app, "text_viewer_folds", null)
            let count = folds?["fold_count"] ?? 0
            for (index in range(count)) {
                let fold = folds?["folds"]?[index]
                let handle_row_y = fold?["handle_y"] ?? -1.0f
                // The handle must be ON SCREEN for a synthetic click to land.
                if ((fold?["handle_valid"] ?? false)
                        && (fold?["end_row"] ?? 0) - (fold?["header_row"] ?? 0) >= 2
                        && handle_row_y > view_top + 10.0f && handle_row_y < view_bottom - 10.0f) {
                    handle_index = fold?["index"] ?? -1
                    handle_x = fold?["handle_x"] ?? -1.0f
                    handle_y = handle_row_y
                    return true
                }
            }
            return false
        }
        t |> success(das_ready, "a das fold reports a visible gutter handle")
        if (!das_ready) return

        let misses_before = viewer_state(app)?["source_layout_misses"] ?? -1
        post_command(app, "imgui_mouse_click_at",
                     JV((x = handle_x, y = handle_y, button = 0, travel_ms = 80.0f)))
        let gutter_collapsed = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["fold_collapsed_count"] ?? 0) == 1
                && (state?["visible_line_count"] ?? lines_before) < lines_before)
        }
        t |> success(gutter_collapsed, "clicking the gutter chevron collapses the region")

        for (_toggle in range(3)) {
            let _1 = post_command(app, "text_viewer_fold",
                                  JV((index = handle_index, collapsed = false)))
            let _2 = post_command(app, "text_viewer_fold",
                                  JV((index = handle_index, collapsed = true)))
        }
        let settled = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["fold_collapsed_count"] ?? 0) == 1
                && (state?["visible_line_count"] ?? lines_before) < lines_before)
        }
        t |> success(settled, "toggle burst settles collapsed")
        let misses_after = viewer_state(app)?["source_layout_misses"] ?? -2
        t |> equal(misses_after, misses_before,
                   "fold toggles reuse the cached flow instead of reflowing")
    }
}

[test]
def test_text_viewer_single_scroller(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    //! The content child is the only scroller: the dock window must never grow
    //! its own scrollbar, and the right-aligned zoom cluster must fit at any
    //! zoom value (both regressed together as "two scrollbars + clipped 100%").
    with_imgui_app(FEATURE) $(app) {
        let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["ok"] ?? false)
                && (state?["render_revision"] ?? 0) > 0
                && (state?["window_docked"] ?? false))
        }
        t |> success(ready, "welcome document renders docked")
        if (!ready) return

        let md_clean = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["window_scroll_max_y"] ?? -1.0f) == 0.0f
                && (state?["footer_overflow_x"] ?? -1.0f) == 0.0f)
        }
        t |> success(md_clean,
                     "rendered mode: no window scrollbar and the footer fits")

        for (percent in [300, 50]) {
            let zoomed = post_command(app, "text_viewer_set_zoom",
                                      JV((percent = percent)))
            t |> success(zoomed?["ok"] ?? false, "zoom command accepts the extreme")
            let zoom_clean = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
                var state = viewer_state(app)
                return ((state?["zoom_percent"] ?? 0) == percent
                    && (state?["window_scroll_max_y"] ?? -1.0f) == 0.0f
                    && (state?["footer_overflow_x"] ?? -1.0f) == 0.0f)
            }
            t |> success(zoom_clean,
                         "extreme zoom keeps the footer inside the window")
        }
        let _zoom_back = post_command(app, "text_viewer_set_zoom",
                                      JV((percent = 100)))

        let plain = post_command(app, "text_viewer_set_mode", JV((mode = "plain")))
        t |> success(plain?["ok"] ?? false, "mode command accepts plain text")
        let plain_clean = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["mode"] ?? "") == "plain"
                && (state?["window_scroll_max_y"] ?? -1.0f) == 0.0f
                && (state?["footer_overflow_x"] ?? -1.0f) == 0.0f)
        }
        t |> success(plain_clean,
                     "source mode: no window scrollbar and the footer fits")

        let found = post_command(app, "text_viewer_find", JV((query = "the")))
        t |> success(found?["ok"] ?? false, "find opens in the source document")
        let find_clean = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var state = viewer_state(app)
            return ((state?["find_open"] ?? false)
                && (state?["window_scroll_max_y"] ?? -1.0f) == 0.0f
                && (state?["footer_overflow_x"] ?? -1.0f) == 0.0f)
        }
        t |> success(find_clean, "an open find bar does not overflow the window")
        let _closed = post_command(app, "text_viewer_find_close", null)
    }
}
