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

//! End-to-end creation-registry scenarios in examples/text: wrap chaining,
//! snippet Tab flow, transforms, context gating, and one REAL Ctrl+B chord.

let FEATURE = "examples/text/main.das"
let DEADLOCK_GUARD_SEC = 30.0f
let SOURCE_EDITOR = "DOCK_ROOT/g_document_windows[1]/DOCUMENT_CONTENT/EDITOR_PANE/editor"

def private viewer_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_state", null)
}

def private edit_text_state(app : ImguiApp) : JsonValue? {
    return post_command(app, "text_viewer_edit_text", null)
}

def private run_create(app : ImguiApp; id : string) : JsonValue? {
    return post_command(app, "text_viewer_command", JV((id = "create." + id)))
}

def private open_md_editing(t : T?; app : ImguiApp; temp_dir, body : string) : bool {
    let fixture = path_join(temp_dir, "create.md")
    t |> success(fwrite(fixture, body), "fixture written")
    let opened = post_command(app, "text_viewer_open", JV((path = fixture)))
    t |> success(opened?["ok"] ?? false, "fixture opens")
    let entered = post_command(app, "text_viewer_edit", JV((on = true)))
    t |> success(entered?["ok"] ?? false, "edit mode enters")
    // creation flows drive the ONE buffer; pin the plain Source surface
    let surfaced = post_command(app, "text_viewer_command",
        JV((id = "view.surface_source")))
    t |> success(surfaced?["ok"] ?? false, "source surface selects")
    let ready = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
        var state = viewer_state(app)
        return ((state?["editing"] ?? false)
            && (state?["edit_surface"] ?? "") == "source")
    }
    t |> success(ready, "editing on the Source surface")
    return ready
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
def test_create_wrap_chaining_and_undo(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_create_wrap", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        // single line WITHOUT a trailing newline: select-all must stay
        // single-line for the wrap rule to keep the selection
        return if (!open_md_editing(t, app, temp_dir, "hello"))
        let selected = post_command(app, "text_viewer_command",
            JV((id = "edit.select_all")))
        t |> success(selected?["ok"] ?? false, "select all queues")
        let has_selection = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return edit_text_state(app)?["has_selection"] ?? false
        }
        t |> success(has_selection, "the buffer text is selected")
        let bolded = run_create(app, "md.bold")
        t |> success(bolded?["ok"] ?? false, "bold command queues")
        let bold_done = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "**hello") >= 0
        }
        t |> success(bold_done, "bold wraps the selection")
        let italized = run_create(app, "md.italic")
        t |> success(italized?["ok"] ?? false, "italic command queues")
        let chained = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "***hello***") >= 0
        }
        t |> success(chained, "the kept selection lets wraps chain")
        let undo_one = post_command(app, "text_viewer_command",
            JV((id = "edit.undo")))
        t |> success(undo_one?["ok"] ?? false, "undo queues")
        let peeled = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            let text = edit_text_state(app)?["text"] ?? ""
            return find(text, "**hello**") >= 0 && find(text, "***") < 0
        }
        t |> success(peeled, "one undo peels exactly one wrap")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_create_link_snippet_tab_flow(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_create_link", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_md_editing(t, app, temp_dir, "hello\n"))
        var probe = wait_for_render(app, SOURCE_EDITOR, 15.0f)
        t |> success(probe != null, "editor pane renders")
        click(app, SOURCE_EDITOR)
        t |> success(wait_for_mouse_idle(app), "editor focus click drains")
        let selected = post_command(app, "text_viewer_command",
            JV((id = "edit.select_all")))
        t |> success(selected?["ok"] ?? false, "select all queues")
        let linked = run_create(app, "md.link")
        t |> success(linked?["ok"] ?? false, "link command queues")
        let snippet_on = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var buffer = edit_text_state(app)
            return (find(buffer?["text"] ?? "", "[hello\n](url)") >= 0
                && (buffer?["snippet_active"] ?? false))
        }
        t |> success(snippet_on, "the link snippet armed with the url stop")
        let typed = post_command(app, "imgui_key_type", JV((text = "x.io")))
        t |> success(typed != null, "url typed over the placeholder")
        let url_done = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "(x.io)") >= 0
        }
        t |> success(url_done, "typing replaced the selected placeholder")
        key_tap(app, int(ImGuiKey.Tab))
        let ended = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return !(edit_text_state(app)?["snippet_active"] ?? true)
        }
        t |> success(ended, "Tab walks out of the snippet session")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_create_transform_and_fence_gate(t : T?) { // nolint:STYLE038 — one event-gated scenario; splitting breaks the wait-chain
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_create_gate", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_md_editing(t, app, temp_dir,
            "plain line\n\n```das\ncode\n```\n"))
        let cycled = run_create(app, "md.heading")
        t |> success(cycled?["ok"] ?? false, "heading command queues")
        let heading_done = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "# plain line") >= 0
        }
        t |> success(heading_done, "heading cycle transforms the caret line")
        // move into the fence: the whole markdown set is gated there
        let moved = post_command(app, "text_viewer_set_cursor",
            JV((line = 3, column = 0)))
        t |> success(moved?["ok"] ?? false, "caret into the fence")
        let gated = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            var creations = post_command(app, "text_viewer_creations", null)
            return ((creations?["in_code_fence"] ?? false)
                && (creations?["count"] ?? -1) == 0)
        }
        t |> success(gated, "not-in-code-fence empties the creatable set")
        let text_before = edit_text_state(app)?["text"] ?? ""
        let declined = run_create(app, "md.bold")
        t |> success(declined?["ok"] ?? false, "bold command still queues")
        let refused = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(viewer_state(app)?["status"] ?? "",
                "Cannot insert here.") >= 0
        }
        t |> success(refused, "execution declines with the status message")
        t |> equal(edit_text_state(app)?["text"] ?? "", text_before,
            "the buffer is untouched inside the fence")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}

[test]
def test_create_real_ctrl_b_chord(t : T?) {
    with_imgui_app(FEATURE) $(app) {
        return if (!viewer_ready(t, app))
        var temp_error = ""
        let temp_dir = create_temp_directory("das_create_chord", temp_error)
        t |> success(empty(temp_error) && !empty(temp_dir), "temp dir created")
        return if (empty(temp_dir))
        {
        return if (!open_md_editing(t, app, temp_dir, "chord"))
        var probe = wait_for_render(app, SOURCE_EDITOR, 15.0f)
        t |> success(probe != null, "editor pane renders")
        click(app, SOURCE_EDITOR)
        t |> success(wait_for_mouse_idle(app), "editor focus click drains")
        let selected = post_command(app, "text_viewer_command",
            JV((id = "edit.select_all")))
        t |> success(selected?["ok"] ?? false, "select all queues")
        let has_selection = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return edit_text_state(app)?["has_selection"] ?? false
        }
        t |> success(has_selection, "selection in place for the chord")
        // the REAL chord: Ctrl+B through the editor's component poll (synth
        // keys are logical - LeftCtrl reads back as io.KeyCtrl everywhere)
        post_command(app, "imgui_key_press", JV((key = int(ImGuiKey.LeftCtrl))))
        post_command(app, "imgui_key_press", JV((key = int(ImGuiKey.B))))
        let wrapped = wait_cond(app, DEADLOCK_GUARD_SEC) $() : bool {
            return find(edit_text_state(app)?["text"] ?? "", "**chord**") >= 0
        }
        post_command(app, "imgui_key_release", JV((key = int(ImGuiKey.B))))
        post_command(app, "imgui_key_release", JV((key = int(ImGuiKey.LeftCtrl))))
        t |> success(wrapped, "Ctrl+B drives create.md.bold end to end")
        } finally {
            rmdir_rec_result(temp_dir)
        }
    }
}
