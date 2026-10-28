options gen2
options _dasllama_internal = true

module _server_rig shared

require dastest/testing_boost public
require daslib/fio
require daslib/clargs   // get_host_binary: the daslang a child runs under
require openai/openai_vision   // file_base64, image_file_data_uri: the one file reader and data: URI builder
require math
require daslib/jobque_boost   // hold_engine: the holding request and its control on threads of their own
require strings
require dashv/dashv_boost   // GET / POST: serve_clients polls and shuts the server down
require dasllama/dasllama_env
require daslib/json_boost   // stream_tally: the SSE chunks' deltas
require daslib/strings_boost   // sse_datas: split / trim_prefix
require daslib/module_path   // get_this_module_dir: with_spawned_server finds main.das beside the rig

//! The server suites' model roots: ONE resolution - `DASLLAMA_MODELS_DIR`, else the rig default -
//! so a test file never spells a second one.
def models_dir() : string => models_dir_resolved()

//! The suites' model gate: a JIT-only run skips interpreted, a missing file skips by name.
def model_missing(t : T?; path : string) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA server test is JIT-only)")
        return true
    }
    if (!stat(path).is_valid) {
        t |> skip("{base_name(path)} not present (set DASLLAMA_MODELS_DIR)")
        return true
    }
    return false
}

//! The same gate over every file a cell needs: the first one missing skips by name.
def model_missing(t : T?; paths : array<string>) : bool {
    for (path in paths) {
        return true if (model_missing(t, path))
    }
    return false
}

//! A file's bytes as base64; "" when unreadable.
def file_base64(path : string) : string => openai_vision::file_base64(path)

//! An image file as the data: URI a browser's FileReader would produce; "" when unreadable.
def image_data_uri(path : string) : string => image_file_data_uri(path)

//! The last `n` bytes of a child's output, for a failure message.
def tail_of(s : string; n : int = 800) : string => slice(s, max(0, length(s) - n))

//! A child daslang's argv: the daslang this test runs under, -jit, the flags a child needs
//! forwarded (-dasroot / -module-cache), `script`, then `args` verbatim.
def child_daslang_argv(script : string; args : array<string>) : array<string> {
    var out : array<string>
    out |> reserve(6 + length(args))
    out |> push(get_host_binary())
    out |> push("-jit")
    var carry = ""   // a forwarded flag, held until its value arrives on the next argv entry
    for (a in get_command_line_arguments()) {   // a LOCKED array: iterate it, never bind or delete it
        if (!empty(carry)) {
            out |> push(clone_string(carry))
            out |> push(clone_string(a))
            carry = ""
        } elif (a == "-dasroot" || a == "-module-cache") {
            carry = clone_string(a)
        }
    }
    out |> push(script)
    out |> push_from(args)
    return <- out
}

def private retry_until_true(attempts : int; req : block<() : bool>) : bool {
    for (_attempt in range(max(attempts, 1))) {
        return true if (invoke(req))
        sleep(100u)
    }
    return false
}

//! The client half of a server test. The caller's server thread boots on `port`; this polls `ready_path` there until it
//! answers 200 (`attempts` tries, 100 ms apart), runs `blk` against the server, then POSTs /shutdown so the thread
//! returns. False when the server never answered - `blk` did not run; the caller joins its thread either way, so the
//! shutdown is posted until the server takes it or as many tries again pass: a server still loading when the poll gave
//! up listens later, and a shutdown posted once to a closed port would leave the caller's join waiting on it forever.
def serve_clients(port, attempts : int; ready_path : string; blk : block<(base_url : string) : void>) : bool {
    let base_url = "http://127.0.0.1:{port}"
    let ready = retry_until_true(attempts) $() : bool {
        var ok = false
        GET("{base_url}{ready_path}") $(resp) {
            ok = resp != null && resp.status_code == http_status.OK
        }
        return ok
    }
    if (ready) {
        invoke(blk, base_url)
    }
    retry_until_true(ready ? 1 : attempts) $() : bool {   // a server still loading when the poll gave up listens later: the join needs this to land

        var taken = false
        POST("{base_url}/shutdown", "") $(resp) {
            taken = resp != null && resp.status_code == http_status.OK
        }
        return taken
    }
    return ready
}

//! what `hold_engine` read: how long the forward held the engine, how long a route the engine answers itself waited
//! inside that hold, and the slowest probe
struct EngineHold {
    hold_ms : int
    engine_ms : int
    worst_ms : int
}

//! Hold the server's engine with an embeddings forward over a long input and run `probe` again and again until the
//! forward returns, `pause_ms` apart. Inside the hold one GET /config - a route the engine answers itself - is timed:
//! it waits the hold out, which is the control a probe's wall is read against.
def hold_engine(base_url : string; pause_ms : int; probe : block<() : void>) : EngineHold {
    var read = EngineHold()
    let long_input = build_string() $(var w) {
        for (i in range(900)) {
            w |> write("word{i} ")
        }
    }
    let body = "\{\"input\":\"{long_input}\"}"
    with_atomic32() $(held_ms) {
        new_thread() <| @capture(= base_url, = body, = held_ms) {
            let t0 = ref_time_ticks()
            POST("{base_url}/v1/embeddings", body) $(_resp) {}
            held_ms |> set(max(1, get_time_usec(t0) / 1000))
        }
        sleep(40u)   // the forward is under way
        with_atomic32() $(waited_ms) {
            with_job_status(1) $(asked) {
                new_thread() <| @capture(= base_url, = waited_ms, = asked) {
                    let t0 = ref_time_ticks()
                    GET("{base_url}/config") $(_resp) {}
                    waited_ms |> set(max(1, get_time_usec(t0) / 1000))
                    asked |> notify_and_release
                }
                while (held_ms |> get == 0) {
                    let t0 = ref_time_ticks()
                    invoke(probe)
                    read.worst_ms = max(read.worst_ms, get_time_usec(t0) / 1000)
                    if (pause_ms > 0) {
                        sleep(uint(pause_ms))
                    }
                }
                asked |> join
            }
            read.engine_ms = waited_ms |> get
        }
        read.hold_ms = held_ms |> get
    }
    return read
}

//! The bars of a held-engine cell over what `hold_engine` read: skipped on a box that ran the forward too fast to tell
//! a waiting route from a served one, else the control (the engine's own route waited the hold out) and every probe in
//! under a quarter of that wait. False when the cell was skipped.
def held_engine_bars(t : T?; read : EngineHold) : bool {
    if (read.hold_ms <= 400) {
        t |> skip("the box ran the forward in {read.hold_ms} ms: too short a hold to tell a waiting route from a served one")
        return false
    }
    t |> success(read.engine_ms * 2 >= read.hold_ms, "the control: a route the engine answers waited out the hold ({read.engine_ms} of {read.hold_ms} ms)")
    t |> success(read.worst_ms * 4 < read.engine_ms, "every probe in under a quarter of what the engine's own route waited (the slowest {read.worst_ms} ms against {read.engine_ms} ms)")
    return true
}

//! Every `data: <payload>` line of an SSE body, in order ("[DONE]" included).
def sse_datas(body : string) : array<string> {
    var lines <- split(body, "\n")
    var out <- [for (ln in lines); trim_prefix(ln, "data: "); where starts_with(ln, "data: ")]
    delete lines
    return <- out
}

//! what a streaming chat request put on the wire: the content deltas counted and joined, the reasoning deltas counted,
//! the tool_calls chunks that carried a call counted, the finish reason
struct StreamTally {
    n_content : int
    n_reasoning : int
    n_calls : int
    content : string
    finish : string
}

def stream_tally(base_url, body : string) : StreamTally {
    var tally = StreamTally()
    var parts : array<string>
    POST("{base_url}/v1/chat/completions", body) $(resp) {
        var datas <- sse_datas(string(resp.body))
        for (d in datas) {
            continue if (d == "[DONE]")
            var err : string
            var js = read_json(d, err)
            continue if (js == null)
            let delta = js?["choices"]?[0]?["delta"]
            let cnt = delta?["content"] ?? ""
            if (!empty(cnt)) {
                tally.n_content ++
                parts |> push(cnt)
            }
            if (!empty(delta?["reasoning_content"] ?? "")) {
                tally.n_reasoning ++
            }
            let tc = delta?["tool_calls"]
            if (tc != null && tc.value is _array && !empty(tc.value as _array)) {
                tally.n_calls ++
            }
            let fin = js?["choices"]?[0]?["finish_reason"] ?? ""
            if (!empty(fin)) {
                tally.finish = fin
            }
            delete_json(js)
        }
        delete datas
    }
    tally.content = join(parts, "")
    delete parts
    return tally
}

let CHILD_TIMEOUT_SEC = 420.0   //! a spawned boot parses, JITs and loads before it listens
let BOOT_POLL_ATTEMPTS = 720    //! x 500 ms - inside the child's own timeout, so a hung boot still joins

struct private BootOutcome {
    rc : int
    tail : string
}

//! `main.das` (beside this rig) spawned as a child on `port` with `flags` after `--`, pinned by `--config` to a temp file
//! holding `config_text` - main.das otherwise auto-discovers a dasllama-server.toml in the cwd or beside itself, and a
//! developer's own would decide what the child serves. The child runs the daslang this test runs under with its -dasroot /
//! -module-cache, so a worktree run never compiles main.das against another tree. `blk` runs once /v1/stats answers; the
//! child is shut down after. A [tune] bootstrap exit relaunches once.
def with_spawned_server(t : T?; what : string; port : int; flags : array<string>; config_text : string; blk : block<(base_url : string) : void>) {
    let server_das = path_join(get_this_module_dir(), "main.das")
    if (!stat(server_das).is_valid) {
        t |> skip("main.das is not beside this test")
        return
    }
    var terr : string
    let tmp = temp_directory(terr)
    let config = path_join(empty(tmp) ? "." : tmp, "dasllama_boot_{port}_{ref_time_ticks()}.toml")
    if (!fwrite(config, config_text)) {
        t |> skip("could not write the child's config at {config}")
        return
    }
    let base_url = "http://127.0.0.1:{port}"
    var ready = false
    var why = ""
    with_channel(1) $(ch) {
        with_job_status(1) $(done) {
            var inscope tail_args <- ["--"]
            tail_args |> push_from(flags, ["--port", "{port}", "--config", config])
            var inscope args <- child_daslang_argv(server_das, tail_args)
            new_thread <| @capture(<- args) {
                var out = ""
                var rc = run_and_capture(args, out, CHILD_TIMEOUT_SEC)
                if (rc == 3 && find(out, "restart to apply") >= 0) {
                    out = ""   // [tune] bootstrap: the child adopted this box's kernels and exited with the restart code
                    rc = run_and_capture(args, out, CHILD_TIMEOUT_SEC)
                }
                ch |> push_clone(BootOutcome(rc = rc, tail = slice(out, max(0, length(out) - 600))))
                ch |> notify()
                ch |> release()
                done |> notify_and_release()
            }
            var died = false
            for (_attempt in range(BOOT_POLL_ATTEMPTS)) {
                GET("{base_url}/v1/stats") $(resp) {
                    if (resp != null && resp.status_code == http_status.OK) {
                        ready = true
                    }
                }
                break if (ready)
                died = ch |> try_pop_clone() $(o : BootOutcome#) {
                    why = "the boot exited rc={o.rc} before it listened - tail: {clone_string(o.tail)}"
                }
                break if (died)
                sleep(500u)
            }
            if (ready) {
                invoke(blk, base_url)
            }
            POST("{base_url}/shutdown", "") $(_resp) {}
            done |> join()
        }
    }
    remove(config)
    t |> success(ready, "{what} listened. {why}")
}
