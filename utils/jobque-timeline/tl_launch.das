options gen2
options indenting = 4

module tl_launch

require daslib/jobque_boost
require daslib/strings_boost
require daslib/command_line
require daslib/fio
require math
require strings

// Launch rail: run a trace-producing app (argv-direct popen_argv — no shell quoting traps)
// in a background job, stream its result back over a channel, and watch trace files for
// changes (auto-refresh). The job captures plain strings; argv is rebuilt job-side.

struct public LaunchMsg {
    kind : int      // 0 = output line, 1 = done
    text : string
    rc : int
}

struct public LaunchRail {
    app : string
    args : string
    affinity : bool = true   // DAS_JOBQUE_AFFINITY for the child
    threads : int = 0        // DAS_JOBQUE_THREADS for the child; 0 = inherit
    running : bool
    has_run : bool
    exit_code : int
    tail : array<string>     // last lines of the finished run's output
    ch : Channel?
}

//! Quote-aware argv split: `-m "a b.gguf" -p 512` -> ["-m", "a b.gguf", "-p", "512"]
def public split_args(s : string) : array<string> {
    var out : array<string>
    var cur : array<uint8>
    var in_quote = false
    peek_data(s) $(data) {
        for (i in range(length(data))) {
            let c = int(data[i])
            if (c == '"') {
                in_quote = !in_quote
            } elif (!in_quote && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) {
                if (!empty(cur)) {
                    unsafe {
                        out |> push(string(cur))
                    }
                    cur |> clear()
                }
            } else {
                cur |> push(uint8(c))
            }
        }
    }
    if (!empty(cur)) {
        unsafe {
            out |> push(string(cur))
        }
    }
    delete cur
    return <- out
}

//! First whitespace-separated token ending in ".json" — how output lines like
//! "jobque trace saved: <path>" surface a clickable trace. "" when none.
def public extract_json_path(line : string) : string {
    var found = ""
    var toks <- split_args(line)
    for (tk in toks) {
        if (ends_with(tk, ".json")) {
            found = tk
            break
        }
    }
    delete toks
    return found
}

//! Start `app args` in a background job. Output (last lines) and the exit code arrive
//! via launch_pump. False when already running or no app set.
def public launch_start(var L : LaunchRail) : bool {
    if (L.running || empty(L.app)) {
        return false
    }
    L.running = true
    L.has_run = false
    L.exit_code = -9999
    delete L.tail
    // the child inherits these: dasLLAMA's env-armed trace rails point at tool-owned
    // paths (the "jobque trace saved:" output line + auto-open pick the file up), and
    // the affinity pin follows the checkbox
    var terr = ""
    let tdir = "{temp_directory(terr)}/jobque-timeline"
    if (empty(terr)) {
        mkdir(tdir)
        set_env_variable("DASLLAMA_PREFILL_TRACE", "{tdir}/launch_prefill.trace.json")
        set_env_variable("DASLLAMA_BATCH_TRACE", "{tdir}/launch_batch.trace.json")
    }
    set_env_variable("DAS_JOBQUE_AFFINITY", L.affinity ? "1" : "0")
    if (L.threads > 0) {
        set_env_variable("DAS_JOBQUE_THREADS", "{L.threads}")
    }
    L.ch = unsafe(channel_create())
    var ch = L.ch
    let app = clone_string(L.app)          // cloned: the job below runs in another context
    let args = clone_string(L.args)
    let das_exe = clone_string(get_das_exe())   // .das apps run as `daslang -jit <script> -- <args>`
    new_job <| @ {
        var outp = ""
        var argv <- split_args(args)
        var full : array<string>
        if (ends_with(app, ".das")) {
            full |> push(das_exe)
            full |> push("-jit")
            full |> push(app)
            full |> push("--")
        } else {
            full |> push(app)
        }
        full |> push_from(argv)
        let rc = unsafe(popen_argv(full, 0.0, $(f) {
            if (f != null) {
                outp = fread(f)
            }
        }))
        var lines <- split(outp, "\n")
        let first = max(0, length(lines) - 60)
        for (i in range(first, length(lines))) {
            ch |> push_clone(LaunchMsg(kind = 0, text = lines[i], rc = 0))
        }
        ch |> push_clone(LaunchMsg(kind = 1, text = "", rc = rc))
        ch |> release()
        delete lines
        delete full
        delete argv
    }
    return true
}

//! Drain the launch channel (call once per frame). Returns true the frame the run finishes.
def public launch_pump(var L : LaunchRail) : bool {
    if (L.ch == null) {
        return false
    }
    var finished = false
    var more = true
    while (more) {
        more = L.ch |> try_pop_clone() $(m : LaunchMsg#) {
            if (m.kind == 1) {
                L.exit_code = m.rc
                L.running = false
                L.has_run = true
                finished = true
            } else {
                var line = ""
                unsafe {
                    line = clone_string(m.text)
                }
                L.tail |> push(line)
                if (length(L.tail) > 60) {
                    L.tail |> erase(0)
                }
            }
        }
    }
    if (finished) {
        unsafe {
            channel_remove(L.ch)
        }
        L.ch = null
    }
    return finished
}

// ---- file watching (auto-refresh): mtime change + size stable across two polls ----

struct public FileWatch {
    mtime : int64
    size : uint64
    pending_mtime : int64
    pending_size : uint64
    pending : bool
}

def public watch_prime(var w : FileWatch; path : string) {
    let st = stat(path)
    if (st.is_valid) {
        w.mtime = int64(st.mtime)
        w.size = st.size
    }
    w.pending = false
}

//! Poll a file (call every ~0.5s). True when it changed AND stayed stable for one poll —
//! the debounce against catching traceSave mid-write.
def public watch_poll(var w : FileWatch; path : string) : bool {
    let st = stat(path)
    if (!st.is_valid) {
        return false
    }
    let mt = int64(st.mtime)
    let sz = st.size
    if (!w.pending) {
        if (mt != w.mtime || sz != w.size) {
            w.pending = true
            w.pending_mtime = mt
            w.pending_size = sz
        }
        return false
    }
    if (mt == w.pending_mtime && sz == w.pending_size) {
        w.mtime = mt
        w.size = sz
        w.pending = false
        return true
    }
    w.pending_mtime = mt
    w.pending_size = sz
    return false
}
