options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require ../../utils/watchdog/watchdog.das

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
        t |> success(ends_with(script_mode.cfg.daslang, "daslang"), "daslang discovered beside it: {script_mode.cfg.daslang}")
        t |> equal(script_mode.cfg.name, base_name(root), "a script names itself after its directory")
        let command <- child_command(script_mode.cfg)
        t |> success(length(command) >= 4 && command[1] == "-jit" && command[3] == "--", "script command shape: {command}")
        remove(path_join(root, "main.das"))
        fwrite(path_join(root, "one.exe"), "")
        fwrite(path_join(root, "two.exe"), "")
        let ambiguous <- resolve_config([], root)
        t |> success(find(ambiguous.error, "2 executables") >= 0, "two exes are refused: {ambiguous.error}")
        remove(path_join(root, "two.exe"))
        let program_mode <- resolve_config([], root)
        t |> equal(program_mode.error, "")
        t |> equal(program_mode.cfg.name, "one", "an executable names itself")
        remove(path_join(root, "one.exe"))
        let nothing <- resolve_config([], root)
        t |> success(find(nothing.error, "nothing to supervise") >= 0, "an empty directory is an error: {nothing.error}")
        let cleanup = rmdir_rec_result(root)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
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
        t |> equal(sup->run(), 0, "supervision ends with 0 after the child's clean exit")
        let events <- read_events(sup.cfg.log)
        t |> equal(count_event(events, "child_started"), 2, "the child ran twice: {event_names(events)}")
        t |> equal(event_int(events, "crash", "code"), 9, "the first exit was reported as a crash")
        t |> equal(count_event(events, "crash_bundle"), 1, "one bundle collected")
        t |> equal(event_int(events, "child_exited", "code", 1), 0, "the second exit was clean")
        t |> equal(count_event(events, "intentional_shutdown"), 1)
        t |> equal(count_event(events, "watchdog_stopped"), 1)
        t |> equal(count_event(events, "stage"), 4, "the JIT line and the ready line moved the stage on both runs")
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
        t |> equal(sup->run(), 0)
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
        t |> equal(sup->run(), 0)
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
