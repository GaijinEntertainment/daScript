options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require ../../utils/watchdog/watchdog.das
require ../stddlg/_tray_rig.das

// argv[0] is the daslang running dastest; the supervised "program" is that binary running the
// fixture, so program mode and script mode both spawn a real daslang child. Absolute, because a
// relative --program resolves against --cwd, which is a temp dir here.
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : get_full_file_name(args[0])
}

def fixture_path() : string {
    return path_join(get_das_root(), "tests/watchdog/_fixture_watchdog_child.das")
}

def make_temp(t : T?; prefix : string) : string {
    let tmp = create_temp_directory_result(prefix)
    if (!(tmp is value)) {
        t |> failure("could not create temp directory: {tmp as error}")
        return ""
    }
    return tmp as value
}

def read_events(log_path : string) : array<JsonValue?> {
    var events : array<JsonValue?>
    if (!fexist(log_path)) return <- events
    var text : string
    fopen(log_path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    for (line in split(text, "\n")) {
        if (empty(strip(line))) continue
        var error : string
        var js = read_json(line, error)
        if (js != null) {
            events |> push(js)
        }
    }
    return <- events
}

def event_names(events : array<JsonValue?>) : array<string> {
    return <- [for (ev in events); ev?["event"] ?? ""]
}

def count_event(events : array<JsonValue?>; name : string) : int {
    var n = 0
    for (ev in events) {
        if ((ev?["event"] ?? "") == name) {
            n++
        }
    }
    return n
}

def event_int(events : array<JsonValue?>; name, key : string; nth : int = 0) : int {
    var seen = 0
    for (ev in events) {
        if ((ev?["event"] ?? "") != name) continue
        if (seen == nth) return int(ev?[key] ?? -999999l)
        seen++
    }
    return -999999
}

def child_args(state_dir, mode : string) : array<string> {
    return ["-dasroot", get_das_root(), fixture_path(), "--", state_dir, mode]
}

def program_mode_args(root, state_dir, mode : string) : array<string> {
    var args <- ["--program", das_exe(), "--name", "wdtest", "--cwd", root, "--no-health",
        "--stable-seconds", "0.1", "--max-restart-delay", "0.5", "--"]
    args |> push_from(child_args(state_dir, mode))
    return <- args
}

// A supervision that never ends fails the test instead of hanging the sweep: tick with a
// deadline, and past it stop the supervisor and name the events it logged.
def run_bounded(t : T?; var sup : Supervisor?; seconds : float) : int {
    let started = ref_time_ticks()
    while (!sup->tick()) {
        if (float(get_time_usec(started)) > seconds * 1000000.0) {
            t |> failure("supervision did not end within {seconds} s: {event_names(read_events(sup.cfg.log))}")
            sup->request_stop()
            let grace = ref_time_ticks()
            while (!sup->tick() && get_time_usec(grace) < 30000000) {
                sleep(50u)
            }
            break
        }
        sleep(10u)
    }
    return sup.result
}

// Script mode is `daslang -jit <script>`, and a daslang that cannot start the JIT (no LLVM
// library beside it) has no script-mode child to supervise. The fixture with no arguments
// returns 2; any other code from a -jit run means the JIT never came up.
def jit_runs() : bool {
    let argv <- [das_exe(), "-jit", fixture_path()]
    let no_env : array<string>
    var code : int
    unsafe {
        var p = spawn_process(argv, "", no_env)
        code = process_wait(p, 60.0)
        close_process(p)
    }
    return code == 2
}

def skip_without_jit(t : T?) : bool {
    if (jit_runs()) return false
    t |> skip("this daslang cannot start the JIT, so script mode (daslang -jit) has no child to supervise here")
    return true
}

[test]
def test_tray_status_line(t : T?) {
    t |> run("each supervisor state has its line and its badge") @(t : T?) {
        var line = tray_status_line(TrayStatus(child_pid = 0, restart_seconds = 3, last_exit_code = 9))
        t |> equal(line.text, "restarting in 3s (exit 9)")
        t |> equal(line.badge, "red")
        line = tray_status_line(TrayStatus(child_pid = 0, capturing = true, last_exit_code = 9))
        t |> equal(line.text, "crashed (exit 9) - collecting the report", "while the report is collected")
        t |> equal(line.badge, "red")
        line = tray_status_line(TrayStatus(child_pid = 0))
        t |> equal(line.text, "starting", "before the first spawn")
        t |> equal(line.badge, "amber")
        line = tray_status_line(TrayStatus(child_pid = 0, stopping = true))
        t |> equal(line.text, "stopped")
        t |> equal(line.badge, "red")
        line = tray_status_line(TrayStatus(child_pid = 1, stopping = true))
        t |> equal(line.text, "shutting down")
        t |> equal(line.badge, "amber")
        line = tray_status_line(TrayStatus(child_pid = 1, tune_total = 5, tune_done = 2, tune_kernel = "gemv"))
        t |> equal(line.text, "tuning this box - 2/5 kernels (gemv)")
        t |> equal(line.badge, "amber")
        line = tray_status_line(TrayStatus(child_pid = 1, tune_total = 5, tune_done = 2))
        t |> equal(line.text, "tuning this box - 2/5 kernels", "between two kernels no name is shown")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = false, up_minutes = 1))
        t |> equal(line.text, "running 1m", "without health polling a live child is running")
        t |> equal(line.badge, "base")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = true, healthy = 1, up_minutes = 65))
        t |> equal(line.text, "serving - healthy 1h05m")
        t |> equal(line.badge, "base")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = true, healthy = 0, served = true, stage = "ready"))
        t |> equal(line.text, "unhealthy (ready)", "unhealthy only after it served once")
        t |> equal(line.badge, "red")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = true, healthy = 0, served = true))
        t |> equal(line.text, "unhealthy", "with no stage seen")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = true, healthy = 0, stage = "jit_codegen"))
        t |> equal(line.text, "starting up - compiling this box's kernels (a first start takes minutes)")
        t |> equal(line.badge, "amber")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = true, stage = "warp_drive"))
        t |> equal(line.text, "starting up - warp_drive", "an unknown stage shows its raw name")
        line = tray_status_line(TrayStatus(child_pid = 1, health_polled = true))
        t |> equal(line.text, "starting up", "healthy defaults to the before-the-first-poll sentinel")
    }
    t |> run("two folds compare without text, and a minute rolling over is a change") @(t : T?) {
        let a = TrayStatus(child_pid = 1, health_polled = true, healthy = 1, up_minutes = 5, stage = "ready")
        t |> success(same_status(a, TrayStatus(child_pid = 1, health_polled = true, healthy = 1, up_minutes = 5, stage = "ready")))
        t |> success(!same_status(a, TrayStatus(child_pid = 1, health_polled = true, healthy = 1, up_minutes = 6, stage = "ready")))
        t |> success(!same_status(a, TrayStatus(child_pid = 1, health_polled = true, healthy = 0, up_minutes = 5, stage = "ready")))
    }
}

[test]
def test_tune_fold(t : T?) {
    t |> run("plan opens the run, begin names the kernel, the last end closes it") @(t : T?) {
        var f = TuneFold()
        let plan <- {"scope" => "kernels", "total" => "2"}
        fold_tune_event(f, "plan", plan)
        t |> equal(f.total, 2)
        let first <- {"name" => "gemv"}
        fold_tune_event(f, "begin", first)
        t |> equal(f.kernel, "gemv")
        fold_tune_event(f, "step", first)
        fold_tune_event(f, "end", first)
        t |> equal(f.done, 1)
        t |> equal(f.kernel, "", "a finished kernel is not named as in flight")
        let second <- {"name" => "gemm"}
        fold_tune_event(f, "begin", second)
        fold_tune_event(f, "end", second)
        t |> equal(f.total, 0, "the run closed after the last kernel")
        let bad <- {"total" => "garbage"}
        fold_tune_event(f, "plan", bad)
        t |> equal(f.total, 0, "an unreadable total opens nothing")
    }
}

[test]
def test_url_origin(t : T?) {
    t |> run("the origin is the scheme and authority with a trailing slash") @(t : T?) {
        t |> equal(url_origin("http://127.0.0.1:8080/v1/models"), "http://127.0.0.1:8080/")
        t |> equal(url_origin("https://box.local"), "https://box.local/")
        t |> equal(url_origin("http://box.local?check=ready"), "http://box.local/", "a query with no path")
        t |> equal(url_origin("http://box.local#frag"), "http://box.local/", "a fragment with no path")
        t |> equal(url_origin("  http://box.local/x  "), "http://box.local/", "surrounding whitespace")
        t |> equal(url_origin("http://[::1]:8080/x"), "http://[::1]:8080/")
        t |> equal(url_origin("nonsense"), "")
        t |> equal(url_origin("http:///v1"), "")
    }
}

[test]
def test_tray_page_and_open(t : T?) {
    t |> run("the page comes from --tray-url, else the polled health URL's origin, else nowhere") @(t : T?) {
        t |> equal(tray_page_url("http://box:1/", "http://127.0.0.1:8080/v1/models", true), "http://box:1/")
        t |> equal(tray_page_url("", "http://127.0.0.1:8080/v1/models", true), "http://127.0.0.1:8080/")
        t |> equal(tray_page_url("", "http://127.0.0.1:8080/v1/models", false), "", "a page nobody checks is not offered")
        t |> equal(tray_page_url("http://box:1/", "http://127.0.0.1:8080/v1/models", false), "http://box:1/")
    }
    t |> run("Open is live while green, or while the child runs when health is not polled") @(t : T?) {
        let url = "http://box:1/"
        t |> success(tray_open_enabled(TrayStatus(child_pid = 1, health_polled = true, healthy = 1), url))
        t |> success(!tray_open_enabled(TrayStatus(child_pid = 1, health_polled = true, healthy = 0), url))
        t |> success(!tray_open_enabled(TrayStatus(child_pid = 1, health_polled = true), url), "not before the first poll")
        t |> success(tray_open_enabled(TrayStatus(child_pid = 1, health_polled = false), url))
        t |> success(!tray_open_enabled(TrayStatus(child_pid = 0, health_polled = false), url), "no child, no page")
        t |> success(!tray_open_enabled(TrayStatus(child_pid = 1, health_polled = false, stopping = true), url))
        t |> success(!tray_open_enabled(TrayStatus(child_pid = 1, health_polled = true, healthy = 1), ""), "no page, no Open")
    }
}

def pixel(px : array<uint8>; side, x, y : int) : int4 {
    let i = (y * side + x) * 4
    return int4(int(px[i]), int(px[i + 1]), int(px[i + 2]), int(px[i + 3]))
}

[test]
def test_tray_mark(t : T?) {
    t |> run("the disc, the moat and the badges land where the states put them") @(t : T?) {
        let base <- tray_mark(32, "base")
        t |> equal(length(base), 32 * 32 * 4)
        t |> success(pixel(base, 32, 16, 16) == int4(70, 130, 200, 255), "the disc at the center")
        t |> equal(pixel(base, 32, 0, 0).w, 0, "a transparent corner")
        t |> equal(pixel(base, 32, 24, 25).w, 255, "the plain mark has no moat")
        let amber <- tray_mark(32, "amber")
        t |> success(pixel(amber, 32, 24, 25) == int4(242, 183, 61, 255), "the triangle: {pixel(amber, 32, 24, 25)}")
        t |> equal(pixel(amber, 32, 17, 24).w, 0, "the moat clears the disc around the badge")
        let red <- tray_mark(32, "red")
        t |> success(pixel(red, 32, 24, 24) == int4(217, 109, 79, 255), "the square: {pixel(red, 32, 24, 24)}")
        t |> success(pixel(red, 32, 16, 16) == int4(70, 130, 200, 255), "the disc stays outside the moat")
    }
}

// The tray arms run the watchdog under the interpreter as a child of this test - the icon has
// to live in a process the desktop can see. Every arm ends through finish(), with the icon torn
// down: the park fixture leaves on the stop file, which the tray's Shutdown writes on Linux and
// the arms that cannot click write themselves; the crash-then-ok fixture leaves on its own.
def tray_log_path(root : string) : string {
    return path_join(root, "logs/wdtray-watchdog.log")
}

def tray_stop_file(root : string) : string {
    return path_join(root, "stop")
}

def spawn_tray_watchdog(root, url, mode : string; env : array<string>) : process {
    let state_dir = path_join(root, "state")
    mkdir(state_dir)
    var argv <- [das_exe(), "-dasroot", get_das_root(), path_join(get_das_root(), "utils/watchdog/main.das"), "--",
        "--tray", "--tray-url", url, "--program", das_exe(), "--name", "wdtray", "--cwd", root,
        "--no-health", "--stop-file", tray_stop_file(root), "--stable-seconds", "0.1", "--max-restart-delay", "0.5", "--"]
    argv |> push_from(child_args(state_dir, mode))
    return unsafe(spawn_process(argv, root, env))
}

//! `unavailable`, `ready` (the child runs under a live icon), or empty after 30 s.
def wait_for_tray(var wd : process; log_path : string) : string {
    let started = ref_time_ticks()
    while (get_time_usec(started) < 30000000) {
        unsafe(process_drain(wd) $(_line) {
        })
        let events <- read_events(log_path)
        if (count_event(events, "tray_unavailable") > 0) return "unavailable"
        if (count_event(events, "child_started") > 0) return "ready"
        sleep(50u)
    }
    return ""
}

//! The exit code once the watchdog ends, draining its stdout meanwhile - it echoes every log
//! line there, and a full pipe would park it; `process_running` past the deadline.
def wait_exit_draining(var wd : process; seconds : float) : int {
    let started = ref_time_ticks()
    while (float(get_time_usec(started)) < seconds * 1000000.0) {
        unsafe(process_drain(wd) $(_line) {
        })
        let rc = unsafe(process_poll(wd))
        if (rc != process_running) return rc
        sleep(20u)
    }
    return process_running
}

def with_tray_watchdog(t : T?; url, mode : string; env : array<string>; blk : block<(var wd : process; root : string; verdict : string) : void>) {
    let root = make_temp(t, "wd tray ")
    if (empty(root)) return
    var wd = spawn_tray_watchdog(root, url, mode, env)
    let verdict = wait_for_tray(wd, tray_log_path(root))
    if (empty(verdict)) {
        t |> failure("the watchdog neither started the tray nor gave it up within 30 s: {event_names(read_events(tray_log_path(root)))}")
    } else {
        invoke(blk, wd, root, verdict)
    }
    unsafe {
        close_process(wd)
    }
    // on a failed arm close_process kills the watchdog; Windows releases the handles on the
    // log and the state dir a moment later, so the removal retries
    var removed = false
    for (_attempt in range(40)) {
        let cleanup = rmdir_rec_result(root)
        removed = cleanup is value && cleanup as value
        if (removed || !fexist(root)) {
            removed = true
            break
        }
        sleep(50u)
    }
    t |> success(removed, "temp dir removed")
}

//! Ends a parked run the way an operator's Shutdown would on a platform the test cannot click.
def stop_parked_run(t : T?; var wd : process; root : string) {
    fwrite(tray_stop_file(root), "stop\n")
    t |> equal(wait_exit_draining(wd, 30.0), 0, "supervision ended with 0 once the child left on the stop file")
    let events <- read_events(tray_log_path(root))
    t |> equal(count_event(events, "intentional_shutdown"), 1)
    t |> equal(count_event(events, "watchdog_stopped"), 1)
}

[test]
def test_tray_shutdown_over_private_bus(t : T?) {
    if (!dbus_tools_present()) {
        t |> skip("no /usr/bin/dbus-daemon or dbus-send here: the Linux tray rides D-Bus")
        return
    }
    t |> run("the menu carries the status line, Open and Shutdown, and Shutdown ends supervision with 0") @(t : T?) {
        var bus <- spawn_session_bus()
        if (empty(bus.address)) {
            t |> failure("dbus-daemon printed no address")
        } else {
            let address = bus.address
            with_tray_watchdog(t, "http://127.0.0.1:1/", "park", ["DBUS_SESSION_BUS_ADDRESS={address}"]) $(var wd; root; verdict) {
                if (verdict == "unavailable") {
                    if (fexist("/usr/include/dbus-1.0/dbus/dbus.h")) {
                        t |> failure("the watchdog reports no tray although the dbus-1 headers are installed here: stddlg was built without LINUX_DBUS, or libdbus-1.so.3 failed to load")
                    } else {
                        t |> skip("this daslang has no Linux tray backend on this box (no libdbus-1-dev at build time)")
                    }
                    return
                }
                let dest = "org.kde.StatusNotifierItem-{unsafe(process_pid(wd))}-1"
                // the first refresh precedes the spawn, so the running line lands a tick later
                var layout = ""
                let started = ref_time_ticks()
                while (find(layout, "running") < 0 && get_time_usec(started) < 5000000) {
                    layout = dbus_send_to(address, dest, true, "/MenuBar", "com.canonical.dbusmenu.GetLayout", ["int32:0", "int32:-1", "array:string:"])
                    sleep(50u)
                }
                dump_has(t, layout, "string \"running ", "the status row")
                dump_has(t, layout, "string \"Open wdtray\"", "the Open item, named by --tray-url under --no-health")
                dump_has(t, layout, "string \"Shutdown\"", "the Shutdown item")
                let tooltip = dbus_send_to(address, dest, true, "/StatusNotifierItem", "org.freedesktop.DBus.Properties.Get", ["string:org.kde.StatusNotifierItem", "string:ToolTip"])
                dump_has(t, tooltip, "wdtray - running", "the tooltip carries the same line")
                dbus_send_to(address, dest, false, "/MenuBar", "com.canonical.dbusmenu.Event", ["int32:{TRAY_SHUTDOWN_ID}", "string:clicked", "variant:int32:0", "uint32:0"])
                t |> equal(wait_exit_draining(wd, 30.0), 0, "supervision ended with 0 after the tray's Shutdown")
                let events <- read_events(tray_log_path(root))
                t |> equal(count_event(events, "tray_started"), 1)
                t |> equal(count_event(events, "tray_shutdown_requested"), 1)
                t |> equal(count_event(events, "stop_file_requested"), 1, "Shutdown asks the parked child through the stop file")
                t |> equal(count_event(events, "watchdog_stopped"), 1)
            }
        }
        if (bus.bus != null) {
            unsafe {
                close_process(bus.bus)
            }
        }
    }
}

//! macOS: the status item's help text is the tooltip, read back through System Events.
[test]
def test_tray_status_item_macos(t : T?) {
    if (!fexist("/usr/bin/osascript")) {
        t |> skip("no /usr/bin/osascript here: this arm reads the macOS status item through System Events")
        return
    }
    t |> run("the status item's tooltip is the watchdog's status line") @(t : T?) {
        let enabled = apple_script("tell application \"System Events\" to get UI elements enabled")
        if (find(enabled, "true") < 0) {
            t |> skip("System Events reports accessibility off for this process, so it cannot reach the status item: {enabled}")
            return
        }
        let no_env : array<string>
        with_tray_watchdog(t, "", "park", no_env) $(var wd; root; verdict) {
            if (verdict == "unavailable") {
                t |> skip("no window server session for this process, so no status bar to put an item on")
                return
            }
            let pid = unsafe(process_pid(wd))
            var help = ""
            let started = ref_time_ticks()
            while (find(help, "running") < 0 && get_time_usec(started) < 5000000) {
                help = apple_script("tell application \"System Events\" to tell (first process whose unix id is {pid}) to get help of menu bar item 1 of menu bar 1")
                sleep(50u)
            }
            dump_has(t, help, "wdtray - running", "the tooltip")
            stop_parked_run(t, wd, root)
        }
    }
}

//! Windows has no scriptable route to the notification area: this arm proves the wiring reaches
//! `tray_started` on an interactive desktop and gives the icon up in session 0.
[test]
def test_tray_windows_smoke(t : T?) {
    if (!fexist("C:\\Windows\\explorer.exe")) {
        t |> skip("not Windows")
        return
    }
    t |> run("the watchdog shows its icon on an interactive desktop, or reports it unavailable") @(t : T?) {
        let no_env : array<string>
        with_tray_watchdog(t, "", "park", no_env) $(var wd; root; verdict) {
            if (verdict == "unavailable") {
                if (has_env_variable("SESSIONNAME")) {
                    t |> failure("the watchdog reports no tray although this is an interactive session (SESSIONNAME is set)")
                } else {
                    t |> skip("no interactive desktop for this process (session 0, SESSIONNAME unset)")
                }
                return
            }
            t |> equal(count_event(read_events(tray_log_path(root)), "tray_started"), 1)
            stop_parked_run(t, wd, root)
        }
    }
}

//! Supervision under --tray completes whether the icon exists - finish() tears it down and the
//! crash notification went through it - or not: the icon is never fatal.
[test]
def test_tray_supervision_completes(t : T?) {
    t |> run("crash-then-ok under --tray ends with 0 with or without an icon") @(t : T?) {
        let no_env : array<string>
        with_tray_watchdog(t, "", "crash-then-ok", no_env) $(var wd; root; verdict) {
            t |> equal(wait_exit_draining(wd, 60.0), 0, "supervision ended with 0 (tray {verdict})")
            let events <- read_events(tray_log_path(root))
            t |> equal(count_event(events, "child_started"), 2)
            t |> equal(count_event(events, "crash"), 1)
            t |> equal(count_event(events, "watchdog_stopped"), 1)
            t |> equal(count_event(events, "tray_started") + count_event(events, "tray_unavailable"), 1, "the icon came up or said why not: {event_names(events)}")
        }
    }
}

def event_number(events : array<JsonValue?>; name, key : string) : double {
    for (ev in events) {
        if ((ev?["event"] ?? "") == name) return ev?[key] ?? -1.0lf
    }
    return -1.0lf
}

//! The supervisor's own heaps stay bounded under a talkative child: every line becomes a log
//! record, and the host collects between ticks.
[test]
def test_heaps_stay_bounded_under_a_chatty_child(t : T?) {
    t |> run("twenty thousand child lines leave the string heap under two megabytes") @(t : T?) {
        let root = make_temp(t, "wd chatty ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        var argv <- [das_exe(), "-dasroot", get_das_root(), path_join(get_das_root(), "utils/watchdog/main.das"), "--",
            "--program", das_exe(), "--name", "wdchatty", "--cwd", root, "--no-health", "--"]
        argv |> push_from(child_args(state_dir, "chatty"))
        let no_env : array<string>
        var wd = unsafe(spawn_process(argv, root, no_env))
        t |> equal(wait_exit_draining(wd, 120.0), 0, "supervision ended with 0")
        unsafe {
            close_process(wd)
        }
        let events <- read_events(path_join(root, "logs/wdchatty-watchdog.log"))
        t |> equal(count_event(events, "child"), 20000, "every line reached the log")
        let strings = event_number(events, "watchdog_stopped", "string_heap_bytes")
        t |> success(strings > 0.0lf && strings < 2.0lf * 1024.0lf * 1024.0lf, "string heap at the end: {strings} bytes")
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_config_layering(t : T?) {
    t |> run("watchdog.json sets flag defaults, a flag on the command line wins, -- goes to the child") @(t : T?) {
        let root = make_temp(t, "wd config ")
        if (empty(root)) return
        fwrite(path_join(root, "watchdog.json"),
            "\{\"name\": \"fromjson\", \"health_interval\": 7, \"no_health\": true, \"server_args\": [\"a\", \"b\"]\}")
        let resolved <- resolve_config(["--name", "fromcli", "--program", das_exe()], root)
        t |> equal(resolved.error, "")
        t |> equal(resolved.cfg.name, "fromcli", "the flag beats the config")
        t |> equal(resolved.cfg.health_interval, 7.0, "the config sets a default")
        t |> success(resolved.cfg.no_health, "a true bool in the config sets the flag")
        t |> equal(length(resolved.cfg.server_args), 2, "server_args default from the config: {resolved.cfg.server_args}")
        let with_tail <- resolve_config(["--program", das_exe(), "--", "-x", "child arg"], root)
        t |> equal(with_tail.error, "")
        t |> equal(length(with_tail.cfg.server_args), 2)
        if (length(with_tail.cfg.server_args) == 2) {
            t |> equal(with_tail.cfg.server_args[0], "-x", "flag-shaped child args survive")
            t |> equal(with_tail.cfg.server_args[1], "child arg")
        }
        t |> equal(with_tail.cfg.name, "fromjson", "the config's name applies when no flag names one")
        t |> success(ends_with(with_tail.cfg.log, "fromjson-watchdog.log"), "log path keyed on the name: {with_tail.cfg.log}")
        fwrite(path_join(root, "watchdog.json"), "\{\"helth_url\": \"x\"\}")
        let typo <- resolve_config(["--program", das_exe()], root)
        t |> success(find(typo.error, "helth_url") >= 0, "an unknown key is fatal and named: {typo.error}")
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_discovery(t : T?) {
    t |> run("main.das beside bin/Release/daslang means script mode; two exes are ambiguous; nothing is an error") @(t : T?) {
        let root = make_temp(t, "wd discover ")
        if (empty(root)) return
        mkdir(path_join(root, "bin"))
        mkdir(path_join(root, "bin/Release"))
        fwrite(path_join(root, "main.das"), "")
        fwrite(path_join(root, "bin/Release/daslang"), "")
        let script_mode <- resolve_config(["--no-health"], root)
        t |> equal(script_mode.error, "")
        t |> success(ends_with(script_mode.cfg.script, "main.das"), "script discovered: {script_mode.cfg.script}")
        t |> success(ends_with(replace(script_mode.cfg.daslang, "\\", "/"), "bin/Release/daslang"), "the daslang beside it is the one discovered, not the bare fallback: {script_mode.cfg.daslang}")
        t |> equal(script_mode.cfg.name, base_name(root), "a script names itself after its directory")
        let command <- child_command(script_mode.cfg)
        t |> success(length(command) >= 4 && command[1] == "-jit" && command[3] == "--", "script command shape: {command}")
        remove(path_join(root, "bin/Release/daslang"))
        mkdir(path_join(root, "tree"))
        mkdir(path_join(root, "tree/bin"))
        fwrite(path_join(root, "tree/bin/daslang"), "")
        let in_tree <- resolve_config(["--no-health", "--cwd", root], path_join(root, "tree/bin"))
        t |> equal(in_tree.error, "")
        t |> success(ends_with(replace(in_tree.cfg.daslang, "\\", "/"), "tree/bin/daslang"), "no daslang beside the script: the one beside the watchdog serves it: {in_tree.cfg.daslang}")
        remove(path_join(root, "tree/bin/daslang"))
        let alone <- resolve_config(["--no-health", "--cwd", root], path_join(root, "tree"))
        t |> success(find(alone.error, "nothing to supervise") >= 0, "main.das with no daslang in reach is refused: {alone.error}")
        remove(path_join(root, "main.das"))
        fwrite(path_join(root, "one.exe"), "")
        fwrite(path_join(root, "two.exe"), "")
        let ambiguous <- resolve_config([], root)
        t |> success(find(ambiguous.error, "2 executables") >= 0, "two exes are refused: {ambiguous.error}")
        remove(path_join(root, "two.exe"))
        fwrite(path_join(root, "watchdog.exe"), "")
        let program_mode <- resolve_config([], root)
        t |> equal(program_mode.error, "")
        t |> equal(program_mode.cfg.name, "one", "an executable names itself, and the watchdog's own exe is never the candidate")
        remove(path_join(root, "one.exe"))
        remove(path_join(root, "watchdog.exe"))
        let nothing <- resolve_config([], root)
        t |> success(find(nothing.error, "nothing to supervise") >= 0, "an empty directory is an error: {nothing.error}")
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
    t |> run("a daspkg bundle on a Unix layout: the extensionless exe beside the watchdog, its config, its assets and a README") @(t : T?) {
        let root = make_temp(t, "wd bundle ")
        if (empty(root)) return
        let windows = get_platform_name() == "windows"
        for (exe in ["dasllama-server", "watchdog"]) {
            fwrite(path_join(root, exe), "")
            if (!windows) {
                var out = ""
                run_and_capture(["chmod", "755", path_join(root, exe)], out, 10.0)
            }
        }
        fwrite(path_join(root, "watchdog.json"), "\{\"no_health\": true\}")
        fwrite(path_join(root, "control.html"), "")
        fwrite(path_join(root, "README"), "")
        fwrite(path_join(root, ".daspkg_release.manifest"), "")
        mkdir(path_join(root, "modules"))
        let bundle <- resolve_config([], root)
        if (windows) {
            t |> success(find(bundle.error, "nothing to supervise") >= 0, "an extensionless file is not a program on Windows: {bundle.error}")
        } else {
            t |> equal(bundle.error, "")
            t |> success(ends_with(bundle.cfg.program, "dasllama-server"), "the extensionless executable is the program, the README beside it is not: {bundle.cfg.program}")
            t |> equal(bundle.cfg.name, "dasllama-server", "and names the supervision")
            fwrite(path_join(root, "dasllama-bench"), "")
            var out = ""
            run_and_capture(["chmod", "755", path_join(root, "dasllama-bench")], out, 10.0)
            let two <- resolve_config([], root)
            t |> success(find(two.error, "2 executables") >= 0, "two extensionless executables are ambiguous: {two.error}")
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_tray_icon(t : T?) {
    t |> run("the dasllama mark's ICO decodes to its largest PNG frame, and the badge lands on it") @(t : T?) {
        let path = path_join(get_das_root(), "utils/dasllama-server/tray.ico")
        var icon <- tray_icon_load(path)
        t |> equal(icon.error, "", "the ICO loads")
        t |> equal(icon.side, 48, "the largest frame within {TRAY_ICON_FRAME_MAX} px is the 48")
        t |> equal(length(icon.rgba), 48 * 48 * 4)
        var opaque = 0
        for (i in range(48 * 48)) {
            if (icon.rgba[i * 4 + 3] == 255u8) {
                opaque++
            }
        }
        t |> success(opaque > 0 && opaque < 48 * 48, "the mark has opaque pixels and transparent ones: {opaque}")
        var red := icon.rgba
        tray_badge(red, 48, "red")
        t |> success(pixel(red, 48, 36, 36) == int4(217, 109, 79, 255), "the square sits at the lower right: {pixel(red, 48, 36, 36)}")
        t |> equal(pixel(red, 48, 25, 36).w, 0, "the moat clears the mark around the badge")
        t |> success(pixel(red, 48, 10, 10) == pixel(icon.rgba, 48, 10, 10), "the mark stays outside the moat")
        var base := icon.rgba
        tray_badge(base, 48, "base")
        var changed = 0
        for (b, o in base, icon.rgba) {
            if (b != o) {
                changed++
            }
        }
        t |> equal(changed, 0, "the base badge leaves the mark alone")
    }
    t |> run("a file that is not an image, and a file that is not there, name the reason") @(t : T?) {
        let text <- tray_icon_load(path_join(get_das_root(), "utils/dasllama-server/watchdog.json"))
        t |> success(find(text.error, "neither a PNG nor an ICO") >= 0, "{text.error}")
        t |> equal(text.side, 0)
        let missing <- tray_icon_load(path_join(get_das_root(), "utils/dasllama-server/no-such.ico"))
        t |> success(find(missing.error, "cannot read") >= 0, "{missing.error}")
    }
}

[test]
def test_stages_are_ranked_and_monotonic(t : T?) {
    t |> run("only a forward move reports; the tune restart rewinds the rank") @(t : T?) {
        var detector = new StageDetector()
        var stage = Stage(name = "", rank = 0, since = 0.0)
        var moved = detector->advance(stage, "LLVM JIT: DLL cache miss, codegen for foo.dll", 1.0)
        t |> success(moved != null && (moved?["stage"] ?? "") == "jit_codegen", "first move: jit_codegen")
        moved = detector->advance(stage, "Library foo linked - ok", 2.5)
        t |> success(moved != null && (moved?["stage"] ?? "") == "jit_linked", "forward move: jit_linked")
        t |> equal(moved?["after"] ?? "", "jit_codegen", "the previous stage is named")
        t |> equal(float(moved?["elapsed_s"] ?? 0.0lf), 1.5, "and how long it took")
        moved = detector->advance(stage, "LLVM JIT: DLL cache miss, codegen for bar.dll", 3.0)
        t |> success(moved == null, "a lower-ranked line mid-tune does not flap the stage back")
        moved = detector->advance(stage, "llvm_tune: restart to apply the winners", 4.0)
        t |> success(moved != null && (moved?["stage"] ?? "") == "tune_restart", "the restart marker is a move")
        t |> equal(stage.rank, 0, "and it resets the rank so the sequence can replay")
        moved = detector->advance(stage, "LLVM JIT: DLL cache hit foo.dll", 5.0)
        t |> success(moved != null && (moved?["stage"] ?? "") == "jit_cached", "the replay advances again")
        t |> success(detector->advance(stage, "nothing to see here", 6.0) == null, "an unmatched line is not a stage")
        unsafe {
            delete detector
        }
    }
}

[test]
def test_parse_child_event(t : T?) {
    t |> run("@tune <kind> k=v ... splits into a kind and fields; other lines do not") @(t : T?) {
        let ev <- parse_child_event("@tune plan scope=metal total=12", "@tune ")
        t |> equal(ev.kind, "plan")
        t |> equal(ev.fields?["scope"] ?? "", "metal")
        t |> equal(ev.fields?["total"] ?? "", "12")
        let none <- parse_child_event("plain child output", "@tune ")
        t |> equal(none.kind, "")
    }
}

[test]
def test_emitter_envelope_and_rotation(t : T?) {
    t |> run("one JSON object per line with ts and event, fields beside them; the file rotates at the size cap") @(t : T?) {
        let root = make_temp(t, "wd log ")
        if (empty(root)) return
        let log_path = path_join(root, "logs/x-watchdog.log")
        var log = new Emitter(log_path)
        log.tee = false
        log->emit("hello", JV((pid = 42, note = "n")))
        let events <- read_events(log_path)
        t |> equal(length(events), 1)
        if (length(events) == 1) {
            t |> equal(events[0]?["event"] ?? "", "hello")
            t |> equal(int(events[0]?["pid"] ?? 0l), 42)
            t |> equal(events[0]?["note"] ?? "", "n")
            t |> success(length(events[0]?["ts"] ?? "") >= 19, "an ISO timestamp: {events[0]?["ts"] ?? ""}")
        }
        log.max_bytes = 1l
        log->emit("second")
        t |> success(fexist("{log_path}.1"), "the full file rotated to .1")
        let after <- read_events(log_path)
        t |> equal(length(after), 1, "the live file holds only the record written after rotation")
        unsafe {
            delete log
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_supervise_crash_then_clean_exit(t : T?) {
    t |> run("a crash is reported, bundled and restarted with backoff; exit 0 ends supervision") @(t : T?) {
        notifications_enabled = false
        let root = make_temp(t, "wd crash ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        // the child names a cached JIT DLL under the (cwd-relative) jit dir; the bundle must
        // copy it and its siblings under jitted_scripts/ keeping the relative layout
        mkdir(path_join(root, ".jitted_scripts"))
        mkdir(path_join(root, ".jitted_scripts/sub"))
        fwrite(path_join(root, ".jitted_scripts/sub/fake.dll"), "dll")
        fwrite(path_join(root, ".jitted_scripts/sub/fake.pdb"), "pdb")
        var code = 0
        var sup = watchdog_start(program_mode_args(root, state_dir, "crash-then-ok"), root, 0, code)
        t |> success(sup != null, "supervision started (code {code})")
        if (sup == null) return
        t |> equal(run_bounded(t, sup, 60.0), 0, "supervision ends with 0 after the child's clean exit")
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "child_started"), 2, "the child ran twice: {event_names(events)}")
        t |> equal(event_int(events, "crash", "code"), 9, "the first exit was reported as a crash")
        t |> equal(count_event(events, "crash_bundle"), 1, "one bundle collected")
        t |> equal(event_int(events, "child_exited", "code", 1), 0, "the second exit was clean")
        t |> equal(count_event(events, "intentional_shutdown"), 1)
        t |> equal(count_event(events, "watchdog_stopped"), 1)
        t |> equal(count_event(events, "stage"), 4, "the JIT line and the ready line moved the stage on both runs")
        t |> equal(count_event(events, "tray_started") + count_event(events, "tray_unavailable"), 0, "no --tray, no icon and no attempt at one")
        var bundles = 0
        dir(sup.cfg.crash_dir) $(fname) {
            if (starts_with(fname, "wdtest-")) {
                bundles++
                let bundle = path_join(sup.cfg.crash_dir, fname)
                t |> success(fexist(path_join(bundle, "crash.json")), "crash.json in the bundle")
                t |> success(fexist(path_join(bundle, "jitted_scripts/sub/fake.dll")), "the named JIT DLL is in the bundle under its relative path")
                t |> success(fexist(path_join(bundle, "jitted_scripts/sub/fake.pdb")), "and its sibling artifacts beside it")
            }
        }
        t |> equal(bundles, 1)
        unsafe {
            delete sup
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_supervise_config_restart(t : T?) {
    t |> run("exit 4 relaunches at once with no crash report") @(t : T?) {
        notifications_enabled = false
        let root = make_temp(t, "wd config restart ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        var code = 0
        var sup = watchdog_start(program_mode_args(root, state_dir, "config-restart"), root, 0, code)
        t |> success(sup != null, "supervision started (code {code})")
        if (sup == null) return
        t |> equal(run_bounded(t, sup, 60.0), 0)
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "config_restart_relaunch"), 1, "{event_names(events)}")
        t |> equal(count_event(events, "crash"), 0, "no crash was reported")
        t |> equal(count_event(events, "child_started"), 2)
        unsafe {
            delete sup
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_supervise_tune_bootstrap(t : T?) {
    t |> run("script mode: exit 3 after the restart marker relaunches at once") @(t : T?) {
        if (skip_without_jit(t)) return
        notifications_enabled = false
        let root = make_temp(t, "wd tune ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        let args <- ["--daslang", das_exe(), "--script", fixture_path(), "--name", "wdtune", "--cwd", root,
            "--no-health", "--stable-seconds", "0.1", "--max-restart-delay", "0.5", "--", state_dir, "tune-then-ok"]
        var code = 0
        var sup = watchdog_start(args, root, 0, code)
        t |> success(sup != null, "supervision started (code {code})")
        if (sup == null) return
        t |> equal(run_bounded(t, sup, 60.0), 0)
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "tune_bootstrap_complete"), 1, "{event_names(events)}")
        t |> equal(count_event(events, "crash"), 0)
        t |> equal(count_event(events, "child_started"), 2)
        unsafe {
            delete sup
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_supervise_tune_abort_backs_off(t : T?) {
    t |> run("script mode: exit 3 without the restart marker is a tuner abort - relaunched with backoff, not a crash") @(t : T?) {
        if (skip_without_jit(t)) return
        notifications_enabled = false
        let root = make_temp(t, "wd tune abort ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        let args <- ["--daslang", das_exe(), "--script", fixture_path(), "--name", "wdabort", "--cwd", root,
            "--no-health", "--stable-seconds", "0.1", "--max-restart-delay", "0.5", "--", state_dir, "tune-abort"]
        var code = 0
        var sup = watchdog_start(args, root, 0, code)
        t |> success(sup != null, "supervision started (code {code})")
        if (sup == null) return
        t |> equal(run_bounded(t, sup, 60.0), 0)
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "tune_incomplete"), 1, "{event_names(events)}")
        t |> equal(count_event(events, "tune_bootstrap_complete"), 0, "no marker, no bootstrap")
        t |> equal(count_event(events, "crash"), 0, "an abort is not a crash")
        t |> equal(count_event(events, "child_started"), 2)
        unsafe {
            delete sup
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_request_stop_terminates_with_no_shutdown(t : T?) {
    t |> run("--no-shutdown: the stop request terminates the child instead of asking it") @(t : T?) {
        notifications_enabled = false
        let root = make_temp(t, "wd stop terminate ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        var args <- ["--program", das_exe(), "--name", "wdterm", "--cwd", root, "--no-health", "--no-shutdown", "--"]
        args |> push_from(child_args(state_dir, "park"))
        var code = 0
        var sup = watchdog_start(args, root, 0, code)
        t |> success(sup != null, "supervision started (code {code})")
        if (sup == null) return
        var parked = false
        let started = ref_time_ticks()
        while (!parked && get_time_usec(started) < 15000000) {
            sup->tick()
            for (ev in read_events(sup.cfg.log)) {
                if ((ev?["event"] ?? "") == "child" && (ev?["message"] ?? "") == "parked") {
                    parked = true
                }
            }
        }
        t |> success(parked, "the child reported it was parked")
        sup->request_stop()
        var done = false
        let stop_started = ref_time_ticks()
        while (!done && get_time_usec(stop_started) < 15000000) {
            done = sup->tick()
        }
        t |> success(done, "supervision ended after the stop request")
        t |> equal(sup.result, 0, "a stop the host asked for ends with 0 whatever the child's code")
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "terminate_requested"), 1, "{event_names(events)}")
        t |> equal(count_event(events, "stop_file_requested"), 0)
        t |> equal(count_event(events, "shutdown_requested"), 0)
        unsafe {
            delete sup
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test]
def test_request_stop_uses_the_stop_file(t : T?) {
    t |> run("the host's stop request writes the stop file; the parked child exits 0 and supervision ends") @(t : T?) {
        notifications_enabled = false
        let root = make_temp(t, "wd stop ")
        if (empty(root)) return
        let state_dir = path_join(root, "state")
        mkdir(state_dir)
        var args <- ["--program", das_exe(), "--name", "wdpark", "--cwd", root, "--no-health",
            "--stop-file", path_join(root, "stop"), "--"]
        args |> push_from(child_args(state_dir, "park"))
        var code = 0
        var sup = watchdog_start(args, root, 0, code)
        t |> success(sup != null, "supervision started (code {code})")
        if (sup == null) return
        var parked = false
        let started = ref_time_ticks()
        while (!parked && get_time_usec(started) < 15000000) {
            sup->tick()
            for (ev in read_events(sup.cfg.log)) {
                if ((ev?["event"] ?? "") == "child" && (ev?["message"] ?? "") == "parked") {
                    parked = true
                }
            }
        }
        t |> success(parked, "the child reported it was parked")
        sup->request_stop()
        var done = false
        let stop_started = ref_time_ticks()
        while (!done && get_time_usec(stop_started) < 15000000) {
            done = sup->tick()
        }
        t |> success(done, "supervision ended after the stop request")
        t |> equal(sup.result, 0)
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "stop_file_requested"), 1, "{event_names(events)}")
        t |> equal(event_int(events, "child_exited", "code"), 0, "the child left through its stop-file handshake")
        t |> success(!fexist(path_join(root, "stop")), "the stop file is cleared at the end")
        unsafe {
            delete sup
        }
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}
