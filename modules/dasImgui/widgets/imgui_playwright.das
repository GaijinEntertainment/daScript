options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_playwright public

require imgui/imgui_transport public
require dashv/dashv_boost public
require daslib/json public
require daslib/json_boost public
require daslib/fio public
require daslib/command_line
require daslib/clargs
require daslib/module_path
require daslib/strings_boost
require strings
require math

// Module overview lives in handmade/module-imgui_playwright.rst.

// ===== Tunables =====

let DEFAULT_LIVE_PORT : int = 9090                   // daslang-live's live_api default port
let DEFAULT_DEADLOCK_SANITY_SEC : float = 120.0f     // backstop for the frame-counter-never-advances case (crash / true deadlock); never trips on slow-but-progressing
let DEFAULT_READY_TIMEOUT_SEC : float = 30.0f        // /status must turn 200 OK within this — covers daslang-live boot + GLFW + ImGui ctx
let MACOS_READY_TIMEOUT_SEC : float = 90.0f          // free Apple-Silicon CI runners (3 vCPU, serial sweep) occasionally take ~40s before daslang-live serves /status — 30s is too tight there
let ATTACH_PROBE_SEC : float = 0.3f                  // with_recording_app's attach-vs-spawn detection — short enough to feel instant when nothing's listening
let DEFAULT_TEST_TIMEOUT_SEC : float = 120.0f        // upper bound on the entire popen lifetime, including body + drain
let DEFAULT_RELOAD_TIMEOUT_SEC : float = 30.0f       // POST /reload to /status.has_error == false convergence
let DEFAULT_FRAME_WAIT_SEC : float = 10.0f           // upper bound on "first frame to render the named widget"

let READY_POLL_INTERVAL_MS : uint = 100u             // ms between /status polls — wait_until_ready and reload convergence
let READY_POLL_REQUEST_TIMEOUT_SEC : int = 2         // per-poll http timeout for the readiness probe: fail-fast-and-retry so one slow round-trip on a loaded runner (the host serves /status off the main loop, so a stalled frame delays it) doesn't burn the whole budget on a single 10s DEFAULT_REQUEST_TIMEOUT_SEC block
let FRAME_POLL_INTERVAL_MS : uint = 100u             // ms between snapshot polls — 10 req/sec; under libhv's Windows
                                                     // event-loop cap (~16 req/sec) that previously stalled the test
                                                     // sweep on heavy imgui_demo harnesses (see tests.yml comment).
                                                     // ~1.5 polls/frame at 60fps — snappy; most calls finish in 1-2.

// ===== Centralized recording timing =====

// One knob set for every record_*.das. RECORD_FPS is the nominal capture+playback
// grid: the synchronous APNG writer never drops, and convert_recording reclocks the
// mp4 to the measured rate, so 30 keeps assets light without timeline drift.

let public RECORD_FPS : int = 30                     // capture + playback grid; synchronous writer never drops, so APNG frame N == N/30s
let public NARRATE_READ_MS : uint = 4500u            // narrative on-screen window — long enough to read with audio muted
let public SETTLE_MS : uint = 1200u                  // post-move cursor settle
let public CLICK_DWELL_MS : uint = 1500u             // post-click visual confirmation

struct ImguiApp {
    base_url : string
    feature_path : string
    transport : Transport
}

// ===== Driver-side mouse helpers (record_*.das) =====

// The host's ``advance_mouse_timeline`` interpolates linearly between adjacent move
// waypoints with distinct ``t_ms``, so each call must emit at least two move events
// spanning a real time window to get visible cursor travel + a mouse trail.

var private last_cursor_x : float = 0.0f
var private last_cursor_y : float = 0.0f

def public reset_cursor_pos(pos : tuple<float; float> = (0.0f, 0.0f)) {
    //! Reset the tracked previous cursor position. Call between recordings
    //! if the host's synth cursor state was already non-(0,0).
    last_cursor_x = pos._0
    last_cursor_y = pos._1
}

def public move_to(app : ImguiApp; pos : tuple<float; float>; duration_ms : int = 600) {
    //! Post a lerped cursor move from the last tracked position to ``pos`` (updates the tracked
    //! position). ``duration_ms`` is a FLOOR: actual duration scales with distance (≈0.325 px/ms,
    //! constant cursor speed) so a long move doesn't cover its span as fast as a short one and read as a teleport.
    let dist = max(abs(pos._0 - last_cursor_x), abs(pos._1 - last_cursor_y))
    let dur = max(duration_ms, int(dist / 0.325f))
    var events <- [
        JV((t_ms = 0,           kind = "move", x = last_cursor_x, y = last_cursor_y)),
        JV((t_ms = dur,         kind = "move", x = pos._0,        y = pos._1)),
        JV((t_ms = dur + 300,   kind = "move", x = pos._0,        y = pos._1))
    ]
    post_command(app, "imgui_mouse_play", JV((events = events)))
    unsafe { delete events; }
    last_cursor_x = pos._0
    last_cursor_y = pos._1
}

def public click_at(var events : array<JsonValue?>;
                    t_ms : int;
                    pos : tuple<float; float>;
                    travel_ms : int = 500;
                    button : int = 0) {
    //! Append lerp-from-last + press + release + dwell to ``events``: cursor travels from the
    //! tracked previous position to ``pos`` over ``travel_ms``, then button ``button`` is pressed
    //! and released. Updates the tracked position.
    events |> push(JV((t_ms = t_ms,                   kind = "move",   x = last_cursor_x, y = last_cursor_y)))
    events |> push(JV((t_ms = t_ms + travel_ms,       kind = "move",   x = pos._0,        y = pos._1)))
    events |> push(JV((t_ms = t_ms + travel_ms + 200, kind = "button", button = button,   action = "press")))
    events |> push(JV((t_ms = t_ms + travel_ms + 300, kind = "button", button = button,   action = "release")))
    events |> push(JV((t_ms = t_ms + travel_ms + 500, kind = "move",   x = pos._0,        y = pos._1)))
    last_cursor_x = pos._0
    last_cursor_y = pos._1
}

def public drag_along(var events : array<JsonValue?>;
                      t_ms : int;
                      from_pos : tuple<float; float>;
                      to_pos : tuple<float; float>;
                      drag_ms : int;
                      approach_ms : int = 400;
                      button : int = 0) {
    //! Append lerp-to-start + press + drag + release + dwell to ``events``: cursor travels from the
    //! tracked position to ``from_pos`` over ``approach_ms``, then presses and drags to ``to_pos``
    //! over ``drag_ms``. Updates the tracked position to ``to_pos``.
    events |> push(JV((t_ms = t_ms,                                kind = "move",   x = last_cursor_x, y = last_cursor_y)))
    events |> push(JV((t_ms = t_ms + approach_ms,                  kind = "move",   x = from_pos._0,   y = from_pos._1)))
    // Dwell at from_pos, then press 1ms later — timestamps STAY strictly increasing. The host sorts
    // by t_ms with no tie-break, so a press sharing the dwell's t_ms could sort first and slip off a
    // thin (8px) handle; the 1ms offset into a drag_ms segment is sub-pixel, so the press still lands.
    events |> push(JV((t_ms = t_ms + approach_ms + 100,            kind = "move",   x = from_pos._0,   y = from_pos._1)))
    events |> push(JV((t_ms = t_ms + approach_ms + 101,            kind = "button", button = button,   action = "press")))
    events |> push(JV((t_ms = t_ms + approach_ms + 101 + drag_ms,  kind = "move",   x = to_pos._0,     y = to_pos._1)))
    events |> push(JV((t_ms = t_ms + approach_ms + 200 + drag_ms,  kind = "button", button = button,   action = "release")))
    events |> push(JV((t_ms = t_ms + approach_ms + 300 + drag_ms,  kind = "move",   x = to_pos._0,     y = to_pos._1)))
    last_cursor_x = to_pos._0
    last_cursor_y = to_pos._1
}

def private daslang_live_exe() : string {
    let das = get_das_exe()
    if (empty(das)) return ""
    let dir = dir_name(das)
    let bin = get_platform_name() == "windows" ? "daslang-live.exe" : "daslang-live"
    return path_join(dir, bin)
}

def public dasimgui_module_root() : string {
    //! dasImgui's module root resolved via `daslib/module_path`. `get_this_module_dir()` returns
    //! `<root>/widgets/`; one `dir_name` up gives the module root. Works in both layouts: CI's
    //! `<das_root>/modules/dasImgui/...` and the local-fix `D:/DASPKG/dasImgui/...`.
    return dir_name(get_this_module_dir())
}

def private resolve_feature_path(path : string; feature_root : string; feature_prefix : string) : string {
    if (is_absolute(path)) return path
    // Caller-supplied root/prefix first — lets another package resolve its own feature files.
    if (feature_prefix != "" && path |> starts_with(feature_prefix)) return path_join(feature_root, slice(path, length(feature_prefix)))
    // Then dasImgui's own layout: strip `modules/dasImgui/` and resolve under the runtime module root.
    let prefix = "modules/dasImgui/"
    if (path |> starts_with(prefix)) return path_join(dasimgui_module_root(), slice(path, length(prefix)))
    return path_join(get_das_root(), path)
}

def public wait_until_ready(app : ImguiApp; timeout_sec : float) : bool {
    //! Poll GET /status until 200 OK or timeout. Returns true on ready.
    // `ref_time_ticks()` is platform-specific (QPC vs nanoseconds), so absolute-deadline math with
    // `* 1_000_000` is wrong on POSIX; `get_time_usec(start)` is the portable elapsed-usec helper.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        if (http_get("{app.base_url}/status", READY_POLL_REQUEST_TIMEOUT_SEC).status == int(http_status.OK)) return true
        sleep(READY_POLL_INTERVAL_MS)
    }
    return false
}

def private host_ready_timeout_sec() : float {
    // Bumped on macOS: free Apple-Silicon CI runners can take ~40s to serve /status, tripping the 30s default.
    return get_platform_name() == "darwin" ? MACOS_READY_TIMEOUT_SEC : DEFAULT_READY_TIMEOUT_SEC
}

def public get_text(app : ImguiApp; uri : string) : string {
    //! GET `uri`; return the raw response body or "" (bounded by the per-request timeout).
    return http_get("{app.base_url}{uri}").body
}

def private post_signal(app : ImguiApp; uri : string) {
    // Response is dropped (caller polls /status); the per-request timeout still bounds a wedged host.
    let _ = http_post("{app.base_url}{uri}", "")
}

// ===== Command dispatch =====

def public post_command(app : ImguiApp; name : string; var args : JsonValue?) : JsonValue? {
    //! Send ``{"name":<name>,"args":<args>}`` through ``app.transport``; returns parsed JSON or null on transport failure.
    //! Escape hatch for custom verbs outside the shipped helper set.
    return send_imgui_command(app.transport, name, args)
}

def public snapshot(app : ImguiApp) : JsonValue? {
    //! Fetch the full widget snapshot (``imgui_snapshot`` command) as a parsed ``JsonValue``.
    return send_imgui_command(app.transport, "imgui_snapshot", null)
}

// ===== Snapshot navigation =====

def public find_widget(var snap : JsonValue?; ident : string) : JsonValue? {
    //! Navigate to ``snapshot.globals[ident]``; may return a ``JV(null)`` placeholder for missing keys (daslib/json ``?[]`` quirk).
    //! For an existence check use ``widget_exists``.
    return snap?["globals"]?[ident]
}

def public widget_exists(var snap : JsonValue?; ident : string) : bool {
    //! True iff ``globals[ident]`` is a real widget entry — works around the daslib/json ``?[]`` quirk by checking the always-present ``kind`` field.
    let kind = snap?["globals"]?[ident]?["kind"] ?? ""
    return !empty(kind)
}

def public widget_payload_field(var snap : JsonValue?; ident : string; field : string) : JsonValue? {
    //! Navigate to ``snapshot.globals[ident].payload[field]``; returns null if any link is missing.
    return snap?["globals"]?[ident]?["payload"]?[field]
}

def public widget_click_point(var snap : JsonValue?; ident : string) : tuple<float; float> {
    //! The point ``imgui_click`` aims at for ``ident``: the left glyph for the toggle family
    //! (checkbox / radio_button / radio_button_int), the bbox center otherwise. Mirrors
    //! ``resolve_target_bbox`` in imgui_boost_runtime — keep the kind list in sync with that resolver.
    var g = find_widget(snap, ident)
    let x0 = g?["bbox"]?["x"] ?? 0.0f
    let y0 = g?["bbox"]?["y"] ?? 0.0f
    let x1 = g?["bbox"]?["z"] ?? 0.0f
    let y1 = g?["bbox"]?["w"] ?? 0.0f
    let cy = (y0 + y1) * 0.5f
    let kind = g?["kind"] ?? ""
    if (kind == "checkbox" || kind == "radio_button" || kind == "radio_button_int") return (x0 + (y1 - y0) * 0.5f, cy)
    return ((x0 + x1) * 0.5f, cy)
}

def public widget_component_point(var snap : JsonValue?; ident : string; component : int) : tuple<float; float> {
    //! Center of multi-component widget ``ident``'s sub-rect ``component`` (slider/drag _2/3/4,
    //! drag_*_range2), read from ``sub_bboxes`` telemetry — the deterministic way to target one handle
    //! of a vector/range widget. Falls back to ``widget_click_point`` when ``sub_bboxes`` is absent.
    let sub = find_widget(snap, ident)?["sub_bboxes"]?[component]
    if (sub == null) return widget_click_point(snap, ident)
    let x0 = sub?["x"] ?? 0.0f
    let y0 = sub?["y"] ?? 0.0f
    let x1 = sub?["z"] ?? 0.0f
    let y1 = sub?["w"] ?? 0.0f
    return ((x0 + x1) * 0.5f, (y0 + y1) * 0.5f)
}

def public widget_rendered(var snap : JsonValue?; ident : string) : bool {
    //! True iff ``globals[ident]`` EXISTS and painted this frame. Unpainted entries carry
    //! ``"rendered": false``; painted widgets omit it (``?? true``). The leading existence check is
    //! load-bearing: an absent/misspelled path yields ``null ?? true`` — a false pass — so it must report not-rendered.
    return widget_exists(snap, ident) && (snap?["globals"]?[ident]?["rendered"] ?? true)
}

def public log_snapshot(app : ImguiApp; note : string = "") : void {
    //! Dump the live UI state to the log (LOG_WARNING) — per registered widget: ident, kind,
    //! ``rendered`` flag, bbox, and ``payload.value``. The way to see what a headless/CI run actually
    //! painted; pair it with a failed assertion so a red run is self-explaining.
    var snap = snapshot(app)
    let g = snap?["globals"]
    if (g == null || !(g is _object)) {
        to_log(LOG_WARNING, "log_snapshot [{note}]: snapshot has no widget registry")
        return
    }
    let obj & = unsafe(g as _object)
    to_log(LOG_WARNING, "log_snapshot [{note}]: {length(obj)} widgets")
    for (k, v in keys(obj), values(obj)) {
        let kind = v?["kind"] ?? "?"
        let rend = v?["rendered"] ?? true
        let bb = v?["bbox"]
        let x = bb?["x"] ?? 0.0f
        let y = bb?["y"] ?? 0.0f
        let z = bb?["z"] ?? 0.0f
        let wy = bb?["w"] ?? 0.0f
        let val = v?["payload"]?["value"]
        let vs = val == null ? "" : " value={val}"
        to_log(LOG_WARNING, "  {k}  kind={kind} rendered={rend} bbox=({x},{y})-({z},{wy}){vs}")
    }
}

// ===== Auto-dump on timeout =====

var private g_auto_dump_on_timeout = true

def public set_auto_dump_on_timeout(enabled : bool) : void {
    //! Toggle the wait-family auto-dump (default **on**): any ``wait_*`` that gives up logs the UI
    //! state once via ``log_snapshot``, so a red run is self-explaining. Turn it **off** around a
    //! *negative* wait (the timeout is the expected outcome) so the log isn't spammed with an expected miss.
    g_auto_dump_on_timeout = enabled
}

def public auto_dump_on_timeout_enabled() : bool => g_auto_dump_on_timeout

def private dump_on_timeout(app : ImguiApp; what : string) : void {
    // One UI dump on a wait-family timeout: `what` labels which wait gave up. Gated by the public toggle.
    if (g_auto_dump_on_timeout) {
        log_snapshot(app, "wait timeout: {what}")
    }
}

// ===== Poll-until family =====

def public wait_until_sec(app : ImguiApp;
                          timeout_sec : float;
                          pred : block<(var snap : JsonValue?) : bool>) : JsonValue? {
    //! Wall-clock variant of ``wait_until``: poll ``pred(snap)`` until true, up to ``timeout_sec``
    //! real seconds; returns the matching snapshot or null on timeout. Prefer over ``wait_until`` for
    //! time-driven server work — the frame-budget form fires prematurely in headless (multi-kHz, no vsync).
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        var snap = snapshot(app)
        if (snap != null && invoke(pred, snap)) return snap
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    dump_on_timeout(app, "wait_until_sec ({timeout_sec}s)")
    return null
}

def public wait_cond(app : ImguiApp;
                     timeout_sec : float;
                     pred : block<() : bool>) : bool {
    //! Bare wall-clock condition poll — no snapshot, no widget coupling. Polls ``pred()`` until true,
    //! up to ``timeout_sec`` real seconds; returns true on match, false on timeout. Use for state gated
    //! on a command round-trip; the frame-count busy-poll it replaces expires prematurely on the multi-kHz headless server.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        if (invoke(pred)) return true
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    dump_on_timeout(app, "wait_cond ({timeout_sec}s)")
    return false
}

def public wait_until(app : ImguiApp;
                      max_frames : int;
                      pred : block<(var snap : JsonValue?) : bool>) : JsonValue? {
    //! Poll ``pred(snap)`` up to ``max_frames`` 60fps-equivalent server frames (``max_frames/60`` s);
    //! returns the matching snapshot or null on timeout. ``max_frames`` is translated to wall-clock at
    //! 60fps (not a raw frame-counter delta, which expires in ~ms on the multi-kHz headless server); prefer ``wait_until_sec`` for new code.
    return wait_until_sec(app, float(max_frames) / 60.0f, pred)
}

def public wait_for_widget(app : ImguiApp; widget_ident : string;
                           timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : JsonValue? {
    //! Poll snapshot until ``globals[widget_ident]`` is present (registered at module init OR rendered at least once).
    //! Use for subprocess-startup gating; for first-paint semantics use ``wait_for_render``.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        var snap = snapshot(app)
        if (snap != null && widget_exists(snap, widget_ident)) return snap
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    dump_on_timeout(app, "wait_for_widget {widget_ident}")
    return null
}

def public wait_for_visible(app : ImguiApp; widget_ident : string;
                            timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : JsonValue? {
    //! Poll snapshot until ``widget_ident`` is registered AND painted this frame (``widget_rendered``).
    //! The robust gate for a widget that can hide and re-show: ``wait_for_widget`` returns on a stale
    //! registry entry (re-opened popup/menu) while it's off-screen, so a click fired right after can be silently dropped.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        var snap = snapshot(app)
        if (snap != null && widget_rendered(snap, widget_ident)) return snap
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    dump_on_timeout(app, "wait_for_visible {widget_ident}")
    return null
}

def public wait_for_render(app : ImguiApp; widget_ident : string;
                           timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : JsonValue? {
    //! First-paint gate — alias of :func:`wait_for_visible`. ``widget_rendered`` now requires
    //! existence, so this no longer false-passes for an absent path. Prefer ``wait_for_visible``
    //! at call sites that target a widget which can hide and re-show.
    return wait_for_visible(app, widget_ident, timeout_sec)
}

def public wait_for_hover(app : ImguiApp; widget_ident : string; expected : bool = true;
                          timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : JsonValue? {
    //! Poll until widget ``widget_ident``'s ``hover`` (``IsItemHovered``) equals ``expected``. Sync gate for
    //! synthetic clicks: without it a batched ``move``+``press`` fires before ``HoveredId`` is set (the press-before-hover
    //! race real mice never hit). The ``widget_exists`` guard stops an absent ident false-passing ``expected=false``.
    return wait_until_sec(app, timeout_sec) $(var snap) {
        return widget_exists(snap, widget_ident) && (find_widget(snap, widget_ident)?["hover"] ?? false) == expected
    }
}

def public wait_for_int_value(app : ImguiApp;
                              target : string;
                              field : string;
                              expected : int;
                              max_frames : int = 300) : bool {
    //! Poll snapshot until `globals[target].payload[field] == expected` (typed int).
    //! Returns true on match, false after `max_frames` frames elapse.
    return wait_until(app, max_frames) $(var snap) {
        return (widget_payload_field(snap, target, field) ?? (expected ^ 1)) == expected
    } != null
}

def public wait_for_bool_value(app : ImguiApp;
                               target : string;
                               field : string;
                               expected : bool;
                               max_frames : int = 300) : bool {
    //! Poll snapshot until `globals[target].payload[field] == expected` (typed bool).
    return wait_until(app, max_frames) $(var snap) {
        return (widget_payload_field(snap, target, field) ?? !expected) == expected
    } != null
}

def public wait_for_string_value(app : ImguiApp;
                                 target : string;
                                 field : string;
                                 expected : string;
                                 max_frames : int = 300) : bool {
    //! Poll snapshot until `globals[target].payload[field] == expected` (typed string).
    return wait_until(app, max_frames) $(var snap) {
        return (widget_payload_field(snap, target, field) ?? "") == expected
    } != null
}

def public wait_for_payload_value(app : ImguiApp;
                                  target : string;
                                  field : string;
                                  expected : auto(T);
                                  max_frames : int = 300) : bool {
    //! Generic poll: snapshot until ``globals[target].payload[field] == expected``; ``T`` is inferred from ``expected``.
    //! Use for any roundtrippable type (float2/3/4, int2/3/4, etc.) where the typed sugars above don't apply.
    return wait_until(app, max_frames) $(var snap) {
        let v = widget_payload_field(snap, target, field)
        return v != null && from_JV(v, type<typedecl(expected)>, default<typedecl(expected)>) == expected  // nolint:STYLE020 — template handles vec types where `??` isn't defined
    } != null
}

// ===== Assertions (panic on timeout) =====

def public expect_value(app : ImguiApp;
                        target : string;
                        field : string;
                        expected : auto(T);
                        max_frames : int = 300) : void {
    //! Assertion form of ``wait_for_payload_value``; panics with a focused want/got/kind/rendered message on timeout.
    if (wait_for_payload_value(app, target, field, expected, max_frames)) return
    panic(format_value_mismatch(app, target, field, expected, max_frames))
}

def public expect_render(app : ImguiApp;
                         widget_ident : string;
                         timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : void {
    //! Assertion form of ``wait_for_render``; panics on timeout with the list of currently-registered widget identifiers (so typos in ``widget_ident`` surface).
    if (wait_for_render(app, widget_ident, timeout_sec) != null) return
    var snap = snapshot(app)
    var keys_buf = "<none>"
    let g = snap?["globals"]
    if (g != null && g is _object) {
        let obj & = unsafe(g as _object)
        let key_list <- [for (k in keys(obj)); k]
        keys_buf = "[" + join(key_list, ", ") + "]"
    }
    panic("expect_render({widget_ident}) timeout after {timeout_sec}s — widget never reported rendered:true.\nRegistered widgets: {keys_buf}")
}

def public await_quiescent(app : ImguiApp;
                           timeout_sec : float = 5.0f) : void {
    //! Assertion wrapper over ``await_imgui(transport, "quiescent", timeout)``; panics on timeout.
    //! For a non-asserting poll, call ``await_imgui`` directly.
    if (await_imgui(app.transport, "quiescent", timeout_sec)) return
    panic("await_quiescent timeout after {timeout_sec}s — server never reported quiescent")
}

// ===== Command verbs =====

def public click(app : ImguiApp; target : string) : JsonValue? {
    //! Synthesize a left-click on ``target`` (``imgui_click`` verb).
    return send_imgui_command(app.transport, "imgui_click", JV((target = target)))
}

def public double_click(app : ImguiApp; target : string) : JsonValue? {
    //! Faithful double-click on ``target``'s bbox center: two press/release taps so
    //! ``IsMouseDoubleClicked`` fires. Frame-paced (``t_ms`` = frame index): the host advances one event
    //! per frame, so the two taps land on distinct frames — never collapsed into one the way a wall-clock timeline is on a slow host (which drops the double-click).
    var snap = snapshot(app)
    if (!(snap |> widget_exists(target))) return JV((ok = false, error = "double_click target unresolved: {target}"))
    if (!(snap |> widget_rendered(target))) return JV((ok = false, error = "double_click target not rendered this frame: {target}"))
    let pos = widget_click_point(snap, target)
    var events <- [
        JV((t_ms = 0, kind = "move",   x = last_cursor_x, y = last_cursor_y)),
        JV((t_ms = 1, kind = "move",   x = pos._0,        y = pos._1)),
        JV((t_ms = 2, kind = "button", button = 0,        action = "press")),
        JV((t_ms = 3, kind = "button", button = 0,        action = "release")),
        JV((t_ms = 4, kind = "button", button = 0,        action = "press")),
        JV((t_ms = 5, kind = "button", button = 0,        action = "release")),
        JV((t_ms = 6, kind = "move",   x = pos._0,        y = pos._1))
    ]
    var r = post_command(app, "imgui_mouse_play", JV((events = events, frame_paced = true)))
    unsafe { delete events; }
    last_cursor_x = pos._0
    last_cursor_y = pos._1
    // SYNC: let the two-tap timeline fully play (both releases must fire) before returning — a following
    // command would otherwise drive the synth mouse while this timeline is in flight, stomping it (the
    // double-click is lost, next click lands in a corrupted mouse state). Mirrors every other mouse_play poster.
    wait_for_mouse_idle(app)
    return r
}

def public right_click(app : ImguiApp; target : string) : JsonValue? {
    //! Synthesize a right-click on ``target`` (``imgui_click`` verb with ``button = 1``).
    //! Used by popup_context_item / popup_context_window drivers.
    return send_imgui_command(app.transport, "imgui_click", JV((target = target, button = 1)))
}

def public force_set_value(app : ImguiApp; target : string; var value : JsonValue?) : JsonValue? {
    //! Set ``target``'s payload value remotely (``imgui_force_set`` verb); ``value`` is the JSON form expected by the widget kind.
    return send_imgui_command(app.transport, "imgui_force_set", JV((target = target, value = value)))
}

def public force_set_verified(app : ImguiApp; target : string; value : auto(T); max_frames : int = 120) {
    //! Recording-as-test ``force_set``: set ``target.value`` to the typed ``value`` and verify it took,
    //! accumulating a miss (surfaced at ``with_recording_app`` teardown) rather than panicking mid-body.
    //! Use only inside a ``with_recording_app`` body — raw tests want the panicking ``expect_value``.
    force_set_value(app, target, JV(value))
    record_check_value(app, target, "value", value, max_frames)
}

def public resize_window(app : ImguiApp; target : string; w : float; h : float) : JsonValue? {
    //! Resize the window named by ``target`` to (``w``, ``h``) on the next frame, via
    //! ``imgui_set_window_size`` (feeds ``state.pending_size`` → ``SetNextWindowSize``). Named
    //! ``resize_window`` (not ``set_window_size``) to avoid the overload collision with imgui_containers_builtin's in-frame form.
    return send_imgui_command(app.transport, "imgui_set_window_size",
                              JV((target = target, value = JV((w = w, h = h)))))
}

def public open_widget(app : ImguiApp; target : string) : JsonValue? {
    //! Flip ``target.state.open = true`` (``imgui_open`` verb) — reopens a window/tree node from the test driver.
    return send_imgui_command(app.transport, "imgui_open", JV((target = target)))
}

def public close_widget(app : ImguiApp; target : string) : JsonValue? {
    //! Flip ``target.state.open = false`` (``imgui_close`` verb) — closes a window/tree node from the test driver.
    return send_imgui_command(app.transport, "imgui_close", JV((target = target)))
}

def public select_widget(app : ImguiApp; target : string) : JsonValue? {
    //! Activate ``target`` tab_item (``imgui_select`` verb) — the automation counterpart of clicking a tab
    //! (ImGui owns active-tab selection, so an inactive tab can't be reached by ``click``).
    return send_imgui_command(app.transport, "imgui_select", JV((target = target)))
}

def public drag(app : ImguiApp; target : string;
                dx : float; dy : float;
                steps : int = 4; button : int = 0) : JsonValue? {
    //! Synthesize a mouse-drag on ``target``'s bbox center: warp → press → drag to ``center+(dx,dy)``
    //! → release, as an ``imgui_mouse_play`` timeline (bbox resolved client-side). Returns ``{ok, value}``
    //! (``ok=false`` if the target is absent/unrendered — else it would drag from a defaulted (0,0) bbox). ``steps`` scales duration.
    var snap = snapshot(app)
    if (!(snap |> widget_exists(target))) return JV((ok = false, error = "drag target unresolved: {target}"))
    if (!(snap |> widget_rendered(target))) return JV((ok = false, error = "drag target not rendered this frame: {target}"))
    let bb = snap?["globals"]?[target]?["bbox"]
    let cx = ((bb?["x"] ?? 0.0f) + (bb?["z"] ?? 0.0f)) * 0.5f
    let cy = ((bb?["y"] ?? 0.0f) + (bb?["w"] ?? 0.0f)) * 0.5f
    let ex = cx + dx
    let ey = cy + dy
    // Frame-paced drag: t_ms is a FRAME index, so the host advances one cursor delta per frame and the
    // press never collapses into the sweep frame — a wall-clock drag does that on a slow host (press + full
    // move together = a zero-delta gesture). A 2-frame dwell pins (cx,cy) on the thin (8px) handle before the press.
    let dframes = steps > 0 ? steps : 8
    var events <- [
        JV((t_ms = 0,           kind = "move",   x = cx, y = cy)),
        JV((t_ms = 1,           kind = "move",   x = cx, y = cy)),
        JV((t_ms = 2,           kind = "button", button = button, action = "press")),
        JV((t_ms = 2 + dframes, kind = "move",   x = ex, y = ey)),
        JV((t_ms = 3 + dframes, kind = "button", button = button, action = "release")),
        JV((t_ms = 4 + dframes, kind = "move",   x = ex, y = ey))
    ]
    let resp = post_command(app, "imgui_mouse_play", JV((events = events, frame_paced = true)))
    unsafe { delete events; }
    if (resp == null) return JV((ok = false, error = "drag post failed: {target}"))
    return JV((ok = true, value = target))
}

def public drag_to(app : ImguiApp; source : string; target : string;
                   steps : int = 6; button : int = 0) : JsonValue? {
    //! Drag from ``source`` widget center to ``target`` widget center. Reads
    //! both bboxes from a fresh snapshot, computes the delta, and dispatches
    //! the L1 drag coroutine (press at source → interpolated moves → release).
    var snap = snapshot(app)
    let sb = snap?["globals"]?[source]?["bbox"]
    let tb = snap?["globals"]?[target]?["bbox"]
    let sx = ((sb?["x"] ?? 0.0f) + (sb?["z"] ?? 0.0f)) * 0.5f
    let sy = ((sb?["y"] ?? 0.0f) + (sb?["w"] ?? 0.0f)) * 0.5f
    let tx = ((tb?["x"] ?? 0.0f) + (tb?["z"] ?? 0.0f)) * 0.5f
    let ty = ((tb?["y"] ?? 0.0f) + (tb?["w"] ?? 0.0f)) * 0.5f
    return drag(app, source, tx - sx, ty - sy, steps, button)
}

def public focus(app : ImguiApp; target : string) : JsonValue? {
    //! Force keyboard focus to the next render of ``target`` (``imgui_focus`` verb); display-only widgets reply ``Result.err("not focusable")``.
    return send_imgui_command(app.transport, "imgui_focus", JV((target = target)))
}

def public key_tap(app : ImguiApp; key : int; hold_frames : int = 2) {
    //! Synthesize a discrete press+release of an ImGuiKey by integer code, FRAME-PACED: held for
    //! ``hold_frames`` frames then released via ``imgui_key_play``, so it fires exactly once regardless
    //! of frame rate (a wall-clock hold crosses ImGui's key-repeat threshold on a slow frame and over-fires). Use ``int(ImGuiKey.Tab)`` etc.
    post_command(app, "imgui_key_play", JV((frame_paced = true, events = [
        JV((t_ms = 0,           kind = "press",   key = key)),
        JV((t_ms = hold_frames, kind = "release", key = key))
    ])))
}

// ===== Voiceover sync (record-time, opt-in via a prepared manifest) =====

// prepare_recording.das pre-renders each say() line to a wav + a voiceover/<stem>.manifest.json;
// with_recording_app auto-arms when it exists, and say() paces each line by its wav duration.
// No manifest -> unchanged behavior.

struct public VoEntry {
    hash  : string      //!< say_hash(spoken) — links a narration line to its wav
    text  : string      //!< on-screen caption (the say() text arg)
    voice : string      //!< spoken line fed to TTS (say() voice arg, or text if none)
    wav   : string      //!< wav basename under the voiceover/ dir
    dur_s : float       //!< measured wav duration, seconds
}

struct public VoManifest {
    voice   : string
    gap_ms  : int = 500
    entries : array<VoEntry>
}

struct public VoSidecarHit {
    frame : int         //!< recorded APNG frame index at the caption's appearance — the sync anchor
    t_s   : float       //!< wall-clock seconds from record_start (reference/debug; convert uses frame)
    wav   : string      //!< wav basename to mux
    text  : string      //!< narration line (for reference)
}

struct public VoSidecar {
    apng       : string
    fps        : int
    frames     : int
    duration_s : float
    voiceovers : array<VoSidecarHit>
}

def public say_hash(text : string) : string {
    //! Stable wav key shared by prepare_recording.das and say(); hash() is deterministic.
    return "{hash(text)}"
}

var private g_vo_active = false
var private g_vo_gap_ms = 500u
var private g_vo_stem : string                  // apng stem, for reconstructing wav names
var private g_vo_dur : table<string; float>     // hash -> wav duration (seconds)
var private g_vo_collect : array<VoSidecarHit>  // accumulated while a voiceover recording runs
var private g_vo_extra_s = 0.0f                 // total added dwell (sum of wav_dur + gap) — extends the recorder cap
var private g_record_failures : array<string>   // self-verification misses during a recording; surfaced (panic) at with_recording_app teardown

def private format_value_mismatch(app : ImguiApp; target : string; field : string; expected : auto(T); max_frames : int) : string {
    //! The want/got/kind/rendered diagnostic shared by ``expect_value`` (panics with it) and the
    //! recording sequencer's ``record_check_value`` (accumulates it).
    var snap = snapshot(app)
    let got_jv = widget_payload_field(snap, target, field)
    let got_str = got_jv != null ? write_json(got_jv) : "<missing>"
    let kind = snap?["globals"]?[target]?["kind"] ?? "<unknown>"
    let exists = widget_exists(snap, target)
    let rendered = exists && widget_rendered(snap, target)
    let want_str = write_json(JV(expected))
    return "expect_value({target}.{field}) gave up after {max_frames} frames:\n  want: {want_str}\n  got:  {got_str}\n  widget kind: {kind}\n  widget rendered: {rendered}"
}

def public record_check_value(app : ImguiApp; target : string; field : string; expected : auto(T); max_frames : int = 120) : bool {
    //! Recording-safe verifier: assert ``target.payload[field] == expected`` within ``max_frames``,
    //! accumulating a miss instead of panicking (a mid-body panic would skip record_stop + /shutdown and
    //! orphan the host). The public rail for self-verifying a NON-CLICK effect; returns true on match.
    if (wait_for_payload_value(app, target, field, expected, max_frames)) return true
    g_record_failures |> push(format_value_mismatch(app, target, field, expected, max_frames))
    return false
}

def public record_check_changed(app : ImguiApp; target : string; field : string;
                                before : string; max_frames : int = 120) : bool {
    //! Recording-safe "the gesture moved it" verifier: assert ``target.payload[field]``'s JSON now
    //! DIFFERS from ``before`` within ``max_frames`` — the honest self-test for a continuous drag (a human
    //! drag lands on no exact pixel, so assert CHANGED not equals). Accumulate-on-miss; returns true once it differs.
    let moved = wait_until(app, max_frames) $(var snap) {
        let v = widget_payload_field(snap, target, field)
        return v != null && write_json(v) != before
    } != null
    if (moved) return true
    var snap = snapshot(app)
    let got = widget_payload_field(snap, target, field)
    let got_str = got != null ? write_json(got) : "<missing>"
    g_record_failures |> push("record_check_changed({target}.{field}) gave up after {max_frames} frames:\n  the gesture never moved the value off {before} (still {got_str})")
    return false
}

def public record_check_unchanged(app : ImguiApp; target : string; field : string; before : string) : bool {
    //! Recording-safe "the gesture left it alone" verifier — mirror of ``record_check_changed``: assert
    //! ``target.payload[field]`` still EQUALS ``before``, the honest self-test for a NEGATIVE claim. No
    //! event to wait for, so the caller must settle first (``wait_for_mouse_idle``); compares once. Accumulate-on-miss.
    var snap = snapshot(app)
    let v = widget_payload_field(snap, target, field)
    let now = v != null ? write_json(v) : ""
    if (now == before) return true
    // Compare on the raw "" sentinel, but render a missing field as <missing> in the
    // diagnostic (matching record_check_changed), so an empty value reads unambiguously.
    let now_disp = empty(now) ? "<missing>" : now
    let before_disp = empty(before) ? "<missing>" : before
    g_record_failures |> push("record_check_unchanged({target}.{field}):\n  expected it to stay {before_disp}\n  but the gesture moved it to {now_disp}")
    return false
}

def public record_check_rendered(app : ImguiApp; target : string; expected : bool; max_frames : int = 120) : bool {
    //! Recording-safe "did the gesture show/hide it" verifier: assert ``target``'s rendered-this-frame
    //! state (``widget_rendered``) equals ``expected`` — the robust open/close signal for a container
    //! (collapsing a tree flips its child to ``rendered:false``), readable even when ``opened``/``flags`` are absent. Accumulate-on-miss.
    let ok = wait_until(app, max_frames) $(var snap) {
        return widget_rendered(snap, target) == expected
    } != null
    if (ok) return true
    var snap = snapshot(app)
    let now = widget_rendered(snap, target)
    g_record_failures |> push("record_check_rendered({target}) gave up after {max_frames} frames:\n  want rendered={expected}, got rendered={now}")
    return false
}

def private count_registry_kind(snap : JsonValue?; kind_prefix : string) : int {
    let globals = snap?["globals"]
    if (globals == null || !(globals.value is _object)) return 0
    let plen = length(kind_prefix)
    var n = 0
    for (v in values(globals.value as _object)) {
        let kind = v?["kind"] ?? ""
        if (length(kind) >= plen && slice(kind, 0, plen) == kind_prefix) {
            n++
        }
    }
    return n
}

def public record_check_kind_count(app : ImguiApp; kind_prefix : string; min_count : int; max_frames : int = 120) : bool {
    //! Recording-safe "did the rail paint" verifier for entries under SYNTHESIZED path keys (drawlist
    //! primitives, kind ``"add_*"``): assert the registry holds ≥ ``min_count`` entries whose ``kind``
    //! starts with ``kind_prefix`` — the only honest signal when the call sites have no stable ident. Accumulate-on-miss.
    let ok = wait_until(app, max_frames) $(var snap) {
        return count_registry_kind(snap, kind_prefix) >= min_count
    } != null
    if (ok) return true
    var snap = snapshot(app)
    let got = count_registry_kind(snap, kind_prefix)
    g_record_failures |> push("record_check_kind_count(kind {kind_prefix}*) gave up after {max_frames} frames:\n  want >= {min_count} entries, got {got}")
    return false
}

[unused_argument(app)]
def public record_check(app : ImguiApp; desc : string; ok : bool) : bool {
    //! Recording-safe verifier for a condition the CALLER already evaluated — the escape hatch for an
    //! effect the field-based record_check_* can't reach (a value nested in a payload ARRAY, a cross-widget
    //! invariant). Pass the bool you computed; on false it accumulates ``desc`` into the failure ledger. Returns ``ok``.
    if (ok) return true
    g_record_failures |> push("record_check failed: {desc}")
    return false
}

def private arm_voiceovers(apng_path : string) : bool {
    // Load voiceover/<stem>.manifest.json beside apng_path; fill g_vo_dur, set g_vo_active. True when armed.
    g_vo_active = false
    g_vo_stem = stem(apng_path)
    g_vo_dur |> clear()
    g_vo_collect |> clear()
    let manifest_path = path_join(path_join(dir_name(apng_path), "voiceover"), "{g_vo_stem}.manifest.json")
    let txt = fread(manifest_path)
    if (empty(txt)) return false
    var err : string
    let jv = read_json(txt, err)
    if (jv == null) {
        print("[voiceover] bad manifest {manifest_path}: {err}\n")
        return false
    }
    let m = from_JV(jv, type<VoManifest>)
    g_vo_gap_ms = uint(m.gap_ms > 0 ? m.gap_ms : 500)
    g_vo_extra_s = 0.0f
    let gap_s = float(g_vo_gap_ms) / 1000.0f
    for (e in m.entries) {
        g_vo_dur[e.hash] = e.dur_s
        g_vo_extra_s += e.dur_s + gap_s
    }
    g_vo_active = !empty(g_vo_dur)
    if (g_vo_active) {
        print("[voiceover] armed: {length(m.entries)} lines, gap {int(g_vo_gap_ms)}ms, voice {m.voice}\n")
    }
    return g_vo_active
}

def private flush_voiceovers(stopped : JsonValue?; apng_path : string; fps : int) {
    //! After record_stop, write voiceover/<stem>.sidecar.json (clip length + per-line offsets).
    if (!g_vo_active) return
    g_vo_active = false
    let sidecar_path = path_join(path_join(dir_name(apng_path), "voiceover"), "{stem(apng_path)}.sidecar.json")
    let sc = VoSidecar(apng = base_name(apng_path), fps = fps,
        frames = stopped?["frames"] ?? 0,
        duration_s = stopped?["duration_s"] ?? 0.0f,
        voiceovers <- g_vo_collect)
    if (!fwrite(sidecar_path, write_json(JV(sc)))) {
        print("[voiceover] FAILED to write {sidecar_path}\n")
        return
    }
    print("[voiceover] wrote {sidecar_path} ({length(sc.voiceovers)} lines, {sc.duration_s}s)\n")
}

// ===== Recording cadence helper =====

def private say_begin_impl(app : ImguiApp; text : string; targets : array<string>;
                           read_ms : uint; voice : string) : uint {
    // Shared say_begin body. In voiceover mode the dwell is the wav length (+gap), held by wall-clock (render fps diverges from the capture grid, so a tick budget would drain early); else read_ms.
    let spoken = voice != "" ? voice : text
    if (g_vo_active) {
        let h = say_hash(spoken)
        if (key_exists(g_vo_dur, h)) {
            let dwell_s = g_vo_dur[h] + float(g_vo_gap_ms) / 1000.0f
            post_command(app, "imgui_narrate", JV((text = text, targets = JV(targets), duration_s = dwell_s)))
            let st = post_command(app, "record_status", null)
            g_vo_collect |> push(VoSidecarHit(frame = st?["frames"] ?? 0,
                t_s = st?["elapsed_s"] ?? 0.0f,
                wav = "{g_vo_stem}_{h}.wav", text = text))
            return uint(dwell_s * 1000.0f)
        }
    }
    post_command(app, "imgui_narrate", JV((text = text, targets = JV(targets),
        duration_s = float(read_ms) / 1000.0f)))
    return read_ms
}

def public say_begin(app : ImguiApp; text : string; target : string = ""; read_ms : uint = NARRATE_READ_MS; voice : string = "") : uint {
    //! Non-blocking ``say``: post the narration overlay (+ record the voiceover anchor) and RETURN the
    //! dwell in ms WITHOUT holding — the caller splits it around a ``click``/``drag`` so the action lands
    //! under the voice. ``text`` is the caption; ``voice`` (when set) is the spoken line; use ``say`` to post+hold in one call.
    return say_begin_impl(app, text, [target], read_ms, voice)
}

def public say_begin(app : ImguiApp; text : string; targets : array<string>; read_ms : uint = NARRATE_READ_MS; voice : string = "") : uint {
    //! Multi-target ``say_begin``: the caption points at ``targets[0]`` and HARD-avoids the union of all
    //! ``targets`` bboxes — pass the same list ``hold_through_voice`` will click so the box never covers a
    //! sibling control. Otherwise identical to the single-target form.
    return say_begin_impl(app, text, targets, read_ms, voice)
}

def public say(app : ImguiApp; text : string; target : string = ""; read_ms : uint = NARRATE_READ_MS; voice : string = "") {
    //! Pop a narration overlay (``imgui_narrate`` verb), then hold the dwell so the recorder captures the
    //! window. Named ``say`` (not ``narrate``) to avoid overload churn with imgui_visual_aids::narrate. For
    //! a click/drag that must land under the voice, use ``say_begin`` + a manual dwell split.
    hold_content(app, say_begin(app, text, target, read_ms, voice))
}

def public say(app : ImguiApp; text : string; targets : array<string>; read_ms : uint = NARRATE_READ_MS; voice : string = "") {
    //! Multi-target ``say``: caption points at ``targets[0]`` and HARD-avoids the union of all
    //! ``targets`` (see the ``say_begin`` list overload), then holds the dwell.
    hold_content(app, say_begin(app, text, targets, read_ms, voice))
}

// ===== Lockstep content-time gating =====

// During a recording session the host clock runs in fixed-dt lockstep (1/fps per captured frame),
// so a beat must be held by CONTENT time (captured-frame count), not sleep(ms) — the app may render
// faster or slower than realtime, but every captured frame is exactly 1/fps of video.

var private g_record_fps : int = 0   // > 0 while a with_recording_app session is active
var private g_stall_warned = false   // one-shot per session: capture-stall -> wall-clock degrade

let private RECORD_POLL_MS = 4u
let private RECORD_HOLD_TIMEOUT_US = 120 * 1000000   // 120s wall-clock safety cap
// Frames idle this long => capture isn't advancing (app not rendering during an input-less hold, e.g.
// an OS-throttled unfocused window on headless macOS CI). Degrade to wall-clock so CI never hangs.
// See https://github.com/borisbat/dasImgui/issues/190.
let private RECORD_STALL_US = 2 * 1000000

def private warn_capture_stalled() {
    return if (g_stall_warned)
    g_stall_warned = true
    print("[record] frame capture stalled (app not rendering during a hold) — content-time " +
          "gating degraded to wall-clock for this session. See dasImgui issue #190.\n")
}

def private record_frames_now(app : ImguiApp) : int {
    // Captured-frame counter from a VALID active status; retries a transient null (a 0 would corrupt a hold anchor). 0 only if never valid.
    let start = ref_time_ticks()
    while (get_time_usec(start) < RECORD_STALL_US) {
        let st = post_command(app, "record_status", null)
        if (st != null && (st?["active"] ?? false)) return st?["frames"] ?? 0
        sleep(RECORD_POLL_MS)
    }
    return 0
}

def public wait_content_frames(app : ImguiApp; frames : int) {
    //! Block until the recorder captures `frames` more frames (= frames/fps s of video); no-op outside a
    //! recording session. Anchored on the first VALID status read (a null keeps polling, never ends early).
    //! If capture stalls for RECORD_STALL_US, degrade to a wall-clock sleep so the beat lands and CI never hangs.
    if (g_record_fps <= 0 || frames <= 0) return
    let start = ref_time_ticks()
    var target = -1                        // resolved from the first valid status read
    var last_frames = -1
    var last_progress = ref_time_ticks()   // reset whenever the frame counter advances
    while (get_time_usec(start) < RECORD_HOLD_TIMEOUT_US) {
        let st = post_command(app, "record_status", null)
        let active = st != null && (st?["active"] ?? false)
        let now_frames = active ? (st?["frames"] ?? -1) : -1
        if (now_frames >= 0) {
            if (target < 0) {
                target = now_frames + frames
            }
            break if (now_frames >= target)
            if (now_frames > last_frames) {
                last_frames = now_frames
                last_progress = ref_time_ticks()
            }
        }
        if (get_time_usec(last_progress) >= RECORD_STALL_US) {
            warn_capture_stalled()
            let remaining = target < 0 ? frames : max(0, target - last_frames)
            sleep(uint(float(remaining) / float(g_record_fps) * 1000.0f))
            return
        }
        sleep(RECORD_POLL_MS)
    }
}

def public hold_content(app : ImguiApp; ms : uint) {
    //! Hold for `ms` of CONTENT time: under lockstep recording wait for ms/1000*fps captured
    //! frames; otherwise a plain wall-clock sleep(ms).
    if (g_record_fps <= 0) {
        sleep(ms)
        return
    }
    wait_content_frames(app, int((float(ms) / 1000.0f) * float(g_record_fps) + 0.5f))
}

def public record_frame_count(app : ImguiApp) : int {
    //! Current captured-frame count (content-time index). Capture right after say_begin to
    //! anchor a hold_remainder_content. Returns 0 outside a recording session.
    if (g_record_fps <= 0) return 0
    return record_frames_now(app)
}

def public hold_remainder_content(app : ImguiApp; dwell_ms : uint; start_frames : int) {
    //! Pad a beat to `dwell_ms` of CONTENT time: wait until the recorder reaches start_frames +
    //! dwell_ms/1000*fps (start_frames from record_frame_count right after say_begin). Work done mid-dwell
    //! consumes content frames; this holds the rest. No-op when idle.
    if (g_record_fps <= 0) return
    let target = start_frames + int((float(dwell_ms) / 1000.0f) * float(g_record_fps) + 0.5f)
    wait_content_frames(app, target - record_frames_now(app))
}

def private is_button_kind(kind : string) : bool {
    return (kind == "button" || kind == "small_button" || kind == "arrow_button" ||
            kind == "invisible_button" || kind == "image_button" || kind == "tab_item_button" ||
            kind == "icon_button")
}

def private split_alias(target : string) : tuple<string; int> {
    //! "X#v" -> (ground "X", v_button v); plain "X" -> ("X", 0). The per-site radio alias suffix.
    let h = find(target, "#")
    if (h < 0) return (target, 0)
    return (slice(target, 0, h), to_int(slice(target, h + 1)))
}

def private verify_click_effect(app : ImguiApp; target : string; var pre : JsonValue?) {
    // Verify the kind-appropriate effect of clicking ``target`` — a no-op click (landed off) leaves the state unmoved, so effect-by-kind doubles as a landing check. Untracked kinds skip.
    let (ground, vb) = split_alias(target)
    let kind = find_widget(pre, ground)?["kind"] ?? ""
    if (empty(kind)) return
    if (is_button_kind(kind)) {
        let pre_cc = widget_payload_field(pre, ground, "click_count") ?? 0
        record_check_value(app, ground, "click_count", pre_cc + 1, 120)
    } elif (kind == "radio_button_int") {
        record_check_value(app, ground, "value", vb, 120)
    } elif (kind == "checkbox" || kind == "radio_button" || kind == "selectable" || kind == "menu_item") {
        let pre_v = widget_payload_field(pre, ground, "value") ?? false
        record_check_value(app, ground, "value", !pre_v, 120)
    }
}

def public hold_through_voice(app : ImguiApp; dwell : uint; clicks : array<string>) {
    //! Pace the click(s) in ``clicks`` UNDER an already-posted voice line: a short lead, then each click
    //! lands (verified, or the recording aborts), then hold the caption for the rest. ``dwell`` is
    //! ``say_begin``'s return; the caller must have moved the cursor onto the first target.
    let lead = dwell > 1200u ? 500u : dwell / 3u
    hold_content(app, lead)
    let n = length(clicks)
    let body_ms = dwell > lead ? dwell - lead : 0u
    let beat = n > 0 ? body_ms / uint(n) : body_ms
    for (target in clicks) {
        var pre = snapshot(app)
        click(app, target)                              // animated approach + press/release
        verify_click_effect(app, target, pre)           // panics if the click had no effect
        reset_cursor_pos(widget_click_point(pre, target))  // track where the click left the cursor
        hold_content(app, beat)
    }
}

def public drag_through_voice(app : ImguiApp; dwell : uint; target : string;
                              to_pos : tuple<float; float>; drag_ms : int = 0; field : string = "value") {
    //! Drag analog of ``hold_through_voice``: scrub from the tracked cursor to ``to_pos`` under a voice
    //! line, then assert the drag CHANGED ``target.payload[field]`` (a no-op drag aborts at teardown). The
    //! caller must ``move_to`` the handle first. ``field`` defaults to ``"value"``; docking passes ``"size"``/``"dock_id"``.
    let before_jv = widget_payload_field(snapshot(app), target, field)
    let before = before_jv != null ? write_json(before_jv) : ""
    let from_pos = (last_cursor_x, last_cursor_y)
    let dist = max(abs(to_pos._0 - from_pos._0), abs(to_pos._1 - from_pos._1))
    let scrub_ms = drag_ms > 0 ? drag_ms : max(700, int(dist / 0.30f))
    let lead = clamp(dwell / 3u, 700u, 3000u)
    hold_content(app, lead)
    var ev : array<JsonValue?>
    ev |> drag_along(0, from_pos, to_pos, scrub_ms, 150)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    let play = uint(150 + 100 + scrub_ms + 200 + 300) + 250u   // drag_along timeline + settle
    hold_content(app, play)
    record_check_changed(app, target, field, before, 120)
    let consumed = lead + play
    hold_content(app, dwell > consumed ? dwell - consumed : 0u)
}

// ImGuiKey codes the recording helpers tap. playwright is transport-only (no imgui native module, so
// no ImGuiKey enum in scope); the host interprets these ints AS ImGuiKey, so they must match its build.
let KEY_TAB   : int = 512    // ImGuiKey.Tab
let KEY_ENTER : int = 525    // ImGuiKey.Enter

// type_into_voice tunables. Every hold in type_into_voice is PACING (visual rhythm only);
// every wait for an effect to LAND (focus, focus-moved, value-committed) is a wait_*/record_check
// POLL, never a fixed hold guessing the duration.
let TYPE_LEAD_MIN_MS   : uint  = 700u    // floor on the pre-type lead (short voice lines)
let TYPE_LEAD_MAX_MS   : uint  = 3000u   // cap on the pre-type lead (long voice lines)
let TYPE_PER_CHAR_S    : float = 0.16f   // wall-clock per typed char (keystrokes stay legible)
let TYPE_MIN_FIELD_S   : float = 0.9f    // floor on one field's typing duration
let TYPE_FIELD_BEAT_MS : uint  = 180u    // pacing pause after a field fills, before the next
let CLICK_RELEASE_MS   : int   = 130     // press->release gap of the focusing click
let TYPE_EFFECT_FRAMES : int   = 120     // poll budget for focus / active-id / value effects

def private snapshot_active_id(var snap : JsonValue?) : string {
    // io.active_id as a JSON string ("" when none). String not int: the id is a hash that overflows int32, only ever compared for change.
    let v = snap?["io"]?["active_id"]
    return v != null ? write_json(v) : ""
}

def public type_into_voice(app : ImguiApp; dwell : uint; target : string;
                           click_pos : tuple<float; float>; cells : array<string>;
                           expected : auto(T)) {
    //! Type analog of drag_through_voice: REAL keyboard edit of ``target``, assert committed == ``expected``.
    //! CLICKS ``click_pos`` (not imgui_focus) because InputScalar's AutoSelectAll fires only on activation, so
    //! the click focuses AND selects → typing REPLACES. Per ``cells``: type, Tab between, Enter; caller must ``move_to`` ``click_pos`` first.
    let started_frames = record_frame_count(app)
    let lead = clamp(dwell / 3u, TYPE_LEAD_MIN_MS, TYPE_LEAD_MAX_MS)   // ~1/3 into the line
    hold_content(app, lead)                                                // PACING: land the click under the voice

    // Real click to focus the (first) field. ImGui InputScalar's AutoSelectAll fires on activation,
    // so the click both focuses AND selects the current text -> typing REPLACES it.
    var ev : array<JsonValue?>
    ev |> push(JV((t_ms = 0,                kind = "button", x = click_pos._0, y = click_pos._1, action = "press")))
    ev |> push(JV((t_ms = CLICK_RELEASE_MS, kind = "button", x = click_pos._0, y = click_pos._1, action = "release")))
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    // SYNC: the field is editable once it reports focus — poll for it (no fixed settle sleep).
    wait_until(app, TYPE_EFFECT_FRAMES) $(var snap) {
        return snap?["globals"]?[target]?["focus"] ?? false
    }

    for (i in range(length(cells))) {
        if (i > 0) {
            // SYNC: Tab moves focus to the next sub-field. Wait until the ACTIVE widget actually
            // becomes a fresh one (active_id non-empty and changed) so the next cell lands in the new
            // field, never the previous — not a fixed sleep guessing the focus moved.
            let before_active = snapshot_active_id(snapshot(app))
            key_tap(app, KEY_TAB)
            wait_until(app, TYPE_EFFECT_FRAMES) $(var snap) {
                let now = snapshot_active_id(snap)
                return now != "" && now != "0" && now != before_active
            }
        }
        let cell = cells[i]
        let type_s = max(TYPE_MIN_FIELD_S, float(length(cell)) * TYPE_PER_CHAR_S)
        post_command(app, "imgui_key_type", JV((text = cell, total_duration_s = type_s)))
        wait_for_key_idle(app)                                 // SYNC: this field's chars finished typing
        hold_content(app, TYPE_FIELD_BEAT_MS)                  // PACING: let the filled field read
    }

    // SYNC: Enter commits, then record_check_value polls until the value lands (no fixed sleep).
    key_tap(app, KEY_ENTER)
    record_check_value(app, target, "value", expected, TYPE_EFFECT_FRAMES)

    hold_remainder_content(app, dwell, started_frames)                    // PACING: caption tail held in content-time
}

// combo_pick_voice tunables. Holds below are PACING only; the popup-open and the selection-commit
// are each POLLED. (Same voice-lead shape as type_into_voice's TYPE_LEAD_*; unify if a third user lands.)
let COMBO_LEAD_MIN_MS : uint = 700u    // floor on the pre-open lead (short voice lines)
let COMBO_LEAD_MAX_MS : uint = 3000u   // cap on the pre-open lead (long voice lines)
let COMBO_OPEN_INSET  : float = 40.0f  // click this far in from the combo's left edge — on the box, not its label
let COMBO_TRAVEL_MS   : int  = 450     // cursor travel to the combo box, then down to the item
let COMBO_POLL_FRAMES : int  = 120     // poll budget: popup-items-rendered, then value-committed

def public combo_pick_voice(app : ImguiApp; dwell : uint; target : string; item : int) {
    //! Open combo ``target`` and click item ``item`` from its popup under a voice line, then assert
    //! ``target.value == item``. Clicks the box (inset from the left, off the label), POLLS until the
    //! popup renders per-item ``sub_bboxes``, then clicks item ``item``. Self-verifying at teardown.
    let started_frames = record_frame_count(app)
    let lead = clamp(dwell / 3u, COMBO_LEAD_MIN_MS, COMBO_LEAD_MAX_MS)   // ~1/3 into the line
    hold_content(app, lead)                                                // PACING: open under the voice
    var snap0 = snapshot(app)
    let bb = snap0?["globals"]?[target]?["bbox"]
    let bx = bb?["x"] ?? 0.0f
    let by = bb?["y"] ?? 0.0f
    let bz = bb?["z"] ?? 0.0f
    let bw = bb?["w"] ?? 0.0f
    let open_pos = (bx + min(COMBO_OPEN_INSET, (bz - bx) * 0.15f), (by + bw) * 0.5f)
    var ev0 : array<JsonValue?>
    ev0 |> click_at(0, open_pos, COMBO_TRAVEL_MS, 0)
    post_command(app, "imgui_mouse_play", JV((events = ev0)))
    unsafe { delete ev0; }
    // SYNC: let the OPEN click's timeline fully play (its release must fire) before the item click —
    // a second imgui_mouse_play REPLACES the one in flight, so without this the open's release is lost
    // and the item click lands on a half-open / wrong-state popup (deterministically dropped the pick).
    wait_for_mouse_idle(app)
    // SYNC: the popup is open once item `item`'s sub-rect renders.
    var opened = wait_until(app, COMBO_POLL_FRAMES) $(var snap) {
        return find_widget(snap, target)?["sub_bboxes"]?[item] != null
    }
    let item_pos = widget_component_point(opened, target, item)
    var ev1 : array<JsonValue?>
    ev1 |> click_at(0, item_pos, COMBO_TRAVEL_MS, 0)
    post_command(app, "imgui_mouse_play", JV((events = ev1)))
    unsafe { delete ev1; }
    wait_for_mouse_idle(app)                                            // SYNC: the item click fully played
    record_check_value(app, target, "value", item, COMBO_POLL_FRAMES)   // SYNC: selection committed
    hold_remainder_content(app, dwell, started_frames)                    // PACING: caption tail held in content-time
}

// toggle_tree_voice tunables. Holds below are PACING only; the toggle effect is POLLED via
// record_check_rendered (child appears/disappears), not guessed. The chevron sits at the header's
// left edge, so the arrow click x is a small inset scaled by the header's own height (font-size aware).
let TREE_LEAD_MIN_MS : uint  = 700u    // floor on the pre-click lead (short voice lines)
let TREE_LEAD_MAX_MS : uint  = 3000u   // cap on the pre-click lead (long voice lines)
let TREE_ARROW_FRAC  : float = 0.6f    // arrow click x = header.x + header_height * frac (chevron is at the left)
let TREE_TRAVEL_MS   : int   = 450     // cursor travel onto the header before press
let TREE_POLL_FRAMES : int   = 120     // poll budget: child rendered-state settles after the toggle
let CLOSE_BTN_FRAC   : float = 0.5f    // X-button click x = header.z - header_height * frac (X sits at the right edge)

def public toggle_tree_voice(app : ImguiApp; dwell : uint; header_target : string; on_arrow : bool;
                             child_target : string; expect_child_rendered : bool) {
    //! Click a ``tree_node``/``collapsing_header`` header under a voice line, then assert the toggle via
    //! ``child_target``'s rendered state (a collapsed node stops drawing children). ``on_arrow=true`` clicks
    //! the chevron, ``on_arrow=false`` the label — the ``OpenOnArrow`` distinction (arrow toggles, label no-op). Self-verifying at teardown.
    let started_frames = record_frame_count(app)
    let lead = clamp(dwell / 3u, TREE_LEAD_MIN_MS, TREE_LEAD_MAX_MS)   // ~1/3 into the line
    hold_content(app, lead)                                                // PACING: land the click under the voice
    var snap = snapshot(app)
    let bb = snap?["globals"]?[header_target]?["bbox"]
    let bx = bb?["x"] ?? 0.0f
    let by = bb?["y"] ?? 0.0f
    let bz = bb?["z"] ?? 0.0f
    let bw = bb?["w"] ?? 0.0f
    let cy = (by + bw) * 0.5f
    let click_x = on_arrow ? bx + (bw - by) * TREE_ARROW_FRAC : (bx + bz) * 0.5f
    var ev : array<JsonValue?>
    ev |> click_at(0, (click_x, cy), TREE_TRAVEL_MS, 0)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    wait_for_mouse_idle(app)                                            // SYNC: the click fully played
    record_check_rendered(app, child_target, expect_child_rendered, TREE_POLL_FRAMES)   // SYNC: toggle landed
    hold_remainder_content(app, dwell, started_frames)                    // PACING: caption tail held in content-time
}

def public close_button_voice(app : ImguiApp; dwell : uint; header_target : string; child_target : string) {
    //! Click the X-button of a CLOSABLE container header under a voice line, then assert the strip
    //! vanished (``child_target`` no longer rendered). The X sits a half header-height in from the
    //! header's right edge (mirrors ImGui's layout). Self-verifying at teardown; caller must ``move_to`` the header first.
    let started_frames = record_frame_count(app)
    let lead = clamp(dwell / 3u, TREE_LEAD_MIN_MS, TREE_LEAD_MAX_MS)   // ~1/3 into the line
    hold_content(app, lead)                                                // PACING: land the click under the voice
    var snap = snapshot(app)
    let bb = snap?["globals"]?[header_target]?["bbox"]
    let by = bb?["y"] ?? 0.0f
    let bz = bb?["z"] ?? 0.0f
    let bw = bb?["w"] ?? 0.0f
    let cy = (by + bw) * 0.5f
    let click_x = bz - (bw - by) * CLOSE_BTN_FRAC
    var ev : array<JsonValue?>
    ev |> click_at(0, (click_x, cy), TREE_TRAVEL_MS, 0)
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    wait_for_mouse_idle(app)                                            // SYNC: the click fully played
    record_check_rendered(app, child_target, false, TREE_POLL_FRAMES)  // SYNC: the strip closed
    hold_remainder_content(app, dwell, started_frames)                    // PACING: caption tail held in content-time
}

def public click_render_voice(app : ImguiApp; dwell : uint; click_target : string;
                              watch_target : string; expect_rendered : bool) {
    //! Real-click ``click_target`` under a voice line, then assert ``watch_target``'s rendered state ==
    //! ``expect_rendered`` — the reveal (true) / dismiss (false) rail. A button whose own click closes the
    //! popup can't be verified by click-count (gone next frame); the watch-target's render state is the durable signal.
    let started_frames = record_frame_count(app)
    let lead = clamp(dwell / 3u, TREE_LEAD_MIN_MS, TREE_LEAD_MAX_MS)   // ~1/3 into the line
    hold_content(app, lead)                                                // PACING: land the click under the voice
    click(app, click_target)                                  // real animated click (imgui_click coro)
    record_check_rendered(app, watch_target, expect_rendered, TREE_POLL_FRAMES)   // SYNC: reveal/dismiss landed
    hold_remainder_content(app, dwell, started_frames)                    // PACING: caption tail held in content-time
}

def public menu_pick_voice(app : ImguiApp; dwell : uint; header_target : string; item_target : string) {
    //! Open a menu-bar menu (``header_target``) and pick ``item_target`` under a voice line, navigating
    //! like a human: click the header, then drop STRAIGHT DOWN the column. A diagonal click would cross a
    //! SIBLING header mid-travel and ImGui hover-switches the open menu, landing the click in the wrong one. Self-verifying at teardown.
    let started_frames = record_frame_count(app)
    let lead = clamp(dwell / 3u, TREE_LEAD_MIN_MS, TREE_LEAD_MAX_MS)   // ~1/3 into the line
    hold_content(app, lead)                                                // PACING: land the clicks under the voice
    click(app, header_target)                                 // open the menu (caller staged below it)
    record_check_rendered(app, item_target, true, TREE_POLL_FRAMES)   // SYNC: menu opened
    var snap = snapshot(app)
    let hb = snap?["globals"]?[header_target]?["bbox"]
    let hcx = ((hb?["x"] ?? 0.0f) + (hb?["z"] ?? 0.0f)) * 0.5f
    let ib = snap?["globals"]?[item_target]?["bbox"]
    let icy = ((ib?["y"] ?? 0.0f) + (ib?["w"] ?? 0.0f)) * 0.5f
    var ev : array<JsonValue?>
    ev |> click_at(0, (hcx, icy), TREE_TRAVEL_MS, 0)          // vertical drop down the column, then click
    post_command(app, "imgui_mouse_play", JV((events = ev)))
    unsafe { delete ev; }
    wait_for_mouse_idle(app)                                            // SYNC: the click fully played
    record_check_rendered(app, item_target, false, TREE_POLL_FRAMES)  // SYNC: pick landed, menu closed
    hold_remainder_content(app, dwell, started_frames)                    // PACING: caption tail held in content-time
}

def public wait_for_key_idle(app : ImguiApp; timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : bool {
    //! Poll ``imgui_key_status`` until ``playing == false`` or timeout; returns true on idle.
    //! Uses one HTTP request per poll (unlike a ``wait_until`` body that pairs a snapshot
    //! with a status call), keeping the per-subprocess connection count low on Windows.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        let r = post_command(app, "imgui_key_status", null)
        if (!(r?["playing"] ?? true)) return true
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    dump_on_timeout(app, "wait_for_key_idle")
    return false
}

def public wait_for_mouse_idle(app : ImguiApp; timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : bool {
    //! Poll ``imgui_mouse_status`` until ``playing == false`` or timeout; returns true on idle.
    //! Uses one HTTP request per poll (unlike a ``wait_until`` body that pairs a snapshot
    //! with a status call), keeping the per-subprocess connection count low on Windows.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        let r = post_command(app, "imgui_mouse_status", null)
        if (!(r?["playing"] ?? true)) return true
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    dump_on_timeout(app, "wait_for_mouse_idle")
    return false
}

def public await_probe(app : ImguiApp; until : string = "quiescent") : JsonValue? {
    //! Single ``imgui_await`` POST; the ``AwaitResult`` lives under ``response.value``.
    //! For poll-until-quiescent use ``await_quiescent``.
    return send_imgui_command(app.transport, "imgui_await", JV((until = until)))
}

def public type_text(app : ImguiApp; target : string; text : string;
                     focus_max_frames : int = 120;
                     duration_s : float = 0.0f) : JsonValue? {
    //! Auto-focus ``target``, wait up to ``focus_max_frames`` for focus, then stream ``text`` as synthetic
    //! key+char events via ``imgui_key_type`` (into whichever widget holds focus). Returns ``{ok, value}``
    //! (``ok=false`` on empty text or if focus never settles). ``duration_s > 0`` paces typing over wall-clock.
    if (empty(text)) return JV((ok = false, error = "missing text"))
    focus(app, target)
    let focused = wait_until(app, focus_max_frames) $(var snap) {
        return snap?["globals"]?[target]?["focus"] ?? false
    }
    if (focused == null) return JV((ok = false, error = "focus never settled: {target}"))
    var args = (duration_s > 0.0f) ? JV((text = text, total_duration_s = duration_s)) : JV((text = text))
    let resp = post_command(app, "imgui_key_type", args)
    unsafe { delete args; }
    if (resp == null) return JV((ok = false, error = "type post failed: {target}"))
    return JV((ok = true, value = target))
}

def public pause(app : ImguiApp) : void {
    //! POST ``/pause`` to halt the host's update() (skips advance_coroutines + render). POST /command
    //! still dispatches and POSTs still drain via ``server.tick()``. Useful for deterministic observation:
    //! probe ``imgui_await`` while the world is frozen, then ``unpause``.
    post_signal(app, "/pause")
}

def public unpause(app : ImguiApp) : void {
    //! POST ``/unpause`` to resume the host's update().
    post_signal(app, "/unpause")
}

def public with_paused(app : ImguiApp; body : block<() : void>) {
    //! Scope: ``pause`` the host, invoke ``body``, ``unpause``. Body observes a frozen world — coroutines
    //! spawned by ``imgui_click`` sit unadvanced until unpause, so ``imgui_await`` reports
    //! ``pending_coroutines >= 1`` deterministically even in headless mode (where the advance races the next POST).
    pause(app)
    body |> invoke()
    unpause(app)
}

def public reload(app : ImguiApp; timeout_sec : float = DEFAULT_RELOAD_TIMEOUT_SEC) : void {
    //! POST ``/reload`` then poll ``/status`` until ``has_error == false``; panics on timeout.
    //! Stays on raw HTTP — reload is daslang-live process control, not a dasImgui command verb.
    post_signal(app, "/reload")
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    while (get_time_usec(start) < timeout_usec) {
        let raw = get_text(app, "/status")
        if (!empty(raw)) {
            var err : string
            let parsed = read_json(raw, err)
            if (parsed != null) {
                let has_err = parsed?["has_error"] ?? false
                if (!has_err) return
            }
        }
        sleep(READY_POLL_INTERVAL_MS)
    }
    panic("reload did not converge on a clean compile within {timeout_sec}s")
}

struct private StatusReading {
    gen : int       // /status `generation`, or -1 when no usable reading
    has_err : bool
}

def private read_status(app : ImguiApp) : StatusReading {
    // gen = -1 when /status is unreadable/unparseable/omits generation; `?? -1` keeps callers from mistaking a missing field for generation 0.
    let raw = get_text(app, "/status")
    return StatusReading(gen = -1, has_err = false) if (empty(raw))
    var err : string
    let parsed = read_json(raw, err)
    return StatusReading(gen = parsed?["generation"] ?? -1,
                         has_err = parsed?["has_error"] ?? false)
}

def public reset(app : ImguiApp; timeout_sec : float = DEFAULT_RELOAD_TIMEOUT_SEC) : void {
    //! POST ``/reset`` — re-simulate the already-compiled program into a fresh context (no parse/infer),
    //! clearing ``@live`` state. ~100x cheaper than ``reload``; use between subtests instead of respawning.
    //! Polls ``/status`` until ``generation`` advances; panics if no baseline generation, has_error, or timeout.
    let start = ref_time_ticks()
    let timeout_usec = int(timeout_sec * 1000000.0f)
    // Baseline: require a real generation BEFORE the reset. Staying at -1 means /status never produced a
    // usable generation — surfaced as a hard, fast failure rather than guessing a baseline (which would
    // mask an unsupported /status schema).
    var gen0 = -1
    while (gen0 < 0 && get_time_usec(start) < timeout_usec) {
        gen0 = read_status(app).gen
        if (gen0 < 0) sleep(READY_POLL_INTERVAL_MS)
    }
    if (gen0 < 0) panic("reset: /status never reported a baseline generation within {timeout_sec}s (daslang-live without POST /reset support?)")
    post_signal(app, "/reset")
    // Fresh timer for the convergence window so a slow baseline poll above can't
    // eat into the time the reset itself gets to land.
    let reset_start = ref_time_ticks()
    while (get_time_usec(reset_start) < timeout_usec) {
        let st = read_status(app)
        // Only a real, advanced generation counts as done — a missing/omitted
        // field reads as -1 and is ignored, so we never return on a stale read.
        if (st.gen >= 0 && st.gen != gen0) {
            if (st.has_err) panic("reset re-simulate failed (generation advanced but has_error is set)")
            return
        }
        sleep(READY_POLL_INTERVAL_MS)
    }
    panic("reset did not converge within {timeout_sec}s")
}

var private g_headless_parsed : bool = false
var private g_headless_cached : bool = false

def private playwright_wants_headless() : bool {
    // ``--headless`` forwarded from dastest's argv, lazily parsed once. Local interactive runs omit it and stay windowed.
    if (g_headless_parsed) return g_headless_cached
    g_headless_parsed = true
    let args <- get_user_args()
    let raw = find_bool_flag_raw_value(args, "--headless")
    g_headless_cached = (raw |> unwrap_or("false")) == "true"
    return g_headless_cached
}

var private g_worker_index_parsed : bool = false
var private g_worker_index_cached : int = 0

def private playwright_worker_index() : int {
    // ``--worker-index N`` from dastest, lazily parsed. Derives a per-worker live-API port (DEFAULT_LIVE_PORT + N) so parallel workers don't collide; defaults to 0.
    if (g_worker_index_parsed) return g_worker_index_cached
    g_worker_index_parsed = true
    let args <- get_user_args()
    let raw = find_flag_raw_value(args, "--worker-index") |> unwrap_or("0")
    g_worker_index_cached = to_int(raw)
    return g_worker_index_cached
}

def public with_imgui_app(feature_path : string;
                          body : block<(app : ImguiApp) : void>) {
    //! Spawn ``daslang-live <feature_path>`` with dasImgui loaded; run ``body`` while alive, then POST
    //! ``/shutdown`` and drain stdout. Panics on non-zero exit / ready-timeout / test-timeout. The common
    //! case (require chain needs ONLY dasImgui); delegates to the 5-arg overload.
    let no_extra : array<string>
    with_imgui_app(feature_path, no_extra, "", "", body)
}

def public with_imgui_app(feature_path : string;
                          extra_module_roots : array<string>;
                          feature_root : string;
                          feature_prefix : string;
                          body : block<(app : ImguiApp) : void>) {
    //! Full form — spawn ``daslang-live`` for a feature whose ``require`` chain needs dasImgui PLUS the
    //! ``extra_module_roots`` (each ``-load_module``'d after dasImgui's, dependency order). Port is always
    //! DEFAULT_LIVE_PORT + worker_index, so N ``--worker-index``-branded parallel workers land on distinct ports. Forwards ``--headless`` into the spawn.
    let exe = daslang_live_exe()
    let port = DEFAULT_LIVE_PORT + playwright_worker_index()
    let ready_timeout_sec = host_ready_timeout_sec()
    let test_timeout_sec = DEFAULT_TEST_TIMEOUT_SEC
    let app_path = resolve_feature_path(feature_path, feature_root, feature_prefix)
    // -load_module points the spawned host at dasImgui so the feature's `require imgui/...` resolves
    // regardless of layout (harmless duplicate in CI, load-bearing locally); extra roots follow in
    // dependency order. --live-port pins the HTTP API to the port playwright dials (single-instance lock).
    var argv <- [exe, "-load_module", dasimgui_module_root()]
    argv |> reserve(length(argv) + length(extra_module_roots) * 2)
    for (root in extra_module_roots) {
        argv |> push("-load_module")
        argv |> push(root)
    }
    argv |> push("--live-port={port}")
    argv |> push(app_path)
    // App user-args follow `--`. Always disable imgui.ini: a test spawn must
    // start from the feature's default layout and never read/write a stale
    // imgui.ini in the spawn CWD (deterministic + no cross-test pollution).
    argv |> push("--")
    argv |> push("--no-imgui-ini")
    if (playwright_wants_headless()) {
        argv |> push("--headless")
        // Cascade guard: harness self-exits a few seconds before the popen watchdog gives up. If body
        // panics and /shutdown is never sent (e.g. expect_value timeout — `finally` is skipped on panic),
        // the subprocess still releases its port before the next test spawns, instead of zombifying.
        let harness_budget = test_timeout_sec - 5.0f
        if (harness_budget > 5.0f) {
            argv |> push("--headless-timeout-sec={harness_budget}")
        }
    }
    let base_url = "http://127.0.0.1:{port}"
    var app = ImguiApp(base_url = base_url,
                       feature_path = app_path,
                       transport <- live_api_transport(base_url))
    var captured : string
    var unready = false
    let exit_code = unsafe(popen_argv(argv, test_timeout_sec, $(f) {
        return if (f == null)
        if (!wait_until_ready(app, ready_timeout_sec)) {
            unready = true
            // Tell the host to exit so fread_to_eof returns once it drains, instead
            // of blocking until the test_timeout_sec popen watchdog (the readiness
            // decision was already made at ready_timeout_sec — don't waste the rest).
            post_signal(app, "/shutdown")
            captured = unsafe(fread_to_eof(f))
            return
        }
        invoke(body, app)
        post_signal(app, "/shutdown")
        captured = unsafe(fread_to_eof(f))
    }))
    if (unready) {
        panic("daslang-live not ready after {ready_timeout_sec}s.\nstdout/stderr:\n{captured}")
    }
    if (exit_code == popen_timed_out) {
        panic("daslang-live timed out (>{test_timeout_sec}s).\nstdout/stderr:\n{captured}")
    }
    if (exit_code != 0) {
        panic("daslang-live exited non-zero ({exit_code}).\nstdout/stderr:\n{captured}")
    }
}

// ===== Recording helper =====

let public RECORD_ASSET_REL = "doc/source/_static/tutorials"
let private RECORD_FPS_DEFAULT = RECORD_FPS  // nominal capture+playback grid — see the tunables section above

def public with_recording_app(feature_path : string;
                              output_apng_basename : string;
                              max_seconds : int;
                              body : block<(app : ImguiApp) : void>) {
    //! Native-fps form (the common case). Delegates to the 4-arg overload — gen2's trailing-block sugar
    //! can't see through a default parameter between the last positional and the block, so the overload
    //! pair keeps the common call site clean.
    with_recording_app(feature_path, output_apng_basename, max_seconds, RECORD_FPS_DEFAULT, body)
}

def public with_recording_app(feature_path : string;
                              output_apng_basename : string;
                              max_seconds : int;
                              fps : int;
                              body : block<(app : ImguiApp) : void>) {
    //! dasImgui-only form — feature + assets rooted under dasImgui, no extra
    //! modules. Delegates to the full form below; another package
    //! (dasImguiNodeEditor) calls the full form directly with its own roots.
    let no_extra : array<string>
    let root = dasimgui_module_root()
    with_recording_app(feature_path, output_apng_basename, max_seconds, fps,
                       no_extra, root, root, body)
}

def public with_recording_app(feature_path : string;
                              output_apng_basename : string;
                              max_seconds : int;
                              fps : int;
                              extra_module_roots : array<string>;
                              feature_root : string;
                              asset_root : string;
                              body : block<(app : ImguiApp) : void>) {
    //! Run a recording session against ``feature_path``: **attach** to a daslang-live already serving the
    //! live-API port (dev workflow: iterate live, flip the recorder on), else **spawn** one at logical 1x
    //! (``--imgui-content-scale=1.0`` + ``--no-hdpi-framebuffer`` keep APNGs encoder-friendly on retina). Attach mode leaves the host running and owns whatever feature is loaded.

    // Relative feature_path resolves under feature_root; the APNG lands under asset_root. The 5-arg delegate defaults both to dasImgui.
    var app_path = feature_path
    if (!is_absolute(app_path)) {
        app_path = path_join(feature_root, app_path)
    }
    let asset_dir = path_join(asset_root, RECORD_ASSET_REL)
    let output_apng_path = path_join(asset_dir, output_apng_basename)
    arm_voiceovers(output_apng_path)
    g_record_failures |> clear()   // fresh self-verification ledger for this recording
    // Voiceover dwells make the body run far longer than the silent-record cap.
    // The cap is only a safety backstop (real length comes from record_stop), so
    // extend it by the total added narration time when armed.
    let eff_max_seconds = g_vo_active ? max_seconds + int(g_vo_extra_s) + 5 : max_seconds

    let base_url = "http://127.0.0.1:{DEFAULT_LIVE_PORT}"
    var app = ImguiApp(base_url = base_url,
                       feature_path = app_path,
                       transport <- live_api_transport(base_url))

    // Attach-mode probe: 0.3 s is enough for three /status polls (instant
    // on a healthy listener, fast TCP-refused if no one's there).
    if (wait_until_ready(app, ATTACH_PROBE_SEC)) {
        print("[record] attaching to existing daslang-live at {base_url}\n")
        post_command(app, "imgui_mouse_trail",   JV((enabled = true)))
        post_command(app, "imgui_cursor_sprite", JV((enabled = true)))
        // Windowed glfw re-polls the real OS cursor into io.MousePos every frame; detach real
        // input so synth fully owns IO, else synth clicks land wherever the real mouse sits.
        post_command(app, "set_user_control",     JV((enabled = false)))
        post_command(app, "record_start", JV((
            file = output_apng_path,
            fps = fps,
            max_seconds = eff_max_seconds
        )))
        g_record_fps = fps   // lockstep: gate say/hold_content by captured-frame count
        g_stall_warned = false
        invoke(body, app)
        let stopped = post_command(app, "record_stop", null)
        g_record_fps = 0
        let dump = stopped != null ? write_json(stopped) : "<null>"
        print("[record] record_stop -> {dump}\n")
        print("[record] APNG saved to {output_apng_path}\n")
        flush_voiceovers(stopped, output_apng_path, fps)
        post_command(app, "set_user_control",     JV((enabled = true)))
        post_command(app, "imgui_cursor_sprite", JV((enabled = false)))
        post_command(app, "imgui_mouse_trail",   JV((enabled = false)))
        // Host stays running (we didn't spawn it). The body never panics on a failed interaction;
        // it accumulates misses, so the recording stops cleanly above — now fail loudly if any.
        if (!empty(g_record_failures)) {
            panic(record_failures_message())
        }
        return
    }

    let exe = daslang_live_exe()
    // -load_module dasImgui (+ each extra root after it, dependency order) so the spawned host resolves
    // the feature's `require imgui/...` regardless of layout (harmless duplicate in CI, load-bearing
    // locally). Same explicit-load model as with_imgui_app.
    var argv <- [exe, "-load_module", dasimgui_module_root()]
    argv |> reserve(length(argv) + length(extra_module_roots) * 2)
    for (extra_root in extra_module_roots) {
        argv |> push("-load_module")
        argv |> push(extra_root)
    }
    argv |> push(app_path)
    argv |> push("--")
    argv |> push("--imgui-content-scale=1.0")
    // Ask glfw_live to skip the HDPI backing on retina / Windows DPI so the framebuffer (and the APNG
    // capture that reads it via glReadPixels) match the requested logical size. Without this, captures
    // stay at physical 2x even when the style is 1x — UI small inside a big frame.
    argv |> push("--no-hdpi-framebuffer")

    var captured : string
    var unready  = false
    // Body must complete within max_seconds + ready/shutdown headroom.
    let test_timeout_sec = float(eff_max_seconds) + 30.0f
    let exit_code = unsafe(popen_argv(argv, test_timeout_sec, $(f) {
        return if (f == null)
        if (!wait_until_ready(app, host_ready_timeout_sec())) {
            unready = true
            captured = unsafe(fread_to_eof(f))
            return
        }
        post_command(app, "imgui_mouse_trail",   JV((enabled = true)))
        post_command(app, "imgui_cursor_sprite", JV((enabled = true)))
        // Windowed glfw re-polls the real OS cursor into io.MousePos every frame; detach real
        // input so synth fully owns IO, else synth clicks land wherever the real mouse sits.
        post_command(app, "set_user_control",     JV((enabled = false)))
        post_command(app, "record_start", JV((
            file = output_apng_path,
            fps = fps,
            max_seconds = eff_max_seconds
        )))
        // The body never panics on a failed interaction — it accumulates misses into g_record_failures
        // (recovering from a daslang panic isn't supported, so we can't catch mid-body and shut down
        // cleanly). So record_stop + /shutdown always run; misses are surfaced after popen drains, below.
        g_record_fps = fps   // lockstep: gate say/hold_content by captured-frame count
        g_stall_warned = false
        invoke(body, app)
        let stopped = post_command(app, "record_stop", null)
        g_record_fps = 0
        let dump = stopped != null ? write_json(stopped) : "<null>"
        print("[record] record_stop -> {dump}\n")
        print("[record] APNG saved to {output_apng_path}\n")
        flush_voiceovers(stopped, output_apng_path, fps)
        post_command(app, "set_user_control",     JV((enabled = true)))
        post_command(app, "imgui_cursor_sprite", JV((enabled = false)))
        post_command(app, "imgui_mouse_trail",   JV((enabled = false)))
        post_signal(app, "/shutdown")
        captured = unsafe(fread_to_eof(f))
    }))
    if (unready) {
        panic("daslang-live not ready after {host_ready_timeout_sec()}s.\nstdout/stderr:\n{captured}")
    }
    if (exit_code == popen_timed_out) {
        panic("daslang-live timed out (>{test_timeout_sec}s).\nstdout/stderr:\n{captured}")
    }
    if (exit_code != 0) {
        panic("daslang-live exited non-zero ({exit_code}).\nstdout/stderr:\n{captured}")
    }
    if (!empty(g_record_failures)) {
        // Host shut down cleanly above; surface the self-verification misses now (after drain) so
        // the recording fails loudly with every want/got diagnostic + the subprocess stdout/stderr.
        panic("{record_failures_message()}\nstdout/stderr:\n{captured}")
    }
}

def private record_failures_message() : string {
    //! Join the accumulated self-verification misses into one loud teardown message.
    return ("recording self-verification failed — {length(g_record_failures)} interaction(s) did not register:\n" +
            join(g_record_failures, "\n---\n"))
}
