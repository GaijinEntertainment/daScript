options gen2
options indenting = 4

module watchdog shared

require strings
require math
require daslib/strings_boost
require daslib/fio
require daslib/clargs
require daslib/result
require daslib/json_boost
require daslib/regex
require daslib/regex_boost
require dashv/dashv_boost

//! Supervise a daslang program (`daslang -jit main.das`) or a standalone executable: restart it with
//! bounded backoff, capture crashes into bundles, report startup stages, poll health. Exit 0 is an
//! intentional shutdown, 3 a tune bootstrap (relaunched at once when the child printed the restart
//! marker), 4 a config restart, anything else a crash. The host drives `Supervisor.tick()` on its
//! own clock and calls `request_stop()` on a signal; `watchdog_main` runs that loop for a host that
//! owns nothing else.

let public TUNE_RESTART_EXIT = 3
let public CONFIG_RESTART_EXIT = 4
let public CONFIG_NAME = "watchdog.json"
let private HEALTH_HEARTBEAT_SECONDS = 300.0
let private TICK_MS = 250u
let private LOG_MAX_BYTES = 20l * 1024l * 1024l
let private LOG_BACKUPS = 5
let private TUNE_EVENT_PREFIX = "@tune "

// ---------------------------------------------------------------------------------------------
// configuration: flags, watchdog.json beside the program, layout discovery, identity

[CommandLineArgs]
struct public WatchdogConfig {
    @clarg_doc = "Identity the log, pid file and notifications key on; derived from the program"
    name : string
    @clarg_doc = "Run this standalone program instead of daslang -jit <script>; relative paths use --cwd"
    program : string
    @clarg_doc = "daslang binary for script mode; discovered beside the program when unset"
    daslang : string
    @clarg_doc = "Script for daslang -jit; discovered as main.das beside the program when unset"
    script : string
    @clarg_doc = "Directory the child runs in; defaults to the watchdog's own directory"
    cwd : string
    @clarg_doc = "JSON-lines log; defaults to logs/<name>-watchdog.log under --cwd"
    log : string
    @clarg_doc = "Single-instance pid file; defaults to logs/<name>-watchdog.pid under --cwd"
    pid_file : string
    @clarg_doc = "GET here every --health-interval seconds; 2xx is healthy"
    health_url : string = "http://127.0.0.1:8080/v1/models"
    @clarg_doc = "POST here to ask the child to stop"
    shutdown_url : string = "http://127.0.0.1:8080/shutdown"
    @clarg_doc = "Do not poll health"
    no_health : bool
    @clarg_doc = "Terminate instead of POSTing the shutdown url"
    no_shutdown : bool
    @clarg_doc = "Create this file to request a lifecycle-safe standalone shutdown"
    stop_file : string
    @clarg_doc = "Environment variable that carries --stop-file to the child"
    stop_env : string = "CADMUS_STOP_FILE"
    @clarg_doc = "Seconds to wait after a shutdown request before terminating"
    stop_timeout : float = 1200.0
    @clarg_doc = "Seconds between health polls"
    health_interval : float = 5.0
    @clarg_doc = "Uptime after which the restart backoff resets"
    stable_seconds : float = 300.0
    @clarg_doc = "Cap on the exponential restart delay"
    max_restart_delay : float = 60.0
    @clarg_doc = "Pass -jit-stack to daslang so every generated call has a logical stack frame"
    jit_stack : bool
    @clarg_doc = "Expected WER LocalDumps output folder"
    dump_dir : string = "logs/dumps"
    @clarg_doc = "Install an app-specific WER minidump policy and exit (Windows, elevated)"
    install_local_dumps : bool
    @clarg_doc = "Dumps WER keeps"
    dump_count : int = 10
    @clarg_doc = "Refuse to start unless a WER minidump policy is active"
    require_dumps : bool
    @clarg_doc = "Crash bundle destination"
    crash_dir : string = "logs/crashes"
    @clarg_doc = "Crash bundles kept"
    crash_bundles : int = 10
    @clarg_doc = "JIT artifact cache copied into each crash bundle"
    jit_dir : string = ".jitted_scripts"
    @clarg_doc = "Do not collect crash bundles"
    no_crash_bundle : bool
    @clarg_skip
    server_args : array<string>
}

def private resolve_from(path, base : string) : string {
    return get_full_file_name(is_absolute(path) ? path : path_join(base, path))
}

//! The config keys are the flag names with underscores; a key sets that flag's default, so a flag
//! on the command line still wins. An unknown key is fatal: a typo must not supervise the program
//! with the wrong wiring and say nothing.
def private config_to_args(config : JsonValue?; cli : array<string>; source : string) : Result<array<string>; string> {
    var out : array<string>
    if (config == null) {
        out |> push_from(cli)
        return <- move_ok(out, type<string>)
    }
    let info <- get_command_info(type<WatchdogConfig>)
    let known <- {for (arg in info.args); arg.field_name => 0}
    for (key, val in keys(config as _object), values(config as _object)) {
        if (key == "server_args") continue
        if (!(known |> key_exists(key))) {
            var names <- [for (arg in info.args); arg.field_name]
            names |> sort()
            return <- err("{source} has unknown key '{key}'; known keys: {join(names, ", ")}", type<array<string>>)
        }
        // the config's value goes first and the command line's copy after it: clargs keeps the
        // last occurrence, so a flag on the command line wins without a lookup here
        let flag = "--" + replace(key, "_", "-")
        if (val is _bool) {
            if (val as _bool) {
                out |> push(flag)
            }
        } elif (val is _string) {
            out |> push(flag)
            out |> push(val as _string)
        } elif (val is _longint) {
            out |> push(flag)
            out |> push("{val as _longint}")
        } elif (val is _number) {
            out |> push(flag)
            out |> push("{val as _number}")
        } elif (!(val is _null)) {
            return <- err("{source}: key '{key}' must be a string, number or bool", type<array<string>>)
        }
    }
    out |> push_from(cli)
    return <- move_ok(out, type<string>)
}

//! The config's `server_args` array, the default for what follows `--` on the command line.
def private config_server_args(config : JsonValue?) : array<string> {
    var out : array<string>
    if (config == null) return <- out
    let val = config?["server_args"]
    if (val == null || !(val is _array)) return <- out
    out |> reserve(length(val as _array))
    for (item in val as _array) {
        out |> push(item is _string ? item as _string : write_json(item))
    }
    return <- out
}

struct private LoadedConfig {
    js : JsonValue?
    error : string
}

def private load_config(folder : string) : LoadedConfig {
    let path = path_join(folder, CONFIG_NAME)
    if (!fexist(path)) return LoadedConfig()
    var text : string
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    var error : string
    var js = read_json(text, error)
    if (js == null) return LoadedConfig(error = "watchdog: {path} is not valid JSON: {error}")
    if (!(js is _object)) return LoadedConfig(error = "watchdog: {path} must hold a JSON object")
    return LoadedConfig(js = js)
}

//! What to supervise, from the layout: `main.das` means script mode when a daslang sits beside it
//! (`bin/Release/daslang(.exe)`) or beside the watchdog itself (a source tree keeps both in `bin/`);
//! exactly one `*.exe` means that program. Ambiguity is an error, never a guess.
def private discover_target(var cfg : WatchdogConfig; folder, program_dir : string) : string {
    let script = path_join(folder, "main.das")
    if (fexist(script)) {
        if (!empty(cfg.daslang)) {
            cfg.script = script
            return ""
        }
        let exe = find_daslang(daslang_roots(program_dir, folder))
        if (fexist(exe)) {
            cfg.script = script
            cfg.daslang = exe
            return ""
        }
    }
    var exes : array<string>
    dir(folder) $(fname) {
        if (ends_with(fname, ".exe")) {
            exes |> push(path_join(folder, fname))
        }
    }
    if (length(exes) == 1) {
        cfg.program = exes[0]
        if (empty(cfg.name)) {
            cfg.name = stem(exes[0])
        }
        return ""
    }
    if (length(exes) > 1) {
        return "watchdog: {folder} holds {length(exes)} executables; pick one with --program or {CONFIG_NAME}"
    }
    return "watchdog: nothing to supervise in {folder} - expected a single *.exe, or main.das with a daslang beside it or beside the watchdog. Use --program/--script or ship {CONFIG_NAME}."
}

//! Where a daslang may sit, nearest first: beside the script, beside the watchdog, and one level
//! up from the watchdog - `bin/watchdog` in a source tree finds `bin/daslang` through its parent.
def private daslang_roots(program_dir, cwd : string) : array<string> {
    return <- [cwd, program_dir, dir_name(program_dir)]
}

def private find_daslang(dirs : array<string>) : string {
    for (root in dirs) {
        for (rel in ["bin/Release/daslang.exe", "bin/daslang.exe", "bin/Release/daslang", "bin/daslang", "build/daslang"]) {
            let candidate = path_join(root, rel)
            if (fexist(candidate)) return candidate
        }
    }
    return get_platform_name() == "windows" ? "daslang.exe" : "daslang"
}

struct public ResolvedConfig {
    cfg : WatchdogConfig = WatchdogConfig()
    error : string
}

//! Flags over `watchdog.json` beside the program over layout discovery; every path then resolves
//! against `cwd`. Everything after the first `--` goes to the child verbatim (the config's
//! `server_args` when there is no `--`). `error` carries the text of a fatal configuration.
def public resolve_config(args : array<string>; program_dir : string) : ResolvedConfig {
    let loaded = load_config(program_dir)
    if (!empty(loaded.error)) return ResolvedConfig(error = loaded.error)
    var head : array<string>
    var tail : array<string>
    var past_separator = false
    for (arg in args) {
        if (!past_separator && arg == "--") {
            past_separator = true
            continue
        }
        if (past_separator) {
            tail |> push(arg)
        } else {
            head |> push(arg)
        }
    }
    var merged <- config_to_args(loaded.js, head, path_join(program_dir, CONFIG_NAME))
    if (merged |> is_err) return ResolvedConfig(error = merged |> unwrap_err)
    var parsed <- parse_args(type<WatchdogConfig>, merged |> move_unwrap)
    if (parsed |> is_err) return ResolvedConfig(error = "watchdog: {parsed |> unwrap_err}")
    var cfg <- parsed |> move_unwrap
    if (past_separator) {
        cfg.server_args <- tail
    } else {
        cfg.server_args <- config_server_args(loaded.js)
    }
    if (empty(cfg.cwd)) {
        cfg.cwd = program_dir
    }
    cfg.cwd = get_full_file_name(cfg.cwd)
    if (empty(cfg.program) && empty(cfg.script)) {
        let why = discover_target(cfg, cfg.cwd, program_dir)
        if (!empty(why)) return ResolvedConfig(error = why)
    }
    if (empty(cfg.name)) {
        cfg.name = !empty(cfg.program) ? stem(cfg.program) : base_name(cfg.cwd)
    }
    if (empty(cfg.daslang)) {
        cfg.daslang = find_daslang(daslang_roots(program_dir, cfg.cwd))
    }
    if (empty(cfg.log)) {
        cfg.log = "logs/{cfg.name}-watchdog.log"
    }
    if (empty(cfg.pid_file)) {
        cfg.pid_file = "logs/{cfg.name}-watchdog.pid"
    }
    cfg.log = resolve_from(cfg.log, cfg.cwd)
    cfg.pid_file = resolve_from(cfg.pid_file, cfg.cwd)
    cfg.dump_dir = resolve_from(cfg.dump_dir, cfg.cwd)
    cfg.crash_dir = resolve_from(cfg.crash_dir, cfg.cwd)
    cfg.jit_dir = resolve_from(cfg.jit_dir, cfg.cwd)
    if (!empty(cfg.script)) {
        cfg.script = resolve_from(cfg.script, cfg.cwd)
    }
    if (!empty(cfg.program)) {
        cfg.program = resolve_from(cfg.program, cfg.cwd)
    }
    if (!empty(cfg.stop_file)) {
        cfg.stop_file = resolve_from(cfg.stop_file, cfg.cwd)
    }
    let daslang_candidate = resolve_from(cfg.daslang, cfg.cwd)
    if (fexist(daslang_candidate)) {
        cfg.daslang = daslang_candidate
    }
    return <- ResolvedConfig(cfg <- cfg)
}

//! The child's argv: the program, or `daslang -jit [-jit-stack] <script> --`, then the server args.
def public child_command(cfg : WatchdogConfig) : array<string> {
    var command : array<string>
    if (!empty(cfg.program)) {
        command |> push(cfg.program)
    } else {
        command |> push(cfg.daslang)
        command |> push("-jit")
        if (cfg.jit_stack) {
            command |> push("-jit-stack")
        }
        command |> push(cfg.script)
        command |> push("--")
    }
    command |> push_from(cfg.server_args)
    return <- command
}

// ---------------------------------------------------------------------------------------------
// the log: one JSON object per line, {"ts","event",...}, to a rotating file and stdout

def private mkdir_p(path : string) {
    if (empty(path) || fexist(path)) return
    mkdir_p(dir_name(path))
    mkdir(path)
}

class public Emitter {
    path : string
    tee : bool = true
    max_bytes : int64 = LOG_MAX_BYTES

    def Emitter(log_path : string) {
        path = log_path
        mkdir_p(dir_name(path))
    }

    def private rotate() {
        for (k in range(LOG_BACKUPS - 1)) {
            let i = LOG_BACKUPS - 1 - k
            let older = "{path}.{i}"
            if (fexist(older)) {
                let target = "{path}.{i + 1}"
                if (fexist(target)) {
                    remove(target)
                }
                rename(older, target)
            }
        }
        let first = "{path}.1"
        if (fexist(first)) {
            remove(first)
        }
        rename(path, first)
    }

    //! `fields` is an object whose keys land beside `ts` and `event`; a `ts`/`event` key in it
    //! would shadow the envelope and is skipped.
    def emit(event : string; var fields : JsonValue? = null) {
        var obj <- {"ts" => JV(iso8601_now()), "event" => JV(event)}
        if (fields != null && fields is _object) {
            for (key, val in keys(fields as _object), values(fields as _object)) {
                if (key == "ts" || key == "event") continue
                obj[key] = val
            }
        }
        let line = write_json_compact(JV(obj))
        if (fexist(path) && stat(path).size >= uint64(max_bytes)) {
            rotate()
        }
        fopen(path, "ab") $(f) {
            if (f != null) {
                fwrite(f, line)
                fwrite(f, "\n")
            }
        }
        if (tee) {
            print("{line}\n")
        }
    }
}

// ---------------------------------------------------------------------------------------------
// desktop notification: best effort, never fatal, no dependency beyond the platform's own helper

def private applescript_quote(s : string) : string {
    return replace(replace(replace(s, "\\", "\\\\"), "\"", "\\\""), "\n", " - ")
}

def private powershell_quote(s : string) : string {
    return replace(s, "'", "''")
}

//! Off under tests and on a headless box, where a balloon has no one to reach.
var public notifications_enabled = true

//! Windows: a PowerShell balloon; macOS: Notification Center via osascript; Linux: notify-send.
//! A platform with none of those gets nothing - the same never-fatal contract everywhere.
def public notify(title, message : string; error : bool) {
    if (!notifications_enabled) return
    let plat = get_platform_name()
    if (plat == "windows") {
        let icon = error ? "Error" : "Info"
        let script = ("Add-Type -AssemblyName System.Windows.Forms;Add-Type -AssemblyName System.Drawing;"
            + "$n=New-Object System.Windows.Forms.NotifyIcon;$n.Icon=[System.Drawing.SystemIcons]::{icon};$n.Visible=$true;"
            + "$n.ShowBalloonTip(10000,'{powershell_quote(title)}','{powershell_quote(message)}',[System.Windows.Forms.ToolTipIcon]::{icon});"
            + "Start-Sleep -Seconds 11;$n.Dispose()")
        unsafe(spawn_argv(["powershell.exe", "-NoProfile", "-NonInteractive", "-Command", script]))
    } elif (plat == "darwin") {
        let script = "display notification \"{applescript_quote(message)}\" with title \"{applescript_quote(title)}\""
        unsafe(spawn_argv(["osascript", "-e", script]))
    } else {
        unsafe(spawn_argv(["notify-send", "-u", error ? "critical" : "normal", "-a", "watchdog", title, message]))
    }
}

// ---------------------------------------------------------------------------------------------
// startup stages: ranked and monotonic, detected from the child's own log prose

struct public StageRule {
    name : string
    rank : int
    pattern : string
}

//! A tune codegens each kernel variant, so the JIT lines fire hundreds of times mid-tune; only a
//! forward move is reported. `tune_restart` is the one legitimate rewind - the process restarts to
//! apply the winners - so it resets the rank and the sequence starts over.
let public TUNE_RESTART_RANK = 5

def public stage_rules() : array<StageRule> {
    return <- [
        StageRule(name = "jit_cached", rank = 1, pattern = "LLVM JIT: DLL cache hit"),
        StageRule(name = "jit_codegen", rank = 1, pattern = "LLVM JIT: DLL cache miss, codegen for"),
        StageRule(name = "jit_linked", rank = 2, pattern = "Library .+ linked - ok"),
        StageRule(name = "exchange_lookup", rank = 3, pattern = "exchange: scope .+ looking up a sidecar"),
        StageRule(name = "tuning", rank = 4, pattern = "llvm_tune: tuning scope"),
        StageRule(name = "tune_restart", rank = TUNE_RESTART_RANK, pattern = "restart to apply the winners"),
        StageRule(name = "model_load", rank = 6, pattern = "dasLLAMA: prepared image mapped"),
        StageRule(name = "asr_init", rank = 7, pattern = "starting \\d+ asynchronous ASR worker"),
        StageRule(name = "ready", rank = 8, pattern = "listening on http")]
}

struct public Stage {
    name : string
    rank : int
    since : float
}

class public StageDetector {
    rules : array<StageRule>
    regexes : array<Regex>

    def StageDetector() {
        rules <- stage_rules()
        for (rule in rules) {
            var re : Regex
            regex_compile(re, rule.pattern)
            regexes |> emplace(re)
        }
    }

    def detect(message : string) : int {
        for (i in range(length(rules))) {
            if (regex_search(regexes[i], message).x >= 0) return i
        }
        return -1
    }

    //! The fields to log for a forward stage move, or null. Mutates `stage` on a move.
    def advance(var stage : Stage&; message : string; now : float) : JsonValue? {
        let found = detect(message)
        if (found < 0) return null
        let rule = rules[found]
        if (rule.rank <= stage.rank) return null
        var fields : table<string; JsonValue?>
        fields["stage"] = JV(rule.name)
        if (!empty(stage.name)) {
            fields["after"] = JV(stage.name)
            fields["elapsed_s"] = JV(round_to(now - stage.since, 1))
        }
        stage.name = rule.name
        stage.rank = rule.rank == TUNE_RESTART_RANK ? 0 : rule.rank
        stage.since = now
        return JV(fields)
    }
}

def private round_to(v : float; digits : int) : float {
    let scale = pow(10.0, float(digits))
    return round(v * scale) / scale
}

// ---------------------------------------------------------------------------------------------
// `@tune <kind> k=v ...` lines from llvm_tune: a supervised child never owns a terminal, so it
// forwards structured progress instead of drawing a bar; the kernel boundaries are logged

//! `(kind, fields)` for a `<prefix><kind> k=v ...` line; kind is empty when the line is not one.
def public parse_child_event(message, prefix : string) : tuple<kind : string; fields : table<string; string>> {
    var fields : table<string; string>
    if (!starts_with(message, prefix)) return (kind = "", fields <- fields)
    let parts <- split(chop(message, length(prefix), length(message) - length(prefix)), " ")
    var kind = ""
    for (token in parts) {
        if (empty(token)) continue
        if (empty(kind)) {
            kind = token
            continue
        }
        let eq = find(token, "=")
        if (eq > 0) {
            fields[chop(token, 0, eq)] = chop(token, eq + 1, length(token) - eq - 1)
        }
    }
    return (kind = kind, fields <- fields)
}

// ---------------------------------------------------------------------------------------------
// health and shutdown over HTTP

def public health_ok(url : string) : bool {
    var ok = false
    with_http_request() $(var req) {
        req.method = http_method.GET
        req.url := url
        req.timeout = 2
        req.connect_timeout = 2
        request(req) $(resp) {
            if (resp != null) {
                let status = int(resp.status_code)
                ok = status >= 200 && status < 300
            }
        }
    }
    return ok
}

def private request_shutdown(url : string; var log : Emitter?) {
    var status = -1
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := url
        req.timeout = 5
        req.connect_timeout = 5
        request(req) $(resp) {
            if (resp != null) {
                status = int(resp.status_code)
            }
        }
    }
    if (status >= 0) {
        log->emit("shutdown_requested", JV((status = status)))
    } else {
        log->emit("shutdown_request_failed", JV((error = "no response from {url}")))
    }
}

// ---------------------------------------------------------------------------------------------
// crash capture: WER local dumps (Windows) and the bundle a crash leaves behind

let private WER_LOCAL_DUMPS = "HKLM\\SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting\\LocalDumps"

def private reg_query_value(key, name : string; var value : string&) : bool {
    var out : string
    let rc = run_and_capture(["reg.exe", "query", key, "/v", name, "/reg:64"], out, 10.0)
    if (rc != 0) return false
    for (line in split(out, "\n")) {
        let trimmed = strip(line)
        if (!starts_with(trimmed, name)) continue
        let parts <- split(trimmed, "    ")
        if (length(parts) >= 3) {
            value = strip(parts[length(parts) - 1])
            return true
        }
    }
    return false
}

//! The effective WER dump policy for an executable: `(dump_type, folder)`, type -1 when none. An
//! application key wins; the root key counts only when it carries a policy value of its own.
def public windows_local_dump_config(exe_name : string) : tuple<dump_type : int; folder : string> {
    if (get_platform_name() != "windows") return (dump_type = -1, folder = "")
    for (app_specific in [true, false]) {
        let key = app_specific ? "{WER_LOCAL_DUMPS}\\{exe_name}" : WER_LOCAL_DUMPS
        var dump_type_text : string
        var folder_text : string
        let has_type = reg_query_value(key, "DumpType", dump_type_text)
        let has_folder = reg_query_value(key, "DumpFolder", folder_text)
        if (!app_specific) {
            var dump_count : string
            var flags : string
            let has_policy = (has_type || has_folder
                || reg_query_value(key, "DumpCount", dump_count) || reg_query_value(key, "CustomDumpFlags", flags))
            if (!has_policy) continue
        } elif (!has_type && !has_folder) {
            var out : string
            if (run_and_capture(["reg.exe", "query", key, "/reg:64"], out, 10.0) != 0) continue
        }
        var dump_type = 1
        if (has_type) {
            let parsed = starts_with(dump_type_text, "0x") ? int(to_uint(dump_type_text, true)) : to_int(dump_type_text)
            dump_type = parsed
        }
        let folder = has_folder ? folder_text : path_join(get_env_variable("LOCALAPPDATA"), "CrashDumps")
        return (dump_type = dump_type, folder = folder)
    }
    return (dump_type = -1, folder = "")
}

//! Install an application-specific normal-minidump policy (needs elevation). Empty on success.
def public install_windows_local_dumps(exe_name, dump_dir : string; count : int) : string {
    if (get_platform_name() != "windows") return "WER LocalDumps is available only on Windows"
    mkdir_p(dump_dir)
    let key = "{WER_LOCAL_DUMPS}\\{exe_name}"
    for (args in [
        ["reg.exe", "add", key, "/v", "DumpFolder", "/t", "REG_EXPAND_SZ", "/d", dump_dir, "/f", "/reg:64"],
        ["reg.exe", "add", key, "/v", "DumpCount", "/t", "REG_DWORD", "/d", "{max(count, 1)}", "/f", "/reg:64"],
        ["reg.exe", "add", key, "/v", "DumpType", "/t", "REG_DWORD", "/d", "1", "/f", "/reg:64"]]) {
        var out : string
        if (run_and_capture(args, out, 10.0) != 0) return "reg add failed: {strip(out)}"
    }
    return ""
}

def private newest_matching(root : string; prefix : string; not_before : clock; suffix : string) : string {
    var newest = ""
    var newest_time : clock
    if (!fexist(root)) return ""
    dir(root) $(fname) {
        if (!starts_with(fname, prefix) || !ends_with(fname, suffix)) return
        let path = path_join(root, fname)
        let st = stat(path)
        if (!st.is_valid || st.mtime < not_before) return
        if (empty(newest) || st.mtime > newest_time) {
            newest = path
            newest_time = st.mtime
        }
    }
    return newest
}

def public latest_wer_report(started : clock) : string {
    if (get_platform_name() != "windows") return ""
    let program_data = has_env_variable("ProgramData") ? get_env_variable("ProgramData") : "C:\\ProgramData"
    let root = path_join(program_data, "Microsoft/Windows/WER")
    var best = ""
    var best_time : clock
    for (sub in ["ReportArchive", "ReportQueue"]) {
        let found = newest_matching(path_join(root, sub), "AppCrash_", started, "")
        if (empty(found)) continue
        let st = stat(found)
        if (empty(best) || st.mtime > best_time) {
            best = found
            best_time = st.mtime
        }
    }
    return best
}

//! Wait for WER to finish publishing the newest dump for this child; empty when none arrives.
def public wait_for_recent_dump(dump_dir, exe_name : string; started : clock; timeout_sec : float) : string {
    let deadline = ref_time_ticks()
    let prefix = to_lower(exe_name)
    while (float(get_time_usec(deadline)) / 1000000.0 < timeout_sec) {
        var newest = ""
        var newest_time : clock
        if (fexist(dump_dir)) {
            dir(dump_dir) $(fname) {
                if (!starts_with(to_lower(fname), prefix) || !ends_with(to_lower(fname), ".dmp")) return
                let path = path_join(dump_dir, fname)
                let st = stat(path)
                if (!st.is_valid || st.mtime < started) return
                if (empty(newest) || st.mtime > newest_time) {
                    newest = path
                    newest_time = st.mtime
                }
            }
        }
        if (!empty(newest)) {
            var readable = false
            fopen(newest, "rb") $(f) {
                readable = f != null
            }
            if (readable) return newest
        }
        sleep(250u)
    }
    return ""
}

struct private CrashMeta {
    timestamp : string
    pid : int
    exit_code : int
    exit_code_hex : string
    started_at : string
    command : array<string>
    cwd : string
    wer_report : string
    dump : string
    jit_dlls : array<string>
    program_artifacts : array<string>
}

let private JIT_ARTIFACT_SUFFIXES = [".dll", ".exp", ".lib", ".map", ".o", ".obj", ".pdb"]

def private prune_crash_bundles(root, name : string; keep : int) {
    if (keep <= 0 || !fexist(root)) return
    var bundles : array<tuple<path : string; mtime : clock>>
    dir(root) $(fname) {
        if (!starts_with(fname, "{name}-")) return
        let path = path_join(root, fname)
        let st = stat(path)
        if (st.is_valid && st.is_dir) {
            bundles |> push((path = path, mtime = st.mtime))
        }
    }
    bundles |> sort() $(a, b) {
        return a.mtime > b.mtime
    }
    for (i in range(keep, length(bundles))) {
        rmdir_rec(bundles[i].path)
    }
}

def private copy_into(src, dst_dir : string) {
    if (!fexist(src)) return
    mkdir_p(dst_dir)
    let _ = copy_file_result(src, path_join(dst_dir, base_name(src)), true)
}

//! Everything a crash leaves behind, in `crash_dir/<name>-<stamp>-pid<pid>/`: crash.json, the log,
//! the minidump, the program's symbols, the tune sidecar, and the JIT artifacts the child loaded
//! (or the newest cached DLL when it named none). Pruned to the newest `crash_bundles`.
def public collect_crash_bundle(cfg : WatchdogConfig; command : array<string>; child_pid, return_code : int;
                                started_at : string; wer_report, dump_path : string; jit_dlls : array<string>) : string {
    let stamp = replace(replace(replace(slice(iso8601_now(), 0, 19), "-", ""), ":", ""), "T", "-")
    let bundle = path_join(cfg.crash_dir, "{cfg.name}-{stamp}-pid{child_pid}")
    mkdir_p(bundle)
    var meta = CrashMeta(
        timestamp = iso8601_now(),
        pid = child_pid,
        exit_code = return_code,
        exit_code_hex = "0x{fmt(":08X", uint(return_code))}",
        started_at = started_at,
        cwd = cfg.cwd,
        wer_report = wer_report,
        dump = empty(dump_path) ? "" : base_name(dump_path))
    meta.command := command
    if (!empty(cfg.program)) {
        for (candidate in [cfg.program, replace_extension(cfg.program, ".map"), replace_extension(cfg.program, ".pdb")]) {
            if (fexist(candidate)) {
                meta.program_artifacts |> push(base_name(candidate))
            }
        }
    }
    var sorted_dlls := jit_dlls
    sorted_dlls |> sort()
    meta.jit_dlls := sorted_dlls
    fwrite(path_join(bundle, "crash.json"), sprint_json(meta, true) + "\n")
    copy_into(cfg.log, bundle)
    if (!empty(dump_path)) {
        copy_into(dump_path, bundle)
    }
    if (!empty(cfg.program)) {
        for (candidate in [cfg.program, replace_extension(cfg.program, ".map"), replace_extension(cfg.program, ".pdb")]) {
            copy_into(candidate, bundle)
        }
    }
    let tune_file = replace_extension(!empty(cfg.program) ? cfg.program : cfg.script, ".tune.json")
    copy_into(tune_file, bundle)
    if (fexist(cfg.jit_dir)) {
        var resolved : array<string>
        for (dll in jit_dlls) {
            let source = get_full_file_name(is_absolute(dll) ? dll : path_join(cfg.cwd, dll))
            let rel = relative_result(source, cfg.jit_dir)
            if (!(rel is value) || starts_with(rel as value, "..") || !fexist(source)) continue
            resolved |> push(source)
        }
        if (empty(resolved)) {
            var newest = ""
            var newest_time : clock
            dir_rec(cfg.jit_dir) $(fname, is_dir) {
                if (is_dir || !ends_with(fname, ".dll")) return
                let path = path_join(cfg.jit_dir, fname)
                let st = stat(path)
                if (empty(newest) || st.mtime > newest_time) {
                    newest = path
                    newest_time = st.mtime
                }
            }
            if (!empty(newest)) {
                resolved |> push(newest)
            }
        }
        for (dll in resolved) {
            let rel = relative_result(dll, cfg.jit_dir)
            if (!(rel is value)) continue
            let target_dir = path_join(path_join(bundle, "jitted_scripts"), dir_name(rel as value))
            for (suffix in JIT_ARTIFACT_SUFFIXES) {
                let source = replace_extension(dll, suffix)
                if (!fexist(source)) continue
                copy_into(source, target_dir)
            }
        }
    }
    prune_crash_bundles(cfg.crash_dir, cfg.name, cfg.crash_bundles)
    return bundle
}

// ---------------------------------------------------------------------------------------------
// the supervision loop, as a state machine the host steps

def private now_seconds(origin : int64) : float {
    return float(get_time_usec(origin)) / 1000000.0
}

class public Supervisor {
    cfg : WatchdogConfig = WatchdogConfig()
    log : Emitter?
    command : array<string>
    child_exe_name : string
    dump_type : int = -1
    self_pid : int
    stages : StageDetector?
    jit_dll_re : Regex

    origin : int64
    stopping : bool = false
    finished : bool = false
    result : int = 0
    failures : int = 0
    recovery_pending : bool = false
    restart_at : float = 0.0
    child_env : array<string>

    child : process = null
    child_pid : int = 0
    started_at : float = 0.0
    started_clock : clock
    started_iso : string
    stage : Stage
    tune_restart_seen : bool = false
    stdout_open : bool = true
    jit_dlls : array<string>
    next_health : float = 0.0
    next_heartbeat : float = 0.0
    last_health : int = -1
    last_health_change : float = 0.0
    shutdown_sent : bool = false
    shutdown_requested_at : float = 0.0
    terminate_sent_at : float = -1.0
    kill_sent_at : float = -1.0

    def Supervisor(config : WatchdogConfig; var emitter : Emitter?; command_line : array<string>; own_pid : int) {
        cfg := config
        log = emitter
        command := command_line
        child_exe_name = base_name(command[0])
        self_pid = own_pid
        stages = new StageDetector()
        regex_compile(jit_dll_re, "LLVM JIT: DLL cache (?:hit |miss, codegen for )(.+\\.dll)$")
        origin = ref_time_ticks()
        if (!empty(cfg.stop_file)) {
            child_env |> push("{cfg.stop_env}={cfg.stop_file}")
        }
    }

    def now() : float {
        return now_seconds(origin)
    }

    //! The host's stop signal (Ctrl-C, SIGTERM, a tray item): the child is asked to stop on the
    //! next tick, and supervision ends with 0 once it has.
    def request_stop() {
        stopping = true
    }

    def private unlink_stop_file() {
        if (!empty(cfg.stop_file) && fexist(cfg.stop_file)) {
            remove(cfg.stop_file)
        }
    }

    def private spawn() : bool {
        unlink_stop_file()
        started_at = now()
        started_clock = get_clock()
        started_iso = iso8601_now()
        child = unsafe(spawn_process(command, cfg.cwd, child_env))
        if (child == null) {
            log->emit("spawn_failed", JV((error = "could not start {command[0]}")))
            notify("{cfg.name} watchdog", "Could not start child: {command[0]}", true)
            return false
        }
        child_pid = unsafe(process_pid(child))
        log->emit("child_started", JV((pid = child_pid)))
        stage = Stage(name = "", rank = 0, since = started_at)
        tune_restart_seen = false
        stdout_open = true
        jit_dlls |> clear()
        next_health = started_at
        next_heartbeat = started_at + HEALTH_HEARTBEAT_SECONDS
        last_health = -1
        last_health_change = started_at
        shutdown_sent = false
        shutdown_requested_at = 0.0
        terminate_sent_at = -1.0
        kill_sent_at = -1.0
        return true
    }

    def private on_child_line(message : string) {
        let event <- parse_child_event(message, TUNE_EVENT_PREFIX)
        if (!empty(event.kind)) {
            // a tune is hundreds of steps; log the kernel boundaries, or the log becomes the
            // flood these events exist to replace
            if (event.kind == "plan" || event.kind == "end" || event.kind == "abort") {
                var fields <- {"pid" => JV(child_pid), "kind" => JV(event.kind)}
                for (key, val in keys(event.fields), values(event.fields)) {
                    if (key == "pid" || key == "kind") continue
                    fields[key] = JV(val)
                }
                log->emit("tune", JV(fields))
            }
            return
        }
        // both the tuner's marker and the exchange client's "sidecar applied" marker license the
        // immediate exit-3 relaunch - the process restarts to stamp fresh winners
        if (find(message, "restart to apply the winners") >= 0) {
            tune_restart_seen = true
        }
        if (regex_search(jit_dll_re, message).x >= 0) {
            jit_dlls |> push(regex_group(jit_dll_re, 1, message))
        }
        var moved = stages->advance(stage, message, now())
        if (moved != null) {
            var fields : table<string; JsonValue?>
            fields["pid"] = JV(child_pid)
            for (key, val in keys(moved as _object), values(moved as _object)) {
                fields[key] = val
            }
            log->emit("stage", JV(fields))
        }
        log->emit("child", JV((pid = child_pid, message = message)))
    }

    def private drain_child() {
        if (!stdout_open) return
        stdout_open = unsafe(process_drain(child) $(line) {
            on_child_line(line)
        })
    }

    def private poll_health(t : float) {
        if (cfg.no_health) {
            if (recovery_pending && t - started_at >= min(cfg.stable_seconds, 10.0)) {
                notify("{cfg.name} recovered", "Process is running (pid {child_pid})", false)
                log->emit("recovered", JV((pid = child_pid)))
                recovery_pending = false
            }
            return
        }
        if (t < next_health) return
        let healthy = health_ok(cfg.health_url)
        let healthy_int = healthy ? 1 : 0
        // transitions, not every poll: a cold start is minutes of `ok:false` the stage events
        // say better, a steady run is hours of `ok:true`; the heartbeat keeps a quiet run observable
        if (healthy_int != last_health) {
            var fields <- {"pid" => JV(child_pid), "ok" => JV(healthy)}
            if (last_health >= 0) {
                fields["was"] = JV(last_health == 1)
                fields["after_s"] = JV(round_to(t - last_health_change, 1))
            }
            if (!healthy && !empty(stage.name)) {
                fields["stage"] = JV(stage.name)
            }
            log->emit("health", JV(fields))
            last_health = healthy_int
            last_health_change = t
            next_heartbeat = t + HEALTH_HEARTBEAT_SECONDS
        } elif (healthy && t >= next_heartbeat) {
            log->emit("health_heartbeat", JV((pid = child_pid, ok = true, healthy_for_s = round(t - last_health_change))))
            next_heartbeat = t + HEALTH_HEARTBEAT_SECONDS
        }
        if (healthy && recovery_pending) {
            notify("{cfg.name} recovered", "Server is healthy (pid {child_pid})", false)
            log->emit("recovered", JV((pid = child_pid)))
            recovery_pending = false
        }
        next_health = t + max(cfg.health_interval, 1.0)
    }

    def private send_stop_request() {
        if (!empty(cfg.stop_file)) {
            mkdir_p(dir_name(cfg.stop_file))
            fwrite(cfg.stop_file, "stop\n")
            log->emit("stop_file_requested", JV((path = cfg.stop_file)))
        } elif (cfg.no_shutdown) {
            unsafe(process_terminate(child))
            terminate_sent_at = now()
            log->emit("terminate_requested")
        } else {
            request_shutdown(cfg.shutdown_url, log)
        }
        shutdown_sent = true
        shutdown_requested_at = now()
    }

    def private backoff(uptime : float) : float {
        failures = uptime >= cfg.stable_seconds ? 0 : failures + 1
        return min(pow(2.0, float(min(failures, 6))), cfg.max_restart_delay)
    }

    //! Classify an exit: 0 ends supervision, 3 with the marker relaunches at once, 3 without it
    //! is a tuner abort and backs off, 4 relaunches at once, anything else is a crash.
    def private on_child_exit(return_code : int) {
        let uptime = now() - started_at
        log->emit("child_exited", JV((pid = child_pid, code = return_code, uptime_seconds = uptime)))
        unsafe(close_process(child))
        child = null
        let script_mode = empty(cfg.program)
        if (stopping) {
            finish(0)
            return
        }
        if (return_code == 0) {
            log->emit("intentional_shutdown")
            finish(0)
            return
        }
        if (script_mode && return_code == TUNE_RESTART_EXIT && tune_restart_seen) {
            log->emit("tune_bootstrap_complete")
            failures = 0
            restart_at = now()
            return
        }
        if (script_mode && return_code == TUNE_RESTART_EXIT) {
            let delay = backoff(uptime)
            log->emit("tune_incomplete", JV((uptime_seconds = uptime, restart_delay_seconds = delay)))
            restart_at = now() + delay
            return
        }
        if (return_code == CONFIG_RESTART_EXIT) {
            log->emit("config_restart_relaunch")
            failures = 0
            restart_at = now()
            return
        }
        let wer = latest_wer_report(started_clock)
        let dump = dump_type == 1 ? wait_for_recent_dump(cfg.dump_dir, child_exe_name, started_clock, 15.0) : ""
        let delay = backoff(uptime)
        log->emit("crash", JV((code = return_code, uptime_seconds = uptime, restart_delay_seconds = delay,
            wer = wer, dump = dump)))
        var bundle = ""
        if (!cfg.no_crash_bundle) {
            bundle = collect_crash_bundle(cfg, command, child_pid, return_code, started_iso, wer, dump, jit_dlls)
            log->emit("crash_bundle", JV((path = bundle)))
        }
        var body = "Exit {return_code}; restarting in {int(delay)}s"
        body += empty(dump) ? "\nNo minidump produced" : "\nDump: {dump}"
        if (!empty(bundle)) {
            body += "\nBundle: {bundle}"
        }
        notify("{cfg.name} crashed", body, true)
        recovery_pending = true
        restart_at = now() + delay
    }

    //! Runs only once the child is gone - reaped by on_child_exit, never spawned, or left behind
    //! as unkillable; the stop ladder in tick() is what ends a live child.
    def private finish(code : int) {
        // only clear the pid file if it is still ours: a second watchdog that took the slot
        // owns it now, and deleting its claim would let a third start alongside it
        if (self_pid != 0 && fexist(cfg.pid_file)) {
            var text : string
            fopen(cfg.pid_file, "rb") $(f) {
                if (f != null) {
                    text = fread(f)
                }
            }
            if (strip(text) == "{self_pid}") {
                remove(cfg.pid_file)
            }
        }
        unlink_stop_file()
        log->emit("watchdog_stopped")
        finished = true
        result = code
    }

    //! One supervision step, at most one tick long. True once supervision has ended; `result`
    //! then carries the exit code.
    def tick() : bool {
        if (finished) return true
        if (child == null) {
            if (stopping) {
                finish(0)
                return true
            }
            if (now() < restart_at) {
                sleep(TICK_MS)
                return false
            }
            if (!spawn()) {
                finish(2)
                return true
            }
            return false
        }
        drain_child()
        let t = now()
        if (stopping && !shutdown_sent) {
            send_stop_request()
        }
        // a stop is a ladder with an end: the request, terminate once the stop timeout passes,
        // kill ten seconds later, and ten seconds after that the child is left behind - a child
        // that ignores every signal must not hold the supervisor forever
        if (stopping && shutdown_sent) {
            if (terminate_sent_at < 0.0) {
                if (t - shutdown_requested_at > cfg.stop_timeout) {
                    unsafe(process_terminate(child))
                    terminate_sent_at = t
                    log->emit("terminate_requested", JV((after_s = round_to(t - shutdown_requested_at, 1))))
                }
            } elif (kill_sent_at < 0.0) {
                if (t - terminate_sent_at > 10.0) {
                    unsafe(process_kill(child))
                    kill_sent_at = t
                    log->emit("kill_requested")
                }
            } elif (t - kill_sent_at > 10.0) {
                log->emit("child_unkillable", JV((pid = child_pid)))
                child = null
                finish(1)
                return true
            }
        }
        poll_health(t)
        let rc = unsafe(process_poll(child))
        if (rc == process_running) {
            sleep(TICK_MS)
            return false
        }
        drain_child()
        on_child_exit(rc)
        return finished
    }

    //! Run ticks until supervision ends; the exit code.
    def run() : int {
        while (!tick()) {
        }
        return result
    }
}

// ---------------------------------------------------------------------------------------------
// the program: configuration, the dump policy, the single-instance guard, then supervision

//! Everything before the loop. Returns the supervisor to drive, or null with `exit_code` set
//! when the program is done (help, --install-local-dumps, a refused start).
def private wants_help(args : array<string>) : bool {
    for (arg in args) {
        if (arg == "--") return false
        if (arg == "--help" || arg == "-h") return true
    }
    return false
}

def public watchdog_start(args : array<string>; program_dir : string; self_pid : int; var exit_code : int&) : Supervisor? {
    exit_code = 0
    if (wants_help(args)) {
        print_help(get_command_info(type<WatchdogConfig>), "watchdog")
        print("Everything after -- goes to the supervised program.\n")
        return null
    }
    var resolved <- resolve_config(args, program_dir)
    if (!empty(resolved.error)) {
        print("{resolved.error}\n")
        exit_code = 2
        return null
    }
    var cfg <- resolved.cfg
    var log = new Emitter(cfg.log)
    let command <- child_command(cfg)
    let child_exe_name = base_name(command[0])
    if (cfg.install_local_dumps) {
        let error = install_windows_local_dumps(child_exe_name, cfg.dump_dir, cfg.dump_count)
        if (!empty(error)) {
            log->emit("wer_install_failed", JV((executable = child_exe_name, dump_dir = cfg.dump_dir, error = error)))
            notify("{cfg.name} dump setup failed", "Run the LocalDumps install from an elevated terminal: {error}", true)
            exit_code = 2
            return null
        }
        log->emit("wer_installed", JV((executable = child_exe_name, dump_dir = cfg.dump_dir,
            dump_type = "minidump", dump_count = max(cfg.dump_count, 1))))
        return null
    }
    let policy = windows_local_dump_config(child_exe_name)
    if (!empty(policy.folder)) {
        cfg.dump_dir = policy.folder
    }
    if (policy.dump_type == 1) {
        log->emit("wer_ready", JV((executable = child_exe_name, dump_dir = cfg.dump_dir, dump_type = "minidump")))
    } else {
        let reason = policy.dump_type < 0 ? "not configured" : "unsafe dump type {policy.dump_type}"
        log->emit("wer_not_ready", JV((executable = child_exe_name, dump_dir = cfg.dump_dir, reason = reason)))
        if (cfg.require_dumps) {
            notify("{cfg.name} watchdog not started", "WER normal minidumps are {reason}; run --install-local-dumps elevated", true)
            exit_code = 2
            return null
        }
    }
    // two watchdogs mean two children fighting over one port; the pid file records, and protects
    if (self_pid != 0) {
        var prior = 0
        if (fexist(cfg.pid_file)) {
            var text : string
            fopen(cfg.pid_file, "rb") $(f) {
                if (f != null) {
                    text = fread(f)
                }
            }
            prior = to_int(strip(text))
        }
        if (prior != 0 && prior != self_pid && unsafe(process_alive(prior))) {
            log->emit("watchdog_already_running", JV((pid = prior, pid_file = cfg.pid_file)))
            print("watchdog: {cfg.name} is already supervised by pid {prior} (per {cfg.pid_file}). Stop it first, or delete the pid file if it is stale.\n")
            exit_code = 2
            return null
        }
        mkdir_p(dir_name(cfg.pid_file))
        fwrite(cfg.pid_file, "{self_pid}")
    }
    var sup = new Supervisor(cfg, log, command, self_pid)
    sup.dump_type = policy.dump_type
    log->emit("watchdog_started", JV((pid = self_pid, command = JV(command), cwd = cfg.cwd)))
    return sup
}

//! The whole program for a host that owns nothing else: configure, supervise until the child
//! shuts down or the supervisor is told to, return the exit code.
def public watchdog_main(args : array<string>; program_dir : string; self_pid : int) : int {
    var code = 0
    var sup = watchdog_start(args, program_dir, self_pid, code)
    if (sup == null) return code
    let rc = sup->run()
    unsafe {
        delete sup
    }
    return rc
}
