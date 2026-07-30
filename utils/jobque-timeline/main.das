options gen2
options stack = 65536
options gc

require imgui/imgui_harness
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require daslib/json_boost
require strings
require math
require imgui/imgui_file_dialog
require tl_model
require tl_loader
require tl_view
require tl_stats
require tl_launch

// jobque timeline viewer — first light: N trace files in dockable windows (stacked),
// shared zoom/timer ON by default, unit markers, hover tooltips, timeline_* live commands.
//   daslang -project_root utils/jobque-timeline utils/jobque-timeline/main.das -- a.trace.json b.trace.json
//   daslang-live -project_root utils/jobque-timeline utils/jobque-timeline/main.das

let MAX_DOCS = 4   // static dock_window slots (TRACE0..TRACE3) — dynamic idents later

var g_docs : array<TraceDoc>
var g_ui : array<ViewUI>
var g_render : array<RenderCache>      // per-doc cached draw runs
var g_view_stats : array<StatsCache>   // per-doc, keyed on the visible range
var g_sel_stats : array<StatsCache>    // per-doc, keyed on the selection range
var g_pending : array<string>   // files handed on the command line, loaded in init
var g_launch = LaunchRail()
var g_watch : array<FileWatch>  // per-doc auto-refresh watchers
var g_poll_frame = 0
var g_dlg_purpose = 0           // the singleton file dialog's pending consumer: 1 = launch app, 2 = open traces

var private DOC_STATS : table<int; NarrativeState>    // per-window header line widgets
var private DOC_CHIPS : table<int; NarrativeState>    // per-window computed stats (idle/par/cats)
var private DOC_SEL : table<int; NarrativeState>      // per-window selection stats
var private FILE_LABEL : table<int; NarrativeState>   // controls-panel file list
var private RELOAD_BTN : table<int; ClickState>       // per-file manual refresh
var private CLOSE_BTN : table<int; ClickState>        // per-file close
var private FILE_SL_A : table<int; EmptyMarkerState>  // per-file row same_line markers
var private FILE_SL_B : table<int; EmptyMarkerState>
var private TAIL_OPEN : table<int; ClickState>        // output lines naming a .json: open it
var private TAIL_COPY : table<int; ClickState>        // ... or copy its path
var private TAIL_SL_A : table<int; EmptyMarkerState>
var private TAIL_SL_B : table<int; EmptyMarkerState>
var private TAIL_LINE : table<int; NarrativeState>    // launch output tail
var g_close_req = -1            // deferred close (icon or window X) — applied after the frame's windows

def open_file(path : string) : bool {
    if (length(g_docs) >= MAX_DOCS) {
        to_log(LOG_ERROR, "jobque-timeline: MAX_DOCS={MAX_DOCS} windows in v0\n")
        return false
    }
    var doc : TraceDoc
    if (!load_trace(path, doc)) {
        to_log(LOG_ERROR, "jobque-timeline: failed to load {path}\n")
        return false
    }
    doc.title = "{length(g_docs)}: {doc.title}"
    g_docs |> emplace(doc)
    var ui = ViewUI()
    g_ui |> push(ui)
    var rc = RenderCache()
    g_render |> emplace(rc)
    var vc = StatsCache()
    g_view_stats |> emplace(vc)
    var sc = StatsCache()
    g_sel_stats |> emplace(sc)
    var fw = FileWatch()
    watch_prime(fw, path)
    g_watch |> push(fw)
    return true
}

def trace_window_open(i : int) : bool {
    if (i == 0) return TRACE0.open
    if (i == 1) return TRACE1.open
    if (i == 2) return TRACE2.open
    return TRACE3.open
}

def close_file(i : int) {
    g_docs |> erase(i)
    g_ui |> erase(i)
    g_render |> erase(i)
    g_view_stats |> erase(i)
    g_sel_stats |> erase(i)
    g_watch |> erase(i)
    for (j in range(length(g_docs))) {
        g_docs[j].title = "{j}: {base_name(g_docs[j].path)}"
    }
    TRACE0.open = true   // dock slots are positional — reset and reseed
    TRACE1.open = true
    TRACE2.open = true
    TRACE3.open = true
    TL_ROOT.has_initial_layout = false
}

def open_or_reload(path : string) {
    for (i in range(length(g_docs))) {
        if (g_docs[i].path == path) {
            reload_file(i)
            return
        }
    }
    if (open_file(path)) {
        TL_ROOT.has_initial_layout = false
    }
}

def reload_file(i : int) {
    var doc : TraceDoc
    if (!load_trace(g_docs[i].path, doc)) {
        to_log(LOG_ERROR, "jobque-timeline: reload failed for {g_docs[i].path}\n")
        return
    }
    doc.title = g_docs[i].title
    doc.generation = g_docs[i].generation + 1   // keep caches strictly invalidating
    g_docs[i] <- doc
    clamp_view(g_ui[i], max(g_docs[i].span, 8.0lf))
}

// ---- view sync: one-way intent from the interacted view; clamping display-local ----

def sync_views(from_idx : int) {
    let src & = unsafe(g_ui[from_idx])
    let w = src.t1 - src.t0
    for (i in range(length(g_ui))) {
        if (i == from_idx) {
            continue
        }
        var ui & = unsafe(g_ui[i])
        let span = max(g_docs[i].span, 8.0lf)
        if (!ui.inited) {   // sync must land on fresh views too, not be overwritten by view_full
            ui.t0 = 0.0lf
            ui.t1 = span
            ui.inited = true
        }
        let nw = SHARE_ZOOM.value ? max(w, 8.0lf) : (ui.t1 - ui.t0)
        var nt0 = ui.t0
        if (SHARE_TIMER.value) {
            nt0 = src.t0
        } elif (SHARE_ZOOM.value) {
            nt0 = (ui.t0 + ui.t1) * 0.5lf - nw * 0.5lf
        }
        // NO clamp on adoption — shared timer means the same absolute clock even when this
        // file's data has run out (it draws empty); direct interaction on this view clamps
        ui.t0 = nt0
        ui.t1 = nt0 + nw
    }
}

def setup_layout(dock_id : uint) {
    // controls left column, trace windows stacked vertically on the right
    DockBuilderRemoveNode(dock_id)
    DockBuilderAddDockSpaceNode(dock_id, ImGuiDockNodeFlags.None)
    let vp = GetMainViewport()
    DockBuilderSetNodeSize(dock_id, ImVec2(vp.Size.x, vp.Size.y))
    var left_id : uint = 0u
    var main_id : uint = 0u
    DockBuilderSplitNode(dock_id, ImGuiDir.Left, 0.16f, left_id, main_id)
    var launch_id : uint = 0u
    var ctrl_id : uint = 0u
    DockBuilderSplitNode(left_id, ImGuiDir.Down, 0.34f, launch_id, ctrl_id)
    DockBuilderDockWindow("Controls", ctrl_id)
    DockBuilderDockWindow("Launch", launch_id)
    let n = length(g_docs)
    var cur = main_id
    for (i in range(n)) {
        if (i == n - 1) {
            DockBuilderDockWindow(g_docs[i].title, cur)
        } else {
            var top_id : uint = 0u
            var bot_id : uint = 0u
            DockBuilderSplitNode(cur, ImGuiDir.Up, 1.0f / float(n - i), top_id, bot_id)
            DockBuilderDockWindow(g_docs[i].title, top_id)
            cur = bot_id
        }
    }
    DockBuilderFinish(dock_id)
}

def draw_doc_window(i : int) {
    let header = "{g_docs[i].total_events} events  {length(g_docs[i].lanes)} lanes  span {fmt_us(g_docs[i].span)}  view {fmt_us(g_ui[i].t1 - g_ui[i].t0)}"
    text(DOC_STATS[i], (text = header))
    if (g_ui[i].inited) {
        cached_stats(g_view_stats[i], g_docs[i], g_ui[i].t0, g_ui[i].t1)
        text(DOC_CHIPS[i], (text = stats_line(g_view_stats[i].stats)))
        if (g_ui[i].sel_active) {
            cached_stats(g_sel_stats[i], g_docs[i], g_ui[i].sel_t0, g_ui[i].sel_t1)
            text(DOC_SEL[i], (text = "sel {fmt_us(g_ui[i].sel_t1 - g_ui[i].sel_t0)}:  " + stats_line(g_sel_stats[i].stats)))
        }
    }
    draw_timeline(g_docs[i], g_ui[i], g_render[i], "tl_canvas_{i}")
    if (g_ui[i].interacted) {
        sync_views(i)
    }
}

[export]
def init() {
    harness_init("jobque timeline", 1500, 950)
    create_job_que()   // the launch rail runs subprocesses in background jobs
    var io & = unsafe(GetIO())
    io.ConfigFlags |= ImGuiConfigFlags.DockingEnable
    SHARE_ZOOM.value = true    // both sync modes ON by default
    SHARE_TIMER.value = true
    AUTO_REFRESH.value = true
    AFFINITY_CB.value = true   // bench discipline: launched runs pin by default
    if (has_env_variable("DAS_JOBQUE_THREADS")) {   // seed from the tool's own env (box config)
        THREADS_INPUT.value = to_int(get_env_variable("DAS_JOBQUE_THREADS"))
    }
    for (path in g_pending) {
        open_file(path)
    }
    delete g_pending
}

[export]
def update() { // nolint:STYLE038 — one frame of immediate-mode UI, splitting is noise
    if (!harness_begin_frame()) {
        return
    }
    harness_new_frame()
    if (launch_pump(g_launch)) {
        to_log(LOG_INFO, "jobque-timeline: launch finished rc={g_launch.exit_code}\n")
        if (g_launch.exit_code == 0) {
            for (line in g_launch.tail) {   // auto-open every trace the run reported saving
                let jp = extract_json_path(line)
                if (!empty(jp)) {
                    open_or_reload(jp)
                }
            }
        }
    }
    g_poll_frame++
    if (AUTO_REFRESH.value && g_poll_frame % 30 == 0) {
        for (i in range(length(g_docs))) {
            if (watch_poll(g_watch[i], g_docs[i].path)) {
                reload_file(i)
            }
        }
    }
    dockspace(TL_ROOT, (flags = ImGuiDockNodeFlags.PassthruCentralNode)) {
        if (!TL_ROOT.has_initial_layout && TL_ROOT.dock_id != 0u) {
            setup_layout(TL_ROOT.dock_id)
            TL_ROOT.has_initial_layout = true
        }
        dock_window(CONTROLS, (text = "Controls", closable = false, flags = ImGuiWindowFlags.None)) {
            checkbox(SHARE_ZOOM, (text = "share zoom"))
            checkbox(SHARE_TIMER, (text = "share timer"))
            checkbox(AUTO_REFRESH, (text = "auto refresh"))
            separator()
            if (icon_button(OPEN_TRACE_BTN, (glyph = "folder", tip = "open trace file(s) — multi-select with ctrl/shift"))) {
                file_dialog_open(FileDialogConfig(title = "Open Trace", start_dir = "",
                    filters <- file_filters("Traces (*.json)|json|All files|"),
                    mode = FileDialogMode.open, max_selection = MAX_DOCS))
                g_dlg_purpose = 2
            }
            same_line()
            text("open trace...")
            for (i in range(length(g_docs))) {
                if (icon_button(RELOAD_BTN[i], (glyph = "refresh", tip = "reload from disk"))) {
                    reload_file(i)
                }
                same_line(FILE_SL_A[i])
                if (icon_button(CLOSE_BTN[i], (glyph = "close", tip = "close"))) {
                    g_close_req = i
                }
                same_line(FILE_SL_B[i])
                text(FILE_LABEL[i], (text = g_docs[i].title))
            }
        }
        dock_window(LAUNCH_WIN, (text = "Launch", closable = false, flags = ImGuiWindowFlags.None)) {
            if (icon_button(BROWSE_BTN, (glyph = "folder", tip = "select the app to run (.das runs via daslang -jit)"))) {
                file_dialog_open(FileDialogConfig(title = "Select App", start_dir = "",
                    filters <- file_filters("Apps (*.das *.exe)|das;exe|All files|"),
                    mode = FileDialogMode.open, max_selection = 1))
                g_dlg_purpose = 1
            }
            same_line()
            text(APP_LABEL, (text = empty(g_launch.app) ? "<no app>" : base_name(g_launch.app)))
            if (icon_button(ARGS_EDIT_BTN, (glyph = "sliders", tip = "edit arguments (multi-line)"))) {
                ARGS_MULTI.capacity = 4096
                ARGS_MULTI.pending_value = g_launch.args
                ARGS_MULTI.has_pending = true
                ARGS_MODAL.pending_open = true
            }
            same_line()
            text(ARGS_LABEL, (text = empty(g_launch.args) ? "<no args>" : g_launch.args))
            checkbox(AFFINITY_CB, (text = "affinity"))
            input_int(THREADS_INPUT, (text = "threads (0 = inherit)"))
            popup_modal(ARGS_MODAL, (text = "Launch arguments", closable = true, flags = ImGuiWindowFlags.None)) {
                text("whitespace-separated; newlines are fine (one argument per line avoids quoting)")
                input_text_multiline(ARGS_MULTI, (text = "##args", size = float2(640.0f, 240.0f)))
                if (button(ARGS_OK, (text = "OK"))) {
                    g_launch.args = ARGS_MULTI.value
                    ARGS_MODAL.pending_close = true
                }
                same_line()
                if (button(ARGS_CANCEL, (text = "Cancel"))) {
                    ARGS_MODAL.pending_close = true
                }
            }
            if (g_launch.running) {
                text("running...")
            } else {
                if (icon_button(LAUNCH_BTN, (glyph = "play", tip = "launch"))) {
                    g_launch.affinity = AFFINITY_CB.value
                    g_launch.threads = THREADS_INPUT.value
                    if (!launch_start(g_launch)) {
                        to_log(LOG_ERROR, "jobque-timeline: launch failed to start\n")
                    }
                }
                if (g_launch.has_run) {
                    same_line()
                    text(RC_LABEL, (text = "exit code {g_launch.exit_code}"))
                }
            }
            separator()
            for (i in range(length(g_launch.tail))) {
                let jpath = extract_json_path(g_launch.tail[i])
                if (!empty(jpath)) {
                    if (icon_button(TAIL_OPEN[i], (glyph = "folder", tip = "open this trace", size = 14.0f))) {
                        open_or_reload(jpath)
                    }
                    same_line(TAIL_SL_A[i])
                    if (icon_button(TAIL_COPY[i], (glyph = "duplicate", tip = "copy path to clipboard", size = 14.0f))) {
                        log_to_clipboard()
                        log_text(jpath)
                        log_finish()
                    }
                    same_line(TAIL_SL_B[i])
                }
                text(TAIL_LINE[i], (text = g_launch.tail[i]))
            }
        }
        let flags = ImGuiWindowFlags.NoScrollbar | ImGuiWindowFlags.NoScrollWithMouse
        if (!empty(g_docs)) {
            dock_window(TRACE0, (text = g_docs[0].title, closable = true, flags = flags)) {
                draw_doc_window(0)
            }
        }
        if (length(g_docs) > 1) {
            dock_window(TRACE1, (text = g_docs[1].title, closable = true, flags = flags)) {
                draw_doc_window(1)
            }
        }
        if (length(g_docs) > 2) {
            dock_window(TRACE2, (text = g_docs[2].title, closable = true, flags = flags)) {
                draw_doc_window(2)
            }
        }
        if (length(g_docs) > 3) {
            dock_window(TRACE3, (text = g_docs[3].title, closable = true, flags = flags)) {
                draw_doc_window(3)
            }
        }
    }
    // apply deferred closes (close icon or a trace window's X)
    for (i in range(length(g_docs))) {
        if (!trace_window_open(i)) {
            g_close_req = i
        }
    }
    if (g_close_req >= 0 && g_close_req < length(g_docs)) {
        close_file(g_close_req)
    }
    g_close_req = -1
    // the file dialog renders at top level (clean FILEDLG/... registry path); one modal
    // serves both consumers, dispatched by g_dlg_purpose
    let fd = file_dialog_draw()
    if (fd == FileDialogResult.confirmed) {
        if (g_dlg_purpose == 1) {
            g_launch.app = file_dialog_selected_path()
        } elif (g_dlg_purpose == 2) {
            var sel <- file_dialog_selection()
            for (p in sel) {
                open_or_reload(p)   // re-picking an open file reloads it instead of duplicating
            }
            delete sel
            TL_ROOT.has_initial_layout = false   // reseed the stacked layout with the new windows
        }
        g_dlg_purpose = 0
    } elif (fd == FileDialogResult.cancelled) {
        g_dlg_purpose = 0
    }
    harness_end_frame()
}

[export]
def shutdown() {
    destroy_job_que()
    harness_shutdown()
}

// ---- live commands (timeline_ prefix): structured state for screenshot-free queries ----

struct TLFileArg {
    file : int
}

struct TLSetViewArg {
    file : int
    t0 : double
    t1 : double
}

struct TLUnitArg {
    file : int
    kind : string
    index : int
}

struct TLOpenArg {
    path : string
}

def doc_entry(i : int) : JsonValue? {
    var mks <- [for (mk in g_docs[i].marker_kinds); "{mk.name}:{length(mk.ts)}"]
    return JV((index = i, title = g_docs[i].title, path = g_docs[i].path,
        lanes = length(g_docs[i].lanes), events = int(g_docs[i].total_events),
        span_us = g_docs[i].span, view_t0 = g_ui[i].t0, view_t1 = g_ui[i].t1,
        markers <- mks))
}

[live_command(description = "list open trace files with spans, views, and marker kinds"), unused_argument(input)]
def timeline_files(input : JsonValue?) : JsonValue? {
    var files <- [for (i in range(length(g_docs))); doc_entry(i)]
    return JV((files <- files, share_zoom = SHARE_ZOOM.value, share_timer = SHARE_TIMER.value))
}

[live_command(description = "open a trace file: args \{path\}")]
def timeline_open(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLOpenArg>)
    if (!open_file(args.path)) {
        return JV("failed to open {args.path}")
    }
    TL_ROOT.has_initial_layout = false   // reseed the stacked layout with the new window
    return doc_entry(length(g_docs) - 1)
}

[live_command(description = "get a file visible time window: args \{file\}")]
def timeline_get_view(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLFileArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    return JV((t0 = g_ui[args.file].t0, t1 = g_ui[args.file].t1, span_us = g_docs[args.file].span))
}

[live_command(description = "set a file's visible window (drives sync like a user interaction): args \{file,t0,t1\}")]
def timeline_set_view(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLSetViewArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    var ui & = unsafe(g_ui[args.file])
    ui.t0 = args.t0
    ui.t1 = args.t1
    ui.inited = true
    clamp_view(ui, max(g_docs[args.file].span, 8.0lf))
    sync_views(args.file)
    var views <- [for (i in range(length(g_ui))); JV((index = i, t0 = g_ui[i].t0, t1 = g_ui[i].t1))]
    return JV((views <- views))
}

[live_command(description = "zoom a file to one unit: args \{file,kind,index\}")]
def timeline_focus_unit(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLUnitArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    let doc & = unsafe(g_docs[args.file])
    for (mk in doc.marker_kinds) {
        if (mk.name == args.kind) {
            if (args.index < 0 || args.index >= length(mk.ts)) {
                return JV("unit index {args.index} out of range 0..{length(mk.ts) - 1}")
            }
            let u0 = mk.ts[args.index]
            let u1 = args.index + 1 < length(mk.ts) ? mk.ts[args.index + 1] : doc.span
            var ui & = unsafe(g_ui[args.file])
            ui.t0 = u0
            ui.t1 = max(u1, u0 + 8.0lf)
            ui.inited = true
            clamp_view(ui, max(doc.span, 8.0lf))
            sync_views(args.file)
            return JV((t0 = ui.t0, t1 = ui.t1, arg = mk.args[args.index]))
        }
    }
    return JV("no marker kind '{args.kind}'")
}

[live_command(description = "computed stats for an arbitrary range, no UI change: args \{file,t0,t1\}")]
def timeline_range_stats(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLSetViewArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    var st <- compute_range_stats(g_docs[args.file], args.t0, args.t1)
    return JV(st)
}

[live_command(description = "the current selection + its computed stats: args \{file\}")]
def timeline_get_selection(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLFileArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    if (!g_ui[args.file].sel_active) {
        return JV((active = false))
    }
    // local compute — a tuple field would MOVE (zero) the cache's stats, not copy them
    var st <- compute_range_stats(g_docs[args.file], g_ui[args.file].sel_t0, g_ui[args.file].sel_t1)
    return JV((active = true, t0 = g_ui[args.file].sel_t0, t1 = g_ui[args.file].sel_t1, stats = st))
}

[live_command(description = "set the selection range (shown in the UI, stats computed): args \{file,t0,t1\}")]
def timeline_set_selection(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLSetViewArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    var ui & = unsafe(g_ui[args.file])
    ui.sel_active = true
    ui.sel_t0 = min(args.t0, args.t1)
    ui.sel_t1 = max(args.t0, args.t1)
    // local compute — a tuple field would MOVE (zero) the cache's stats, not copy them
    var st <- compute_range_stats(g_docs[args.file], ui.sel_t0, ui.sel_t1)
    return JV((active = true, t0 = ui.sel_t0, t1 = ui.sel_t1, stats = st))
}

struct TLLaunchArg {
    app : string
    args : string
    start : bool = true   // false = just pre-fill the launch panel
}

struct TLAutoArg {
    on : bool
}

[live_command(description = "launch a trace-producing app in the background: args \{app,args,start?\} (start=false pre-fills only)")]
def timeline_launch(input : JsonValue?) : JsonValue? {
    let a = from_JV(input, type<TLLaunchArg>)
    if (!empty(a.app)) {
        g_launch.app = a.app
    }
    g_launch.args = a.args
    g_launch.affinity = AFFINITY_CB.value
    g_launch.threads = THREADS_INPUT.value
    if (!a.start) {
        return JV((running = false, app = g_launch.app, args = g_launch.args))
    }
    if (!launch_start(g_launch)) {
        return JV("launch failed (already running, or no app set)")
    }
    return JV((running = true, app = g_launch.app, args = g_launch.args))
}

[live_command(description = "launch status: running flag, exit code, output tail"), unused_argument(input)]
def timeline_launch_status(input : JsonValue?) : JsonValue? {
    return JV((running = g_launch.running, has_run = g_launch.has_run,
        exit_code = g_launch.exit_code, tail := g_launch.tail))
}

[live_command(description = "re-parse an open trace file from disk: args \{file\}")]
def timeline_refresh(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TLFileArg>)
    if (args.file < 0 || args.file >= length(g_docs)) {
        return JV("no such file index {args.file}")
    }
    reload_file(args.file)
    watch_prime(g_watch[args.file], g_docs[args.file].path)
    return doc_entry(args.file)
}

[live_command(description = "toggle mtime-watch auto refresh: args \{on\}")]
def timeline_set_autorefresh(input : JsonValue?) : JsonValue? {
    let a = from_JV(input, type<TLAutoArg>)
    AUTO_REFRESH.value = a.on
    return JV((auto_refresh = AUTO_REFRESH.value))
}

[live_command(description = "what am I looking at: per-file view, visible events, busy-by-category per open file"), unused_argument(input)]
def timeline_describe(input : JsonValue?) : JsonValue? {
    var files : array<JsonValue?>
    files |> reserve(length(g_docs))
    for (i in range(length(g_docs))) {
        let doc & = unsafe(g_docs[i])
        let ui & = unsafe(g_ui[i])
        var visible = 0
        var cat_busy : array<double>
        cat_busy |> resize(length(doc.cats))
        for (lane in doc.lanes) {
            var j = lane_lower_bound(lane, ui.t0 - lane.max_dur)
            while (j < length(lane.ev)) {
                let e & = unsafe(lane.ev[j])
                if (e.t0 > ui.t1) {
                    break
                }
                j++
                let te = e.t0 + double(e.dur)
                if (te < ui.t0) {
                    continue
                }
                visible++
                let kind = ev_kind(e.meta)
                if (kind == EV_CHUNK || kind == EV_FIFO) {
                    cat_busy[ev_cat(e.meta)] += min(te, ui.t1) - max(e.t0, ui.t0)
                }
            }
        }
        var cats <- [for (c in range(length(doc.cats)));
            JV((name = doc.cats[c].name, busy_us = cat_busy[c]));
            where cat_busy[c] > 0.0lf]
        files |> push(JV((index = i, title = doc.title, view_t0 = ui.t0, view_t1 = ui.t1,
            visible_events = visible, busy_by_category <- cats)))
        delete cat_busy
    }
    return JV((files <- files, share_zoom = SHARE_ZOOM.value, share_timer = SHARE_TIMER.value))
}

[export]
def main() {
    for (arg in get_cli_arguments()) {
        if (ends_with(arg, ".json")) {
            g_pending |> push(arg)
        }
    }
    init()
    while (!exit_requested()) {
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
