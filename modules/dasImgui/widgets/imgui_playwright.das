options gen2
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
require daslib/strings_boost
require strings

//! Playwright-style test harness for daslang-live + dasImgui.
//!
//! `with_imgui_app(feature_path) <| $(app) { ... }` spawns daslang-live
//! against the given .das file, polls `/status` until ready, runs the body,
//! POSTs `/shutdown`, drains stdout, and panics on non-zero subprocess
//! exit. The body drives the live_api via the `imgui_*` command verbs
//! through `app.transport`; helpers like `click`, `set_value`, and
//! `expect_value` wrap the dispatch and snapshot navigation.
//!
//! Headless / offscreen rendering is deferred (Phase 4+); the spawned
//! daslang-live process runs with a visible window today.

// ===== Tunables =====
//
// Sleep durations and timeouts. Pulled out of the call sites so a single
// place documents what each gate is waiting on, and so per-test overrides
// (slower CI box, longer-warmup demos) are a one-line change.

let DEFAULT_LIVE_PORT : int = 9090                   //! daslang-live's live_api default port
let DEFAULT_DEADLOCK_SANITY_SEC : float = 120.0f     //! frame-budget waits bail out after this wall-clock if the frame counter never advances (subprocess crash / true deadlock). Not a per-test timeout — it should never fire on a slow-but-progressing run.
let DEFAULT_READY_TIMEOUT_SEC : float = 30.0f        //! /status must turn 200 OK within this — covers daslang-live boot + GLFW + ImGui ctx
let DEFAULT_TEST_TIMEOUT_SEC : float = 120.0f        //! upper bound on the entire popen lifetime, including body + drain
let DEFAULT_RELOAD_TIMEOUT_SEC : float = 30.0f       //! POST /reload to /status.has_error == false convergence
let DEFAULT_FRAME_WAIT_SEC : float = 10.0f           //! upper bound on "first frame to render the named widget"

let READY_POLL_INTERVAL_MS : uint = 100u             //! ms between /status polls — used by wait_until_ready and reload-convergence loops
let FRAME_POLL_INTERVAL_MS : uint = 50u              //! ms between snapshot polls — wait_for_widget polls per-frame, ~3 polls/frame at 60fps

struct ImguiApp {
    base_url : string
    feature_path : string
    transport : Transport
}

def private daslang_live_exe() : string {
    //! Resolve the daslang-live executable next to the running interpreter.
    //! Mirrors `get_das_exe()` (daslib/command_line) — same dir, swap the
    //! last path component for the platform-appropriate `daslang-live` name.
    let das = get_das_exe()
    if (empty(das)) return ""
    let dir = dir_name(das)
    let bin = get_platform_name() == "windows" ? "daslang-live.exe" : "daslang-live"
    return path_join(dir, bin)
}

def private resolve_feature_path(path : string) : string {
    if (is_absolute(path)) return path
    return path_join(get_das_root(), path)
}

def public wait_until_ready(app : ImguiApp; timeout_sec : float) : bool {
    //! Poll GET /status until 200 OK or timeout. Returns true on ready.
    let deadline = ref_time_ticks() + int64(timeout_sec * 1000000.0f)
    while (ref_time_ticks() < deadline) {
        var ok = false
        GET("{app.base_url}/status") $(resp) {
            if (resp != null && resp.status_code == http_status.OK) {
                ok = true
            }
        }
        if (ok) return true
        sleep(READY_POLL_INTERVAL_MS)
    }
    return false
}

def public get_text(app : ImguiApp; uri : string) : string {
    //! GET `uri`; return the raw response body or "".
    var out : string
    GET("{app.base_url}{uri}") $(resp) {
        if (resp != null) {
            out = string(resp.body)
        }
    }
    return out
}

def private post_signal(app : ImguiApp; uri : string) {
    //! Fire-and-forget POST with empty body. Used for /reload and /shutdown.
    POST("{app.base_url}{uri}", "", {"Content-Type" => "application/json"}) $(resp) {
        // intentionally drop the response — caller polled /status to confirm
    }
}

// ===== Command dispatch =====

def public post_command(app : ImguiApp; name : string; var args : JsonValue?) : JsonValue? {
    //! Send `{"name":<name>, "args":<args>}` through `app.transport` and
    //! return the parsed JSON response (or null on transport failure).
    //! Public escape hatch for custom verbs not in the shipped helper set.
    return send_imgui_command(app.transport, name, args)
}

def public snapshot(app : ImguiApp) : JsonValue? {
    return send_imgui_command(app.transport, "imgui_snapshot", null)
}

// ===== Snapshot navigation =====

def public find_widget(var snap : JsonValue?; ident : string) : JsonValue? {
    //! Convenience navigator: `snapshot.globals[ident]`. May return a non-null
    //! `JV(null)` placeholder for missing keys (daslib/json `?[]` quirk).
    //! For an existence check use `widget_exists` instead.
    return snap?["globals"]?[ident]
}

def public widget_exists(var snap : JsonValue?; ident : string) : bool {
    //! True iff `globals[ident]` is a real widget entry. Catches the
    //! `?[]` quirk where missing object keys come back as `JV(null)` rather
    //! than a literal-null pointer — a non-null pointer to the null variant
    //! still passes `!= null`. Test for the `kind` field instead, which is
    //! always a non-empty string on a real entry.
    let kind = snap?["globals"]?[ident]?["kind"] ?? ""
    return !empty(kind)
}

def public widget_payload_field(var snap : JsonValue?; ident : string; field : string) : JsonValue? {
    //! Convenience navigator: `snapshot.globals[ident].payload[field]`.
    //! Returns null if any link in the chain is missing.
    return snap?["globals"]?[ident]?["payload"]?[field]
}

def public widget_rendered(var snap : JsonValue?; ident : string) : bool {
    //! True iff `globals[ident]` is for a widget that rendered this frame.
    //! Phase 1 widgets registered at module init but not yet painted (closed
    //! window, untaken branch) carry an explicit `"rendered": false`;
    //! rendered widgets omit the field. The `?? true` default reads the
    //! absent-field path as rendered. Don't use raw `?[]` != null — the
    //! daslib/json ?[] quirk returns a JV(null) pointer for missing keys,
    //! not literal null.
    return snap?["globals"]?[ident]?["rendered"] ?? true
}

// ===== Poll-until family =====

def public wait_until(app : ImguiApp;
                      max_frames : int;
                      pred : block<(var snap : JsonValue?) : bool>) : JsonValue? {
    //! Poll-until-condition with a *frame budget* instead of wall-clock seconds.
    //! Reads ``snap["frame"]`` on each tick and gives up after ``max_frames``
    //! frames have elapsed since the first successful snapshot — deterministic
    //! w.r.t. the work the subprocess does, immune to "what if the machine is
    //! slow today / someone's watching video on the same CPU." A slow machine
    //! just spends more wall-clock per frame; coverage is unchanged.
    //!
    //! ``DEFAULT_DEADLOCK_SANITY_SEC`` is a backstop only — fires when the
    //! frame counter never advances (subprocess crashed or truly hung). It
    //! must never trip on normal slow runs; if it does, raise it.
    var start_frame   = -1
    let sanity_deadline = ref_time_ticks() + int64(DEFAULT_DEADLOCK_SANITY_SEC * 1000000.0f)
    while (true) {
        var snap = snapshot(app)
        if (snap != null) {
            if (invoke(pred, snap)) return snap
            let f = snap?["frame"] ?? 0
            if (start_frame < 0) {
                start_frame = f
            } elif (f - start_frame >= max_frames) return null
        }
        return null if (ref_time_ticks() >= sanity_deadline)
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    return null
}

def public wait_for_widget(app : ImguiApp; widget_ident : string;
                           timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : JsonValue? {
    //! Poll snapshot until `globals.<widget_ident>` is present in the registry.
    //! Phase 1: "present" means registered at module [init] OR rendered at
    //! least once. Use this to wait for subprocess startup (any widget
    //! visible == compile+load done). For "wait until the widget is actually
    //! painting this frame" use `wait_for_render` instead.
    let deadline = ref_time_ticks() + int64(timeout_sec * 1000000.0f)
    while (ref_time_ticks() < deadline) {
        var snap = snapshot(app)
        if (snap != null && widget_exists(snap, widget_ident)) return snap
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    return null
}

def public wait_for_render(app : ImguiApp; widget_ident : string;
                           timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : JsonValue? {
    //! Poll snapshot until `globals.<widget_ident>` reports `rendered:true`
    //! (i.e. the widget actually painted this frame). For tests asserting
    //! that imgui_open / state.open flips re-paint a previously-hidden
    //! widget. Phase 1.1: registered-but-not-rendered widgets carry
    //! `rendered:false` and pass `wait_for_widget` immediately, so use this
    //! variant when you specifically need first-paint semantics.
    let deadline = ref_time_ticks() + int64(timeout_sec * 1000000.0f)
    while (ref_time_ticks() < deadline) {
        var snap = snapshot(app)
        if (snap != null && widget_rendered(snap, widget_ident)) return snap
        sleep(FRAME_POLL_INTERVAL_MS)
    }
    return null
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
    //! Generic poll: snapshot until `globals[target].payload[field] == expected`.
    //! T is inferred from `expected` and drives both the from_JV decode and the
    //! equality check. Use for any roundtrippable type — float, float2/3/4,
    //! int2/3/4, etc. — where the typed sugars above don't already apply.
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
    //! Assertion form of `wait_for_payload_value`. Polls until
    //! `globals[target].payload[field] == expected`; if the value never
    //! converges within `max_frames` frames, panics with a focused failure
    //! message (want / got / widget kind / rendered) — never dumps the full
    //! snapshot. If the user wants the full snapshot at the failure point,
    //! they call `snapshot(app)` before the assertion or after recovering.
    if (wait_for_payload_value(app, target, field, expected, max_frames)) return
    var snap = snapshot(app)
    let got_jv = widget_payload_field(snap, target, field)
    let got_str = got_jv != null ? write_json(got_jv) : "<missing>"
    let kind = snap?["globals"]?[target]?["kind"] ?? "<unknown>"
    let exists = widget_exists(snap, target)
    let rendered = exists && widget_rendered(snap, target)
    let want_str = write_json(JV(expected))
    panic("expect_value({target}.{field}) gave up after {max_frames} frames:\n  want: {want_str}\n  got:  {got_str}\n  widget kind: {kind}\n  widget rendered: {rendered}")
}

def public expect_render(app : ImguiApp;
                         widget_ident : string;
                         timeout_sec : float = DEFAULT_FRAME_WAIT_SEC) : void {
    //! Assertion form of `wait_for_render`. Polls until the widget reports
    //! `rendered:true`; on timeout, panics with a list of currently-
    //! registered widget identifiers so a typo in `widget_ident` is
    //! obvious in the failure message.
    if (wait_for_render(app, widget_ident, timeout_sec) != null) return
    var snap = snapshot(app)
    var keys_buf = "<none>"
    let g = snap?["globals"]
    if (g != null && g is _object) {
        let obj & = unsafe(g as _object)
        var key_list : array<string>
        for (k in keys(obj)) {
            key_list |> push(k)
        }
        keys_buf = "[" + join(key_list, ", ") + "]"
    }
    panic("expect_render({widget_ident}) timeout after {timeout_sec}s — widget never reported rendered:true.\nRegistered widgets: {keys_buf}")
}

def public await_quiescent(app : ImguiApp;
                           timeout_sec : float = 5.0f) : void {
    //! Assertion-style wrapper over `await_imgui(transport, "quiescent",
    //! timeout)`. Panics on timeout — tests want hard-fail per spec §5.2.
    //! For a non-assertion poll, call `await_imgui` directly.
    if (await_imgui(app.transport, "quiescent", timeout_sec)) return
    panic("await_quiescent timeout after {timeout_sec}s — server never reported quiescent")
}

// ===== Command verbs =====

def public click(app : ImguiApp; target : string) : JsonValue? {
    return send_imgui_command(app.transport, "imgui_click", JV((target = target)))
}

def public set_value(app : ImguiApp; target : string; var value : JsonValue?) : JsonValue? {
    return send_imgui_command(app.transport, "imgui_set", JV((target = target, value = value)))
}

def public open_widget(app : ImguiApp; target : string) : JsonValue? {
    return send_imgui_command(app.transport, "imgui_open", JV((target = target)))
}

def public close_widget(app : ImguiApp; target : string) : JsonValue? {
    return send_imgui_command(app.transport, "imgui_close", JV((target = target)))
}

def public drag(app : ImguiApp; target : string;
                dx : float; dy : float;
                steps : int = 4; button : int = 0) : JsonValue? {
    //! Synthesize an L1 mouse-drag on `target`'s bbox center: press → `steps`
    //! interpolated moves → release. Coroutine on the host advances one
    //! event per frame, so observable settle is `steps + 2` frames.
    return send_imgui_command(app.transport, "imgui_drag",
                              JV((target = target, dx = dx, dy = dy, steps = steps, button = button)))
}

def public focus(app : ImguiApp; target : string) : JsonValue? {
    //! Force keyboard focus to the next render of `target`. Server-side
    //! `imgui_focus` adds the path to g_pending_focus; the next time
    //! widget_prelude runs for that path, it calls SetKeyboardFocusHere(0)
    //! and clears the entry. Display-only widgets (text_show, plot)
    //! return Result.err("not focusable").
    return send_imgui_command(app.transport, "imgui_focus", JV((target = target)))
}

def public await_probe(app : ImguiApp; until : string = "quiescent") : JsonValue? {
    //! Single imgui_await POST. Server replies with a `Result<AwaitResult,
    //! string>` — the AwaitResult lives under `response.value`. This is a
    //! one-shot probe; for poll-until-quiescent use `await_quiescent`.
    return send_imgui_command(app.transport, "imgui_await", JV((until = until)))
}

def public type_text(app : ImguiApp; target : string; text : string;
                     focus_max_frames : int = 120;
                     duration_s : float = 0.0f) : JsonValue? {
    //! Auto-focus `target` first, wait for focus to settle (up to
    //! `focus_max_frames` frames), then synthesize UTF-8 characters via L1
    //! AddInputCharactersUTF8. `duration_s > 0` paces typing over that
    //! wall-clock duration (server translates to `total_frames =
    //! int(duration_s * 60)` — exact rate is fps-dependent, visible-to-
    //! viewer-only). Default 0 keeps the 1-char/frame legacy rate. Phase 3
    //! closes the gap noted in Phase 2: the legacy `type_text` was
    //! "informational target" and required the example to call
    //! `SetKeyboardFocusHere(0)` itself.
    //!
    //! Settle is a `wait_until` poll on `globals[target].focus == true`
    //! (server-side `entry.focus = IsItemFocused()` lands on this top-level
    //! field via @optional). Server-side `imgui_focus` sets g_pending_focus
    //! → next widget_prelude calls SetKeyboardFocusHere(0) → frame after
    //! that, IsItemFocused returns true. So expect ~2 frames of latency.
    //! Falls through to send chars even on focus timeout; the caller can
    //! `expect_value` afterward to surface "the value never landed."
    focus(app, target)
    wait_until(app, focus_max_frames) $(var snap) {
        return snap?["globals"]?[target]?["focus"] ?? false
    }
    let total_frames = duration_s > 0.0f ? int(duration_s * 60.0f) : 0
    return send_imgui_command(app.transport, "imgui_type_text",
        JV((target = target, text = text, total_frames = total_frames)))
}

def public reload(app : ImguiApp; timeout_sec : float = DEFAULT_RELOAD_TIMEOUT_SEC) : void {
    //! POST /reload then poll /status until has_error=false. Panics on
    //! timeout. Stays on raw HTTP — reload is daslang-live's process
    //! control, not a dasImgui command verb.
    post_signal(app, "/reload")
    let deadline = ref_time_ticks() + int64(timeout_sec * 1000000.0f)
    while (ref_time_ticks() < deadline) {
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

def public with_imgui_app(feature_path : string; body : block<(app : ImguiApp) : void>) {
    //! Convenience form using the module-level default port + timeouts.
    //! For non-default knobs, use `with_imgui_app_opt`.
    with_imgui_app_opt(feature_path,
                       DEFAULT_LIVE_PORT,
                       DEFAULT_READY_TIMEOUT_SEC,
                       DEFAULT_TEST_TIMEOUT_SEC,
                       body)
}

def public with_imgui_app_opt(feature_path : string;
                              port : int;
                              ready_timeout_sec : float;
                              test_timeout_sec : float;
                              body : block<(app : ImguiApp) : void>) {
    //! Spawn daslang-live <feature_path>. Body runs while the subprocess is
    //! alive; on body return, POST /shutdown and drain stdout. Panics on
    //! non-zero exit code so the surrounding [test] surfaces the failure.
    let exe = daslang_live_exe()
    let app_path = resolve_feature_path(feature_path)
    let argv <- [exe, app_path]
    let base_url = "http://127.0.0.1:{port}"
    var app = ImguiApp(base_url = base_url,
                       feature_path = app_path,
                       transport <- live_api_transport(base_url))
    var captured : string
    var unready = false
    print("[imgui_playwright] spawn: {exe} {app_path}\n")
    let exit_code = unsafe(popen_argv(argv, test_timeout_sec, $(f) {
        if (f == null) {
            print("[imgui_playwright] popen returned null FILE\n")
            return
        }
        print("[imgui_playwright] subprocess up, polling /status...\n")
        if (!wait_until_ready(app, ready_timeout_sec)) {
            print("[imgui_playwright] readiness gate FAILED\n")
            unready = true
            captured = unsafe(fread_to_eof(f))
            return
        }
        print("[imgui_playwright] ready, invoking body\n")
        invoke(body, app)
        print("[imgui_playwright] body done, sending /shutdown\n")
        post_signal(app, "/shutdown")
        print("[imgui_playwright] /shutdown posted, draining pipe...\n")
        captured = unsafe(fread_to_eof(f))
        print("[imgui_playwright] drain done\n")
    }))
    print("[imgui_playwright] popen exit_code={exit_code}\n")
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
